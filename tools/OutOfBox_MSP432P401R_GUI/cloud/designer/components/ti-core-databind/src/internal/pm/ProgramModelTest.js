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
gc.databind = gc.databind || {};
gc.databind.internal = gc.databind.internal || {};

(function() 
{
	var DSStub = function() 
	{
	};
	
    var testCount = 0;
    var errorCount = 0;
	var testExpression = "";

	DSStub.prototype.expressions = 
	{ 
		data : {}, 
		evaluate : function(exp)
		{
			var pos = exp.indexOf('=');
			var value;
			
			if (pos > 0)
			{
				// assignment operation
				var lvalue = exp.substring(0, pos);
				value = exp.substring(pos+1);
				
				this.data[lvalue] = value;
			}
			else
			{
				value = this.data[exp];
			}
		
			return Q.Promise(function(resolve, reject) 
			{
				// set random delay between 1 and 10ms.
				setTimeout(function() 
				{ 
					resolve(value, 'number'); 
				}, 0);
		
			});
		}
	};
	var ProgramModelTest = function()
	{
	}; 
	
	var dsStub = new DSStub();
	
	// fire target access ready with the stub for ds
	gc.target.access.fireReady(dsStub);
	
	var fail = function(message)
	{
        errorCount++;
		console.log("Failed test " + testCount + ' "' + testExpression + '" : ' + message);
	};
	
	var assertNull = function(value)
	{
		if (value !== null && value !== undefined)
		{
			fail("non null value found.  Expected it to be null");
		}
	};
	
	var assertNotNull = function(value)
	{
		if (value === null || value === undefined)
		{
			fail("null value found.  Expected it to be not null");
		}
	};
	
	var assertTrue = function(value)
	{
		if (value !== true)
		{
			fail("assertion failed. Expected true result but it was false instead.");
		}
	};
	
	var assertEquals = function(expected, actual)
	{
		if (actual != expected)
		{
			fail("got '" + actual +"' but was expecting '" + expected +"'");
		}
	};
	
	var setValue = function(binding, newValue)
	{
		return function() 
		{
			return Q.Promise(function(resolve, reject) 
			{
				testCount++;
				testExpression = "set value of " + binding.getName() + " to " + newValue;
				
				var progress = new gc.databind.ProgressCounter(function() 
				{
					// finished operation
					assertEquals(dsStub.expressions.data[binding.getName()], newValue); 
					resolve();
				});
				binding.setValue(newValue, progress);
				progress.done();
			});
		};
	};
	
	var changeValue = function(binding, newValue, init)
	{
		return function() 
		{
			return Q.Promise(function(resolve, reject) 
			{
				testCount++;
				testExpression = "change value of " + binding.getName() + " to " + newValue;
				
				var changedListener = 
				{
					onValueChanged : function() 
					{
						assertEquals(binding.getValue(), newValue);
						binding.removeChangedListener(changedListener);
						resolve();
					}
				};
				
				binding.addChangedListener(changedListener);
				setTimeout(function() 
				{
					dsStub.expressions.data[binding.getName()] = newValue;
				}, 0);
			});
		};
	};
	
	var verifyChangeNotification = function(binding, expectedValue)
	{
		return function() 
		{
			return Q.Promise(function(resolve, reject) 
			{
				testCount++;
				testExpression = "change notification for " + binding.getName();
				
				var changedListener = 
				{
					onValueChanged : function() 
					{
						assertEquals(binding.getValue(), expectedValue);
						binding.removeChangedListener(changedListener);
						resolve();
					}
				};
				binding.addChangedListener(changedListener);
			});
		};
	};
	
	ProgramModelTest.prototype.test = function()
	{
		var promise = Promise.resolve();

		// set refreshRate
		var bindings = gc.databind.registry;
		
		var refreshBind = bindings.getBinding('pm.$refresh_interval');
		refreshBind.setValue(500);
		
		assertEquals(refreshBind.getValue(), 500);
		
		// get binding - ensure it is stale until value is set.
		var X = 'x';
		dsStub.expressions.data[X] = '56';
		var bind = bindings.getBinding(X);
		assertTrue(bind.isStale());
		
		promise = promise.then(verifyChangeNotification(bind, '56'));
		promise = promise.then(setValue(bind, 'testing'));
		promise = promise.then(changeValue(bind, 'ok'));
		promise = promise.then(function() 
		{ 
            if (errorCount === 0)
            {
                console.log("testing complete: " + testCount + " test cases passed.");
            }
            else
            {
                console.error("testing complete: " + errorCount + " test cases failed out of " + testCount);
            }
		});
	};
	
	return new ProgramModelTest();
	
}().test());