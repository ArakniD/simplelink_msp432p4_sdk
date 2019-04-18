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
	 * The basic bindable object with qualifiers that derived models provide. 
	 * Qualifiers are bind names ending with ".$<qualifier>" and are model
	 * specific.  Qualifiers do not create additional bindings, but are applied
	 * to existing bindings.  For example, model.data and model.data.$readonly
	 * are the same underlying binding, and the readonly qualifier will apply to both.
	 * 
	 *	@interface
	 */
	gc.databind.IQualifierBind = function() 
	{
	};

	/**
	 * Add a qualifier to this bindable object.  All qualifier names will be converted to lower case.
	 * to minimize errors to due mis-typing names.  
	 *  
	 * @param {string} qualifier - the lower case name of the qualifier to add to this binding. 
	 */
	gc.databind.IQualifierBind.prototype.addQualifier = function(qualifier)
	{
	};
	
	/**
	 * Abstract class that provides default implementation of IBindFactory for models with bindings
	 * that have qualifiers.  A qualifier is any binding name that ends with '.$<qualifierName>'.  
	 * This class strips the terminating '.$" and qualifier name then finds the base binding for 
	 * the name only.  If the binding does not already exist it will be created.  Finally, the
	 * the qualifier is applied to the exsiting (or new) binding.  Each binding can have only one
	 * qualifier.  Examples of qualifiers include .$nonvolatile .$const .$readonly .$writeonly, but these are 
	 * all model specific qualifiers which have specific meanings for each model.  
	 *
	 * @constructor
	 * @implements gc.databind.IBindFactory
	 */
	gc.databind.QualifierBindFactoryWrapper = function(factory) 
	{
		this._factory = factory;
	};
	
	gc.databind.QualifierBindFactoryWrapper.prototype = new gc.databind.IBindFactory();
	
	gc.databind.QualifierBindFactoryWrapper.prototype.getName = function()
	{
		return this._factory.getName();
	};
	
	gc.databind.QualifierBindFactoryWrapper.prototype.getBinding = function(name)
	{
		return this._factory.getBinding(name);
	};
	
	gc.databind.QualifierBindFactoryWrapper.prototype.getAllBindings = function()
	{
		return this._factory.getAllBindings ? this._factory.getAllBindings() : {};
	};
	
	gc.databind.QualifierBindFactoryWrapper.prototype.createNewBind = function(name) 
	{
		var pos = name.lastIndexOf('.');
		if (pos > 0 && pos === name.lastIndexOf('.$'))
		{
			// found a qualifier
			var qualifier = name.substring(pos+'.$'.length).toLowerCase();
			var bindName = name.substring(0, pos);
			var binding = this._factory.getBinding(bindName);
			
			if (binding.addQualifier)
			{
				binding.addQualifier(qualifier);
			}
			else
			{
				ti_logger.error(gc.databind.name, 'Binding "' + binding.getName() +'" does not support the .$' + qualifier + 'qualifier.'); 
			}
			return binding;
		}
		else
		{
			return this._factory.createNewBind(name);
		}
	};
}());
