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

(function() 
{
	gc.databind.StorageProviderBindFactoryWrapper = function(factory)
	{
		this._bindings = {};
		if (factory)
		{
			this._factory = factory;
			var name = factory.getName();
			
			var that = this;

			gc.File = gc.File || {};
			gc.File.ready = gc.File.ready || Q.Promise(function(resolve) { gc.File.fireReady = resolve; });
			gc.File.ready.then(function() 
			{
				// register data provider to load and store data from the factory provided when called upon.
				gc.File.addDataProvider(name, that);
			});
		}
	};
	
	gc.databind.StorageProviderBindFactoryWrapper.prototype = new gc.databind.IBindFactory();
	
	gc.databind.StorageProviderBindFactoryWrapper.prototype.getName = function()
	{
		return this._factory.getName();
	};
	
	gc.databind.StorageProviderBindFactoryWrapper.prototype.createNewBind = function(name) 
	{
		var bind = this._factory.createNewBind(name);
		if (bind)
		{
			this._bindings[name] = bind;
		}
		return bind;
	};
	
	gc.databind.StorageProviderBindFactoryWrapper.prototype.getBinding = function(name)
	{
		return this._factory.getBinding(name);
	};
	
	gc.databind.StorageProviderBindFactoryWrapper.prototype.getAllBindings = function()
	{
		return this._bindings;
	};
	
	gc.databind.StorageProviderBindFactoryWrapper.prototype.readData = function()
	{
		var data = {};
		for(var bindName in this._bindings)
		{
			if (this._bindings.hasOwnProperty(bindName))
			{
				var bind = this._bindings[bindName];
				if (!bind.isReadOnly() && bind.excludeFromStorageProviderData === undefined)
				{
					data[bindName] = this._bindings[bindName].getValue();
				}
			}
		}
		return data;
	};
	
	gc.databind.StorageProviderBindFactoryWrapper.prototype.writeData = function(data)
	{
		for(var bindName in data)
		{
			if (data.hasOwnProperty(bindName))
			{
				var bind = this._bindings[bindName];
				if (bind)
				{
					bind.setValue(data[bindName]);
				}
			}
		}
	};
}());
