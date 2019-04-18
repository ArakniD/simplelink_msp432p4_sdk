/*****************************************************************
 * Copyright (c) 2013-15 Texas Instruments and others
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
gc.databind.internal.pm = gc.databind.internal.pm || {};

(function() 
{
	gc.databind.internal.pm.DSEvalBind = function(uri, refreshIntervalProvider) 
	{
		gc.databind.AbstractAsyncBindValue.call(this);
		
		var that = this;
		
		that.uri = uri;
		
		that._onFailure = function(err)
		{
			that.reportErrorStatus(err.message);
			return that.fCachedValue;  // don't record a new value, keep the same value as before. 
		};
		
		// add refresh listener and create dispose() method to remove listener when done.
		refreshIntervalProvider.addRefreshListener(that);
		that.dispose = function()
		{
			refreshIntervalProvider.removeRefreshListener(that);
		};
	}; 
	
	gc.databind.internal.pm.DSEvalBind.prototype = new gc.databind.AbstractAsyncBindValue('number');

	gc.databind.internal.pm.DSEvalBind.prototype.writeValue = function(callback)
	{
		gc.target.access.writeValue(this.uri, this.fCachedValue).then(callback).fail(this._onFailure); 
	};
	
	gc.databind.internal.pm.DSEvalBind.prototype.readValue = function(callback)
	{
		gc.target.access.readValue(this.uri).then(callback).fail(this._onFailure); 
	};
	
	gc.databind.internal.pm.DSEvalBind.prototype.reportErrorStatus = function(dsErrorMessage)
	{
		var status = null;
		if (dsErrorMessage && dsErrorMessage.length > 0)
		{
			if (dsErrorMessage.indexOf('identifier not found') >= 0)
			{
				dsErrorMessage = 'Missing identifier: ' + this.uri + ", it cannot be found in the target program's symbols.";
			}
			status = gc.databind.AbstractStatus.createErrorStatus(dsErrorMessage, 'target');
		}
		this.reportCriticalError(status);
	};
	
}());


