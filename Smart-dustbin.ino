#include <Servo.h>

// Ultrasonic
const int trigPin = 7;
const int echoPin = 6;

// Servos
const int lidPin = 9;
const int selectorPin = 10;

// Moisture
const int moisturePin = A0;

Servo lidServo;
Servo selectorServo;

long duration;
int distance;
int moistureValue;
int moisturePercent;

bool processDone = false;

// Servo positions
int centerPos = 90;
int wetPos = 60;
int dryPos = 160;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lidServo.attach(lidPin);
  selectorServo.attach(selectorPin);

  lidServo.write(0);
  selectorServo.write(centerPos);
}

void loop() {

  // Measure Distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > 0 && distance < 30 && !processDone) {

    processDone = true;

    lidServo.write(150);
    Serial.println("Lid Opened");

    delay(3000);   // Wait for garbage placement

    moistureValue = analogRead(moisturePin);

    // Map 0–1023 to 0–100
    moisturePercent = map(moistureValue, 0, 1023, 0, 100);

    // Serial.print("Moisture Raw: ");
    // Serial.print(moistureValue);
    Serial.print("  Percentage: ");
    Serial.println(moisturePercent);

    // Ignore if plate empty (very high % = dry air)
    // if (moisturePercent < 95) {   // garbage placed condition

      if (moisturePercent < 40) {
        Serial.println("Wet Waste");
        selectorServo.write(wetPos);
      } 
      else {
        Serial.println("Dry Waste");
        selectorServo.write(dryPos);
      }

      delay(3000);
      selectorServo.write(centerPos);
    }
    else {
      Serial.println("No Garbage Detected");
    }
  // }

  if (distance > 30 && processDone) {
    lidServo.write(0);
    Serial.println("Lid Closed");

    delay(1000);
    processDone = false;
  }

  delay(1000);
}
