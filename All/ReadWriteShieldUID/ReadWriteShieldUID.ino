#include <Wire.h>
#include <avr/wdt.h>

#define EEPROM_ADDR 0x50 //private
#define SHIELD_ID 1122 //Shield ID
#define SHIELD_VERSION 14 //Shield version
#define SHIELD_Variant 16 //Shield variant

void setup() {

  Serial.begin(115200);
  Wire.begin();

  i2c_eeprom_write_byte(EEPROM_ADDR,0, 12);
  delay(10);

  i2c_eeprom_write_byte(EEPROM_ADDR, 1, ((SHIELD_ID >> 8) & 0xFF));
  delay(10);
  i2c_eeprom_write_byte(EEPROM_ADDR, 2, (SHIELD_ID & 0xFF));
  delay(10);
  
  i2c_eeprom_write_byte(EEPROM_ADDR,3, SHIELD_VERSION);
  delay(10);
  i2c_eeprom_write_byte(EEPROM_ADDR,4, SHIELD_Variant);
  delay(10);

  delay(1000);

  bool isHat = ((uint8_t) i2c_eeprom_read_byte(EEPROM_ADDR, 0)) == 12;
  Serial.print("Is Shield:");
  Serial.println(isHat);

  unsigned int HatID = (i2c_eeprom_read_byte(EEPROM_ADDR, 1) << 8) & 0xFF00;
  HatID += i2c_eeprom_read_byte(EEPROM_ADDR, 2) & 0xFF;
  Serial.print("SHIELD_ID:");
  Serial.println(HatID);

  uint8_t HatType = i2c_eeprom_read_byte(EEPROM_ADDR, 3);
  Serial.print("SHIELD_VERSION:");
  Serial.println(HatType);

  uint8_t HatVariant = i2c_eeprom_read_byte(EEPROM_ADDR, 4);
  Serial.print("SHIELD_Variant:");
  Serial.println(HatVariant);
}

void loop() {
  // put your main code here, to run repeatedly:

}

/* Fonctions EEPROM*/
void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data )
{
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));    // Address High Byte
  Wire.write((int)(eeaddress & 0xFF));  // Address Low Byte
  Wire.write(rdata);
  Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress )
{
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));    // Address High Byte
  Wire.write((int)(eeaddress & 0xFF));  // Address Low Byte
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}
