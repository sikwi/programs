/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

#include <SoftwareSerial.h>

SoftwareSerial mySerial(A1, 13); // RX, TX

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(A0, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  
  mySerial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}
