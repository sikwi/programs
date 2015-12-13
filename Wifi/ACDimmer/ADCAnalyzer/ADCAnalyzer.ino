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

#define  fullOn    10
#define  fullOff   110
#define  FQ_50      1 // in case of 50Hz
#define  FQ_60      0 // in case of 50Hz
#define  VER       "2.0"

int inbyte;
int AC_LOAD = 0;    // Output to Opto Triac pin
int dimming = 90;  // Dimming level (0-128)  0 = ON, 128 = OFF

boolean frq = FQ_50;     // change the frequency here. 
boolean countUp = false;
boolean simulate = true; 

const int analogInPin = 2;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 1; // Analog output pin that the LED is attached to

float sensorValue = 0;        // value read from the pot
float outputValue = 0;        // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  analogReference(DEFAULT);
  pinMode(AC_LOAD, OUTPUT);        // Set the AC Load as output
  pinMode(1, OUTPUT);        // Set the AC Load as output
  attachInterrupt(0, zero_crosss_int, RISING);  // Choose the zero cross interrupt # from the table above  

}

void zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation
  // 50Hz-> 10ms (1/2 Cycle) → (10000us - 10us) / 128 = 78 (Approx)
  // 60Hz-> 8.33ms (1/2 Cycle) → (8333us - 8.33us) / 128 = 65 (Approx)
  int dimtime = 0;
  float propTime = 0;
  if(frq){
    dimtime = (77*dimming);
    propTime = 10.0; 
  }
  else{
    dimtime = (65*dimming);
    propTime = 8.33; 
  }
  
  delayMicroseconds(dimtime);    // Off cycle
  digitalWrite(AC_LOAD, HIGH);   // triac firing
  delayMicroseconds(propTime);   // triac On propogation delay
  digitalWrite(AC_LOAD, LOW);    // triac Off
}

void loop() {
  // read the analog in value:
  float lastOutputValue = sensorValue;
  sensorValue = ((float)analogRead(analogInPin));
  // map it to the range of the analog out:
  // change the analog out value:
  analogWrite(analogOutPin, sensorValue/1024.0*255.0);
  sensorValue = (lastOutputValue - (lastOutputValue-sensorValue)/2.0)/1024.0*255.0;
  return;
  
  if(lastOutputValue < sensorValue-1 || lastOutputValue > sensorValue+1)
  {
    
    analogWrite(analogOutPin, sensorValue);
    delay(20);
    analogWrite(analogOutPin, 0);
   // delay(50);
    
    if(sensorValue<=100)
      dimming = 110;
    else if(sensorValue>=200)
      dimming = 10;
    else
      dimming = 110-(sensorValue-100);
  }
  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  //delay(2);
}
