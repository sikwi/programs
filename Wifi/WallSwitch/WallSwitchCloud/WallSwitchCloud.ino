#include <SoftwareSerial.h>
#include <SikwiWifi.h>
#include <SikwiWifiServer.h>
#include <SikwiWifiAccessPointConfiguration.h>

#define SIKWI_USER "XXXX"
#define SIKWI_PASS "XXXX"

#define SIKWI_NAME "WallSwitch"

SikwiWifiMini wifi;
SikwiWifiAccessPointConfiguration accessPoint(&wifi);
SikwiWifiServer server(&wifi);

boolean buttonState = LOW;

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
  pinMode(4, INPUT);
  
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

  if(digitalRead(3) == HIGH)
    delay(50);
  
  if(digitalRead(4) == HIGH && buttonState == LOW)
  {
    buttonState = HIGH;
    
    wifi.setData(PSTR("btn(Up)"), 1);
    delay(100);
  }
  else if(digitalRead(3) == HIGH && buttonState == LOW)
  {
    buttonState = HIGH;
    
    wifi.setData(PSTR("btn(Down)"), 1);
    delay(100);
    
  }else if (buttonState == HIGH && digitalRead(3) == LOW){
    buttonState = LOW;
  }
}

void serverDataHandler(String type, String value)
{
  
}
