// constants won't change. They're used here to
// set pin numbers:
const int buttonPin3 = 3;    // the number of the pushbutton pin
const int buttonPin4 = 4;    // the number of the pushbutton pin
const int ledPin = A0;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  // read the state of the switch into a local variable:
  int reading3 = digitalRead(buttonPin3);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading3 != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    int reading4 = digitalRead(buttonPin4);
    if(reading4 == HIGH && buttonState == LOW) {
      Serial.println("4-1");
      buttonState = HIGH;
    }
    else if (reading3 == HIGH && buttonState == LOW) {
      Serial.println("3-1");
      buttonState = HIGH;
    }
    else if(reading3 == LOW && buttonState == HIGH){
      Serial.println("0-0");
      buttonState = LOW;
    }
  }
  
  lastButtonState = reading3;
}

