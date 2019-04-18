/*****************************************************************
 * Copyright (c) 2013-2014 Texas Instruments and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Paul Gingrich, Dobrin Alexiev - Initial API and implementation
 *****************************************************************/
var gc = gc || {};
gc.databind = gc.databind || {};
gc.databind.internal = gc.databind.internal || {};

(function() 
{
	var BindingListener = function(srcBinding, destBinding, computeValue)
	{
		this.srcBinding = srcBinding;
		this.destBinding = destBinding;
		this.computeValue = computeValue;
	};
	
	BindingListener.prototype = new gc.databind.BindListenerAdapter();
	
	BindingListener.prototype.onValueChanged = function(prevValue, nextValue, progress)
	{
		if (this.srcBinding.isStale())
		{
			// defer passing the data along until the value has fully changed.
			this.srcBinding.addStaleListener(this);
		}
		else
		{
			var newValue = this.srcBinding.getValue();
			var oldValue = this.destBinding.getValue();
			
			var srcType = this.srcBinding.getType();
			if (this.computeValue)
			{
				newValue = this.computeValue(newValue);
				srcType = undefined;
			}
			newValue = gc.databind.DataConverter.convert(newValue, srcType, this.destBinding.getType());
			this.destBinding.setValue(newValue, progress);
		}
	};
	
	BindingListener.prototype.onStatusChanged = function(status)
	{
		this.destBinding.setStatus(status);
	};
	
	BindingListener.prototype.onStaleChanged = function()
	{
		this.srcBinding.removeStaleListener(this);
		this.onValueChanged();  // force the value to be synced 
	};
	
	BindingListener.prototype.enable = function()
	{
		this.srcBinding.addChangedListener(this);
		this.srcBinding.addStatusListener(this);
	};
	
	BindingListener.prototype.disable = function()
	{
		this.srcBinding.removeChangedListener(this);
		this.srcBinding.removeStatusListener(this);
	};
	
	var nullListener = new gc.databind.BindListenerAdapter();
	nullListener.enable = function() {};
	nullListener.disable = function() {};
	
	var Binder = function(targetBinding, modelBinding, getter, setter)
	{
		// supprt for readOnly bindings, don't write values.
		if (getter && !setter)
		{
			this.targetListener = nullListener;
			this.modelListener = new BindingListener(modelBinding, targetBinding, getter);
		}
		else if (setter && !getter)
		{
			// switch model and target, so model gets initialized from the target.
			this.targetListener = nullListener;
			this.modelListener = new BindingListener(targetBinding, modelBinding, setter);
		}
		else // two-way binding support (with both getter or setter, or neither getter or setter (no computation)
		{
			this.targetListener = new BindingListener(targetBinding, modelBinding, setter);
			this.modelListener = new BindingListener(modelBinding, targetBinding, getter);
		}
	};
	
	Binder.prototype = new gc.databind.IDataBinder();
	
	Binder.prototype.enable = function(enable)
	{
		if (enable === undefined)
		{
			return this._enabled;
		}
		else if (this._enabled != enable)
		{
			this._enabled = enable;
			
			if (enable)
			{
				this.targetListener.enable();
				this.modelListener.enable();

				// force model to sync the target value in case it changed between disable() and subsequent enable() calls.
				this.modelListener.onValueChanged();
			}
			else
			{
				this.targetListener.disable();
				this.modelListener.disable();
			}
			return this;
		}
	};

	gc.databind.internal.DataBinder = function()
	{
	};

	gc.databind.internal.DataBinder.bind = function(targetBinding, modelBinding, getter, setter)
	{
		if ((targetBinding !== null && modelBinding !== null) &&
			(targetBinding instanceof gc.databind.IBindValue || targetBinding instanceof gc.databind.AbstractBindValue) &&
			(modelBinding instanceof gc.databind.IBindValue || modelBinding instanceof gc.databind.AbstractBindValue))
		{
			var binder = new Binder(targetBinding, modelBinding, getter, setter);
			binder.enable(true);
			
			return binder;
		}
		ti_logger.error(gc.databind.name, "Cannot bind target and model bindings together because one of them is not an IBindValue.");
		return null;
	};
}());