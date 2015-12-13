#include <SoftwareSerial.h>
#include <SikwiWifi.h>
#include <SikwiWifiCloud.h>
#include <SikwiWifiAccessPointConfiguration.h>
#include <SikwiWifiIfttt.h>

#define SIKWI_USER "sikwi"
#define SIKWI_PASS "test"

#define SIKWI_NAME "LedDimmer"

SikwiWifiOne wifi;
SikwiWifiAccessPointConfiguration accessPoint(&wifi);
SikwiWifiCloud server(&wifi);
SikwiWifiIfttt ifttt(&wifi, "bXBg1twg3DMiN9l507oCx0");

int ledStatus;

SoftwareSerial dimmer(6, 5);


void resetWifi() {
  wifi.reset(9600, &Serial);

  wifi.setName(SIKWI_NAME); //SetName

  if (!accessPoint.connect())
    return resetWifi();//Error while creating AccessPoint, or join new wifi,...

  server.setURL(PSTR("192.168.1.41"), 1883);
  server.connect(PSTR(SIKWI_USER), PSTR(SIKWI_PASS), serverDataHandler);

  wifi.setName(SIKWI_NAME); //SetName

  ledStatus = wifi.getData(PSTR("dim%")).toInt();

  updateObject();

  Serial.println("Init");

}

void setup()
{
  pinMode(A0, OUTPUT);

  pinMode(5, OUTPUT);

  Serial.begin(115200);
  dimmer.begin(9600);

  resetWifi();
}

void loop()
{
  if (!wifi.process()) {
    Serial.println("mustReset");
    resetWifi();
  }

  server.process();
}

void serverDataHandler(String type, String value)
{
  if (type == "dim%") {
    ledStatus = value.toInt();

    Serial.print("type:");
    Serial.println(type);
    Serial.print("value:");
    Serial.println(value);
  
    updateObject();

    ifttt.triggerEvent(PSTR("front_door_opened"), value);
  }
}


void updateObject()
{
  digitalWrite(A0, ledStatus);
  dimmer.print((char)ledStatus);
  /*
  if (ledStatus == 0)
    analogWrite(5, 0);
  else if (ledStatus == 100)
    analogWrite(5, 255);
  else
    analogWrite(5, 100 + ledStatus);
  */
  wifi.setData(PSTR("dim%"), ledStatus);
}
