
#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 5); // RX, TX
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by


void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
}

void loop() { // run over and over
  if (Serial.available()) {
    Serial.read();
    uint8_t reading = 128;
    reading = 90;
    mySerial.write(reading);
    Serial.println(reading);
  }
  
  mySerial.write(brightness);
  //Serial.println((uint8_t)brightness);
  
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
  
}
