/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13.
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead().

 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground

 * Note: because most Arduinos have a built-in LED attached
 to pin 13 on the board, the LED is optional.


 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/AnalogInput

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

void setup() {
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
  // read the value from the sensor:
  int sensorValue = analogRead(A3)/1024.0*255;
  int lastDimming = dimming;
  if(sensorValue<=100)
    dimming = 110;
  else if(sensorValue>=200)
    dimming = 10;
  else
    dimming = 110-(sensorValue-100);
    
  if(lastDimming == dimming || lastDimming == dimming+1 || lastDimming == dimming-1)
  {
    dimming = lastDimming;
  }
  else{
    digitalWrite(1, HIGH);   // triac firing
    delay(50);
    digitalWrite(1, LOW);   // triac firing
    delay(50);
    
  }
}
