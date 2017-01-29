
// Define output pins (motor shield channel A):
int pinOutDirection = 12;
int pinOutSpeed = 3;
int pinOutBrake = 9;
int pinOutStatusLED = 10;

// Define input pins:
int pinInCurrentSensing = A0;
int pinInBtnStart = 7;
//int pinInBtnStop = 6;

// Define constant vars:
int speedStep = 10;

// Define global vars:
int chanASpeed = 0;

void setup() {
  
  // Init serial connection:
  Serial.begin(9600);
  
  // Define pin modes:
  pinMode(pinOutDirection, OUTPUT);
  pinMode(pinOutSpeed, OUTPUT);
  pinMode(pinOutBrake, OUTPUT);
  pinMode(pinOutStatusLED, OUTPUT);
  
  pinMode(pinInCurrentSensing, INPUT);
  pinMode(pinInBtnStart, INPUT);
  
  // Init motor shield channel A:
  digitalWrite(pinOutDirection, HIGH); // Set motor direction to ?
  digitalWrite(pinOutBrake, LOW); // Disengage brake
  digitalWrite(pinOutSpeed, LOW); // Turn off motor
  
  // Init other outputs:
  digitalWrite(pinOutStatusLED, LOW);
  
  // Start sequence:
  
  // Wait for start button to be pressed:
  while (LOW == digitalRead(pinInBtnStart)) {
    delay(100);
  }
  
  // Blink to indicate that train will start in 1 sec:
  digitalWrite(pinOutStatusLED, HIGH);
  delay(100);
  digitalWrite(pinOutStatusLED, LOW);
  
  // Wait another second:
  delay(1000);
  
}

void loop() {
  
  digitalWrite(pinOutSpeed, HIGH);
  delay(2000);
  Serial.println(getRealCurrent());
    
  digitalWrite(pinOutSpeed, LOW);
  delay(2000);
  Serial.println(getRealCurrent());
  
  /*
  while (chanASpeed <= 255 - speedStep) {
    //analogWrite(pinOutSpeed, chanASpeed);
    analogWrite(pinOutStatusLED, chanASpeed);
    chanASpeed += speedStep;
    delay(200);
  }
  // chanASpeed should be 255 now
  
  while (chanASpeed >= speedStep) {
    //analogWrite(pinOutSpeed, chanASpeed);
    analogWrite(pinOutStatusLED, chanASpeed);
    chanASpeed -= speedStep;
    delay(200);
  }
  // chanASpeed should be 0 now
  */
  
}

float getRealCurrent() {
  
  // analogRead reads a range from 0 to 1023 steps for 0 to 5 volts.
  // The Artuino Motor shield can deliver up to 2 A per motor.
  // This means, that each step represents 2 A / 1024 = 0.001953125 ~ 0.00196
  float amps_per_step = 0.00196;
  
  float steps = analogRead(pinInCurrentSensing);
  float amps = steps * amps_per_step;
  
  return amps;
} 
