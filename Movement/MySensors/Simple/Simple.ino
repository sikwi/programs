//#include <SPI.h>
//#include <MySensor.h>

//MySensor gw;
//unsigned long SLEEP_TIME = 10000;  // sleep time between reads (seconds * 1000 milliseconds)
//#define DELAY_PIN2 1000 //delay pin1
#define PRIVATE_PIN  1000  //private : delay pin2
#define PRIVATE2_PIN  1000  //private : delay pin3
#define PRIVATE3_PIN  1000  //private : delay pin4
#define PRIVATE4_PIN  1000  //private : delay pin5

#define DELAY_LED  1000  //delay pin
#define LED_PIN 8 		    //leds pin
#define NAME_LED_PIN 	"coucou" //un nom de pin "alacon" ;D
#define ANALOG_PIN A0 		    //analog pin
#define BOOLEAN 	true //un boolean comme ça! ;D

void setup()  
{
   //gw.begin();

   // Send the sketch version information to the gateway and Controller
   //gw.sendSketchInfo("BlinkTest", "1.0");
   
   pinMode(PIN, OUTPUT);
}

void loop()
{
  digitalWrite(PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY_PIN);                 // wait for a second
  digitalWrite(PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(DELAY_PIN); 
  sleep();                 // wait for a second
}

void sleep()
{
  //gw.sleep(SLEEP_TIME);
} 

