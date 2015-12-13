#include <SoftwareSerial.h>
#include <SikwiWifi.h>
#include <SikwiWifiCloud.h>
#include <SikwiWifiAccessPointConfiguration.h>
#include <Adafruit_NeoPixel.h>

#define SIKWI_USER "XXXX"
#define SIKWI_PASS "XXXX"

#define SIKWI_NAME "LedDimmer"

SikwiWifiOne wifi;
SikwiWifiAccessPointConfiguration accessPoint(&wifi);
SikwiWifiCloud server(&wifi);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, 5, NEO_GRB + NEO_KHZ800);

int ledStatus;

void resetWifi(){
  wifi.reset(9600, &Serial);
  
  wifi.setName(SIKWI_NAME); //SetName

  if(!accessPoint.connect())
    return resetWifi();//Error while creating AccessPoint, or join new wifi,...
  
  server.connect(PSTR(SIKWI_USER), PSTR(SIKWI_PASS), serverDataHandler);

  ledStatus = wifi.getData(PSTR("dim%")).toInt();
  
  updateObject();

  Serial.println("Init");
}

void setup()
{
  pinMode(A0, OUTPUT);
  
  Serial.begin(115200);  

  pixels.begin(); // This initializes the NeoPixel library.
  
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
    Serial.print("type:");
    Serial.println(type);
    Serial.print("value:");
    Serial.println(value);

    ledStatus = value.toInt();
    
    updateObject();
  }
}


void updateObject()
{
  digitalWrite(A0, ledStatus);
  
  unsigned long color = pixels.Color(ledStatus*2.55,ledStatus*2.55,ledStatus*2.55);
  
  for(int i=0;i<7;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(ledStatus*2.55,ledStatus*2.55,ledStatus*2.55)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(10);
  }
    
  wifi.setData(PSTR("dim%"), ledStatus);
}
