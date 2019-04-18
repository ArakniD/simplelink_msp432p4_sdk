/*****************************************************************
 * Copyright (c) 2013-2014 Texas Instruments and others
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

(function() 
{
	var DataConverterTest = function()
	{
		
	}; 
	
	var converter = gc.databind.DataConverter;
	var testCount = 0;
	var testExpression = "";
	
	var fail = function(message)
	{
		console.log("Failed test " + testCount + ' "' + testExpression + '" : ' + message);
	};
	
	var assertNull = function(value)
	{
		if (value !== null && value !== undefined)
		{
			fail("non null value found.  Expected it to be null");
		}
	};
	
	var assertNotNull = function(value)
	{
		if (value === null || value === undefined)
		{
			fail("null value found.  Expected it to be not null");
		}
	};
	
	var assertTrue = function(value)
	{
		if (value !== true)
		{
			fail("assertion failed. Expected true result but it was false instead.");
		}
	};
	
	var assertEquals = function(expected, actual)
	{
		if (actual != expected)
		{
			fail("got '" + actual +"' but was expecting '" + expected +"'");
		}
	};
	
	var evaluate = function(result, source, destinationType)
	{
		testCount++;
		testExpression = source.toString() + ' --> ' + destinationType;
		
		try
		{
			var convertedResult = converter.convert(source, typeof source, destinationType);
			assertEquals(result, convertedResult);
		}
		catch(e)
		{
			assertEquals(result, e.toString());
		}
	};
	
	var evalString = function(fmt, source)
	{
		var formatter = new Formatter();
		try
		{
			var result = new FormattedString(formatter.format(fmt, source).toString(), fmt);
			evaluate(result, source, new FormattedString("", fmt));
			evaluate(source, result, source);
		}
		finally
		{
			formatter.close();
		}
	};
	
	DataConverterTest.prototype.test = function()
	{
		// numeric conversion
		var A = 13;
		var B = 263;
		var C = 19;
		var D = -2343;
		var X = 11883.54834;
		var Y = -1893.94943;
		var BOOL = true;
		var S = "test with a space";
		
		evaluate(Number(A), A, "number");
		evaluate(Number(B), B, "number");
		evaluate(Number(C), C, "number");
		evaluate(Number(D), D, "number");
		evaluate(Number(X), X, "number");
		evaluate(Number(Y), Y, "number");
		evaluate(Number(BOOL), BOOL, "number");

		// String conversions
		evaluate(String(S), S, "string");
		
		evaluate(String(A), A, "string");
		evaluate(String(B), B, "string");
		evaluate(String(C), C, "string");
		evaluate(String(D), D, "string");
		evaluate(String(X), X, "string");
		evaluate(String(Y), Y, "string");
		evaluate(String(BOOL), BOOL, "string");
		
		// formatted string conversions
		evalString("%d", new Integer(A));
		evalString("%d", new Long(B));
		evalString("%d", new Short(D));
		evalString("%d", new Byte(C));
		evalString("%f", new Double(X));
		evalString("%f", new Float(Y));

		evalString("0x%x", new Integer(A));
		evalString("0x%x", new Long(B));
//		evalString("0x%x", new Short(D));
		evalString("0x%x", new Byte(C));
//		evalString("%5.3f", new Double(X));
//		evalString("%5.3f", new Float(Y));
		
		evalString("0", new Integer(0));

	};
	
	return new DataConverterTest();
	
}().test());