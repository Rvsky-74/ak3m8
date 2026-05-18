// Motor Servo SG90 - requires 4.8 - 6V and ~500mA, so the arduino isnt enough. 
// The BAT525 has enough power but only 3.7V so I need 2. 
// Then, i need a LM7805 to place that back to 5V
// or i can try to use 4 1.5V AA batteries

// Compass Module HMC5883L (GY-273 board)
// SCL  -> A5/SCL,
// SDA  -> A4/SDA,
// DRDY -> Not Connected

// Arduino Nano
// GPS Module NEO-6M GPS
// Bateria + interruptor + fios

#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#include <Arduino.h>
#include <Wire.h>
#include <HMC5883L_Simple.h>




float deviceOrientation;
int turningAngle;
double lonDiff;
double x,y, bearing;
unsigned long lastUpdate = 0;

struct Coords {
  double latitude;
  double longitude;
};
Coords deviceLocation;
Coords targetLocation = {0,0};

int servoPosition;
float offSetX[2];
float offSetY[2];

float declinationOffset = -0.85; // 0 51 W

Servo myServo;
TinyGPSPlus gps;
SoftwareSerial ss(4, 3); // TX / RX = 4 / 3 (https://randomnerdtutorials.com/guide-to-neo-6m-gps-module-with-arduino/)
HMC5883L_Simple Compass;


void setup(){
    Serial.begin(9600);
    ss.begin(9600);
    
    myServo.attach(9); // servo's PWM pin conected to pin 9

    Wire.begin();
    // Compass.SetDeclination(0, 51, 'W');
    Compass.SetSamplingMode(COMPASS_SINGLE);
    Compass.SetScale(COMPASS_SCALE_130);
    Compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH); // Silkscreen x points north

    compassCalibration(); // During calibration, the compass must stand still

}

void loop(){
  while (ss.available() > 0) { // Read GPS data
    gps.encode(ss.read());
  }
  if (gps.location.isUpdated() && millis() - lastUpdate > 1000) { //prevent the servo from updating too often
    lastUpdate = millis();

    deviceLocation.latitude = radians(gps.location.lat());
    deviceLocation.longitude = radians(gps.location.lng());
  
    // Compute relative angle to target
    lonDiff = targetLocation.longitude - deviceLocation.longitude;
    y = sin(lonDiff) * cos(targetLocation.latitude);
    x = cos(deviceLocation.latitude) * sin(targetLocation.latitude) - sin(deviceLocation.latitude) * cos(targetLocation.latitude) * cos(lonDiff);
    bearing = atan2(y, x);  // result in radians
    bearing = degrees(bearing);      // convert radians → degrees
    bearing = fmod((bearing + 360), 360);  // normalize to 0–360°
    

    // Check how much to rotate the servo
    // This is given by the diference between the orientation of the device and the target orientation (both having north as the reference direction)

    // when measuring the orientation, mind the magnetic field ofset due to the servo, measured during setup
    float compensatedX = Compass.GetHeadingDegrees().x - offSetX[servoPosition];
    float compensatedY = Compass.GetHeadingDegrees().y - offSetY[servoPosition];
    deviceOrientation = atan2(compensatedY, compensatedX) * 180.0 / PI + declinationOffset;
    

    // Ensure the angle diference is in the range 0-180 and write the angle to the servo
    // The servo's 90deg should be aligned with the compass north so it can turn both ways
    turningAngle = round(bearing - deviceOrientation) + 90;
    while (turningAngle > 360) turningAngle -= 360;
    while (turningAngle < 0) turningAngle += 360;

    if (turningAngle > 270) myServo.write(0);
    if (turningAngle > 180 && turningAngle <= 270) myServo.write(180);
    if (turningAngle <= 180) myServo.write(turningAngle);
    

    //Update servo position for compass compensation
    servoPosition = (turningAngle > 90 && turningAngle < 270) ? 1: 0;
  }
}


void compassCalibration(){
  myServo.write(0);
  delay(500);
  offSetX[0] = Compass.GetHeadingDegrees().x;
  offSetY[0] = Compass.GetHeadingDegrees().y;

  myServo.write(180);
  delay(500);
  offSetX[1] = Compass.GetHeadingDegrees().x;
  offSetY[1] = Compass.GetHeadingDegrees().y;
}