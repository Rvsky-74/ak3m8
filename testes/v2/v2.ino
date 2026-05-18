#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <math.h>

// The serial connection to the GPS device
static const int RXPin = 4, TXPin = 3;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);


Servo servo;
const int servoPin = 9;
float headingDegrees = 0;
float prevHeading = 90;
static unsigned long lastServo = 500; // for the timing of servo updates

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
static unsigned long lastCompass = 0; // for the timing of compass updates


float targetLat = 41.5577919 * PI/180;
float targetLon = -8.371417 * PI/180;
double lat;
double lon;

void setup()
{
  Serial.begin(9600);
  ss.begin(9600);

  servo.attach(servoPin);
  servo.write(90);  // posição inicial neutra

  if(!mag.begin()){
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }

  

}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0){
    gps.encode(ss.read());
  }

  if (gps.location.isUpdated()) {
    // displayInfo();
    lat = gps.location.lat() * PI/180;
    lon = gps.location.lng() * PI/180;
  }

  if (millis() - lastCompass > 2000) { // update orientation once per second
    lastCompass = millis();
    updateCompass();
  }

  if (millis() - lastServo > 2000) {
    lastServo = millis();
    updateServo();
  }


  if (millis() > 5000 && gps.charsProcessed() < 10){
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.println();
}



void updateCompass(){
  if (!gps.location.isValid()) return;

  sensors_event_t event; 
  mag.getEvent(&event);

  float heading = atan2(event.magnetic.y, event.magnetic.x);
  float declinationAngle = -0.05;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0) heading += 2*PI;  
  // Check for wrap due to addition of declination.
  if(heading > 2*PI) heading -= 2*PI;

  headingDegrees = heading * 180/M_PI; 
  
}



void updateServo(){
  // find the angle relative to the goal
  float lonDiff = targetLon - lon;
  float targetAngle = atan2(sin(lonDiff)*cos(targetLat), cos(lat)*sin(targetLat) - sin(lat)*cos(targetLat)*cos(lonDiff));
  if (targetAngle < 0) targetAngle += 2*PI;

  float angleToTurn = (targetAngle * 180/PI) - headingDegrees;

  if (angleToTurn > 180) angleToTurn -= 360;
  if (angleToTurn < -180) angleToTurn += 360;

  if (abs(angleToTurn - prevHeading) < 30) return;
  prevHeading = angleToTurn;

  float servoAngle = angleToTurn + 90;
  servoAngle = constrain(servoAngle, 0, 180);

  servo.write(servoAngle);

  Serial.print(headingDegrees); Serial.print(" | "); Serial.println(servoAngle);


}
