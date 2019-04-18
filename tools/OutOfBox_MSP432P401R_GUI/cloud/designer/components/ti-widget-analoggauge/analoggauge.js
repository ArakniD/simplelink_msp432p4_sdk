// Constructor for SteelSeries objects
window.AnalogGauge = function(options) {
    debugPrint('Constructor for AnalogGauge objects called... options='+options);
	return this;//.init(options);
}

var isDebuggingEnabled = false;	
	
var debugPrint = function(str){
   if (isDebuggingEnabled) {
      console.log(str);
   };
};

// Find all canvas elements that have the SteelSeries class
// Config contains some defaults, and skins
AnalogGauge.config = {
	value: 0,
	minValue: 0,
	maxValue: 100,
	size: 177,
	defaultSkin: "brushedStainless",
	defaultRadius: 85,
	renderRadius: 100,


	// Test for IE so we can nurse excanvas in a couple of places
	isIE: !!document.all,

	// For giving a unique id to coolclock canvases with no id
	noIdCount: 0
};

window.AnalogGauge.findAndCreateGauges = function(root) {
	// (Let's not use a jQuery selector here so it's easier to use frameworks other than jQuery)
	var canvases = (root || document).getElementsByTagName("canvas");
	for (var i=0;i<canvases.length;i++) {
		// Pull out the fields from the class. Example "SteelSeries:chunkySwissOnBlack:1000"
		var fields = canvases[i].className.split(" ")[0].split(":");
		debugPrint('in findAndCreateGauges: canvases[i].className = '+canvases[i].className);
		if (fields[0] == "SteelSeries") {
				
			if (!canvases[i].id) {
				// If there's no id on this canvas element then give it one
				canvases[i].id = '_analoggauge_auto_id_' + AnalogGauge.config.noIdCount++;
			}
			
			// Create a gauge object for this element
			var obj = new SteelSeries({
			  root:           root,
				canvasId:       canvases[i].id,
				skin:         fields[1],
				size:           fields[2],
				minValue:       fields[3],
				maxValue:       fields[4],
				flatFrameColor: fields[5],
				flatFaceColor: fields[6],
				flatFontColor: fields[7],
				pointerType: fields[8],
				pointerColor: fields[9],
				numberFormat: fields[10],
				fractionalScaleDecimals : fields[11]
			});
			obj.repaint();
			return(obj);
		} 
	}
};


