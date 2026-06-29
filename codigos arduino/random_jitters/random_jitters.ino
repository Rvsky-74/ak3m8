#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <math.h>

Servo servo;
const int servoPin = 9;
static unsigned long lastServo = 500; // for the timing of servo updates


void setup(){
  Serial.begin(9600);
}



void loop(){
  if (millis() - lastServo > 2000) {
    lastServo = millis();
    servo.attach(servoPin);
    updateServo();
  }
  if (millis() - lastServo > 500) {
    servo.detach();
  }
}



int current_direction = 90;
void updateServo(){
  // find the angle relative to the goal
  current_direction += random(-10,10);
  current_direction = constrain(current_direction, 0, 180);

  servo.write(current_direction);
  Serial.println(current_direction);
}
