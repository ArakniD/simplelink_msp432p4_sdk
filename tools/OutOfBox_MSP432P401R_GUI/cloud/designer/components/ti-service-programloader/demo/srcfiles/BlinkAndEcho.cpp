#line 1 "BlinkWithSerialDataOutput_bcEcho.ino"
/* Blink without Delay
 
 Turns on and off a light emitting diode(LED) connected to a digital  
 pin, without using the delay() function.  This means that other code
 can run at the same time without being interrupted by the LED code.
 
 The circuit:
 * LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 that's attached to pin 13, so no hardware is needed for this example.
 
 
 created 2005
 by David A. Mellis
 modified 8 Feb 2010
 by Paul Stoffregen
 modified 27 Apr 2012
 by Robert Wessels
 
 This example code is in the public domain.

 
 http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
 */

// constants won't change. Used here to 
// set pin numbers:
#include "Energia.h"
void setup();
void loop();
#line 28
const int ledPin =  GREEN_LED;      // the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
unsigned int previousValue = 0;        // will store last time LED was updated
unsigned int lastValue = 0;
unsigned int incomingByte = 0;
unsigned long previousMillis = 0;
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 8;           // interval at which to blink (milliseconds)

void setup() {
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);    
  Serial.begin(9600);   
}

void loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
  unsigned long currentMillis = millis();
  if (currentMillis > (previousMillis + interval)){
     previousMillis = currentMillis;
  if ((lastValue & 0xC000) != (previousValue & 0xC000)) {
    // save the last time you blinked the LED 

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
         // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    }
    previousValue = lastValue;   
      
    lastValue += 100;
            // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte,HEX );
        }
    //Serial.println(String(lastValue, DEC));
  }

  
}


