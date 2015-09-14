#include <SoftwareSerial.h>
#include <SikwiWifi.h>
#include <SikwiWifiServer.h>
#include <SikwiWifiAccessPointConfiguration.h>

#define SIKWI_USER "XXX"
#define SIKWI_PASS "XXX"

#define SIKWI_NAME "BistableRelayCloud"

SikwiWifiOne wifi;
SikwiWifiAccessPointConfiguration accessPoint(&wifi);
SikwiWifiServer server(&wifi);

uint8_t ledStatus = 0;

void resetWifi(){
  wifi.reset(9600, &Serial);
  
  wifi.setName(SIKWI_NAME); //SetName

  if(!accessPoint.connect())
    return resetWifi();//Error while creating AccessPoint, or join new wifi,...
  
  server.connect(PSTR(SIKWI_USER), PSTR(SIKWI_PASS), serverDataHandler);

  wifi.setName(SIKWI_NAME); //SetName
  
  ledStatus = wifi.getData(PSTR("switch")).toInt();
  updateObject();

  Serial.println("Init");
}

void setup()
{
  pinMode(A0, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
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
    
  wifi.setData(PSTR("switch"), ledStatus);
}
