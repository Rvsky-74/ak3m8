#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <math.h>

Servo servo;
const int servoPin = 9;
float prevHeading = 90;


TinyGPSPlus gps;
static const int RXPin = 4, TXPin = 3; 
SoftwareSerial ss(RXPin, TXPin);
double lat = 0;
double lon = 0;


Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void setup() {
  servo.attach(servoPin);
  Serial.begin(9600);
  servo.write(90);  // posição inicial neutra

  ss.begin(9600);

  if(!mag.begin()){
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
}


void loop() {
  // read GPS position
  while (ss.available() > 0){
    if (gps.encode(ss.read())){
      if (gps.location.isValid()){
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
      }
      else{
        Serial.print(F("INVALID"));
      }
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }


  sensors_event_t event; 
  mag.getEvent(&event);
 
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  // Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  // Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  // Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

  // Como o sensor vai estar na vertical, usamos os valores de x e z (y é constante)
  // Existe uma corrente residual que provoca um offset nas medições
  // A média das medições é X: -1.6349999999999998 Z: 2.55
  // logo temos de retirar esse valor para podermos medir o ângulo
  float heading = atan2(event.magnetic.z - 2.55, event.magnetic.x + 1.63);
  float declinationAngle = 1.2;
  heading += declinationAngle; // acho que isto é irrelevante porque podemos compensar com a montagem mas qsf
  
  // Correct for when signs are reversed.
  if(heading < 0) heading += 2*PI;  
  // Check for wrap due to addition of declination.
  if(heading > 2*PI) heading -= 2*PI;

  // At this point, the angle is [0,360] but the servo only handles up to 180
  if (heading > 1.5*PI) heading = 0;
  if (heading > PI && heading <= 1.5*PI) heading = PI;

   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
  Serial.print("Coords: "); Serial.print(lat); Serial.print(" | "); Serial.println(lon);





  if (abs(headingDegrees - prevHeading) >= 10) {
    Serial.println("Turning servo");
    servo.write(headingDegrees);
    prevHeading = headingDegrees;
  }

  delay(1000);
}

