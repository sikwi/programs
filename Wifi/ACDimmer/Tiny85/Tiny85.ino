/* ATtiny85 as an I2C Slave            BroHogan                           1/12/11
 * Example of ATtiny I2C slave receiving and sending data to an Arduino master.
 * Gets data from master, adds 10 to it and sends it back.
 * SETUP:
 * ATtiny Pin 1 = (RESET) N/U                      ATtiny Pin 2 = (D3) N/U
 * ATtiny Pin 3 = (D4) to LED1                     ATtiny Pin 4 = GND
 * ATtiny Pin 5 = I2C SDA on DS1621  & GPIO        ATtiny Pin 6 = (D1) to LED2
 * ATtiny Pin 7 = I2C SCK on DS1621  & GPIO        ATtiny Pin 8 = VCC (2.7-5.5V)
 * NOTE! - It's very important to use pullups on the SDA & SCL lines!
 * Current Rx & Tx buffers set at 32 bytes - see usiTwiSlave.h
 * Credit and thanks to Don Blake for his usiTwiSlave code. 
 * More on TinyWireS usage - see TinyWireS.h
 */
#include "avr/interrupt.h"
#include "TinyWireS.h"                  // wrapper class for I2C slave routines

#define I2C_SLAVE_ADDR  0x26            // i2c slave address (38)

#define LED1_PIN         1              // ATtiny Pin 3
#define LED2_PIN         3              // ATtiny Pin 3
#define  fullOn    10
#define  fullOff   110
#define  FQ_50      1 // in case of 50Hz
#define  FQ_60      0 // in case of 50Hz
#define  VER       "2.0"

int inbyte;

int dimming = fullOff;  // Dimming level (0-128)  0 = ON, 128 = OFF

boolean frq = FQ_50;     // change the frequency here. 
boolean countUp = false;
boolean simulate = true; 

void setup(){
  pinMode(LED1_PIN, OUTPUT);             // for general DEBUG use
  pinMode(LED2_PIN, OUTPUT);             // for general DEBUG use
  
  Blink(LED1_PIN,2);                    // show it's alive
  TinyWireS.begin(I2C_SLAVE_ADDR);      // init I2C Slave mode
  
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
  digitalWrite(LED1_PIN, HIGH);   // triac firing
  delayMicroseconds(propTime);         // triac On propogation delay
  digitalWrite(LED1_PIN, LOW);    // triac Off
}

void loop(){
  Blink(LED2_PIN,2);           // master must wait for this to finish before calling Wire.requestFrom
  delay(1000);
}


void Blink(byte led, byte times){ // poor man's display
  for (byte i=0; i< times; i++){
    digitalWrite(led,HIGH);
    delay (50);
    digitalWrite(led,LOW);
    delay (50);
  }
}



