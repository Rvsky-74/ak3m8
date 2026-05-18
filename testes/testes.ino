#include <Servo.h>

Servo servo;
const int servoPin = 9;

void setup() {
  servo.attach(servoPin);
  Serial.begin(9600);
  servo.write(90);  // posição inicial neutra
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    int angle = input.toInt();

    if (angle >= 0 && angle <= 180) {
      servo.write(angle);
      Serial.print("Angulo: ");
      Serial.println(angle);
    }
  }
}