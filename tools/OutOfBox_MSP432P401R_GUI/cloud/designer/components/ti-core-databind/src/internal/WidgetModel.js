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

(function() {
	
	var WidgetBindValue = function(widget, initialValue) 
	{
		gc.databind.AbstractBindValue.call(this);
		this.fCachedValue = initialValue;
		this.widgetId = widget.id;
		this._widget = widget;
	};
	
	WidgetBindValue.prototype = new gc.databind.AbstractBindValue();
	
	WidgetBindValue.prototype.onValueChanged = function(oldValue, newValue, progress)
	{
		this.value = newValue;
	};
	
	WidgetBindValue.prototype.onStatusChanged = function(oldStatus, newStatus) 
	{
		if (newStatus && gc.widget)
		{
			var newMessage = newStatus.getMessage();
			
			var type = newStatus.getType();
			if (type ===  gc.databind.StatusType.WARNING)
			{
				type = gc.widget.StatusIndicatorType.WARNING;
			}
			else
			{
				type = gc.widget.StatusIndicatorType.ERROR;
			}
			gc.widget.StatusIndicator.Factory.get(this._widget).addMessage(newMessage, type);
		}
		else if (oldStatus && gc.widget)
		{
			gc.widget.StatusIndicator.Factory.get(this._widget).removeMessage(oldStatus.getMessage());
		}
	};
	
	var EditOperation = function(bind, oldValue, newValue, time)
	{
		this.newValue = newValue;
		this.oldValue = oldValue;
		this.bind = bind;
		this.time = time;
	};
	
	EditOperation.prototype.undo = function()
	{
		this.bind.setValue(this.oldValue);
	};
	
	EditOperation.prototype.redo = function()
	{
		this.bind.setValue(this.newValue);
	};
	
	EditOperation.prototype.toString = function()
	{
		return "edit";
	};

	var doUserEditOperation = function(bind, newValue)
	{
		var oldValue = bind.getValue();
		if (oldValue != newValue)
		{
			if (gc.history && gc.history.push)
			{
				var now = Date.now();
				var lastOperation = gc.history.getLastUndoOperation();
				if (lastOperation instanceof EditOperation && now - lastOperation.time < 250 && lastOperation.bind === bind &&
						lastOperation.oldValue !== newValue)  // make sure it's also different from original value; e.g., checkbox toggled quickly.  
				{
					// not enough time has elapsed, so just modify the top of history stack with new value
					lastOperation.newValue = newValue;
					lastOperation.time = now;  
					lastOperation.redo(); // perform action now.
				}
				else
				{
					if (oldValue !== undefined && newValue !== undefined)
					{
						var operation = new EditOperation(bind, oldValue, newValue, now);
						gc.history.push(operation);
						operation.redo();
					}
					else
					{
						bind.setValue(newValue);
					}
				}
			}
			else
			{
				bind.setValue(newValue);
			}
		}
	};
	
	var WidgetModel = function()
	{
	};

	WidgetModel.prototype = new gc.databind.AbstractBindFactory('widget');
	
	WidgetModel.prototype.createNewBind = function(name)
	{
		var bind = null;
		var pos = name.lastIndexOf('.');
		if (pos > 0)
		{
			var widgetName = name.substring(0, pos);
			
			var widgetProperty = name.substring(pos+1);

			var widget = document.querySelector('#' + widgetName);
			if (widget)
			{
				bind = new WidgetBindValue(widget, widget[widgetProperty]);
				var observer = new PathObserver(bind, 'value');
				observer.setValue = function(newValue) 
				{
					doUserEditOperation(bind, newValue);
					PathObserver.prototype.setValue.call(this, newValue);
				};
				
				widget.bind(widgetProperty.toLowerCase(), observer);
			}
			else
			{
				ti_logger.error(gc.databind.name, "Failed to find widget #" + widgetName);
			}
		}
		return bind;
	};

	gc.databind.registry.registerModel(new WidgetModel());
	
}());
