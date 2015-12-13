// Voltage controlled dimmer with ATtiny85
//
// This arduino sketch includes a zero 
// crossing detect function and an opto-isolated triac.
// 
// AC Phase control is accomplished using the internal 
// hardware timer1 in the ATtiny85
//
// Timing Sequence
// * timer is set up but disabled
// * zero crossing detected
// * timer starts counting from zero
// * comparator set to "delay to on" value
// * counter reaches comparator value
// * comparator ISR turns on triac gate
// * counter set to overflow - pulse width
// * counter reaches overflow
// * overflow ISR turns off triac gate
// * triac stops conducting at next zero cross

// The hardware timer runs at 8MHz. 
// A half period of a 50Hz AC signal takes 10 ms is 80000 counts.
// Prescaler set to 1024 gives 78 counts per half period

// A half period of a 60Hz AC signal takes 8,333 ms is 66666,66... counts.
// Prescaler set to 1024 gives 65 counts per half period

///// http://forum.arduino.cc/index.php?topic=314773.0


#include <avr/io.h>
#include <avr/interrupt.h>

#define DETECT 2      //zero cross detect, interrupt 0, is physical pin 7
#define GATE 3        //triac gate is physical pin 2
#define PULSE 2        //trigger pulse width (counts)
#define INSTELPIN 2   // =A2 (digital pin4) is physical pin 3 
#define LED 0

uint8_t dim;

void setup(){
  Serial.begin(9600);
  // set up pins
  pinMode(DETECT, INPUT);      //zero cross detect
  digitalWrite(DETECT, HIGH);  //enable pull-up resistor
  pinMode(GATE, OUTPUT);       //triac gate control
  pinMode(LED, OUTPUT);       //triac gate control
  digitalWrite(LED, LOW);  //enable pull-up resistor

  // set up Timer1 
  TCCR1 = 0;     // stop timer 
  OCR1A = 50;    //initialize the comparator
  TIMSK = _BV(OCIE1A) | _BV(TOIE1);  //interrupt on Compare Match A | enable timer overflow interrupt
  sei();  // enable interrupts
  // set up zero crossing interrupt1
  attachInterrupt(0,zeroCrossingInterrupt, FALLING);    
}  

//Interrupt Service Routines
void zeroCrossingInterrupt(){   
  TCNT1 = 0;   //reset timer - count from zero
  TCCR1 = B00001011;        // prescaler on 1024, see table 12.5 of the tiny85 datasheet
}

ISR(TIMER1_COMPA_vect){    //comparator match
  digitalWrite(GATE,HIGH); //set triac gate to high
  analogWrite(0,dim);
  TCNT1 = 255-PULSE;       //trigger pulse width, when TCNT1=255 timer1 overflows
} 
 
ISR(TIMER1_OVF_vect){       //timer1 overflow
  digitalWrite(GATE,LOW);   //turn off triac gate
  TCCR1 = 0;                //disable timer stop unintended triggers
}

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

void loop(){
  if (Serial.available()) {
    dim = Serial.read();
    //analogWrite(LED , dim);
    OCR1A = map(dim, 0, 255, 65, 2);   
  }
}
