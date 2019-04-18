var gc = gc || {};
gc.nav = gc.nav || {};

(function() // closure for private static methods and data.
{
	gc.nav.IRunable = function()
	{
	};
	gc.nav.IRunable.prototype.run = function()
	{
	};
	
	gc.nav.StateListener = function(node)
	{
		this.node = node;
	};
	gc.nav.StateListener.prototype.onStateChanged = function(enabled)
	{
		this.node.disabled = !enabled;
		this.node.className = enabled ? "" : "unenabled";
	};
	
	gc.nav.showMenu = function(id)
	{
		var element = document.getElementById(id);
		if (element)
		{
			element.style.display = "";
		}
	};

	gc.nav.hideMenu = function(id)
	{
		var element = document.getElementById(id);
		if (element)
		{
			element.style.display = "none";
		}
	};

	var actionRegistry = 
	{
	};
	
	gc.nav.registerAction = function(id, runable)
	{
		if (typeof runable === 'function')
		{
			runable = { run: runable };
		}
		if (runable.run)
		{
			actionRegistry[id] = runable;

			// fire listener to bring it up to current state
			fireStateChangedListeners(id, gc.nav.isEnabled(id));
		}
		else
		{
			console.log('gc.nav.registerAction failed because the runable does not have a run() method');
		}
	};
	
	var stateListeners = {};
	gc.nav.addStateListener = function(id, listener)
	{
		// ensure it is not already in the list
		gc.nav.removeStateListener(id, listener);
		
		var listeners = stateListeners[id];
		if (listeners === undefined)
		{
			listeners = [];
			stateListeners[id] = listeners;
		}
		// add to the list
		listeners.push(listener);
		
		// fire listener to bring it up to current state
		fireStateChangedListeners(id, gc.nav.isEnabled(id));
	};
	
	gc.nav.removeStateListener = function(id, listener)
	{
		var listeners = stateListeners[id];
		if (listeners)
		{
			for(var i = listeners.length; i-- > 0; )
			{
				if (listeners[i] == listener)
				{
					listeners.splice(i, 1);
				}
			}
		}
	};
	
	var fireStateChangedListeners = function(id, enabled)
	{
		var listeners = stateListeners[id];
		if (listeners)
		{
			for(var i = 0; i < listeners.length; i++)
			{
				listeners[i].onStateChanged(enabled);
			}
		}
	};
	
	gc.nav.onClick = function(id)
	{
		var action = actionRegistry[id];
		if (action && gc.nav.isEnabled(id))
		{
			return action.run() || true;
		}
		return false;
	};
	
	var actionStates = {};
	gc.nav.disableAction = function(id)
	{
		if (this.isEnabled(id))
		{
			actionStates[id] = false;
			fireStateChangedListeners(id, false);
		}
	};
	
	gc.nav.enableAction = function(id)
	{
		if (!this.isEnabled(id))
		{
			actionStates[id] = true;
			fireStateChangedListeners(id, true);
		}
	};
	
	gc.nav.isEnabled = function(id)
	{
		var action = actionRegistry[id];
		if (action)
		{
			var enabled = actionStates[id];
			return enabled === undefined || enabled;
		}
		return false;  // return disabled for missing actions.
	};
	
    document.dispatchEvent(new CustomEvent("gc-nav-ready", { "detail": "Navigation actions registry is available" }));
	
}());

gc.nav.ready = gc.nav.ready || Q.Promise(function(resolve) { gc.nav.fireReady = resolve; });
gc.nav.fireReady();

