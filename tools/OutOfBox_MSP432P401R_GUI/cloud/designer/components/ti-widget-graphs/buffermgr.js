function LimitedArray(length) {
  this.defaultSize = 2000;
  this.setSize(length);
  this.reset();
}

LimitedArray.prototype.reset = function() {
  this.buffer = [];
  this.min = 0;
  this.max = 0;
}

LimitedArray.prototype.setSize = function(length) {
  this.size = length;
}

LimitedArray.prototype.calMinMax = function(data) {
  if(this.buffer.length == 0) {
    this.min = data;
    this.max = data;
  }
  else {
    this.min = Math.min(data, this.min);
    this.max = Math.max(data, this.max);
  }
}

LimitedArray.prototype.push = function(data) {
  this.calMinMax(data);
  
  if(this.buffer.length == this.size) {
    this.buffer.shift();
  }
  this.buffer.push(data);
  return this.buffer;
}

LimitedArray.prototype.concat = function(array) {
  var trim = array.length + this.buffer.length - this.size;
  if(trim > 0) {
    this.buffer = this.buffer.slice(trim);
  }
  for(i=0; i<array.length; i++) {
    this.push(array[i]);
  }
  return this.buffer;
}

LimitedArray.prototype.getArray = function() {
  return this.buffer;
}

LimitedArray.prototype.isFull = function() {
  return this.buffer.length >= this.size;
}

LimitedArray.prototype.generate = function(start, delta, length) {
  this.reset();
  for(i=0, x=start; i<length; i++, x+=delta) {
    this.push(x);
  }
  return this.buffer;
}

LimitedArray.prototype.merge = function(array) {
  var len1 = this.buffer.length;
  var len2 = array.buffer.length;
  var len = Math.min(len1, len2);
  var darry = [];
  for(i=0;i<len;i++) {
    darry.push([this.buffer[i],array.buffer[i]]);
  }
  return darry;
}

function MultiLimitedArray(width, length) {
  this.defaultSize = 2;
  this.setSize(width, length);
  
  this.arrays = [];
  for(i=0; i<this.width; i++) {
    var array = new LimitedArray(this.length);
    this.arrays.push(array);
  }

  this.reset();
}

MultiLimitedArray.prototype.setSize = function(width, length) {
  this.width = width;
  this.length = length;
}

MultiLimitedArray.prototype.reset = function() {
  for(i=0; i<this.width; i++) {
    this.arrays[i].reset();
  }
}

MultiLimitedArray.prototype.getArrays = function() {
  return this.arrays;
}

MultiLimitedArray.prototype.getArray = function(index) {
  return this.arrays[index];
}

MultiLimitedArray.prototype.push = function(index, data) {
  var _array = this.arrays[index];
  _array.push(data);
  return _array;
}

MultiLimitedArray.prototype.concat = function(index, inputArray) {
  var _array = this.arrays[index];
  _array.concat(inputArray);
  return _array;
}

MultiLimitedArray.prototype.getMerged = function(index1, index2) {
  var _array1 = this.arrays[index1];
  var _array2 = this.arrays[index2];
  return _array1.merge(_array2);
}

function TickGenerator(initVal, freq) {
  this.initVal = initVal;
  this.period = 1.0/freq;
  this.reset();
}

TickGenerator.prototype.reset = function() {
  return this.resetTo(this.initVal);
}

TickGenerator.prototype.resetTo = function(initVal) {
  this.value = initVal;
  return this.value;
}

TickGenerator.prototype.next = function() {
  this.value += this.period;
  return this.value;
}

TickGenerator.prototype.getValue = function() {
  return this.value;
}