/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
  
  Need help? http://forum.ardumote.com
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

const int buttonPin = A1;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(115200);
  mySwitch.enableReceive(1);  // Receiver on inerrupt 0 => that is pin #2
  mySwitch.enableTransmit(6);
  pinMode(buttonPin, INPUT);
}

void loop() {
  if(buttonState != digitalRead(buttonPin)){
    buttonState = digitalRead(buttonPin);
    if(buttonState == HIGH){
      mySwitch.send(1328465, 24);
    }
  }
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
    }

    mySwitch.resetAvailable();
  }
}
