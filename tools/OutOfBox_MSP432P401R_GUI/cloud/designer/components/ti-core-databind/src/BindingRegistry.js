/***************************************************************************************************
 * Copyright (c) 2013-2014 Texas Instruments and others All rights reserved. This program and the
 * accompanying materials are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 * 
 * Contributors: Paul Gingrich, Dobrin Alexiev - Initial API and implementation
 **************************************************************************************************/
var gc = gc || {};
gc.databind = gc.databind || {};

(function() // closure for private static methods and data.
{
	/**
	 * Singleton class where all bindings, and binding expressions are registered. This is also
	 * where data model {gc.databind.IBindFactory} instances are registered.
	 * 
	 * @constructor
	 * @implements {gc.databind.IBindProvider}
	 */
	gc.databind.BindingRegistry = function()
	{
	};

	gc.databind.BindingRegistry.prototype = new gc.databind.IBindProvider();

	var models = {};
	var bindings = {};
	var bindCount = 0;
	var expressionParser = null;
	var defaultModelName;
	var dataBinder = new gc.databind.internal.DataBinder();

	var instance = null;
	/**
	 * Method to get the singleton DataConverter instance.
	 * 
	 * @returns {BindingRegistry} the singleton DataConverter instance.
	 */
	gc.databind.BindingRegistry.getSingleton = function()
	{
		if (instance === null)
		{
			instance = new gc.databind.BindingRegistry();
			expressionParser = new gc.databind.internal.expressionParser.ExpressionParser(instance);
		}
		return instance;
	};

	// below is a regular expression. It has three alternatives to match
	// 1. ^\s+ this matches all leading spaces
	// 2. this matches two alternatives plus the optional spaces around it
	// 2a. [^A-Za-z0-9$_ ']+ this matches anything that is not an identifier or anything in quotes.
	// The space is a terminator for the character group. Dots are not included because we can
	// remove the spaces around them.
	// 2b. '[^']' this matches quoted text and includes spaces in between quotes
	// 3. \s+$ this matches trailing spaces
	// Atlernative 1 & 3 have an empty capture group, and alternative 2's capture group excludes the
	// surrounding spaces.
	var stripSpacesMatchString = /^\s+|\s*([^A-Za-z0-9$_ ']+|'[^']*')\s*|\s+$/g;

	/**
	 * If the cache contains an object with the given name, this method will returns it. Otherwise
	 * the binding is created by first evaluating any expression then by using the registered models
	 * to create the appropriate bindings to satisfy the binding expression.
	 * 
	 * @param {string} name - the name of the bindable object.
	 * @return {gc.databind.IBind} the object if found in the cache or created, null otherwise.
	 */
	gc.databind.BindingRegistry.prototype.getBinding = function(name, hint)
	{
		if (hint === undefined)
		{
			// first time strip extra the spaces in the expression so that expressions that
			// differ only in extra spaces can be matched by string compares.
			// second time (called from expressionParser) there will be a hint provided.
			name = name.replace(stripSpacesMatchString, "$1");
		}
		var bind = bindings[name]; // lookup an existing binding for the same expression
		if (bind === undefined) // if not created yet, use expressionParser to create the new
		// binding.
		{
			// pass hint to expressionParser so skip operators already tested for in
			// sub-expressions.
			bind = expressionParser.parse(name, hint) || null;
			if (bind !== null)
			{
				bind.setName(name);
			}
			bindings[name] = bind;
			bindCount++;

		}
		return bind;
	};

	/**
	 * Register a data model with the binding registry. At least one model must be registered in
	 * order for this class to be able to create bindings.
	 * 
	 * @param {gc.databind.IBindFactory} model - the models binding factory to create new bindings.
	 * @param {boolean} [makedefault] - optional flag to make this the new default model.
	 */
	gc.databind.BindingRegistry.prototype.registerModel = function(model, makeDefault)
	{
		var name = model.getName();
		defaultModelName = defaultModelName || name; // use first registered model as default, if
		// not already specified.
		if (makeDefault)
		{
			defaultModelName = name;
		}
		models[name] = model;
	};

	/**
	 * get a data model that has already been registered.
	 * 
	 * @param {string} [name] - identifier for the model. E.g. widget. If missing returns the
	 *            default model.
	 * @returns {gc.databind.IBindFactory} - the model found or undefined if it is not registered.
	 */
	gc.databind.BindingRegistry.prototype.getModel = function(name)
	{
		name = name || defaultModelName; // use default if not specified.
		return models[name];
	};

	/**
	 * Combined Getter/Setter for the default model name. Called without parameters and it will
	 * return the name of the current default model. E.g., var name = registry.defaultModel(); Pass
	 * in a model name and it will change the default model to the one specified; for example,
	 * registry.defaultModel("widget"); Usually binding names start with the model identifier; for
	 * example, "widget.widgetid.property". However, if the default model is set to "widget", then
	 * users can omit the model identifier and use binding names like "widgetid.property" as a short
	 * cut.
	 * 
	 * @param {string} [name] - identifier for the new default model when used as a setter function.
	 *            E.g. widget.
	 * @param {gc.databind.IBindFactory} model - the name of the default model when used as getter,
	 *            or the this pointer when used as a setter.
	 */
	gc.databind.BindingRegistry.prototype.defaultModel = function(name)
	{
		if (name === undefined)
		{
			return defaultModelName;
		}
		defaultModelName = name;
		return this;
	};

	/**
	 * Method to delete and dispose of all bindings and models in the binding registry.
	 */
	gc.databind.BindingRegistry.prototype.dispose = function()
	{
		for ( var name in bindings)
		{
			if (bindings.hasOwnProperty(name))
			{
				var bind = bindings[name];
				if (bind.dispose !== undefined)
				{
					bind.dispose();
				}
			}
		}
		bindings = {};

		for (name in models)
		{
			if (models.hasOwnProperty(name))
			{
				var model = models[name];
				if (model.dispose !== undefined)
				{
					model.dispose();
				}
			}
		}
		models = {};
	};

	/**
	 * <p>
	 * Method to bind together a target and a model binding.
	 * </p>
	 * <p>
	 * The difference between the target binding and the model binding is subtle. The modelBinding
	 * contains the initial value. Otherwise there is no distinction between model and target. Once
	 * the bindings are bound together, their value and status will forever be the same. If either
	 * value of status changes on one binding, the other will be updated to reflect the change. This
	 * is typically used to keep widget and model data in sync.
	 * </p>
	 * <p>
	 * This method returns a binder object that can be used to control the enabled disabled state of
	 * this two-way data binding between target and model bindings.
	 * </p>
	 * 
	 * @param {string|object} targetBinding - name or expression for the target binding.
	 * @param {string|object} modelBinding - name or expression for the model binding.
	 * @param {function} [getter] - getter/preprocessing for a computed value
	 * @param {function} [setter] - setter/postprocessing for a computed value
	 * @returns {IDataBinder} - interface to control the binding created between the the target and
	 *          model bindings.
	 */
	gc.databind.BindingRegistry.prototype.bind = function(targetBinding, modelBinding, getter, setter)
	{
		var targetBind = typeof targetBinding === 'object' ? 
				createBindingCollection(targetBinding) : this.getBinding(targetBinding);
		
		var modelBind = typeof modelBinding === 'object' ? 
				createBindingCollection(modelBinding) : this.getBinding(modelBinding);
		return gc.databind.internal.DataBinder.bind(targetBind, modelBind, getter, setter);
	};

	var createBindingCollection = function(bindings)
	{
		var result = {};
		for ( var bindName in bindings)
		{
			if (bindings.hasOwnProperty(bindName))
			{
				var binding = gc.databind.registry.getBinding(bindings[bindName]);
				if (binding !== null)
				{
					result[bindName] = binding;
				}
				else
				{
					ti_logger.error(gc.databind.name, 'Binding "' + bindName + '" could not be found.');
				}
			}
		}
		return new gc.databind.CollectionBindValue(result);
	};

	gc.databind.BindingRegistry.prototype.getBindingCount = function()
	{
		return bindCount;
	};

	gc.databind.BindingRegistry.prototype.loadBindingsFromFile = function(jsonFile)
	{
		var promise = Q.Promise(function(resolve, reject)
		{
			var xmlHttp = new XMLHttpRequest();
			xmlHttp.onload = function()
			{
				if (xmlHttp.status === 200 || (xmlHttp.status === 0 && xmlHttp.responseText))
				{
					resolve(JSON.parse(xmlHttp.responseText));
				}
				else
				{
					reject(new Error("Can't read json file: " + jsonFile + ".  Status Code = " + xmlHttp.status));
				}
			};

			xmlHttp.open("GET", jsonFile, true);
			xmlHttp.send(null);
		}).then(function(data)
		{
			var bindingProvider = gc.databind.registry;
			for ( var prop in data.widgetBindings)
			{
				if (data.widgetBindings.hasOwnProperty(prop))
				{
					var wb = data.widgetBindings[prop];
					var i = 0;

					// set the default type for the widget binding
					var widgetBind = bindingProvider.getBinding('widget.' + wb.widgetId + '.' + wb.propertyName);
					if (wb.options && wb.options.dataType)
					{
						var defaultType = wb.options.dataType.toLowerCase();
						if (defaultType === 'long' || defaultType === 'short' || defaultType === "int" ||
						        defaultType === 'double' || defaultType === 'float')
						{
							defaultType = 'number';
						}
						widgetBind.setDefaultType(defaultType);
					}

					// Binding records with no widgetId are used to initialize backplane bindings.
					if (!(wb.widgetId) && wb.serverBindName && wb.options &&
					        (typeof wb.options.defaultValue !== 'undefined'))
					{
						var bind = bindingProvider.getBinding(wb.serverBindName);
						bind.setValue(wb.options.defaultValue);
					}
					else
					{
						bindingProvider.bind('widget.' + wb.widgetId + '.' + wb.propertyName, wb.serverBindName);
					}
				}
			}
		}).fail(function(error) 
		{
			ti_logger.error(gc.databind.name, error);
		});

		return promise;
	};

	var matchIDRegEx = /\s+id="([^"]+)"/;
	gc.databind.BindingRegistry.prototype.parseBindingsFromGist = function(modelName, arrayOfLines, modelID)
	{
		var re = new RegExp('\\s+(\\w+)\\s*=\\s*"\\s*{{\\s*\\$\\.' + modelName + '\\.([a-zA-Z0-9_\\.$]+)', 'g');
		var bindingsData = [];
		for (var i = 0; i < arrayOfLines.length; i++)
		{
			var pos = arrayOfLines[i].indexOf('$.' + modelName + '.');
			if (pos >= 0)
			{
				// parse binding expression and property name
				var widgetID = arrayOfLines[i].match(matchIDRegEx);
				if (widgetID)
				{
					widgetID = widgetID[1];
					var match = re.exec(arrayOfLines[i]);
					while (match)
					{
						var bindingExpression = match[2];
						var propertyName = match[1];

						bindingsData.push({
						    propertyName : propertyName,
						    serverBindName : modelID + '.' + bindingExpression,
						    widgetId : widgetID
						});

						match = re.exec(arrayOfLines[i]);
					}
				}
			}
		}
		return bindingsData;
	};

	gc.databind.BindingRegistry.prototype.saveBindingsToFile = function(filename, bindings)
	{
		var jsonObject = {
			widgetBindings : bindings
		};
		var jsonData = JSON.stringify(jsonObject, null, 4);

		var fs = require('fs');
		fs.writeFileSync(filename, jsonData);
	};

	gc.databind.registry = gc.databind.BindingRegistry.getSingleton();

}());