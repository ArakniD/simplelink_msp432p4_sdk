"use strict";

/*global chrome*/
/*global console*/
/*global InstallTrigger*/
/*global process*/
/*global require*/
/*global Q*/
// use jsdoc agent.js to generate documentation
/**
 *
 * Main access point for utilizing cloud agent API's.
 *
 * @namespace
 */
var TICloudAgent = TICloudAgent || {};

( function() {

	// base error
	TICloudAgent.Error = function( name, msg ) {
		if ( name )
			this.name = name;
		else
			this.name = "TICloudAgent.Error";

		if ( msg )
			this.msg = msg;
		else
			this.msg = "ERROR";
	};
	TICloudAgent.Error.prototype = new Error( "TICloudAgent.Error", "ERROR" );

	TICloudAgent.AlreadyInitialized = function() {};

	TICloudAgent.AlreadyInitialized.prototype = new TICloudAgent.Error( "TICloudAgent.AlreadyInitialized", "AGENT_ALREADY_INITIALIZED" );

	// Base for installation related errors.
	TICloudAgent.InstallError = function( name, msg ) {
		TICloudAgent.Error.call( this, name, msg );
	};
	TICloudAgent.InstallError.prototype = new TICloudAgent.Error();

	// Base error type that may require installation
	TICloudAgent.InvalidAgentVersion = function() {
		this.msg = "Installed version is out of date";
	};
	TICloudAgent.InvalidAgentVersion.prototype = new TICloudAgent.InstallError( "TICloudAgent.InvalidAgentVersion", "AGENT_INVALID_VERSION" );

	TICloudAgent.InvalidExtensionVersion = function() {
		this.msg = "Installed browser extension version is out of date";
	};
	TICloudAgent.InvalidExtensionVersion.prototype = new TICloudAgent.InstallError( "TICloudAgent.InvalidExtensionVersion", "EXTENSION_INVALID_VERSION" );

	TICloudAgent.MissingExtension = function() {};
	TICloudAgent.MissingExtension.prototype = new TICloudAgent.InstallError( "TICloudAgent.MissingExtension", "MISSING_EXTENTSION" );

	TICloudAgent.AgentNotStarted = function( msg ) {
		this.msg = msg;
	};
	TICloudAgent.AgentNotStarted.prototype = new TICloudAgent.InstallError( "TICloudAgent.AgentNotStarted", "AGENT_NOT_STARTED" );


	// OS Enum
	TICloudAgent.OS = {
		WIN: "win",
		LINUX: "linux",
		OSX: "osx"
	};

	// Browser Enum
	TICloudAgent.BROWSER = {
		CHROME: "chrome",
		SAFARI: "safari",
		FIREFOX: "firefox",
		IE: "ie"
	};

	var dynamicLink = function( url, newWindow ) {
		var appendChild = TICloudAgent.getBrowser() === TICloudAgent.BROWSER.FIREFOX;
		var a = document.createElement( "a" );
		a.href = url;

		if ( newWindow ) {
			a.target = "_blank";
		}

		if ( appendChild ) {
			document.body.appendChild( a );
		}
		a.click();
	};



	// utility function to figure out the browser
	var getBrowser = function() {
		var browser = TICloudAgent.BROWSER.CHROME;
		// chrome claims to be safari and chrome.. so special care is needed
		if ( navigator.userAgent.indexOf( "Safari" ) !== -1 && navigator.userAgent.indexOf( "Chrome" ) == -1 ) {
			browser = TICloudAgent.BROWSER.SAFARI;
		} else if ( navigator.userAgent.indexOf( "Firefox" ) !== -1 ) {
			browser = TICloudAgent.BROWSER.FIREFOX;
		} else if ( !!navigator.userAgent.match( /Trident.*rv\:11\./ ) ) {
			browser = TICloudAgent.BROWSER.IE;
		}

		return browser;
	};

	TICloudAgent.getBrowser = getBrowser;

	// utility function to figure out OS
	var getOS = function() {
		// default to linux because it is not always possible to tell it from
		// the
		// appVersion
		var os = TICloudAgent.OS.LINUX;
		if ( navigator.appVersion.indexOf( "Mac" ) != -1 )
			os = TICloudAgent.OS.OSX;
		if ( navigator.appVersion.indexOf( "Win" ) != -1 )
			os = TICloudAgent.OS.WIN;
		return os;
	};

	var getOSFolderName = function() {
		var osFolderName = "win32";
		var os = getOS();
		switch ( os ) {
			case TICloudAgent.OS.OSX:
				osFolderName = "mac";
				break;
			case TICloudAgent.OS.LINUX:
				osFolderName = "linux";
				break;
		}
		return osFolderName;
	};

	var hostInfo = function() {

		// Host and port of the server
		var protocol = window.location.protocol;
		var host = window.location.hostname;
		var port = window.location.port ? ":" + window.location.port : "";

		return {

			tiCloudAgentServer: function() {
				return protocol + "//" + host + port + "/ticloudagent";
			},
			tiCloudAgentInstallerServer: function() {
				return "http://" + host + port + "/ticloudagentinstaller";
			},
			isProduction: function() {
				return ( host === "dev.ti.com" );
			}
		};

	}();

	// chrome ext specific
	var chromeExt = function() {

		var ID = "pfillhniocmjcapelhjcianojmoidjdk";

		var port = null;

		var lastRegLisenter = null;

		return {
			init: function() {
				// connect to app if possible
				if ( chrome.runtime && chrome.runtime.connect ) {
					port = chrome.runtime.connect( ID );
					port.onDisconnect.addListener( function() {
						port = null;
					} );
					if ( port ) {
						// disconnect before exiting to clean up extension
						window.addEventListener( "unload", function() {
							port.disconnect();
						} );
					}
				}
			},
			postMessage: function( msgObj ) {
				port.postMessage( msgObj );
			},
			regOnMessage: function( listener ) {
				lastRegLisenter = listener;
				port.onMessage.addListener( lastRegLisenter );
			},
			removeLastOnMessage: function() {
				port.onMessage.removeListener( lastRegLisenter );
			},
			install: function( success, failure ) {
				var url = "https://chrome.google.com/webstore/detail/" + ID;
				//scheme is very delicate make sure to test all the use cases if you make changes
				if ( hostInfo.isProduction() ) {
					try {
						// inline- install only works for production
						chrome.webstore.install( url, success, failure );
					} catch ( e ) {
						dynamicLink( url, true ); // do non inline install instead
					}
				} else {
					dynamicLink( url, true ); // do non inline install instead
				}
			}
		};

	}();

	var firefoxExt = function() { // firefox and safari

		var TO_APP_MSG_STR = "ti-cloud-agent-msg-app";
		var TO_EXT_MSG_STR = "ti-cloud-agent-msg-ext";
		var lastRegLisenter = null;

		return {
			init: function() {},
			postMessage: function( msgObj ) {
				var event = window.document.createEvent( "CustomEvent" );
				event.initCustomEvent( TO_EXT_MSG_STR, true, true, msgObj );
				window.dispatchEvent( event );
			},
			regOnMessage: function( listener ) {
				lastRegLisenter = function( event ) {
					listener( event.detail );
				};
				window.addEventListener( TO_APP_MSG_STR, lastRegLisenter, false );
			},
			removeLastOnMessage: function() {
				window.removeEventListener( TO_APP_MSG_STR, lastRegLisenter, false );
			},
			install: function() {
				var extUrl = hostInfo.tiCloudAgentServer() + "/getExtension?browser=" + getBrowser();
				if ( getBrowser() === TICloudAgent.BROWSER.SAFARI ) {
					dynamicLink( extUrl );
				} else {

					var params = {
						"TICloudAgentXPI": {
							URL: extUrl,
							Hash: "sha1:6695cbbff19e0eba41fdfa205f990738c043326e",
							toString: function() {
								return this.URL;
							}
						}
					};

					InstallTrigger.install( params );
				}
			}
		};

	}();

	// for firefox and safari use the firefoxExt object.
	var browserExt = firefoxExt;
	if ( getBrowser() === TICloudAgent.BROWSER.CHROME ) {
		browserExt = chromeExt;
	}

	browserExt.isInstalled = function() {
		var extDomItem = document.getElementById( "com.ti.TICloudAgent.Bridge" );
		return extDomItem ? true : false;
	};

	browserExt.getVersion = function() {
		var extDomItem = document.getElementById( "com.ti.TICloudAgent.Bridge" );
		return extDomItem.title;
	};

	// abstraction for interfacting with the host app
	var AgentHostApp = function() {


		var AGENT_VERSION = "2.2";
		var EXT_VERSION = "2.0";


		// message types for the init TI cloud agent
		var EXT_MSG_TYPES = {
			INIT: "INIT_EXTENTION",
			INIT_COMPLETE: "INIT_EXTENTION_COMPLETE"
		};

		var POST_MESSAGE_TYPES = {
			CREATE: "CREATE",
			CLOSE: "CLOSE",
			SEND: "SEND"
		};

		var ON_MESSAGE_TYPES = {
			ON_ERROR: "ON_ERROR",
			ON_CLOSE: "ON_CLOSE",
			ON_MESSAGE: "ON_MESSAGE",
			ON_OPEN: "ON_OPEN"
		};

		var socketCache = {};

		var id = 0;
		var Socket = function( url, subProtocol ) {

			this.url = url;
			this.subProtocol = subProtocol;
			this.key = id++;
			socketCache[ this.key ] = this;

			browserExt.postMessage( {
				key: this.key,
				url: this.url,
				subProtocol: this.subProtocol,
				type: POST_MESSAGE_TYPES.CREATE
			} );

		};

		Socket.prototype.close = function() {

			browserExt.postMessage( {
				key: this.key,
				type: POST_MESSAGE_TYPES.CLOSE
			} );


		};

		Socket.prototype.send = function( data ) {
			browserExt.postMessage( {
				key: this.key,
				data: data,
				type: POST_MESSAGE_TYPES.SEND
			} );
		};

		var dispatchSocketEvent = function() {

			var typeToFunc = {
				ON_ERROR: "onerror",
				ON_CLOSE: "onclose",
				ON_MESSAGE: "onmessage",
				ON_OPEN: "onopen"
			};

			return function( msgObj ) {

				var funcName = typeToFunc[ msgObj.type ];
				var key = msgObj.key;

				var socket = socketCache[ key ];
				if ( socket && socket[ funcName ] ) {
					var func = socket[ funcName ];
					func( msgObj.msgEvt );
				}

				if ( msgObj.type === ON_MESSAGE_TYPES.ON_CLOSE ) {
					if ( socketCache[ key ] ) {
						delete socketCache[ key ];
					}
				}
			};
		}();

		return {

			Init: function() {

				var deferred = Q.defer();

				try {

					var errors = []; // list of issues while starting up

					browserExt.init();

					// browser ext is not installed return error
					// we can't do anything else
					if ( !browserExt.isInstalled() ) {
						errors.push( new TICloudAgent.MissingExtension() );
						deferred.reject( errors );
						return deferred.promise;
					}

					// if the browser ext verision does not match, we can still try to start the agent
					if ( browserExt.getVersion() !== EXT_VERSION ) {
						errors.push( new TICloudAgent.InvalidExtensionVersion() );
					}


					var initEventListener = function( msgObj ) {

						if ( msgObj.type === EXT_MSG_TYPES.INIT_COMPLETE ) {

							browserExt.removeLastOnMessage(); // stop listening to the init event

							if ( msgObj.data.error ) {
								// there was an issues starting the agent
								errors.push( new TICloudAgent.AgentNotStarted( msgObj.data.error ) );
							} else if ( msgObj.data.version !== AGENT_VERSION ) {
								// we started fine, but the version was invalid
								errors.push( new TICloudAgent.InvalidAgentVersion() );
							}

							if ( errors.length === 0 ) { // we have no errors
								// expect all other messages to be for the
								// socket interface
								browserExt.regOnMessage( dispatchSocketEvent );
								deferred.resolve( msgObj.data );
							} else {
								deferred.reject( errors );
							}

						} else {
							console.log( "UNEXPECTED MESSAGE TYPE" );
						}
					};

					// set up listeners
					browserExt.regOnMessage( initEventListener );

					// send message to init
					browserExt.postMessage( {
						type: EXT_MSG_TYPES.INIT
					} );


				} catch ( e ) {
					deferred.reject( e );
				}

				return deferred.promise;
			},

			Socket: Socket

		};
	}();

	if ( getBrowser() === TICloudAgent.BROWSER.IE ) {

		// IE specific AgentHostApp impl
		AgentHostApp = function() {

			return {

				Init: function() {

					var deferred = Q.defer();

					var plugin = window.TICloudAgent_HACK.plugin;
					var errors = [];
					if ( plugin && plugin.start ) {
						var initParamsRaw = plugin.start();
						var initParams = JSON.parse( initParamsRaw );
						if ( initParams.error ) {
							errors.push( new TICloudAgent.AgentNotStarted( initParams.error ) );
							deferred.reject( errors );
						} else {
							deferred.resolve( initParams );
						}
					} else {
						errors.push( new TICloudAgent.MissingExtension() );
						deferred.reject( errors );
					}

					return deferred.promise;
				},

				Socket: WebSocket

			};


		}();
	}

	if ( typeof process !== "undefined" ) {
		// Node webkit specific AgentHostApp
		AgentHostApp = function() {
			var osFolderName = getOSFolderName();
			var isWin = /^win/.test( process.platform );
			var ext = isWin ? ".bat" : ".sh";

			var spawn = require( "child_process" ).spawn;
			var fileToExec = "ticloudagent" + ext;
			var pathObj = require( "path" );
			var workingDir = pathObj.dirname( process.cwd() );

			// folder structure is <install directory>/<app name>
			// need <install directory>/win32/TICloudAgent
			// start by going up to parent folder
			workingDir = pathObj.join( workingDir, ".." );
			var dirName = "" + workingDir.toLowerCase();

			// if current folder name doesn't already end in the OS name, add it
			if ( ( dirName.indexOf( osFolderName, dirName.length - osFolderName.length ) === -1 ) ) {
				workingDir = pathObj.join( workingDir, osFolderName );
			}
			workingDir = pathObj.join( workingDir, "TICloudAgent" );
			console.log( "NodeWebkit TICloudAgent workingDir found to be " + workingDir );
			fileToExec = pathObj.join( workingDir, fileToExec );
			var initParams = null;
			var errors = [];
			return {

				Init: function() {

					var deferred = Q.defer();

					var lp = spawn( fileToExec, [ "not_chrome" ], {
						cwd: workingDir
					} );

					lp.stdout.on( "data", function( data ) {
						if ( initParams )
							throw "AREADY INITIALIZED";
						initParams = JSON.parse( data.toString() );
						deferred.resolve( initParams );
					} );


					lp.stderr.on( "data", function( data ) {
						errors.push( new TICloudAgent.AgentNotStarted( data.toString() ) );
						deferred.reject( errors );
					} );

					return deferred.promise;
				},

				Socket: WebSocket
			};
		}();
	}

	// get an object to help with installer related operations
	TICloudAgent.Install = function() {

		var wizardProto = {

			title: "TI Cloud Agent Setup",
			detailsLink: {
				text: "What's this?",
				url: "http://processors.wiki.ti.com/index.php/TI_Cloud_Agent"
			},
			steps: [],
			helpLink: {
				text: "Help. I already did this",
				url: "http://processors.wiki.ti.com/index.php/TI_Cloud_Agent_Troubleshooting_Guide"
			}

		}; // base object

		var createAPIPath = function( api ) {
			return hostInfo.tiCloudAgentServer() + "/" + api;
		};

		var getDescriptionText = function( errors ) {

			var wizardDesc;
			for ( var i = 0; i < errors.length; i++ ) {
				if ( errors[ i ] instanceof TICloudAgent.MissingExtension ) {
					wizardDesc = "Hardware interaction requires additional one time set up.";
				} else if ( errors[ i ] instanceof TICloudAgent.AgentNotStarted ) {
					wizardDesc = "Could not launch TI Cloud Agent : " + errors[ i ].msg;
				} else {
					wizardDesc = "Invalid version of TI Cloud Agent found.";
				}
			}

			wizardDesc += " Please perform the actions listed below and try your operation again.";

			return wizardDesc;
		};


		var createInstallExtStep = function() {

			var stepInstallExt = {
				description: "Install the TI Cloud Agent browser extention.",
				action: {
					text: "$Install$ browser extention",
					handler: function() {
						var success = function() {
							console.log( "Extension Installed Successfully" );
						};
						var failure = function( msg ) {
							console.log( "Extension Installation Failed: " + msg );
						};
						browserExt.install( success, failure );
					}
				}
			};


			return function() {
				return stepInstallExt;
			};

		}();

		var createInstallAgentStep = function() {

			var stepInstallAgent = {
				description: "Download and install the TI Cloud Agent host application.",
			};

			return function( target ) {

				// can't re-use the same step object, because the handler needs to be based on the target
				var step = Object.create( stepInstallAgent );
				step.action = {
					text: "$Download$ and install the TI Cloud Agent Application"
				};
				step.action.handler = function() {
					var url = createAPIPath( "getInstaller" ) + "?os=" + getOS();

					url += "&deviceID=" + target.deviceID;
					url += "&connectionID=" + target.connectionID;

					dynamicLink( url );
				};

				return step;

			};
		}();

		var createPageRefreshStep = function() {

			var stepPageRefresh = {

				description: "Refresh the current browser page",

				action: {

					text: "$Refresh$ Page",
					handler: function() {
						window.location.reload();
					}

				}
			};

			return function() {
				return stepPageRefresh;
			};

		}();

		var createRestartBrowserStep = function() {

			var stepBrowserRestart = {
				description: "Restart the browser",
				action: {
					text: "Restart the browser"
				}
			};

			return function() {
				return stepBrowserRestart;
			};

		}();



		var getSteps = function( errors, target ) {

			var steps = [];

			for ( var i = 0; i < errors.length; i++ ) {
				if ( errors[ i ] instanceof TICloudAgent.MissingExtension ) {
					steps.push( createInstallExtStep() );
					steps.push( createInstallAgentStep( target ) );
				} else if ( errors[ i ] instanceof TICloudAgent.AgentNotStarted || errors[ i ] instanceof TICloudAgent.InvalidAgentVersion ) {
					steps.push( createInstallAgentStep( target ) );
				} else if ( errors[ i ] instanceof TICloudAgent.InvalidExtensionVersion ) {
					steps.push( createInstallExtStep() );
				}
			}

			// safari. we also need to restart the browser
			if ( getBrowser() === TICloudAgent.BROWSER.SAFARI ) {
				steps.push( createRestartBrowserStep() );
			}

			return steps;

		};



		var getInstallWizard = function( params ) {

			// make it async, to be consistent with all other API's
			var def = Q.defer();

			var installErrors = params.errors;
			var target = {};

			target.deviceID = params.deviceID;
			target.connectionID = params.connectionID;

			var wizardObj = Object.create( wizardProto );
			wizardObj.description = getDescriptionText( installErrors );
			wizardObj.steps = getSteps( installErrors, target );

			wizardObj.finishStep = createPageRefreshStep();

			def.resolve( wizardObj );

			return def.promise;

		};

		return {
			getInstallWizard: getInstallWizard
		};

	}();

	// create the client side module
	var createClientModule = function( name, port, subProtocol ) {

		var moduleObj = {};

		// add events related functions
		var eventListeners = {};
		moduleObj.addListener = function( type, listener ) {
			if ( !eventListeners[ type ] )
				eventListeners[ type ] = [];

			eventListeners[ type ].push( listener );
		};

		moduleObj.removeListener = function( type, listener ) {
			if ( eventListeners[ type ] ) {
				var typeListeners = eventListeners[ type ];
				for ( var i = 0; i < typeListeners.length; i++ ) {
					if ( typeListeners[ i ] === listener ) {
						// don't remove it, just null it out
						// if we remove it and the remove was called from a dispatch
						// it could impact the dispatch becuase the length of eventsListeners will change
						typeListeners[ i ] = null;
						break;
					}
				}
			}
		};

		// command dispatch module
		var commandDispatch = function() {

			var commandID = 1; // start from 1 ( 0 could be mistaken for false, in certain places)
			var pendingCommands = {};

			var commandDispatchObj = {

				exec: function( ws, commandName, data ) {

					var defCommand = Q.defer();

					var obj = {
						command: commandName,
						id: commandID++,
						data: data
					};

					pendingCommands[ obj.id ] = defCommand;
					ws.send( JSON.stringify( obj ) );

					return defCommand.promise;
				},

				ret: function( retObj ) {

					var response = retObj.response;
					var error = retObj.error;
					var data = retObj.data;

					// it's should only be one of these
					var id = response || error;
					var defCommand = pendingCommands[ id ];

					if ( defCommand ) {

						if ( response ) {
							defCommand.resolve( data );
						} else {
							defCommand.reject( data );
						}

						// delete it instaed of nulling so the map doesn't grow too large over time
						delete pendingCommands[ id ];

					} else {
						console.log( "commandDispatch : ret , Error, no promise found corresponding to id : " + id );
					}
				}
			};


			var rejectMsg = {
				message: "Module Closed"
			};
			var cleanUp = function() {

				// reject all outstanding requests
				for ( var key in pendingCommands ) {
					var commandPromise = pendingCommands[ key ];
					commandPromise.reject( rejectMsg );
				}

				// replace exec and ret to reject and ignore incoming requests
				commandDispatchObj.exec = function() {

					var defCommand = Q.defer();
					defCommand.reject( rejectMsg );

					return defCommand.promise;
				};

				commandDispatchObj.ret = function() {
					// do nothing
					return;
				};

			};

			moduleObj.addListener( "close", cleanUp );

			return commandDispatchObj;
		}();



		var eventsDispatch = function() {

			return {

				dispatch: function( listeners, retObj ) {
					var typeListeners = listeners[ retObj.event ];
					if ( typeListeners ) {
						for ( var i = 0; i < typeListeners.length; i++ ) {
							var listener = typeListeners[ i ];
							if ( listener ) {
								listener( retObj.data );
							}
						}
					}
				}

			};

		}();


		var socketUrl = function() {
			return "ws://127.0.0.1:" + port;
		};

		moduleObj.getSubModule = function() {

			var subModulePromises = {};

			return function( subModuleName ) {

				var subModulePromise = subModulePromises[ subModuleName ];

				if ( !subModulePromise ) {

					subModulePromise = moduleObj.createSubModule( subModuleName ).then(

						function( data ) {

							return createClientModule( subModuleName, data.port, data.subProtocol ).then( function( subModule ) {

								// lets register for an onclose and onerror events to clean ourselves up
								var cleanUp = function() {
									subModulePromises[ subModuleName ] = null;
								};

								subModule.addListener( "close", cleanUp );

								return subModule; // pass the module down the chain

							} );

						}

					);

					subModulePromises[ subModuleName ] = subModulePromise;
				}

				return subModulePromise;
			};

		}();



		var createCommand = function( ws, fullCommandName ) {

			// add namespace
			var commandNameParts = fullCommandName.split( "." );
			// everything up to the last part is part of the namespace
			var parentObj = moduleObj;
			// keep track of nested namespaces
			var parentNamespace = "";

			var createAddListener = function( eventTypePrefix ) {
				return function( type, listener ) {
					// add name spaces
					type = eventTypePrefix + type;
					moduleObj.addListener( type, listener );
				};
			};

			var createRemoveListener = function( eventTypePrefix ) {

				return function( type, listener ) {
					// add name spaces
					type = eventTypePrefix + type;
					moduleObj.removeListener( type, listener );
				};

			};

			for ( var i = 0; i < commandNameParts.length - 1; i++ ) {

				var currentNamespacePart = commandNameParts[ i ];
				parentNamespace += commandNameParts[ i ];

				var newObj = parentObj[ currentNamespacePart ];

				if ( !newObj ) {
					// lets create it
					newObj = {};
					// lets add convenience methods for adding events
					var eventTypePrefix = parentNamespace + ".";
					newObj.addListener = createAddListener( eventTypePrefix );
					newObj.removeListener = createRemoveListener( eventTypePrefix );
				}

				// it becomes the new parent
				parentObj[ currentNamespacePart ] = newObj;
				parentObj = newObj;

				parentNamespace = parentNamespace + ".";
			}

			var commandName = commandNameParts[ commandNameParts.length - 1 ];

			parentObj[ commandName ] = function() {
				var data = Array.prototype.slice.call( arguments );
				return commandDispatch.exec( ws, fullCommandName, data );
			};
		};

		var setUpWS = function() {

			var setUpDef = Q.defer();

			var ws = subProtocol ? new AgentHostApp.Socket( socketUrl(), subProtocol ) : new AgentHostApp.Socket( socketUrl() );

			var pageUnloadHandler = function() {
				ws.close();
			};

			ws.onclose = function() {

				eventsDispatch.dispatch( eventListeners, {
					event: "close",
					data: {
						message: "socket closed"
					}
				} );

				// remove the listener
				window.removeEventListener( "unload", pageUnloadHandler );
			};

			ws.onerror = function() {
				eventsDispatch.dispatch( eventListeners, {
					event: "error",
					data: {
						message: "socket error"
					}
				} );
			};

			ws.onopen = function() {

				// close the socket before unloading to clean up agent
				window.addEventListener( "unload", pageUnloadHandler );

				// set up command and event return messages
				ws.onmessage = function( msgEvt ) {
					var retObj = JSON.parse( msgEvt.data );
					if ( retObj.event ) {
						eventsDispatch.dispatch( eventListeners, retObj );
					} else {
						commandDispatch.ret( retObj );
					}
				};

				createCommand( ws, "listCommands" ); // everymodule should have a listCommands command

				moduleObj.listCommands().then( function( dataObj ) {
					var commands = dataObj.commands;
					// create commands
					for ( var i = 0; i < commands.length; i++ ) {
						createCommand( ws, commands[ i ] );
					}

					setUpDef.resolve( moduleObj );

				}, function() {
					setUpDef.reject( moduleObj.name + " : listCommands is not defined" );
				} );
			};

			return setUpDef.promise;

		};

		return setUpWS();

	};



	TICloudAgent.Init = function() {

		return AgentHostApp.Init().then( function( initParams ) {
			return createClientModule( "Agent", initParams.port );
		} ).then( function( agent ) {
			// do configuration set up
			agent.addConfigProperty( "cloudAgentInstallerServerURL", hostInfo.tiCloudAgentInstallerServer() );
			return agent;
		} );

	};

	// hack API to figure out weather a target only supports flashing.
	// it is the only sync api we have
	TICloudAgent.isFlashOnly = function( targetName ) {

		if ( targetName.match( ".*MSP430.*G2.*" ) && getOS() !== TICloudAgent.OS.WIN )
			return true;

		return false;
	};

	TICloudAgent.Util = {

		encodeAsBase64: function( data ) {

			var def = Q.defer();

			if ( data instanceof Blob ) {
				// encode data as base 64string
				var reader = new window.FileReader();
				reader.readAsDataURL( data );
				reader.onloadend = function() {
					def.resolve( reader.result.split( "," )[ 1 ] );
				};
			} else {
				def.resolve( btoa( data ) );
			}

			return def.promise;

		},

		selectDefaultPort: function() {

			var keywords = [ "Texas Instruments", "Texas_Instruments", "MSP", "FTDI" ];

			// iterate over all ports with matching key
			// word and
			// perform additional action to determine
			// if it is the port that needs to be
			// selected
			var iteratePortsWithMatchingKeyWords = function( ports, action ) {

				var found = false;
				for ( var i = 0; i < ports.length; ++i ) {
					for ( var j = 0; j < keywords.length; ++j ) {
						if ( ports[ i ].displayName.indexOf( keywords[ j ] ) != -1 ) {
							found = action( ports[ i ] );
							ports[ i ].selected = found;
							if ( found )
								break;
						}
					}
					if ( found )
						break;
				}

				if ( !found && ports.length > 0 )
					ports[ 0 ].selected = true;
				return ( found );
			};

			// default is to select any port matching
			// the keyword
			var selectDefault = function( ports ) {
				var result = iteratePortsWithMatchingKeyWords( ports, function() {
					return true;
				} );
				return ( result );
			};

			var selectDefaultMSP432 = function( ports ) {
				var result = iteratePortsWithMatchingKeyWords( ports, function( port ) {
					var suffix = "00";
					return port.pnpId.indexOf( suffix, port.pnpId.length - suffix.length ) !== -1;
				} );
				return ( result );
			};
			// this is the default selection for 5529
			// and 5969
			// msp430's on linux/windows
			// the pnpid which refers to the interface
			// should always
			// be 02 on linux/windows
			// as per MSP team instructions
			var selectDefaultMSP5529And5969 = function( ports ) {
				var result = iteratePortsWithMatchingKeyWords( ports, function( port ) {
					var suffix = "02";
					return port.pnpId.indexOf( suffix, port.pnpId.length - suffix.length ) !== -1;
				} );
				return ( result );
			};

			// this is the default selection for 5529
			// and 5969
			// msp430's
			// on osx; which is an special case
			var selectDefaultMSP5529And5969OnOsx = function( ports ) {
				var found = false;
				for ( var i = 0; i < ports.length; ++i ) {
					if ( ports[ i ].comName.indexOf( "/dev/cu.usb" ) !== -1 && ports[ i ].manufacturer === "" ) {
						found = true;
						ports[ i ].selected = found;
					}

					if ( found )
						break;
				}

				if ( !found && ports.length > 0 )
					ports[ 0 ].selected = true;
				return ( found );
			};

			return function( params ) {

				var def = Q.defer();

				var ports = params.ports;
				var targetName = params.targetName ? params.targetName : "";

				var selectFunc = selectDefault;
				// msp430 5529 and 5969
				if ( targetName.indexOf( "5529" ) > -1 || targetName.indexOf( "5969" ) > -1 ) {
					if ( getOS() === TICloudAgent.OS.OSX )
						selectFunc = selectDefaultMSP5529And5969OnOsx;
					else
						selectFunc = selectDefaultMSP5529And5969;
				}
				if ( targetName.indexOf( "432" ) > 0 ) {
					if ( getOS() === TICloudAgent.OS.OSX ) {
						selectFunc = selectDefault;
					} else {
						selectFunc = selectDefaultMSP432;
					}
				}

				var found = selectFunc( ports );
				def.resolve( found );

				return def.promise;

			};
		}(),

		getBaudRates: function() {

			var baudRates = [ {
				rate: "50",
				selected: false
			}, {
				rate: "75",
				selected: false
			}, {
				rate: "110",
				selected: false
			}, {
				rate: "134",
				selected: false
			}, {
				rate: "150",
				selected: false
			}, {
				rate: "300",
				selected: false
			}, {
				rate: "600",
				selected: false
			}, {
				rate: "1200",
				selected: false
			}, {
				rate: "1800",
				selected: false
			}, {
				rate: "2400",
				selected: false
			}, {
				rate: "4800",
				selected: false
			}, {
				rate: "7200",
				selected: false
			}, {
				rate: "9600",
				selected: true
			}, {
				rate: "14400",
				selected: false
			}, {
				rate: "19200",
				selected: false
			}, {
				rate: "28800",
				selected: false
			}, {
				rate: "38400",
				selected: false
			}, {
				rate: "56000",
				selected: false
			}, {
				rate: "57600",
				selected: false
			}, {
				rate: "115200",
				selected: false
			}, {
				rate: "128000",
				selected: false
			}, {
				rate: "153600",
				selected: false
			}, {
				rate: "230400",
				selected: false
			}, {
				rate: "256000",
				selected: false
			}, {
				rate: "460800",
				selected: false
			}, {
				rate: "921600",
				selected: false
			} ];

			return function() {
				var def = Q.defer();
				def.resolve( baudRates );
				return def.promise;
			};

		}()


	};

}() );