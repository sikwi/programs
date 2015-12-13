#include <SoftwareSerial.h>
#include <TimerOne.h>//https://code.google.com/p/arduino-timerone/downloads/detail?name=TimerOne-r11.zip&can=2&q=

#include <SikwiWifi.h>
#include <SikwiWifiCloud.h>
#include <SikwiWifiAccessPointConfiguration.h>

#define SIKWI_USER "sikwi"
#define SIKWI_PASS "test"

#define SIKWI_NAME "ACDimmer"

SikwiWifiOne wifi;
SikwiWifiAccessPointConfiguration accessPoint(&wifi);
SikwiWifiCloud server(&wifi);

#define FADE_DELAY 10  // Delay in ms for each percentage fade up/down (10ms = 1s full-range dim)
#define FADE_PERCENTAGE 5 //The percentage the fade level will be changed when a button is pressed

volatile int i=0;               // Variable to use as a counter of dimming steps. It is volatile since it is passed between interrupts
volatile boolean zero_cross=0;  // Flag to indicate we have crossed zero
int AC_pin = 5;                 // Output to Opto Triac

#define FREQ 60   // 60Hz power in these parts

int freqStep = 1000000 / (2 * FREQ)/128;              // This is the delay-per-brightness step in microseconds. It allows for 128 steps
                                // If using 60 Hz grid frequency set this to 65

static int currentLevel = 128;  // Current dim level...
uint8_t fadeLevel = 128; //used to store the fade level when using the buttons
uint8_t upPreviousValue;
uint8_t downPreviousValue;
uint8_t powerPreviousValue;
boolean initialize = false;

int ledStatus;

void resetWifi(){
  wifi.reset(9600, &Serial);
  
  wifi.setName(SIKWI_NAME); //SetName

  if(!accessPoint.connect())
    return resetWifi();//Error while creating AccessPoint, or join new wifi,...
  
  server.connect(PSTR(SIKWI_USER), PSTR(SIKWI_PASS), serverDataHandler);

  wifi.setName(SIKWI_NAME); //SetName

  ledStatus = wifi.getData(PSTR("dim%")).toInt();
  
  updateObject();

  Serial.println("Init");
}

void interruptFunction() {
  Serial.println("c");
}

void setup()
{
  pinMode(A0, OUTPUT);

  Serial.begin(115200);
  
  pinMode(AC_pin, OUTPUT);                          // Set the Triac pin as output
  attachInterrupt(1, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);      // Go to dim_check procedure every 75 uS (50Hz)  or 65 uS (60Hz)
  Timer1.disablePwm(10);
  resetWifi();
}

void loop()
{
  if(!wifi.process()){
    Serial.println("mustReset");
    resetWifi();
  }
  
  server.process();
}

void serverDataHandler(String type, String value)
{
  if(type == "dim%"){
    ledStatus = value.toInt();
    
    Serial.print("type:");
    Serial.println(type);
    Serial.print("value:");
    Serial.println(value);
    
    updateObject();
  }
}


void updateObject()
{
  digitalWrite(A0, ledStatus);
  wifi.setData(PSTR("dim%"), ledStatus);

  float percent_level;
  percent_level = 128 - (ledStatus * 1.28);
  fadeToLevel( percent_level );
}

void _serial_int(){
  int requestedLevel = Serial.readStringUntil('\n').toInt();
  Serial.print("requestedLevel");
  Serial.println(requestedLevel);
  float percent_level;
  percent_level = 128 - (requestedLevel * 1.28);
  fadeToLevel( percent_level );
}

void zero_cross_detect() {
  zero_cross = true;               // set flag for dim_check function that a zero cross has occured
  i=0;                             // stepcounter to 0.... as we start a new cycle
  digitalWrite(AC_pin, LOW);
}                                 

// Turn on the TRIAC at the appropriate time
// We arrive here every 75 (65) uS
// First check if a flag has been set
// Then check if the counter 'i' has reached the dimming level
// if so.... switch on the TRIAC and reset the counter

void dim_check() {                   
  if(zero_cross == true) {              
    if(i>=fadeLevel) {                     
      digitalWrite(AC_pin, HIGH);  // turn on light       
      i=0;  // reset time step counter                         
      zero_cross=false;    // reset zero cross detection flag
    } 
    else {
      i++;  // increment time step counter                     
    }                                
  }    
}

/***
 *  This method provides a graceful fade up/down effect
 */
void fadeToLevel( int toLevel ) {
 Serial.print("toLevel Value: ");
 Serial.println(toLevel);

  
  int delta = ( currentLevel - toLevel ) < 0 ? 1 : -1;
  Serial.print("delta Value: ");
 Serial.println(delta);
  while ( currentLevel != toLevel ) {
    currentLevel += delta;

 fadeLevel= ((int)currentLevel);
    delay( FADE_DELAY );
    //fadeLevel = toLevel;
  }
}
