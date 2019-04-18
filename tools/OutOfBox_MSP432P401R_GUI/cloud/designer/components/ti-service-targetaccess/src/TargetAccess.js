/*****************************************************************
 * Copyright (c) 2015 Texas Instruments and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Paul Gingrich - Initial API and implementation
 *****************************************************************/
var gc = gc || {};
gc.target = gc.target || {};
gc.target.access = gc.target.access || {};

(function() {
	
	var dsLite; 

	var supportedScalarTypes = 
	{
			'int': true,
			'short': true,
			'long': true,
			'float': true,
			'double': true
	}; 
	var arrayValueConverters = {};
	
	var getArrayValueConverter = function(type)
	{
		if (arrayValueConverters[type] === undefined)
		{
			arrayValueConverters[type] = function(data) 
			{
				for(var i = data.values.length; i-- > 0; )
				{
					data.values[i] = convert(type, data.values[i]);
				}
				return data.values;
			};
		}
		return arrayValueConverters[type];
	};
	
	var convert = function(type, value)
	{
		if (type)
		{
			if (type.indexOf('enum') === 0 || supportedScalarTypes[type])
			{
				// check for negative numbers
				if (value.indexOf('0x') === 0 && value.charAt(2) > '7')
				{
					value = (+value)-(1 << (4*(value.length - 2)));  // converts text to number while performing a negation at the same time.
				}
				else
				{
					value = +value;  // converts text to number without altering it.
				}
			}
			else if (type === 'boolean')
			{
				value = !!value;
			}
			else if (type.indexOf('unsigned') === 0)
			{
				value = +value;
			}
		}
		return value;
	};

	var onReadStringComplete = function(data)
	{
		return data.text;
	};
	
	var readStructValue = function(members)
	{
		if (dsLite)
		{
			var promises = [];
			for(var i = members.length; i-- > 0; )
			{
				promises[i] = dsLite.expressions.evaluate(members[i].expression);
			}
			return Q.all(promises).then(function(results) 
			{
				var result = {};
				for(var i = members.length; i--> 0; )
				{
					result[members[i].name] = convert(results[i].type, results[i].value);
				}
				return result;
			});
		}
		else
		{
			throw 'target has been disconnected';
		}
	};
	
	var readArrayValue = function(location, type)
	{
		var pos = type.indexOf('[');
		if (pos > 0)
		{
			var baseType = type.substring(0, pos).trim();
			var endPos = type.indexOf(']');
			if (endPos == type.length-1 && endPos > pos)
			{
				var size = +(type.substring(pos+1,endPos));
				if (isNaN(size) || size === 0)
				{
					throw 'invalid target array length of "' + type.substring(pos+1,endPos) + '" for type "' + baseType + '".';
				}
				else if (baseType === 'char' || baseType === 'unsigned char')
				{
					throw 'string values are not supported at this time'; 
//					return dsLite.expressions.readstring(+location, size, 0).then(onReadStringComplete);
				}
				if (dsLite)
				{
					return dsLite.memory.read(+location, 0, baseType, size).then(getArrayValueConverter(baseType));
				}
				else
				{
					throw 'target has been disconnected';
				}
			}
			else
			{
				throw 'target type "' + type + '" is not supported.';
			}
		}
		return undefined;
	};
	
	var onReadValueComplete = function(data)
	{
		var result = readArrayValue(data.value, data.type);
		
		if (result === undefined)
		{
			if (data.members.length > 0)
			{
				result = readStructValue(data.members); 
			}
			else if (data.type === 'char *' || data.type === 'unsigned char *')
			{
//				result = dsLite.expressions.readstring(+data.value, 80, 0).then(onReadStringComplete);
				throw 'string values are not supported at this time'; 
			}
			else
			{
				result = convert(data.type, data.value);
			}
		}
		
		return result;
	};
	
	gc.target.access.readValue = function(exp)
	{
		if (dsLite === undefined)
		{
			return gc.target.access.ready.then(function(dsLite) 
			{
				return gc.target.access.readValue(exp);
			});
		}
		return dsLite.expressions.evaluate(exp).then(onReadValueComplete);
	};

	var doWriteValue = function(exp, value)
	{
		if (typeof value === 'object')
		{
			var promises = []; 
			if (value instanceof Array)
			{
				// write array values one at a time using expression, since writeMem is not supported yet.
				for(var i = 0; i < value.length; i++)
				{
					promises.push(dsLite.expressions.evaluate(exp + '[' + i + ']=' + value[i]));
				}
			} 
			else
			{
				for(var fieldName in value)
				{
					if (value.hasOwnProperty(fieldName))
					{
						promises.push(dsLite.expressions.evaluate(exp + '.' + fieldName + '=' + value[fieldName]));
					}
				}
			}
			return Q.all(promises);
		}
		else
		{
			return dsLite.expressions.evaluate(exp + '=' + value);
		}
	};
	
	gc.target.access.writeValue = function(exp, value)
	{
		if (dsLite === undefined)
		{
			return gc.target.access.ready.then(function(dsLite) 
			{
				return gc.target.access.writeValue(exp, value);
			});
		}
		return doWriteValue(exp, value);
	};
	
	gc.target.access.readMemory = function(addrs, size, typeOrBytes)
	{
		size = size || 1;
		typeOrBytes = typeOrBytes || 'int';
		
		if (dsLite === undefined)
		{
			return gc.target.access.ready.then(function() 
			{
				return gc.target.access.readMemory(addrs, size, typeOrBytes);
			});
		}
		return dsLite.memory.read(+addrs, 0, type, size, page);
	};
	
	gc.target.access.disconnect = function()
	{
		console.log('target access is disconnected');
		dsLite = undefined; // clear old dsLite object.
		gc.target.access.ready = Q.Promise(function(resolve) { gc.target.access.fireReady = resolve; });
		gc.target.access.ready.then(function(dsLiteInstance) 
		{
			console.log('target access is connected');
			gc.target.access.fireReady = undefined;  // prevent multiple fires on the same resolve method.
			dsLite = dsLiteInstance;
		});
	};
	gc.target.access.disconnect();  // start in disconnected state.
}());
