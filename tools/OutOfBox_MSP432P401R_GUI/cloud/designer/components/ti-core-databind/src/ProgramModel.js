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

(function() // closure for private static methods and data.
{
	var PROGRAM_MODEL = "pm";
	var DS_REFRESH_INTERVAL = "$refresh_interval";
	
	gc.databind.ProgramModel = function() 
	{
	};
	
	gc.databind.ProgramModel.prototype = new gc.databind.AbstractBindFactory(PROGRAM_MODEL);
	
	gc.databind.ProgramModel.prototype.createNewBind = function(uri)
	{
		if( uri === DS_REFRESH_INTERVAL)
		{
			return new gc.databind.RefreshIntervalBindValue();
		}
		
		if (!this._refreshIntervalProvider)
		{
			this._refreshIntervalProvider = this.getBinding(DS_REFRESH_INTERVAL);
		}

		return new gc.databind.internal.pm.DSEvalBind(uri, this._refreshIntervalProvider);
	};
	
	// register program model with bindingRegistry
	gc.databind.registry.registerModel(
			new gc.databind.QualifierBindFactoryWrapper(
			new gc.databind.StorageProviderBindFactoryWrapper(
			new gc.databind.ProgramModel())), true);
}());






