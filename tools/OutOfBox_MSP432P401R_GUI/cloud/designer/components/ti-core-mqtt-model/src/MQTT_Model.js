/*****************************************************************
 * Copyright (c) 2015 Texas Instruments and others
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Raymond Pang - Initial API and implementation
 *****************************************************************/
var gc = gc || {};
gc.databind = gc.databind || {};


(function() // closure for private static methods and data.
{
    var MQTTBind = function(topic, fieldName)
    {
		gc.databind.AbstractBindValue.call(this);
		
		this.setStale(true);
		
		var that = this;
		var topicChangeListener = new gc.databind.IChangedListener();
		topicChangeListener.onValueChanged = function(oldValue, newValue)
		{
			// topic has changed
			var data = JSON.parse(newValue);
			var value = data[fieldName];
			if ( value )
			{
				that.setValue(value);		
			}
		};
		topic.addChangedListener(topicChangeListener);
    }; 
    
    MQTTBind.prototype = new gc.databind.AbstractBindValue();
    
    MQTTBind.prototype.onValueChanged = function()
    {
    	this.setStale(false);
    };
    
    var MQTTTopicBind = function(name)
    {
		gc.databind.AbstractBindValue.call(this);
		
    	this.setStale(true);
    };
    
    MQTTTopicBind.prototype = new gc.databind.AbstractBindValue();

    MQTTTopicBind.prototype.onValueChanged = function()
    {
    	this.setStale(false);
    };
    
	gc.databind.MQTT_Model = function() 
	{
	};
	
	gc.databind.MQTT_Model.prototype = new gc.databind.AbstractBindFactory('mqtt');
	
	gc.databind.MQTT_Model.prototype.createNewBind = function(name)
	{
		// topicName.valueName
		
		var pos = name.indexOf('.');
		if (pos > 0)
		{
			var topicBind = this.getBinding(name.substring(0, pos));
			
		//	alert( "create T (" + topicBind + ", " + name.substring(pos+1) + ")");
			return new MQTTBind(topicBind, name.substring(pos+1));
		}
		else
		{
		//	alert( "create MQTTTopicBind (" + name + ")");
			return new MQTTTopicBind(name);
		}
	};
	
 	// register program model with bindingRegistry
 	gc.databind.registry.registerModel(new gc.databind.MQTT_Model(), true);

}());






