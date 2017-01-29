int pinBtnDown = 6;
int pinBtnUp = 7;

int pinLed = 10;

int valOut = 0;

// the setup routine runs once when you press reset:
void setup() {
  
  pinMode(pinBtnDown, INPUT);
  pinMode(pinBtnUp, INPUT);
  
  pinMode(pinLed, OUTPUT);
  
  analogWrite(pinLed, 255);
  delay(100);
  analogWrite(pinLed, 0);
}

// the loop routine runs over and over again forever:
void loop() {
  
  if (HIGH == digitalRead(pinBtnDown)) {
    if (valOut >= 5) {
      valOut -= 5;
    }
  } else if (HIGH == digitalRead(pinBtnUp)) {
    if (valOut <= 250) {
      valOut += 5;
    }
  }
  
  analogWrite(pinLed, valOut);
  
  delay(100);        // delay in between reads for stability
}
