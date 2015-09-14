//#include <SPI.h>
//#include <MySensor.h>

//MySensor gw;
//unsigned long SLEEP_TIME = 10000;  // sleep time between reads (seconds * 1000 milliseconds)
//#define DELAY_PIN2 1000 //delay pin
#define DELAY_PIN  1000  //delay pin
#define PIN 13 		//leds pin
#define PIN 	"coucou" //leds pin

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

