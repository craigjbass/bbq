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
   } else if (isCold(currentTemp)) {
    highOxygen();
   } else if(isWarm(currentTemp)) {
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

void highOxygen() {
  startMotor();
}

void startMotor() {
  motor.run(BACKWARD);
}

void stopMotor() {
  motor.run(RELEASE);
}

const double WARM_FIRE_TEMPERATURE = 70;
const double COOKING_TEMPERATURE = 110;

bool isCold(double temperature) {
  return temperature <= WARM_FIRE_TEMPERATURE;
}

bool isWarm(double temperature) {
  return temperature < COOKING_TEMPERATURE && temperature > WARM_FIRE_TEMPERATURE;
}

bool isAtCookingTemperature(double temperature) {
  return temperature >= COOKING_TEMPERATURE;
}

void beforeChangeMotor(double temperature) {
  Serial.print("C = ");
  Serial.println(temperature);
}

