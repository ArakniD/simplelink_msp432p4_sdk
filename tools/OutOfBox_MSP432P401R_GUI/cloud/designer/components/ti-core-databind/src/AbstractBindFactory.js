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
	/**
	 * Abstract class that provides default implementation of IBindFactory.  This class
	 * implements the getName() method for IBindFactory.
	 *
	 * @constructor
	 * @implments gc.databind.IBindFactory
	 * @param {string} name - uniquely identifiable name for this bind factory.
	 */
	gc.databind.AbstractBindFactory = function(name) 
	{
		this._id = name;
	};
	
	gc.databind.AbstractBindFactory.prototype = new gc.databind.IBindFactory();

	/** @inheritdoc IBindFactory#getName */
	gc.databind.AbstractBindFactory.prototype.getName = function()
	{
		return this._id;
	};
	
	/**
	 * Helper method for finding and or creating bindings on this model.  This method simply wraps a call
	 * to gc.databind.registry.getBinding() by prepending the model name to the binding name. 
	 * For example, getBinding('volume') on model 'audio' would
	 * turn into a call to gc.databind.registry.getBinding('audio.volume');    
	 * 
	 * @param {String} name - uniquely identifying the bindable object within the model.  
	 * @return {gc.databind.IBind} - the existing or newly created bindable object, or null if this name is not supported.
	 */
	gc.databind.AbstractBindFactory.prototype.getBinding = function(name)
	{
		return gc.databind.registry.getBinding(this._id + '.' + name);
	};

}());
