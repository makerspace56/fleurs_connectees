/*
  Analog input, analog output, serial output

 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

// Photoresistance avec pont 220 Ohm: 
// Interne avec neon = 500 +-20
// Interne sans neon = 240
// Dehors soleil ciel blanc hier 16h = 840

// Photoresistance avec pont 1k Ohm: 
// Interne avec neon = 115
// Interne sans neon = 50
// Dehors soleil ciel blanc hier 16h = 640

// Dehors soleil ciel blanc hier 16h = 840


// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the pot

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  // print the results to the serial monitor:
  Serial.print("sensor = ");
  Serial.println(sensorValue);

  // Wait for 1s
  delay(1000);
}
