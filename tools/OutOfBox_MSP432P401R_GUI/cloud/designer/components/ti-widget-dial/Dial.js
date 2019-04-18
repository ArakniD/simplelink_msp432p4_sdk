/**
 * Dial.
 */
//window.gc_dial_value = 0;  //bc

/*define([ 
 "dojo/_base/declare",
 "dojo/dom-geometry",
 "dojo/dom-style",
 "dojo/_base/lang",
 "dijit/_WidgetBase",
 "./_GCBaseWidget"
 ], function(declare, domGeom, domStyle, lang, _WidgetBase, _GCBaseWidget) {
 return declare("gc.dijit.Dial", [_WidgetBase, _GCBaseWidget], {
 baseClass: "dijit dijitDial", */

// Constructor for SteelSeries objects
window.Dial = function(options) {
	console.log('window.Dial(options) called... options.canvasId='+options.canvasId);

	return this.constructor(options);
}




// Define the Dial object's methods
Dial.prototype = {
	init: function(options) {
		this._RADIAN = Math.PI/180;

		this._halfArcLength = 150;
		this._outerRadius = 0.98;
		this._innerRadius = 0.65;

		this._numberHashMarks = 10;
		this._hashMarkIncrement = 3.1415;
		this._numberSmallHashMarks = 9;
		this._minHashRatio = 0.73;
		this._maxHashRatio = 0.70;
		this._labelRatio = 0.83;

		this._needleValue = 0;
		this._animateValue = 0;

		this._mainCanvas = null;
		this._outerRingBuffer = null;
		this._dialBuffer = null;
		this._dialDesign = null;
		this._labels = [];
		this.dt =2;
		// public attributes
		this.animate = false;
		this.font = '12px sans-serif';
		this.title = '';
		this.unit = '';
		this.size = 0;
		this.dialDesign = 'stainless';
		this.frameDesign = 'shinyMetal';
		this.labelNumberFormat = '';
		this.fractionalScaleDecimals = 0;
		this.labels = [];
		this.minValue = 0;
		this.maxValue = 0;
		this.value = 0;
		this.setValueOnDrag = true;

		// mouse attributes
		this._dragging = false;
		this._startAngle = 0;
		this._valueDiff = 0;
		this._oldValue = 0;

		this.steelSeries = new SteelSeries(options);
		this.labelNumberFormat = this.steelSeries.labelNumberFormat;
		this.fractionalScaleDecimals = this.steelSeries.fractionalScaleDecimals;
		this._params = {}; // SGE



	},
	skins:	{
		// There are more skins in moreskins.js
		// Try making your own skin by copy/pasting one of these and tweaking it
		brushedStainless: {
			dialDesign: 'stainless',
			needleStyle: 'small',
			showSmallTicks: true
		},
		paper: {
			dialDesign: 'white',
			frameDesign: 'transparent',
			needleStyle: 'large',
			showSmallTicks: false

		},
		flat: {
			dialDesign: 'white',
			frameDesign: 'transparent',
			needleStyle: 'large',
			showSmallTicks: false
		},
		blue_gradient: {
			dialDesign: 'blue',
			frameDesign: 'transparent',
			needleStyle: 'small',
			showSmallTicks: true
		},
		gold: {
			dialDesign: 'white',
			frameDesign: 'gold',
			needleStyle: 'large',
			showSmallTicks: false
		}

	},
	skin: 'brushedStainless',
	needleStyle: 'small',
	showSmallTicks: true,
	constructor: function(params) {
		console.log('Dial.constructor: params = '+params);
		this.root = params.root;
		this.id = params.canvasId;
		this.size = params.size;
		this.flatFrameColor = (undefined === params.flatFrameColor ? '#FFFFFF' : params.flatFrameColor);
		this.flatFaceColor = (undefined === params.flatFaceColor ? '#FFFFFF' : params.flatFaceColor);
		this.flatFontColor = (undefined === params.flatFontColor ? '#000000' : params.flatFontColor);
		this.needleStyle = (undefined === params.pointerType ? 'small' : params.pointerType);
		this.needleColor = (undefined === params.pointerColor ? '#CC0000' : params.pointerColor);
		this._mainCanvas = this.findCanvas();
		this.init(params);
		console.log("Dial constructor");
		this._params = params; // SGE

		this.size = params.size = (params.size != undefined) ? Number(params.size) : 150;
		this.minValue = params.minValue = (params.minValue != undefined) ? Number(params.minValue) : 0;
		this.maxValue = params.maxValue = (params.maxValue != undefined) ? Number(params.maxValue) : 100;

		// prevent min >= max
		if (this.minValue >= this.maxValue) {
			this.maxValue = this.minValue + 0.001;
			params.maxValue = this.maxValue;
		}
		this._set('minValue',this.minValue);
		this._set('maxValue',this.maxValue);

		this.setValueOnDrag = true;/*bc (params.setValueOnDrag != null) ? params.setValueOnDrag : this.setValueOnDrag;*/
		this.value = params.value = (params.value != undefined) ? Number(params.value) : this.minValue;
		this.value = params.value = (this.value < this.minValue) ? this.minValue : this.value;
		this.fractionalScaleDecimals = params.fractionalScaleDecimals = (params.fractionalScaleDecimals != undefined) ? Number(params.fractionalScaleDecimals) : 1;
		if ((params.skin !== undefined) && (params.skin !== null)){
			this.skin = params.skin;
		}
		this._userLabels = (params.labels != undefined && params.labels.length != 0) ? true : false;
		this._labels = /* bc: params.labels = this._userLabels ? params.labels : */this._createLables(this.minValue, this.maxValue);

		if (this.skins[this.skin] !== null) {
			var dialSkin = this.skins[this.skin];
			if (this.skin === 'flat'){
				this.dialDesign = 'flat';
				this.frameDesign = 'flat';
				//this.needleStyle = 'large';
			} else {
				this.dialDesign = (dialSkin.dialDesign != null) ? dialSkin.dialDesign : 'stainless';
				this.frameDesign = (dialSkin.frameDesign != null) ? dialSkin.frameDesign : 'shinyMetal';
				this.needleStyle =   (dialSkin.needleStyle != null) ? dialSkin.needleStyle : 'small';
			}
			this.fractionalScaleDecimals =  (dialSkin.fractionalScaleDecimals != null) ? dialSkin.fractionalScaleDecimals : 1;

			this.showSmallTicks = (dialSkin.showSmallTicks != null) ? dialSkin.showSmallTicks : true;
		}
		this._dialDesign = this._createSteelSeriesDialDesign(this.dialDesign);
		console.log('this._dialDesign = '+this._dialDesign);

	},

	_createSteelSeriesDialDesign: function(design) {
		var result = undefined;
		switch (design) {
			case 'punchedSheet':
				result = this.steelSeries.BackgroundColor.PUNCHED_SHEET;
				break;
			case 'carbon':
				result = this.steelSeries.BackgroundColor.CARBON;
				break;
			case 'brushedMetal':
				result = this.steelSeries.BackgroundColor.BRUSHED_METAL;
				break;
			case 'stainless':
				result = this.steelSeries.BackgroundColor.BRUSHED_STAINLESS;
				break;
			case 'brushedStainless':
				result = this.steelSeries.BackgroundColor.BRUSHED_STAINLESS;
				break;
			case 'turned':
				result = this.steelSeries.BackgroundColor.TURNED;
				break;
			case 'darkGray':
				result = this.steelSeries.BackgroundColor.DARK_GRAY;
				break;
			case 'satinGray':
				result = this.steelSeries.BackgroundColor.SATIN_GRAY;
				break;
			case 'lightGray':
				result = this.steelSeries.BackgroundColor.LIGHT_GRAY;
				break;
			case 'white':
				result = this.steelSeries.BackgroundColor.WHITE;
				break;
			case 'black':
				result = this.steelSeries.BackgroundColor.BLACK;
				break;
			case 'beige':
				result = this.steelSeries.BackgroundColor.BEIGE;
				breakl
			case 'brown':
				result = this.steelSeries.BackgroundColor.BROWN;
				break;
			case 'red':
				result = this.steelSeries.BackgroundColor.RED;
				breakl
			case 'green':
				result = this.steelSeries.BackgroundColor.GREEN;
				break;
			case 'blue':
				result = this.steelSeries.BackgroundColor.BLUE;
				break;
			case 'anthracite':
				result = this.steelSeries.BackgroundColor.ANTHRACITE;
				break;
			case 'mud':
				result = this.steelSeries.BackgroundColor.MUD;
				break;
			case 'flat':
				result = this.steelSeries.getFlatBackgroundColorFromParam(this.flatFrameColor,this.flatFontColor);
				break;
			default:
				result = null;
				break;
		}
		console.log('design = '+design+'. _dialDesign = '+result);
		return(result);
	},
	buildRendering:function(){
		this.inherited(arguments);
		this.domNode.width=this.size;
		this.domNode.height=this.size;
	},
	postCreate: function(parent) {
		console.log('in postCreate...');
		/*
		 var _self = this;
		 dojo.connect(this.domNode, 'mousedown', function(e) {_self._mouseDown(e);});
		 dojo.connect(this.domNode, 'mouseup', function(e) {_self._mouseUp(e);});
		 dojo.connect(this.domNode, 'mousemove', function(e) {_self._mouseMove(e);});
		 dojo.connect(this.domNode, 'mouseenter', function(e) {_self._mouseEnter(e);});
		 dojo.connect(this.domNode, 'mouseout', function(e) {_self._mouseOut(e);});
		 if( this.domNode.ondragstart) {
		 this.domNode.ondragstart = function() { return false; };
		 }
		 */
		//BC: alternate form, if there are multiple listeners, is
		// this._mainCanvas.addEventListener('mousedown', function (e) {
		//     // React to the mouse down event
		//});
		// see http://www.informit.com/articles/article.aspx?p=1903884&seqNum=6
		parent._mainCanvas.onmousedown = function(e){
			parent._mouseDown(e);
		};
		parent._mainCanvas.onmouseup = function(e){
			parent._mouseUp(e);
		};
		parent._mainCanvas.onmousemove = function(e){
			//console.log('in _mainCanvas.onmousemove');


			parent._mouseMove(e);

		};
		parent._mainCanvas.onmouseenter = function(e){
			parent._mouseEnter(e);
		};
		parent._mainCanvas.onmouseout = function(e){
			parent._mouseOut(e);
		};
		//onmousemove, onmouseup, onmouseover, and onmouseout.
	},

	startup: function() {
		//this.inherited(arguments);
		//this.domNode.removeAttribute("title");
		//this.domNode.width = this.size;
		//this.domNode.height = this.size;
		this._calSize();
		//BC this._mainCanvas = document.getElementById(this.id);
		// Get the canvas element
		//this.canvas = document.getElementById(this.canvasId);
		this._mainCanvas = this.findCanvas();
		console.log('mainCanvas = '+this._mainCanvas);
		// Make the canvas the requested size. It's always square.
		this._mainCanvas.setAttribute("width",this.size);
		this._mainCanvas.setAttribute("height",this.size);
		this._mainCanvas.style.width = this.size + "px";
		this._mainCanvas.style.height = this.size + "px";

		// Explain me please...?
		this.renderRadius = Dial.config.renderRadius;
		this.scale = this.displayRadius / this.renderRadius;

		// Initialise canvas context

		this._mainCtx = this._mainCanvas.getContext('2d');
		console.log('mainCtx = '+this._mainCtx);
		//domStyle.set(this._mainCanvas, 'height', this.size + "px");
		//domStyle.set(this._mainCanvas, 'width', this.size + "px");
		//this.domNode.width = this.size;
		//this.domNode.height = this.size;
		this.postCreate(this);
		if (this.animate) {
			this._initAnimation();
		}

		this._setValueAttr(this.value); // initialize the needle
		this._redraw();
		console.log('dial.js: redraw called');
	},

	_setLabelsAttr: function(value) {
		if (value !== undefined) {
			if (this._userLabels)
				this._set('labels', value);
			this._set('_labels', value);

			this._numberHashMarks = value.length;

			this._computeHashMarkMarkIncrement();
			this._redraw(); //bc
		}
	},

	_getLabelsAttr: function() {
		if (this._userLabels)
			return this.labels;
		else
			return [];
	},

	_setSizeAttr: function(value) {
		this._set('size', value);

		var size = Math.ceil(value * 0.05);
		this._set('font', size+'px sans-serif');
	},

	_setValueAttr: function(inputValue) {
		if (typeof(inputValue) !== 'number')
			inputValue = 0;

		if (inputValue > this.maxValue)
			inputValue = this.maxValue;
		else if (inputValue < this.minValue)
			inputValue = this.minValue;
		this.value = inputValue;//bc
		if (this.ti_widget_dial !== undefined){
			this.ti_widget_dial.value = this.value;
		}
		console.log('_setValueAttr: inputValue = '+inputValue);
		var evt = document.createEvent("Event");
		evt.initEvent("valueChanged",true,true);
		// custom param
		evt.newValue = inputValue;
		/*			var evt = new CustomEvent(
		 "valueChanged",
		 {
		 detail: {
		 newValue: inputValue,
		 },
		 bubbles: true,
		 cancelable: true
		 }
		 );
		 */

		document.dispatchEvent(evt);
		this._set('value', inputValue);
		this._set('_animateValue', inputValue);

		var speedVector = this._getSpeedVector(this._needleValue, inputValue);
		this._updateNeedle(speedVector, 0);
	},

	_setAnimateValueAttr: function(value) {
		if (value > this.maxValue)
			value = this.maxValue;
		else if (value < this.minValue)
			value = this.minValue;

		this._set('_animateValue', value);

		var speedVector = this._getSpeedVector(this._needleValue, value);
		this._updateNeedle(speedVector, 0);
	},

	_set: function(prop, val) {
		//this.inherited(arguments);

		if (prop == 'minValue' || prop == 'maxValue') {
			this.labels = this._createLables(this.minValue, this.maxValue);
			this._setLabelsAttr(this.labels);
			this.regenerate();
		}
	},

	setDialValue: function(dialValue){
		this.value = dialValue;
		var evt = document.createEvent("Event");
		evt.initEvent("valueChanged",true,true);
		// custom param
		evt.newValue = dialValue;
		document.dispatchEvent(evt);
		this._redraw();
	},
	getDialValue: function(){
		return(this.value);
	},
	regenerate: function() {
		if (this._params.minValue != this.minValue) {
			this._params.minValue = this.minValue;
		}

		if (this._params.maxValue != this.maxValue) {
			this._params.maxValue = this.maxValue;
		}

		var self = this;
		setTimeout(function() { // wait for labels to be re-created
			self._redraw();
		}, 250);
	},

	/********************************************************************************************
	 /* Drawing methods
	 /********************************************************************************************/
	_redraw: function() {
		var canvas = this._mainCanvas;
		console.log('in _redraw: canvas != null is '+(canvas != null));
		if (canvas != null) {
			var ctx = canvas.getContext('2d');
			ctx.save();

			var bounds = this._getBounds(canvas);
			var radius = this._getRadius(canvas);
			var center = this._getCenter(canvas);
			var outerRadius = radius * this._outerRadius;
			var innerRadius = radius * this._innerRadius;

			ctx.clearRect(bounds.x, bounds.y, bounds.width, bounds.height);
			ctx.translate(center.x, center.y);

			this._drawCircularDial(center, outerRadius, innerRadius);

			this._drawHashMarks(radius);

			this._drawNeedle(radius);
			this._drawTitle();
			ctx.restore();
		}
	},

	_drawNeedle: function(radius) {
		var ctx = this._mainCtx;
		ctx.save();

		var rotation = this._getNeedleStartAngle() - this._getNeedleAngleOffset();
		console.log('drawNeedle: rotation = '+rotation+', value = '+this.value+', needleValue = '+this._needleValue);

		ctx.rotate( -rotation * this._RADIAN);

		if (this.needleStyle !== 'large') {
			ctx.shadowOffsetX = -1;
			ctx.shadowOffsetY = -1;
			ctx.shadowBlur = 3;
			ctx.shadowColor = '#202020';


			var size = this.size * 0.03;
			var x = radius * this._innerRadius * 0.80;

			ctx.beginPath();
			ctx.moveTo(x, 0);
			ctx.lineTo(x - size, size);
			ctx.lineTo(x - size, -size);
			ctx.lineTo(x, 0);
			ctx.closePath();

			ctx.fillStyle = this.needleColor;// 'rgba(255, 80, 80, 0.9)';
		} else {
			var size = this.size * 0.09;
			var x = radius * this._innerRadius * 0.80;

			ctx.beginPath();
			ctx.moveTo(x, 0);
			ctx.lineTo(x - size, size/1.4);
			ctx.lineTo(x - size, -size/1.4);
			ctx.lineTo(x, 0);
			ctx.closePath();

			ctx.fillStyle = this.needleColor;//'rgba(127, 127, 127, 0.9)';
		}
		//ctx.fillStyle = 'rgba(4, 128, 190, 0.7)';
		ctx.fill();

		ctx.restore();
	},


	_drawCircularDial: function(center, outerRadius, innerRadius) {
		var ctx = this._mainCtx;

		if ((this.frameDesign != 'transparent') && (this.frameDesign != 'paper')) {

			// draw background

			if (this.dialDesign !== 'flat') {
				ctx.save();
				ctx.beginPath();
				ctx.fillStyle = 'rgb(100, 100, 100)';
				ctx.arc(0, 0, outerRadius, 0, 2 * Math.PI, false);
				ctx.closePath();
				ctx.fill();
				ctx.restore();
			} else {
				ctx.save();
				ctx.beginPath();
				ctx.fillStyle = this.flatFrameColor;
				ctx.arc(0, 0, outerRadius, 0, 2 * Math.PI, false);
				ctx.fill();
				ctx.restore();
			}
		}


		// draw the meter value
		ctx.save();
		ctx.lineWidth = this.size * 0.05;
		//ctx.strokeStyle = '#CF2900';
		ctx.strokeStyle = '#0772A8';
		if (this.dialDesign === 'flat'){
			ctx.strokeStyle = this.needleColor;
		}
		if (this.needleStyle === 'small'){
			ctx.shadowBlur = 6;
			ctx.shadowColor = '#202020';
		}
		ctx.beginPath();
		var startAngle = -this._getNeedleStartAngle();
		var endAngle = startAngle+this._getNeedleAngleOffset();
		ctx.arc(0, 0, innerRadius*0.9, startAngle*this._RADIAN, endAngle*this._RADIAN, false);
		ctx.stroke();
		ctx.restore();

		if (this.frameDesign != 'transparent') {
			// draw outer ring
			ctx.save();
			if (this._outerRingBuffer == undefined) {
				this._outerRingBuffer = this._createOuterRingBuffer(center, outerRadius, innerRadius);
			}
			ctx.drawImage(this._outerRingBuffer, -center.x, -center.y); // undo translation
			ctx.restore();
		}

		// draw inner dial
		ctx.save();
		var rotate = this._dialDesign === this.steelSeries.BackgroundColor.CARBON || this._dialDesign === this.steelSeries.BackgroundColor.PUNCHED_SHEET ||
			this._dialDesign === this.steelSeries.BackgroundColor.BRUSHED_METAL || this._dialDesign === this.steelSeries.BackgroundColor.BRUSHED_STAINLESS ||
			this._dialDesign === this.steelSeries.BackgroundColor.TURNED || this._dialDesign == this.steelSeries.BackgroundColor.BLUE;

		if (rotate) {
			ctx.rotate( this._getNeedleAngleOffset() * this._RADIAN);
		}

		if (this._dialBuffer == undefined) {
			this._dialBuffer = this._createDialBuffer(center, outerRadius, innerRadius*0.90);
		}
		ctx.drawImage(this._dialBuffer, -center.x, -center.y); // undo translation
		ctx.restore();
	},
	getRgbaString : function(param){
		var r = parseInt(param.substring(1,3),16);
		var g = parseInt(param.substring(3,5),16);
		var b = parseInt(param.substring(5,7),16);
		var result  = 'rgba('+ r.toString()+','+ g.toString()+','+ b.toString()+',0.8)';
		return result;
	},
	_drawLabelText: function(ctx, x, y, label, index) {
		//console.log('in _drawLabelText: label='+label);
		ctx.save();
		ctx.textAlign = 'center';
		ctx.textBaseline = 'middle';
		ctx.font = this.font;

		var labelColor = 'rgba(50, 50, 50, 0.8)';
		switch (this.frameDesign) {
			case 'anthracite':
			case 'steel':
			case 'blackMetal':
			case 'tiltedBlack':
				labelColor = 'rgba(210, 210, 210, 0.8)';
				break;
			case 'flat':
				labelColor = this.getRgbaString(this.flatFontColor);
				break;

		}

		ctx.fillStyle = labelColor;

		ctx.translate(x, y);
		ctx.rotate(Math.PI);
		ctx.fillText(label.trim(), 0, 0);
		ctx.restore();
	},

	_drawTitle: function() {
		var ctx = this._mainCtx;
		ctx.save();
		ctx.textAlign = 'center';
		ctx.textBaseline = 'middle';

		ctx.strokeStyle = this._dialDesign.labelColor.getRgbaColor();
		ctx.fillStyle = this._dialDesign.labelColor.getRgbaColor();

		ctx.font = 0.046728 * this.size + 'px sans-serif';
		ctx.fillText(this.title, 0, -(this.size*0.15), this.size * 0.3);
		ctx.fillText(this.unit, 0, -(this.size*0.08), this.size * 0.2);

		ctx.restore();
	},

	_drawHashMarks: function(radius) {
		var innerHash = this._minHashRatio * radius;
		var outerHash =  this._maxHashRatio * radius;
		var middleHash = (innerHash + outerHash) / 2;
		var labelHash = this._labelRatio * radius;
		//var hashColor = 'rgba(50, 50, 50, 0.8)';
		var hashColor = 'rgba(10, 10, 10, 0.8)';
		switch (this.frameDesign) {
			case 'anthracite':
			case 'steel':
			case 'blackMetal':
			case 'tiltedBlack':
				hashColor = 'rgba(210, 210, 210, 0.8)';
				break;
			case 'flat':
				hashColor = this.flatFontColor;
				break;
		}

		console.log('in drawHashmarks.  this._numberHashMarks = '+this._numberHashMarks+', this._RADIAN='+this._RADIAN+', this._hashMarkIncrement='+this._hashMarkIncrement+', this._halfArcLength='+this._halfArcLength);
		var ctx = this._mainCtx;
		ctx.save();

		// draw hash marks
		ctx.rotate((this._halfArcLength + this._hashMarkIncrement + 180)*this._RADIAN);

		if (this._numberHashMarks < 2) {
			if (this._numberSmallHashMarks + this._numberHashMarks < 2) {
				ctx.rotate(-this._hashMarkIncrement*this._RADIAN);  // skip beginning hash.
			}
			for (var j = this._numberSmallHashMarks; j-- > 0;) {
				ctx.rotate(-this._hashMarkIncrement*this._RADIAN);
				this._drawLine(ctx, 0, innerHash, 0, middleHash, 1, hashColor);
			}
		}


		for (var i = this._numberHashMarks; i-- > 0; ) {
			ctx.rotate(-this._hashMarkIncrement * this._RADIAN);
			this._drawLine(ctx, 0, innerHash, 0, outerHash, 2, hashColor);

			var label = this._labels[i];
			this._drawLabelText(ctx, 0, labelHash, label);

			if (i == 0 && this._numberHashMarks > 1) {
				break;
			}

			for (var j = this._numberSmallHashMarks; j-- > 0;) {
				ctx.rotate(-this._hashMarkIncrement * this._RADIAN);
				if (this.showSmallTicks) {
					this._drawLine(ctx, 0, innerHash, 0, middleHash, 1, hashColor);
				}
			}

		}

		if (this._numberSmallHashMarks + this._numberHashMarks < 2) {
			ctx.rotate(-this._hashMarkIncrement*this._RADIAN);
		}
		ctx.restore();
	},

	_drawLine: function(ctx, x1, y1, x2, y2, lineWidth, strokStyle) {
		ctx.save();
		ctx.beginPath();
		ctx.strokeStyle = strokStyle;
		ctx.lineWidth = lineWidth;

		ctx.moveTo(x1, y1);
		ctx.lineTo(x2, y2);
		ctx.stroke();
		ctx.restore(0);
	},

	/********************************************************************************************
	 /* Create buffer methods
	 /********************************************************************************************/
	_createOuterRingBuffer: function(center, outerRadius, innerRadius) {

		var buffer = this._createBuffer();
		var ctx = buffer.getContext('2d');
		var imageWidth = outerRadius*2;
		var imageHeight = imageWidth;

		if (this.frameDesign === 'paper'){
			ctx.fillStyle = '#FFFFFF';
			ctx.fill();
			outerRadius -=  6;
			ctx.fillStyle = '#FFFFFF';
			ctx.shadowOffsetX = 0;
			ctx.shadowOffsetY = 5;
			ctx.shadowBlur = 6;
			ctx.shadowColor =  '#ccc';
			ctx.strokeStyle = 'rgba(255, 255, 255, 0.3)';

			ctx.fill();
			ctx.fillStyle = '#FFFFFF';
			ctx.strokeStyle = 'rgba(255, 255, 255, 0.5)';
			ctx.beginPath();
			ctx.arc(center.x, center.y, outerRadius, 0, Math.PI * 2, true);
			ctx.closePath();
			ctx.fill();
			ctx.stroke();

			ctx.beginPath();
			ctx.arc(center.x, center.y, outerRadius * 0.990654, 0, Math.PI * 2, true);
			ctx.closePath();


		} else if (this.frameDesign === 'flat') {
			ctx.fillStyle = this.flatFrameColor;
			ctx.fill();
		} else {
			// outer gray frame

			ctx.fillStyle = '#848484';
			ctx.strokeStyle = 'rgba(132, 132, 132, 0.5)';
			ctx.beginPath();
			ctx.arc(center.x, center.y, outerRadius, 0, Math.PI * 2, true);
			ctx.closePath();
			ctx.fill();
			ctx.stroke();

			ctx.beginPath();
			ctx.arc(center.x, center.y, outerRadius * 0.990654, 0, Math.PI * 2, true);
			ctx.closePath();
		}
		var grad;
		var outerX = outerRadius*0.990654;
		var innerX = innerRadius;
		var fractions;
		var colors;

		switch(this.frameDesign) {
			case 'metal':
				grad = ctx.createLinearGradient(0, imageWidth * 0.004672, 0, imageHeight * 0.990654);
				grad.addColorStop(0, '#fefefe');
				grad.addColorStop(0.07, 'rgb(210, 210, 210)');
				grad.addColorStop(0.12, 'rgb(179, 179, 179)');
				grad.addColorStop(1, 'rgb(213, 213, 213)');
				ctx.fillStyle = grad;
				ctx.fill();
				break;

			case 'brass':
				grad = ctx.createLinearGradient(0, imageWidth * 0.004672, 0, imageHeight * 0.990654);
				grad.addColorStop(0, 'rgb(249, 243, 155)');
				grad.addColorStop(0.05, 'rgb(246, 226, 101)');
				grad.addColorStop(0.10, 'rgb(240, 225, 132)');
				grad.addColorStop(0.50, 'rgb(90, 57, 22)');
				grad.addColorStop(0.90, 'rgb(249, 237, 139)');
				grad.addColorStop(0.95, 'rgb(243, 226, 108)');
				grad.addColorStop(1, 'rgb(202, 182, 113)');
				ctx.fillStyle = grad;
				ctx.fill();
				break;

			case 'steel':
				grad = ctx.createLinearGradient(0, imageWidth * 0.004672, 0, imageHeight * 0.990654);
				grad.addColorStop(0, 'rgb(231, 237, 237)');
				grad.addColorStop(0.05, 'rgb(189, 199, 198)');
				grad.addColorStop(0.10, 'rgb(192, 201, 200)');
				grad.addColorStop(0.50, 'rgb(23, 31, 33)');
				grad.addColorStop(0.90, 'rgb(196, 205, 204)');
				grad.addColorStop(0.95, 'rgb(194, 204, 203)');
				grad.addColorStop(1, 'rgb(189, 201, 199)');
				ctx.fillStyle = grad;
				ctx.fill();
				break;

			case 'gold':
				grad = ctx.createLinearGradient(0, imageWidth * 0.004672, 0, imageHeight * 0.990654);
				grad.addColorStop(0, 'rgb(255, 255, 207)');
				grad.addColorStop(0.15, 'rgb(255, 237, 96)');
				grad.addColorStop(0.22, 'rgb(254, 199, 57)');
				grad.addColorStop(0.3, 'rgb(255, 249, 203)');
				grad.addColorStop(0.38, 'rgb(255, 199, 64)');
				grad.addColorStop(0.44, 'rgb(252, 194, 60)');
				grad.addColorStop(0.51, 'rgb(255, 204, 59)');
				grad.addColorStop(0.6, 'rgb(213, 134, 29)');
				grad.addColorStop(0.68, 'rgb(255, 201, 56)');
				grad.addColorStop(0.75, 'rgb(212, 135, 29)');
				grad.addColorStop(1, 'rgb(247, 238, 101)');
				ctx.fillStyle = grad;
				ctx.fill();
				break;

			case 'anthracite':
				grad = ctx.createLinearGradient(0, 0.004672 * imageHeight, 0, 0.995326 * imageHeight);
				grad.addColorStop(0, 'rgb(118, 117, 135)');
				grad.addColorStop(0.06, 'rgb(74, 74, 82)');
				grad.addColorStop(0.12, 'rgb(50, 50, 54)');
				grad.addColorStop(1, 'rgb(79, 79, 87)');
				ctx.fillStyle = grad;
				ctx.fill();
				break;

			case "tiltedGray":
				grad = ctx.createLinearGradient(0.233644 * imageWidth, 0.084112 * imageHeight, 0.81258 * imageWidth, 0.910919 * imageHeight);
				grad.addColorStop(0, '#ffffff');
				grad.addColorStop(0.07, 'rgb(210, 210, 210)');
				grad.addColorStop(0.16, 'rgb(179, 179, 179)');
				grad.addColorStop(0.33, '#ffffff');
				grad.addColorStop(0.55, '#c5c5c5');
				grad.addColorStop(0.79, '#ffffff');
				grad.addColorStop(1, '#666666');
				ctx.fillStyle = grad;
				ctx.fill();
				break;

			case 'tiltedBlack':
				grad = ctx.createLinearGradient(0.228971 * imageWidth, 0.079439 * imageHeight, 0.802547 * imageWidth, 0.898591 * imageHeight);
				grad.addColorStop(0, '#666666');
				grad.addColorStop(0.21, '#000000');
				grad.addColorStop(0.47, '#666666');
				grad.addColorStop(0.99, '#000000');
				grad.addColorStop(1, '#000000');
				ctx.fillStyle = grad;
				ctx.fill();
				break;
			case 'paper':
				ctx.fillStyle = '#FFFFFF';
				ctx.fill();
				ctx.stroke();
				break;

			case 'white':
				ctx.fillStyle = '#FFFFFF';
				ctx.fill();
				ctx.stroke();
				break;
			case 'flat':
				ctx.fillStyle = this.flatFrameColor;
				ctx.fill();
				ctx.stroke();
				break;
			case 'glossyMetal':
				grad = ctx.createRadialGradient(0.5 * imageWidth, 0.5 * imageHeight, 0, 0.5 * imageWidth, 0.5 * imageWidth, 0.5 * imageWidth);
				grad.addColorStop(0, 'rgb(207, 207, 207)');
				grad.addColorStop(0.96, 'rgb(205, 204, 205)');
				grad.addColorStop(1, 'rgb(244, 244, 244)');
				ctx.fillStyle = grad;
				ctx.fill();
				ctx.beginPath();
				ctx.arc(center.x, center.y, outerRadius, 0, Math.PI * 2);
				ctx.closePath();
				grad = ctx.createLinearGradient(0, imageHeight - 0.971962 * imageHeight, 0, 0.971962 * imageHeight);
				grad.addColorStop(0, 'rgb(249, 249, 249)');
				grad.addColorStop(0.23, 'rgb(200, 195, 191)');
				grad.addColorStop(0.36, '#ffffff');
				grad.addColorStop(0.59, 'rgb(29, 29, 29)');
				grad.addColorStop(0.76, 'rgb(200, 194, 192)');
				grad.addColorStop(1, 'rgb(209, 209, 209)');
				ctx.fillStyle = grad;
				ctx.fill();
				break;

			case 'blackMetal':
				fractions = [
					0,
					0.125,
					0.347222,
					0.5,
					0.680555,
					0.875,
					1
				];

				colors = [
					new RgbaColor(254, 254, 254, 1),
					new RgbaColor(0, 0, 0, 1),
					new RgbaColor(153, 153, 153, 1),
					new RgbaColor(0, 0, 0, 1),
					new RgbaColor(153, 153, 153, 1),
					new RgbaColor(0, 0, 0, 1),
					new RgbaColor(254, 254, 254, 1)
				];

				ctx.save();
				ctx.clip(ctx.arc(center.x, center.y, imageWidth * 0.990654 / 2, 0, Math.PI * 2, true));
				grad = new this.ConicalGradient(fractions, colors);
				grad.fillCircle(this,ctx, center.x, center.y, innerX, outerX);
				// fade outer edge
				ctx.strokeStyle = '#848484';
				ctx.strokeStyle = 'rgba(132, 132, 132, 0.8)';
				ctx.beginPath();
				ctx.lineWidth = imageWidth / 90;
				ctx.arc(center.x, center.y, imageWidth / 2, 0, Math.PI * 2, true);
				ctx.closePath();
				ctx.stroke();
				ctx.restore();
				break;

			case 'shinyMetal':
				fractions = [
					0,
					0.125,
					0.25,
					0.347222,
					0.5,
					0.652777,
					0.75,
					0.875,
					1
				];

				colors = [
					new RgbaColor(254, 254, 254, 1),
					new RgbaColor(210, 210, 210, 1),
					new RgbaColor(179, 179, 179, 1),
					new RgbaColor(238, 238, 238, 1),
					new RgbaColor(160, 160, 160, 1),
					new RgbaColor(238, 238, 238, 1),
					new RgbaColor(179, 179, 179, 1),
					new RgbaColor(210, 210, 210, 1),
					new RgbaColor(254, 254, 254, 1)];

				grad = new this.ConicalGradient(fractions, colors);
				grad.fillCircle(this,ctx, center.x, center.y, innerX, outerX);
				break;

			case 'chrome':
				fractions = [
					0,
					0.09,
					0.12,
					0.16,
					0.25,
					0.29,
					0.33,
					0.38,
					0.48,
					0.52,
					0.63,
					0.68,
					0.8,
					0.83,
					0.87,
					0.97,
					1
				];

				colors = [
					new RgbaColor(255, 255, 255, 1),
					new RgbaColor(255, 255, 255, 1),
					new RgbaColor(136, 136, 138, 1),
					new RgbaColor(164, 185, 190, 1),
					new RgbaColor(158, 179, 182, 1),
					new RgbaColor(112, 112, 112, 1),
					new RgbaColor(221, 227, 227, 1),
					new RgbaColor(155, 176, 179, 1),
					new RgbaColor(156, 176, 177, 1),
					new RgbaColor(254, 255, 255, 1),
					new RgbaColor(255, 255, 255, 1),
					new RgbaColor(156, 180, 180, 1),
					new RgbaColor(198, 209, 211, 1),
					new RgbaColor(246, 248, 247, 1),
					new RgbaColor(204, 216, 216, 1),
					new RgbaColor(164, 188, 190, 1),
					new RgbaColor(255, 255, 255, 1)
				];

				ctx.save();
				ctx.clip(ctx.arc(center.x, center.y, imageWidth * 0.990654 / 2, 0, Math.PI * 2, true));
				grad = new this.ConicalGradient(fractions, colors);
				grad.fillCircle(this,ctx, center.x, center.y, innerX, outerX);
				// fade outer edge
				ctx.strokeStyle = '#848484';
				ctx.strokeStyle = 'rgba(132, 132, 132, 0.8)';
				ctx.beginPath();
				ctx.lineWidth = imageWidth / 90;
				ctx.arc(center.x, center.y, imageWidth / 2, 0, Math.PI * 2, true);
				ctx.closePath();
				ctx.stroke();
				ctx.restore();
				break;

			default:
				ctx.fillStyle = 'transparent';
				ctx.fill();
		}

		if (this.frameDesign === 'paper')
		{
			// fade outer edge
			ctx.fillStyle = '#848484';
			ctx.strokeStyle = 'rgba(132, 132, 132, 0.8)';
			ctx.beginPath();
			ctx.lineWidth = outerRadius / 90;
			ctx.arc(center.x, center.y, outerRadius, 0, Math.PI * 2, true);
			ctx.closePath();
			ctx.stroke();
			ctx.restore();

			// inner bright frame
			ctx.strokeStyle = 'rgba(120, 120, 120, 0.8)';
			ctx.lineWidth = 2;
			ctx.beginPath();
			ctx.arc(center.x, center.y, innerRadius, 0, Math.PI * 2, true);
			ctx.closePath();
			ctx.stroke();
		}
		// clip out center so it is transparent if the background is not visible
		ctx.globalCompositeOperation = 'destination-out';
		ctx.beginPath();
		ctx.arc(center.x, center.y, innerRadius, 0, Math.PI*2, true);
		ctx.closePath();
		ctx.fill();

		return buffer;
	},
	getColorFromFraction: function (sourceColor, destinationColor, range, fraction, returnRawData) {
		var INT_TO_FLOAT = 1 / 255,
			sourceRed = sourceColor.getRed(),
			sourceGreen = sourceColor.getGreen(),
			sourceBlue = sourceColor.getBlue(),
			sourceAlpha = sourceColor.getAlpha(),

			deltaRed = destinationColor.getRed() - sourceRed,
			deltaGreen = destinationColor.getGreen() - sourceGreen,
			deltaBlue = destinationColor.getBlue() - sourceBlue,
			deltaAlpha = destinationColor.getAlpha() * INT_TO_FLOAT - sourceAlpha * INT_TO_FLOAT,

			fractionRed = deltaRed / range * fraction,
			fractionGreen = deltaGreen / range * fraction,
			fractionBlue = deltaBlue / range * fraction,
			fractionAlpha = deltaAlpha / range * fraction;

		returnRawData = returnRawData || false;
		if (returnRawData) {
			return [(sourceRed + fractionRed).toFixed(0), (sourceGreen + fractionGreen).toFixed(0), (sourceBlue + fractionBlue).toFixed(0), sourceAlpha + fractionAlpha];
		} else {
			return new RgbaColor((sourceRed + fractionRed).toFixed(0), (sourceGreen + fractionGreen).toFixed(0), (sourceBlue + fractionBlue).toFixed(0), sourceAlpha + fractionAlpha);
		}
	},

	/* copied from SteelSeries.js: */
	ConicalGradient: function (fractions, colors) {
		var limit = fractions.length - 1,
			i;

		// Pre-multipy fractions array into range -PI to PI
		for (i = 0; i <= limit; i++) {
			fractions[i] = TWO_PI * fractions[i] - PI;
		}

		this.fillCircle = function (parent,ctx, centerX, centerY, innerX, outerX) {
			var angle,
				radius = Math.ceil(outerX),
				diameter = radius * 2,
				pixels, alpha,
				x, y, dx, dy, dy2, distance,
				indx, pixColor,
				buffer, bufferCtx;

			// Create pixel array
			console.log('fillCircle: ctx = '+ctx+', ctx.createImageData = '+ctx.createImageData);
			pixels = ctx.createImageData(diameter, diameter);
			alpha = 255;

			for (y = 0; y < diameter; y++) {
				dy = radius - y;
				dy2 = dy * dy;
				for (x = 0; x < diameter; x++) {
					dx = x - radius;
					distance = Math.sqrt((dx * dx) + dy2);
					if (distance <= radius && distance >= innerX) { // pixels are transparent by default, so only paint the ones we need
						angle = Math.atan2(dx, dy);

						for (i = 0; i < limit; i++) {
							if (angle >= fractions[i] && angle < fractions[i + 1]) {

								pixColor = parent.getColorFromFraction(colors[i], colors[i + 1], fractions[i + 1] - fractions[i], angle - fractions[i], true);
							}
						}
						// The pixel array is addressed as 4 elements per pixel [r,g,b,a]
						indx = ((diameter - y) * diameter * 4) + (x * 4);  // plot is 180 rotated from orginal method, so apply a simple invert (diameter - y)
						pixels.data[indx]     = pixColor[0];
						pixels.data[indx + 1] = pixColor[1];
						pixels.data[indx + 2] = pixColor[2];
						pixels.data[indx + 3] = alpha;
					}
				}
			}

			// Create a new buffer to apply the raw data so we can rotate it
			buffer = parent._createBuffer(diameter, diameter);
			bufferCtx = buffer.getContext('2d');
			bufferCtx.putImageData(pixels, 0, 0);
			// Apply the image buffer
			ctx.drawImage(buffer, centerX - radius, centerY - radius);
		};

		this.fillRect = function (parent, ctx, centerX, centerY, width, height, thicknessX, thicknessY) {
			var angle,
				width2,
				height2,
				pixels, alpha,
				x, y, dx, dy,
				indx,
				pixColor,
				buffer, bufferCtx;

			width = Math.ceil(width);
			height = Math.ceil(height);
			width2 = width / 2;
			height2 = height / 2;
			thicknessX = Math.ceil(thicknessX);
			thicknessY = Math.ceil(thicknessY);

			// Create pixel array
			pixels = ctx.createImageData(width, height);
			alpha = 255;

			for (y = 0; y < height; y++) {
				dy = height2 - y;
				for (x = 0; x < width; x++) {
					if (y > thicknessY && y < height - thicknessY) {
						// we are in the range where we only draw the sides
						if (x > thicknessX && x < width - thicknessX) {
							// we are in the empty 'middle', jump to the next edge
							x = width - thicknessX;
						}
					}
					dx = x - width2;
					angle = Math.atan2(dx, dy);
					for (i = 0; i < limit; i++) {
						if (angle >= fractions[i] && angle < fractions[i + 1]) {
							pixColor = parent.getColorFromFraction(colors[i], colors[i + 1], fractions[i + 1] - fractions[i], angle - fractions[i], true);
						}
					}
					// The pixel array is addressed as 4 elements per pixel [r,g,b,a]
					indx = ((height - y) * width * 4) + (x * 4); // plot is 180 rotated from orginal method, so apply a simple invert (height - y)
					pixels.data[indx]     = pixColor[0];
					pixels.data[indx + 1] = pixColor[0];
					pixels.data[indx + 2] = pixColor[0];
					pixels.data[indx + 3] = alpha;
				}
			}
			// Create a new buffer to apply the raw data so we can clip it when drawing to canvas
			buffer = parent._createBuffer(width, height);
			bufferCtx = buffer.getContext('2d');
			bufferCtx.putImageData(pixels, 0, 0);

			// draw the buffer back to the canvas
			ctx.drawImage(buffer, centerX - width2, centerY - height2);
		};

	},

	_createDialBuffer: function(center, outerRadius, innerRadius) {
		var buffer = this._createBuffer();
		var ctx = buffer.getContext('2d');

		var imageWidth = outerRadius*2;
		var imageHeight = imageWidth;

		ctx.beginPath();
		ctx.arc(center.x, center.y, innerRadius, 0, Math.PI*2, true);
		ctx.closePath();

		var fractions;
		var colors;
		console.log('this._dialDesign = '+this._dialDesign);

		if (this._dialDesign === this.steelSeries.BackgroundColor.CARBON || this._dialDesign === this.steelSeries.BackgroundColor.PUNCHED_SHEET ||
			this._dialDesign === this.steelSeries.BackgroundColor.BRUSHED_METAL || this._dialDesign === this.steelSeries.BackgroundColor.BRUSHED_STAINLESS) {

			if (this._dialDesign === this.steelSeries.BackgroundColor.CARBON) {
				ctx.fillStyle = ctx.createPattern(this.steelSeries.CarbonBuffer, 'repeat');
				ctx.fill();
			}

			if (this._dialDesign === this.steelSeries.BackgroundColor.PUNCHED_SHEET) {
				ctx.fillStyle = ctx.createPattern(this.steelSeries.PunchedSheetBuffer, 'repeat');
				ctx.fill();
			}

			// Add another inner shadow to make the look more realistic
			var backgroundOffsetX = imageWidth / 2;
			var fadeGradient = ctx.createLinearGradient(backgroundOffsetX, 0, imageWidth - backgroundOffsetX, 0);
			fadeGradient.addColorStop(0, 'rgba(0, 0, 0, 0.25)');
			fadeGradient.addColorStop(0.5, 'rgba(0, 0, 0, 0)');
			fadeGradient.addColorStop(1, 'rgba(0, 0, 0, 0.25)');
			ctx.fillStyle = fadeGradient;
			ctx.beginPath();
			ctx.arc(center.x, center.y, innerRadius, 0, Math.PI * 2, true);
			ctx.closePath();
			ctx.fill();

			if (this._dialDesign === this.steelSeries.BackgroundColor.BRUSHED_METAL || this._dialDesign === this.steelSeries.BackgroundColor.BRUSHED_STAINLESS) {
				ctx.beginPath();
				ctx.arc(center.x, center.y, innerRadius, 0, 2 * Math.PI, false);
				ctx.closePath();

				var mono = (this._dialDesign === this.steelSeries.BackgroundColor.BRUSHED_METAL ? true : false);
				var textureColor = parseInt(this._dialDesign.gradientStop.getHexColor().substr(-6), 16);
				var texture = this.steelSeries.brushedMetalTexture(textureColor, 5, 0.1, mono, 0.5);
				ctx.fillStyle = ctx.createPattern(texture.fill(0, 0, imageWidth, imageHeight), 'no-repeat');
				ctx.fill();
			}


		} else if (this._dialDesign === this.steelSeries.BackgroundColor.TURNED) {
			// Define the fraction of the conical gradient paint
			fractions = [
				0,
				0.03,
				0.10,
				0.14,
				0.24,
				0.33,
				0.38,
				0.5,
				0.62,
				0.67,
				0.76,
				0.81,
				0.85,
				0.97,
				1 ];

			// Define the colors of the conical gradient paint
			colors = [
				new RgbaColor('#FDFDFD'),
				new RgbaColor('#FDFDFD'),
				new RgbaColor('#B2B2B4'),
				new RgbaColor('#ACACAE'),
				new RgbaColor('#FDFDFD'),
				new RgbaColor('#6E6E70'),
				new RgbaColor('#6E6E70'),
				new RgbaColor('#FDFDFD'),
				new RgbaColor('#6E6E70'),
				new RgbaColor('#6E6E70'),
				new RgbaColor('#FDFDFD'),
				new RgbaColor('#ACACAE'),
				new RgbaColor('#B2B2B4'),
				new RgbaColor('#FDFDFD'),
				new RgbaColor('#FDFDFD')];

			var grad = new this.ConicalGradient(fractions, colors);
			grad.fillCircle(this,ctx, center.x, center.y, 0, innerRadius);
			ctx.closePath();
			console.log('this._dialDesign = '+this._dialDesign);
			if (this._dialDesign === this.steelSeries.BackgroundColor.TURNED) {
				var TWO_PI = Math.PI * 2;
				// Define the turning radius
				var radius = innerRadius;
				var turnRadius = radius * 0.55;
				// Step size proporational to radius
				var stepSize = TWO_PI / 360 * (500 / radius);

				// Save before we start
				ctx.save();
				// restrict the turnings to the desired area
				ctx.clip(ctx.arc(center.x, center.y, radius, 0, TWO_PI));

				// set the style for the turnings
				ctx.lineWidth = 0.5;

				var end = TWO_PI - stepSize * 0.3;
				// Step the engine round'n'round
				for (var i = 0 ; i < end; i += stepSize) {
					// draw a 'turn'
					ctx.strokeStyle = 'rgba(240, 240, 255, 0.25)';
					ctx.beginPath();
					ctx.arc(center.y + turnRadius, center.y, turnRadius, 0, TWO_PI);
					ctx.stroke();
					// rotate the 'piece'
					ctx.translate(center.x, center.x);
					ctx.rotate(stepSize * 0.3);
					ctx.translate(-center.x, -center.y);
					// draw a 'turn'
					ctx.strokeStyle = 'rgba(25, 10, 10, 0.1)';
					ctx.beginPath();
					ctx.arc(center.x + turnRadius, center.y, turnRadius, 0, TWO_PI);
					ctx.stroke();
					ctx.translate(center.x, center.y);
					ctx.rotate(-stepSize * 0.3);
					ctx.translate(-center.x, -center.y);

					// rotate the 'piece'
					ctx.translate(center.x, center.y);
					ctx.rotate(stepSize);
					ctx.translate(-center.x, -center.y);
				}
				// Restore canvas now we are done
				ctx.restore();
			}

		} else if (this.dialDesign === 'flat') {
			ctx.fillStyle = this.flatFaceColor;
			ctx.fill();
		} else {
			var grad = ctx.createLinearGradient(0, imageWidth*0.084112, 0, imageHeight*0.831775);
			grad.addColorStop(0, this._dialDesign.gradientStart.getRgbaColor());
			grad.addColorStop(0.4, this._dialDesign.gradientFraction.getRgbaColor());
			grad.addColorStop(1, this._dialDesign.gradientStop.getRgbaColor());
			ctx.fillStyle = grad;
			ctx.fill();
		}


		// outer dark frame
		ctx.strokeStyle = 'rgba(132, 132, 132, 0.5)';
		ctx.lineWidth = 2;
		ctx.beginPath();
		ctx.arc(center.x, center.y, innerRadius*0.9995, 0, Math.PI * 2, true);
		ctx.closePath();
		ctx.stroke();

		return buffer;
	},

	/********************************************************************************************
	 /* Mouse input methods
	 /********************************************************************************************/
	_getElOffsets: function(el) {
		var offsets = {
			left: el.offsetLeft || 0,
			top: el.offsetTop || 0
		};

		// loop through ancestors while anestor is an offsetParent
		while (el = el.offsetParent) {
			// sum the offset values
			offsets.left += el.offsetLeft || 0;
			offsets.top += el.offsetTop || 0;
		}

		return offsets;
	},
	_getMouseInputLocation: function(canvas,x, y) {

		//BC: for more info, see http://stackoverflow.com/questions/10626292/determining-mouse-position-on-an-html5-canvas-after-zooming
		//var pos = domGeom.position(this.domNode);
		x = x - canvas.offsetLeft;
		y = y - canvas.offsetTop;

		var pt = {x:x, y:y};
		var center = this._getCenter(canvas);

		pt.x = pt.x - center.x;
		pt.y = pt.y - center.y;

		var distance = Math.sqrt(pt.x*pt.x + pt.y*pt.y);
		var angle =  Math.atan2(pt.y, pt.x) / this._RADIAN;
		return {distance:distance, angle:angle};
	},


	_hitTest: function(pt) {
		var maxRadius = this._getRadius(this._mainCanvas) * this._innerRadius;
		var result = (pt.distance >= 0 && pt.distance <= maxRadius);
		console.log("ti-widget-dial:_hitTest = "+result+", pt.distance="+pt.distance+", maxRadius="+maxRadius+", pt.angle="+pt.angle);
		return(result);
	},

	_mouseDown: function(event) {
		console.log('in _mouseDown');

		if (event.button == 0) { // left mouse button
			var bounds = this._getBounds(this._mainCanvas);
			var rect = this._mainCanvas.getBoundingClientRect();
			var root = document.documentElement;
			var elementOffsets = this._getElOffsets(this._mainCanvas);
			var xe = event.clientX - elementOffsets.left;//rect.left - root.scrollLeft;
			var x = event.clientX - rect.left;
			var ye = event.clientY - elementOffsets.top;// rect.top - root.scrollTop;
			var y = event.clientY - rect.top;
			var pt = this._getMouseInputLocation(this._mainCanvas, x, y);
			if (this._hitTest(pt, bounds)) {

				this._dragging = true;
				this._startAngle = pt.angle;
				this._oldValue = this.value;
				this._valueDiff = 0;
				console.log('Drag started startAngle = ' + this._startAngle);
			}
		}
	},

	_mouseUp: function(event) {
		console.log('in _mouseUp: this._dragging = '+this._dragging);
		if (event.button == 0) {
			this._dragging = false;
			var elementOffsets = this._getElOffsets(this._mainCanvas);
			var rect = this._mainCanvas.getBoundingClientRect();
			var xe = event.clientX - elementOffsets.left;//rect.left - root.scrollLeft;
			var x = event.clientX - rect.left;
			var ye = event.clientY - elementOffsets.top;// rect.top - root.scrollTop;
			var y = event.clientY - rect.top;
			var pt = this._getMouseInputLocation(this._mainCanvas, x, y);


//				console.log('Drag ended endAngle = ' + pt.angle);
			this._setValueWithMouse(pt.angle);
		}
	},

	_mouseMove: function(event) {
		//console.log('in _mouseMove');
		if (this._dragging) {
			var elementOffsets = this._getElOffsets(this._mainCanvas);
			var rect = this._mainCanvas.getBoundingClientRect();
			var xe = event.clientX - elementOffsets.left;//rect.left - root.scrollLeft;
			var x = event.clientX - rect.left;
			var ye = event.clientY - elementOffsets.top;// rect.top - root.scrollTop;
			var y = event.clientY - rect.top;
			var pt = this._getMouseInputLocation(this._mainCanvas, x, y);
			this._setValueWithMouse(pt.angle);
			var speedVector = this._getSpeedVector(this._needleValue, this.value);
			this._updateNeedle(speedVector, 0);
		}
	},

	_mouseEnter: function(event) {
		console.log('in _mouseEnter');

	},

	_mouseOut: function(event) {
		console.log('in _mouseOut');

	},

	_setValueWithMouse: function(endAngle) {

		var diff = endAngle - this._startAngle;

		if (diff < -180.0) {
			diff += 360.0;
		} else if (diff > 180.0) {
			diff -= 360.0;
		}
		this._valueDiff += diff / (2.0 * this._halfArcLength);
		//console.log("setValueWithMouse: startAngle="+this._startAngle+", endAngle="+endAngle+", diff="+diff+", valueDiff="+this._valueDiff);
		this._startAngle = endAngle;

		var value = this._oldValue + (this._valueDiff * (this.maxValue - this.minValue));

		var setValue = this._dragging ? this.setValueOnDrag : true;
		//window.gc_dial_value = value;  //bc
		if (setValue) {
			console.log('in _setValueWithMouse: value='+value);
			this._setValueAttr(value);
		} else {
			this._setAnimateValueAttr(value);
		}
		this._redraw();//bc
	},

	/********************************************************************************************
	 /* Utilities methods
	 /********************************************************************************************/
	_updateNeedle: function(speedVector, frame) {
		//var value = this.value;
		var value = this._animateValue;

		if (this.animate && window.requestAnimFrame) {
			var isInc = true;
			if (this._needleValue > value) {
				isInc = false;
			}

			if (frame >= speedVector.length || frame < 0) {
				this._needleValue  = value;

			} else {
				var inc = isInc ? speedVector[frame] : -speedVector[frame];
				if (((this._needleValue+inc >= value) && (inc > 0) ) || ((this._needleValue+inc <= value) && (inc < 0))) {
					this._needleValue = value;
				} else {
					this._needleValue = this._needleValue+inc;
				}
			}

//				log('Refreshing gauge needle ' + this._needleValue);
			this._redraw();

			if (this._needleValue != value) {
				var _self = this;
				window.requestAnimFrame(function() {
					_self._updateNeedle(speedVector, ++frame);
				});
			}

		} else {
			this._needleValue = value;
			this._redraw();
		}
	},

	_createBuffer: function() {
		var buffer = document.createElement('canvas');
		console.log('createBuffer: buffer.ctx = '+buffer.getContext('2d'));
		buffer.width = this.size;
		buffer.height = this.size;
		return buffer;
	},

	_computeHashMarkMarkIncrement: function() {
		this._hashMarkIncrement = this._halfArcLength;
		if (this._numberHashMarks == 0) {
			this._hashMarkIncrement = 2 * this._hashMarkIncrement / (this._numberSmallHashMarks + (this._numberSmallHashMarks > 1 ? -1 : 1));
		} else if (this._numberHashMarks == 1) {
			this._hashMarkIncrement = this._hashMarkIncrement / Math.max(this._numberSmallHashMarks, 1) ;
		} else {
			this._hashMarkIncrement = 2 * this._hashMarkIncrement / ((this._numberHashMarks - 1) * (this._numberSmallHashMarks + 1));
		}
	},

	_getNeedleStartAngle: function() {
		var remainder = (360-(this._halfArcLength*2));
		return this._halfArcLength*2 - remainder;
	},

	_getNeedleAngleOffset: function() {
		this._needleValue = this.value;  //bc
		return Math.abs(this._needleValue - this.minValue) / Math.abs(this.maxValue - this.minValue) * (2*this._halfArcLength);
	},

	_getBounds: function(canvas) {
		return {
			x: 0,
			y: 0,
			width: canvas.width,
			height: canvas.height
		};
	},

	_getRadius: function(canvas) {
		return Math.floor(Math.min(canvas.width/2, canvas.height/2));
	},

	_getCenter: function(canvas) {
		var centerX = Math.floor(canvas.width/2);
		var centerY = Math.floor(canvas.height/2);
		return {
			x: centerX,
			y: centerY
		};
	},

	_createLables: function(minValue, maxValue) {
		var labels = new Array();
		var numberHashMarks = 10;//this._numberHashMarks;  //bc was 10
		var nextTick = minValue;

		var tickInc = Math.abs(maxValue - minValue) / numberHashMarks;
		for (var i = 0; i < numberHashMarks+1; ++i) {
			labels.push(this._formatLabelText(nextTick));
			nextTick += tickInc;
		}
		return labels;
	},

	_formatLabelText: function(label) {
		switch (this.labelNumberFormat.format) {
			case 'fractional':
				return label.toFixed(this.fractionalScaleDecimals);
			case 'scientific':
				return label.toPrecision(2);
			case 'standard':
			default:
				return label.toFixed(0);
		}
	},

	_getSpeedVector: function(startNeedle, endNeedle) {
		var tmp = startNeedle;
		if (startNeedle > endNeedle) {
			tmp = endNeedle;
			endNeedle = startNeedle;
			startNeedle = tmp;
		}

		var totalDistance = this.maxValue - this.minValue;
		var maxSpeed = totalDistance * (10/(2*this._halfArcLength));
		var acceleration = maxSpeed/20;

		var distance = endNeedle-startNeedle;
		var halfDistance = distance/2;
		var retArray = new Array();
		var S = startNeedle;
		var V = acceleration;
		while (S < startNeedle+halfDistance) {
			retArray.push(V);
			if (V < maxSpeed)
				V = V + acceleration;
			S = S + V;
		}

		var index = retArray.length-1;
		for (; index >= 0; index--) {
			retArray.push(retArray[index]);
		}

		return retArray;
	},

	_initAnimation: function() {
		window.requestAnimFrame = (function(callback) {
			return window.requestAnimationFrame ||
				window.webkitRequestAnimationFrame ||
				window.mozRequestAnimationFrame ||
				window.oRequestAnimationFrame ||
				window.msRequestAnimationFrame ||
				function(callback){
					window.setTimeout(callback, 1000 / 60);
				};
		})();
	},
	_calSize:function(){
		var w1=0,h1=0,s1=0;
		//h1 = domStyle.get(this.domNode,"height");
		//w1= domStyle.get(this.domNode,"width");
		h1 = this.size;
		w1 = this.size;
		var dh=Math.abs(this.size-h1);
		var dw=Math.abs(this.size-w1);
		if(dh<this.dt&&dw<this.dt)
			return 0;
		if(dh>=this.dt&&dw<this.dt)
			s1=h1;
		else if(dh<this.dt&&dw>=this.dt)
			s1=w1;
		else {
			s1=(w1<h1)?w1:h1;
		}
		s1=Math.round(s1);
		if(s1>0)
			this.set("size",s1);
		if(this.size<=0)
			this.size=150;
		return s1;
	},
	_setStyleAttr:function(s){
		/*this.inherited(arguments);
		var s1=this._calSize();
		if(this._started&&s1>0){
			domStyle.set(this.domNode, 'height', this.size + "px");
			domStyle.set(this.domNode, 'width', this.size + "px");
			this.domNode.width = s1;
			this.domNode.height = s1;
			this._set("style",this.domNode.style.cssText);
			if(this.domNode._dvWidget){
				this.domNode._dvWidget._srcElement.setAttribute("style",this.style);
				this.domNode._maqDeltas = undefined;				}
			delete this._outerRingBuffer;
			delete this._dialBuffer;
			this._redraw();
		}
		*/
	},

/*	resize:function(){
		this.set("style",this.style);

	},
	*/
	// find canvas
	findCanvas: function() {
		console.log('this.id = '+this.id);
		return (this.root || document).querySelector("[id=" + this.id + "]");

	}


};


