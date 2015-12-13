#define DELAY_PIN  1000  //(suffix:ms)delay between switch
#define PIN        A0 	 //leds pin

void setup(){
  pinMode(PIN, OUTPUT);
}

void loop(){
  digitalWrite(PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY_PIN);                 // wait for a second
  digitalWrite(PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(DELAY_PIN);
}
