void setup()
{
  Serial.begin(115200);
  pinMode(A0, OUTPUT);
  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop()
{
  digitalWrite(3, HIGH);
  delay(2000);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  delay(2000);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(2000);
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  delay(2000);
  digitalWrite(6, LOW);
}
