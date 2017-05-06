#include "max6675.h"
#include <AFMotor.h>

int thermoDO = 5;
int thermoCS = 6;
int thermoCLK = 9;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
AF_DCMotor motor(1, MOTOR12_1KHZ);

void setup() {
  Serial.begin(9600);
  motor.setSpeed(255);
  delay(1000);
  startMotor();
}

void loop() {
   double currentTemp = thermocouple.readCelsius();
  
   beforeChangeMotor(currentTemp);
   
   if(isAtCookingTemperature(currentTemp)) {
    stopMotor();
   } else if(isTooCold(currentTemp)) {
    heat();
   }

   delay(1000);
}

int secondsRunning = 0;
int secondsToWait = 0;
void heat() {
  if(secondsRunning > 20) {
    stopMotor();
    secondsRunning = 0;
    secondsToWait = 120;
  } else if(secondsToWait <= 0) {
    startMotor();  
    secondsRunning++;
  } else {
    secondsToWait--;
  }
}

void startMotor() {
  motor.run(BACKWARD);
}

void stopMotor() {
  motor.run(RELEASE);
}

bool isTooCold(double temperature) {
  return temperature < 110;
}

bool isAtCookingTemperature(double temperature) {
  return temperature >= 110;
}

void beforeChangeMotor(double temperature) {
  Serial.print("C = ");
  Serial.println(temperature);
}

