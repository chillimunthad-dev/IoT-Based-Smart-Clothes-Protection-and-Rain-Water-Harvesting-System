#include <ESP32Servo.h>

// ESP32 pin connections
#define RAIN_PIN   27
#define SERVO_PIN  33
#define WATER_PIN  35
#define BUZZER_PIN 26

Servo shelterServo;

// Servo positions for your shelter
const int RAIN_ANGLE = 0;
const int DRY_ANGLE  = 90;

// Adjust after checking water sensor readings
const int WATER_FULL_THRESHOLD = 2000;

int lastRainState = HIGH;

void setup() {
  Serial.begin(115200);

  pinMode(RAIN_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);

  // Initialize servo
  shelterServo.setPeriodHertz(50);
  shelterServo.attach(SERVO_PIN, 500, 2400);

  // Read rain sensor when system starts
  lastRainState = digitalRead(RAIN_PIN);

  // Set correct initial position
  if (lastRainState == LOW) {
    shelterServo.write(RAIN_ANGLE);
    Serial.println("Rain detected - Shelter covering");
  } else {
    shelterServo.write(DRY_ANGLE);
    Serial.println("No rain - Shelter in dry position");
  }

  delay(500);
  Serial.println("Rain Shelter System Started");
}

void loop() {
  int rainState = digitalRead(RAIN_PIN);
  int waterLevel = analogRead(WATER_PIN);

  // Rain starts: move roof to cover shelter
  if (rainState == LOW && lastRainState != LOW) {
    shelterServo.write(RAIN_ANGLE);
    Serial.println("RAIN DETECTED - SHELTER COVERING");
  }

  // Rain stops: return roof to dry position
  else if (rainState == HIGH && lastRainState != HIGH) {
    shelterServo.write(DRY_ANGLE);
    Serial.println("RAIN STOPPED - SHELTER RETURNING");
  }

  lastRainState = rainState;

  // Tank full: turn buzzer ON
  if (waterLevel >= WATER_FULL_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("TANK FULL - BUZZER ON");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(100);
}