// Find all canvas elements that have the SteelSeries class and turns them into clocks
// Config contains some defaults, and skins
Dial.config = {
	value: 0,
	minValue: 0,
	maxValue: 100,
	size: 177,
	defaultSkin: "brushedStainless",
	defaultRadius: 85,
	renderRadius: 100,

	skins:	{
		// There are more skins in moreskins.js
		// Try making your own skin by copy/pasting one of these and tweaking it
		brushedStainless: {
			backgroundColor: 'brushedStainless',
			frameDesign: 'glossyMetal'
		},
		gold: {
			backgroundColor: 'brushedMetal',
			frameDesign: 'gold'

		},
		carbon: {
			backgroundColor: 'brushedMetal',
			frameDesign: 'blackMetal'
		}

	},

	// Test for IE so we can nurse excanvas in a couple of places
	isIE: !!document.all,

	// Will store (a reference to) each clock here, indexed by the id of the canvas element
	clockTracker: {},

	// For giving a unique id to coolclock canvases with no id
	noIdCount: 0
};

window.Dial.findAndCreateDials = function(root) {
	// (Let's not use a jQuery selector here so it's easier to use frameworks other than jQuery)
	var canvases = (root || document).getElementsByTagName("canvas");
	for (var i=0;i<canvases.length;i++) {
		// Pull out the fields from the class. Example "SteelSeries:chunkySwissOnBlack:1000"
		var fields = canvases[i].className.split(" ")[0].split(":");
		console.log('in findAndCreateDials: canvases[i].className = '+canvases[i].className);
		if (fields[0] == "SteelSeries") {

			if (!canvases[i].id) {
				// If there's no id on this canvas element then give it one
				canvases[i].id = '_dial_auto_id_' + Dial.config.noIdCount++;
			}
			console.log('Dial.findAndCreateDials: canvases.length= '+canvases.length+', i='+i);
			console.log('canvases[i].id = '+canvases[i].id);
			// Create a gauge object for this element
			var obj = new Dial({
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
			obj.startup();
			//obj.repaint();

			return(obj);
		}
	}
};


