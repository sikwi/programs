#include <SoftwareSerial.h>
#include <SikwiWifi.h>
#include <SikwiWifiServer.h>
#include <SikwiWifiAccessPointConfiguration.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SIKWI_USER "XXXX"
#define SIKWI_PASS "XXXX"

#define SIKWI_NAME "DoorSensor"

SikwiWifiMini wifi;
SikwiWifiAccessPointConfiguration accessPoint(&wifi);
SikwiWifiServer server(&wifi);

#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

boolean reedswitch = LOW;
float temperature;
unsigned long seconds;

void resetWifi(){
  wifi.reset(9600, &Serial);
  
  wifi.setName(SIKWI_NAME); //SetName

  if(!accessPoint.connect())
    return resetWifi();//Error while creating AccessPoint, or join new wifi,...
  
  server.connect(PSTR(SIKWI_USER), PSTR(SIKWI_PASS), serverDataHandler);

  Serial.println("Init");
}

void setup()
{
  pinMode(A0, OUTPUT);
  pinMode(3, INPUT);
  
  Serial.begin(115200);  

  resetWifi();
  sensors.begin();
}

void loop()
{
  if(!wifi.process()){
    Serial.println("mustReset");
    resetWifi();
  }
  
  server.process();

  if(digitalRead(3) != reedswitch)
    delay(50);

  int currentReedswitchState = digitalRead(3);
  if(currentReedswitchState != reedswitch)
  {
    reedswitch = currentReedswitchState;
    Serial.print("reedswitch:");
    Serial.println(reedswitch);
    
    wifi.setData(PSTR("btn"), reedswitch);
    delay(100);
  }
  
  int currentSecond = millis()/1000;
  if(currentSecond > seconds+2){
    sensors.requestTemperatures(); // Send the command to get temperatures
    seconds = currentSecond;
    
    float currentTemp = sensors.getTempCByIndex(0);
    Serial.print("currentTemp:");
    Serial.print(currentTemp);
    Serial.print("/");
    Serial.println(temperature);
    
    if(currentTemp != temperature)
    {
      temperature = currentTemp;
      wifi.setData(PSTR("temp"), temperature);
    }
  }
}

void serverDataHandler(String type, String value)
{
  //
}
