boolean buttonState = LOW;
boolean ledState = LOW;

void setup(){ 
  Serial.begin(115200);
  pinMode(A0, OUTPUT);
  pinMode(3, INPUT);

  digitalWrite(A0, ledState);
}

void loop()     {
  
  int btn3 = digitalRead(3);
  if(buttonState != btn3){
    buttonState = btn3;
    Serial.println(buttonState);
    if(buttonState == HIGH){
      ledState = !ledState;
      digitalWrite(A0, ledState);
    }
  }
}
