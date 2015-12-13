#include <SoftwareSerial.h>
#include <SikwiWifi.h>
#include <SikwiWifiCloud.h>
#include <SikwiWifiAccessPointConfiguration.h>

#define SIKWI_USER "sikwi"
#define SIKWI_PASS "test"

#define SIKWI_NAME "Bistable"

SikwiWifiOne wifi;
SikwiWifiAccessPointConfiguration accessPoint(&wifi);
SikwiWifiCloud server(&wifi);

const int currentPin = A2;
const unsigned long sampleTime = 100000UL;                           // sample over 100ms, it is an exact number of cycles for both 50Hz and 60Hz mains
const unsigned long numSamples = 250UL;                               // choose the number of samples to divide sampleTime exactly, but low enough for the ADC to keep up
const unsigned long sampleInterval = sampleTime/numSamples;  // the sampling interval, must be longer than then ADC conversion time

int adc_zero = 507;//When Simple Relay is On but no load
uint8_t ledStatus = 0;
unsigned long lastCurrentSensing = 0;

#define DELAY_CURRENT_SENSING 1000


void resetWifi(){
  wifi.reset(9600, &Serial);
  
  wifi.setName(SIKWI_NAME); //SetName

  if(!accessPoint.connect())
    return resetWifi();//Error while creating AccessPoint, or join new wifi,...
  
  server.connect(PSTR(SIKWI_USER), PSTR(SIKWI_PASS), serverDataHandler);
  
  ledStatus = wifi.getData(PSTR("switch")).toInt();
  updateObject();

  Serial.println("Init");
}

void setup()
{
  pinMode(A0, OUTPUT);
  
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  
  Serial.begin(115200); 

  
  digitalWrite(4, HIGH);
  delay(10);
  digitalWrite(4, LOW);

  adc_zero = determineVQ(currentPin); //Quiscent output voltage - the average voltage ACS712 shows with no load (0 A)

  digitalWrite(5, HIGH);
  delay(10);
  digitalWrite(5, LOW);

  Serial.println(adc_zero);
  
  resetWifi();
}

void loop()
{ 
  if(!wifi.process()){
    Serial.println("mustReset");
    resetWifi();
  }
  
  server.process();

  if(millis()>lastCurrentSensing+DELAY_CURRENT_SENSING){
    lastCurrentSensing = millis();
    sendCurrent(readCurrent());
  }
}

void sendCurrent(float current){
  Serial.print("ACS712@A2:");
  Serial.print(analogRead(currentPin));
  Serial.print("\t");
  Serial.print(current,3);
  Serial.print(" A");
  Serial.print("\t");
  Serial.print(current*225);
  Serial.print(" W");
  Serial.print("\t");
  Serial.print(current*225/1000,3);
  Serial.println("kW/h");
}

void serverDataHandler(String type, String value)
{
  if(type == "switch")
  {
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
  if(ledStatus){
    digitalWrite(4, HIGH);
    delay(10);
    digitalWrite(4, LOW);
  }else{
    digitalWrite(5, HIGH);
    delay(10);
    digitalWrite(5, LOW);
  }
    
  wifi.setData(PSTR("switch/value"), ledStatus);
}

float readCurrent()
{
  unsigned long currentAcc = 0;
  unsigned int count = 0;
  unsigned long prevMicros = micros() - sampleInterval ;
  while (count < numSamples)
  {
    if (micros() - prevMicros >= sampleInterval)
    {
      long adc_raw = analogRead(currentPin) - adc_zero;
      //if(adc_raw<0)
      //  adc_raw = 0;
      currentAcc += (unsigned long)(adc_raw * adc_raw);
      ++count;
      prevMicroso += sampleInterval;
    }
  }

  float rms = sqrt((float)currentAcc/(float)numSamples) * (27.02 / 1024.0);//(27.02 for 5A, 50 for 20A, 75.7576 for 30A)=> 5/0.185; 5/0.100 or 5/0.066  return rms;
  return rms;
  //Serial.println(rms);
}

int determineVQ(int PIN) {
  Serial.print("estimating avg. quiscent voltage:");
  long VQ = 0;
  //read 5000 samples to stabilise value
  for (int i=0; i<5000; i++) {
    VQ += analogRead(PIN);
    delay(1);//depends on sampling (on filter capacitor), can be 1/80000 (80kHz) max.
  }
  VQ /= 5000;
  Serial.print(map(VQ, 0, 1023, 0, 5000));Serial.println(" mV");
  return int(VQ);
}



