#include <SoftwareSerial.h>
#include <SikwiWifi.h>
#include <SikwiWifiServer.h>
#include <SikwiWifiAccessPointConfiguration.h>

#define SIKWI_USER "XXX"
#define SIKWI_PASS "XXX"

#define SIKWI_NAME "LedDimmer"

SikwiWifiOne wifi;
SikwiWifiAccessPointConfiguration accessPoint(&wifi);
SikwiWifiServer server(&wifi);

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

void setup()
{
  pinMode(A0, OUTPUT);
  
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  Serial.begin(115200);  

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
   
  analogWrite(3, ledStatus*2.55);
  analogWrite(5, ledStatus*2.55);
  analogWrite(6, ledStatus*2.55);
    
  wifi.setData(PSTR("dim%"), ledStatus);
}
