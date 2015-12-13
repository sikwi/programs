boolean buttonState = LOW;

void setup()
{
  pinMode(A0, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  
  Serial.begin(115200);  
}

void loop()
{
  int btn3 = digitalRead(3);
  int btn4 = digitalRead(4);
  
  if(btn3 == HIGH)
    delay(100);
  
  if(btn4 == HIGH && buttonState == LOW){
    buttonState = HIGH;
    Serial.print("btn4 ");
    Serial.print(btn4);
    Serial.print(", btn3 ");
    Serial.println(btn3);
    delay(100);
  }
  else if(btn3 == HIGH && buttonState == LOW){
    buttonState = HIGH;
    Serial.print("btn3 ");
    Serial.print(btn3);
    Serial.print(", btn4 ");
    Serial.println(btn4);
    delay(100);
  }else if (buttonState == HIGH && btn3 == LOW){
    buttonState = LOW;
  }
}

void serverDataHandler(String type, String value)
{
  
}
