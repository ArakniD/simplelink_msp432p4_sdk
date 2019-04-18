/**
 * This file includes implementation of gc.File operations. If they have already been defined (
 * implying we are running in GC ) then the implementation is skipped.
 * 
 */

var gc = gc || {};
gc.File = gc.File || {};

(function() // closure for private static methods and data.
{
	gc.File._saveFile = function(filePath, data, callback) {
		var fs = require('fs');
		fs.writeFile(filePath, data, function(err) {
			if (err)
			{
				callback({
					localPath : filePath
				}, {
					message : err
				});
			}
			else
			{
				callback({
					localPath : filePath
				}, null);
			}
		});
	};

	gc.File._loadFile = function(filePath, callback) {
		var fs = require('fs');
		fs.readFile(filePath, 'utf-8', function(err, contents) {
			if (err)
			{
				callback(null, {
					localPath : filePath
				}, {
					message : err
				});
			}
			else
			{
				callback(contents, {
					localPath : filePath
				}, null);
			}
		});
	};


	/** See Gui Composer for API doc * */
	gc.File.browseAndSave = function(data, fileInfo, options, callback, dialog) {

		var chooser = dialog || document.querySelector('#fileSaveDialog');
		chooser.value = null;

		chooser.addEventListener("change", function changeEvtListner(evt) {

			var filePath = this.value;
			gc.File._saveFile(filePath, data, callback);
			chooser.removeEventListener("change", changeEvtListner, false);

		}, false);

		chooser.click();
	};

	gc.File.save = function(data, fileInfo, options, callback) {
		gc.File._saveFile(fileInfo.localPath, data, function(fileInfo, errorInfo) {
			callback(errorInfo);
		});
	};

	gc.File.load = function(fileInfo, options, callback) {
		gc.File._loadFile(fileInfo.localPath, callback);
	};


	gc.File.browseAndLoad = function(fileInfo, options, callback, dialog) {

		var chooser = dialog || document.querySelector('#fileLoadDialog');
		chooser.value = null;

		chooser.addEventListener("change", function changeEvtListner(evt) {

			var filePath = this.value;
			gc.File._loadFile(filePath, callback);
			chooser.removeEventListener("change", changeEvtListner, false);

		}, false);

		chooser.click();

	};

	gc.File.browseFolder = function(fileInfo, options, callback, dialog) {

		var chooser = dialog || document.querySelector('#chooseDirDialog');
		chooser.value = null;

		chooser.addEventListener("change", function changeEvtListner(evt) {

			var filePath = this.value;
			callback({
				localPath : filePath
			}, null);
			chooser.removeEventListener("change", changeEvtListner, false);

		}, false);

		chooser.click();

	};

	var dataProviders = {};

	gc.File.addDataProvider = function(id, callback) {
		dataProviders[id] = callback;
	};

	gc.File.removeDataProvider = function(id) {
		this.addDataProvider(id, null);
	};

	var readDataForSave = function() {
		var data = {};

		for ( var providerName in dataProviders)
		{
			if (dataProviders.hasOwnProperty(providerName))
			{
				var dataProvider = dataProviders[providerName];

				data[providerName] = dataProvider.readData();
			}
		}
		return data;
	};

	var writeDataForLoad = function(data) {
		for ( var providerName in dataProviders)
		{
			if (dataProviders.hasOwnProperty(providerName))
			{
				var dataProvider = dataProviders[providerName];
				var providerData = data[providerName];
				if (providerData)
				{
					dataProvider.writeData(providerData);
				}
			}
		}
	};

	gc.File.IDataProvider = function() {
	};

	gc.File.IDataProvider.prototype.readData = function() {
	};

	gc.File.IDataProvider.prototype.writeData = function(data) {
	};

	var FileLoadOperation = function(oldData, newData) {
		this.oldData = oldData;
		this.newData = newData;
	};

	FileLoadOperation.prototype.undo = function() {
		writeDataForLoad(this.oldData);
	};

	FileLoadOperation.prototype.redo = function() {
		writeDataForLoad(this.newData);
	};

	FileLoadOperation.prototype.toString = function() {
		return "File Load";
	};

	var doFileLoad = function() {
		gc.File.browseAndLoad(null, null, function(contents) {
			console.log("finished loading file");

			try
			{
				var data = JSON.parse(contents);
				var oldData = (gc.history && gc.history.push) ? readDataForSave() : null;
				var operation = new FileLoadOperation(oldData, data);
				operation.redo();

				if (gc.history && gc.history.push)
				{
					gc.history.push(operation);
				}
			}
			catch (e)
			{
				console.log("Error parsing JSON data file");
			}
		}, loadDialogContext);
	};

	var doFileSave = function() {
		var data = readDataForSave();
		var jsonText = JSON.stringify(data, null, 4);

		gc.File.browseAndSave(jsonText, null, null, function() {
			console.log("finished saving file");
		}, saveDialogContext);
	};

	var doFileProgram = function() {
		var programLoaderDialog =  document.querySelector('ti-widget-filemenu')._programLoaderDialog;
		if ((programLoaderDialog !== undefined)&&(programLoaderDialog !== null)) {
			programLoaderDialog.toggle();
		}
	};

	var loadDialogContext;
	gc.File.registerLoadDialog = function(node) {
		loadDialogContext = node;
	};
	var saveDialogContext;
	gc.File.registerSaveDialog = function(node) {
		saveDialogContext = node;
	};
	var programDialogContext;
	gc.File.registerProgramDialog = function(node){
		programDialogContext = node;
	};

	gc.nav = gc.nav || {};
	gc.nav.ready = gc.nav.ready || Q.Promise(function(resolve) {
		gc.nav.fireReady = resolve;
	});
	gc.nav.ready.then(function() {
		gc.nav.registerAction('FileLoad', {
			run : doFileLoad
		});
		gc.nav.registerAction('FileSave', {
			run : doFileSave
		});
		gc.nav.registerAction('FileProgram',{
			run : doFileProgram
		});
	});

}());

gc.File.ready = gc.File.ready || Q.Promise(function(resolve) {
	gc.File.fireReady = resolve;
});
gc.File.fireReady();
