// Example of creating computed bindings.  This code registers a getter and setter between a date widget and day, month, and year widgets.  
// And, a getter between a raw value and a rounded value.

document.addEventListener('gc-databind-ready', function() 
{
    /* 
    *   function gc.databind.registry.bind(targetBinding, modelBinding, getter, [setter]);
    */
    gc.databind.registry.bind('widget.date.value', 
        // dependant bindings needed in order to compute the date, in name/value pairs.
        {
            weekday: 'widget.dayOfWeek.selectedText',
            day: 'widget.dayOfMonth.value',
            month: 'widget.month.selectedText',
            year: 'widget.year.value'
        }, 
        // getter for date computation
        function(values) 
        {
            // compute and return the string value to bind to the widget with id 'date'
            return values.weekday + ', ' + values.month + ' ' + values.day + ', ' + values.year;
            
        }, 
        // setter for date calculation (optional - needed for two-way databinding)
        function(value) 
        {
            // return an object with name value pairs for all the dependent bindings that need to change
            // based on new value of the widget 
            value = value.toString();  // make sure we start with a string.
            
            var fields = value.split(',');
            if (fields.length != 3)
            {
                return {};  // commit no changes, can't parse input
            }
            var result = 
            { 
                weekday : fields[0].trim(),
                year : fields[2].trim()
            };
            fields = fields[1].trim().split(' ');
            if (fields.length != 2)
            {
                return {};  // commit no changes, can't parse month and day
            }
            result.day = +(fields[1]);  // convert to number
            result.month = fields[0];
            
            return result; // commit changes back to dependant bindings.
        }
    );
    
    /* 
    *   function gc.databind.registry.bind(targetBinding, modelBinding, getter, [setter]);
    */
    gc.databind.registry.bind('widget.rounded_value.label', 'widget.raw_value.value', 
        // getter for computing the rounded value
        function(value)
        {
            return 'rounded value = ' + Math.round(+value);  // using unary plus (+) operator to convert string to number
        }
    );
});





