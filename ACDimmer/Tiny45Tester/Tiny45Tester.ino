#define LED 0
uint8_t dim;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);       //triac gate control
  digitalWrite(LED, LOW);  //enable pull-up resistor
}

void loop() {
  if (Serial.available()) {
    dim = Serial.read();
    analogWrite(LED , dim);
  }
  /*
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    dim = Serial.read();
    digitalWrite(LED, HIGH);  //enable pull-up resistor
    delay(100);
    digitalWrite(LED, LOW);  //enable pull-up resistor
    delay(100);
    digitalWrite(LED, HIGH);  //enable pull-up resistor
    delay(100);
    digitalWrite(LED, LOW);  //enable pull-up resistor
    delay(2000);
    
    
    int dixaine = (dim-(dim%10))/10;
    int unite = (dim%10);
    for(int i = 0; i<dixaine; i++){
      digitalWrite(LED, HIGH);  //enable pull-up resistor
      delay(200);
      digitalWrite(LED, LOW);  //enable pull-up resistor
      delay(200);
    }
    delay(2000);
    digitalWrite(LED, HIGH);  //enable pull-up resistor
    delay(100);
    digitalWrite(LED, LOW);  //enable pull-up resistor
    delay(100);
    digitalWrite(LED, HIGH);  //enable pull-up resistor
    delay(100);
    digitalWrite(LED, LOW);  //enable pull-up resistor
    delay(2000);
    
    for(int i = 0; i<unite; i++){
      digitalWrite(LED, HIGH);  //enable pull-up resistor
      delay(200);
      digitalWrite(LED, LOW);  //enable pull-up resistor
      delay(200);
    }
    delay(2000);
    digitalWrite(LED, HIGH);  //enable pull-up resistor
    delay(100);
    digitalWrite(LED, LOW);  //enable pull-up resistor
    delay(100);
    digitalWrite(LED, HIGH);  //enable pull-up resistor
    delay(100);
    digitalWrite(LED, LOW);  //enable pull-up resistor
    delay(100);
    digitalWrite(LED, HIGH);  //enable pull-up resistor
    delay(100);
    digitalWrite(LED, LOW);  //enable pull-up resistor
  }
  */
}
