#include <Wire.h>    /* Communication with Arduino */
#include "rgb_lcd.h" /* LCD */
#include "calculations.h"
#include "settings.h"
#include "constants.h"
#include "writeToCSV.h"
#include "readFromCSV.h"
#include "OneWire.h"

// PIN's
//input
const int tempPin = 1;            // temperature sensor
const int openTapPin = 1;         // open tap manually
const int operatingModePin = 1;      // switch operating mode
const int defineTargetTemperatureButtonPin = 1;     // insert button pin
//output
const int eletroValveHeaterPin = 5; // eletrovalve 2
const int eletroValveTapPin = 4;    // eletrovalve 1
const int lcdPin = 1;             	// lcd

// Variables
int redValue = 0;           // red value lcd 0-255
int greenValue = 0;         // green value lcd 0-255
int blueValue = 0;          // blue value lcd 0-255

int targetTemp = /*MIN_TEMP*/5;         // target temperature the user wants, received from the APP
int currentTemp = 0;			// current temperature, to be sent to the APP

int temperatureSetting = CELSIUS_TEMP;	// pretended temperature unit, received from the APP

// Timers
int thermometerTimer = 0;
int cantReachTargetTemperatureTimer = 0;

// States
int operatingMode = AUTOMATIC_STATE;    // operating mode, to be sent to the APP
int systemState;

// Flags
int reachedTargetTemperature = 0; //0 no, 1 yes
int cantReachTargetTemperature = 0; //0 no, 1 yes
int heating = 0;

// LCD
rgb_lcd lcd;

void setup() {

  pinMode(tempPin, INPUT);
  pinMode(openTapPin, INPUT);
  pinMode(operatingModePin, INPUT);
  pinMode(defineTargetTemperatureButtonPin, INPUT);

  pinMode(eletroValveTapPin, OUTPUT);
  pinMode(eletroValveHeaterPin, OUTPUT);
  pinMode(lcdPin, OUTPUT);

  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setRGB(0, 0, 255);

  delay(3000);

  lcd.setCursor(3, 0);
  lcd.print("Insta Heat");

  int i;
  for(i = 0; i < 100; i+=2) {
    blueValue = adjustBlueColorWithTemperature(i, 100);
    redValue = adjustRedColorWithTemperature(i, 100);
    lcd.setRGB(redValue, 0, blueValue);
    delay(25);
  }
  delay(3000);
  lcd.setRGB(0, 0, 0);
  lcd.clear();

  if(digitalRead(openTapPin) == HIGH) {
    systemState = MANUAL_STATE;
  } else {
    systemState = AUTOMATIC_STATE;
  }
}

void loop() {
  // measures the temperature registered by the thermometer every second
  if(thermometerTimer == 0) {
    //currentTemp = convertAnalogSignalToTemperature(analogRead(tempPin), temperatureSetting);
    currentTemp++;
    startThermometerTimer();
  }

  if(currentTemp < targetTemp) {
    if(heating == 0) {  // the first time the TT > CT, this will happen
      startCantReachTargetTemperatureTimer();
      heating = 1;
    }
    if(cantReachTargetTemperatureTimer == 20) {
      cantReachTargetTemperature = 1;
    }
  } else {
    if(operatingMode == AUTOMATIC_STATE) {
      switchWaterFlowToTap();
      resetTargetTemp();
    } else if(operatingMode == MANUAL_STATE){
      reachedTargetTemperature = 1;
    }
    heating = 0;
  }

  

  defaultDisplayInOpenedMode();

  updateTimers();
  delay(DEFAULT_DELAY);
}

void updateTimers() {
	if(thermometerTimer > 0) thermometerTimer--;
	if(cantReachTargetTemperatureTimer > 0) cantReachTargetTemperatureTimer--;
}

void startThermometerTimer() {
	thermometerTimer = ONE_SEC;
}

void startCantReachTargetTemperatureTimer() {
	cantReachTargetTemperatureTimer = ONE_SEC * 10;
}

bool isTimerActive(int timer) {
	return timer > 0;
}

void switchWaterFlowToTap() {
	digitalWrite(eletroValveTapPin, HIGH);
	digitalWrite(eletroValveHeaterPin, LOW);
}

void switchWaterFlowToHeater() {
	digitalWrite(eletroValveTapPin, LOW);
	digitalWrite(eletroValveHeaterPin, HIGH);
}

// when tap is opened manually
void tapOpenManuallyDisplay() {
  lcd.clear();
  lcd.setRGB(0, 255, 0);
  lcd.setCursor(4, 0);
  lcd.print("Tap Open");
}

// default display
/*
CT: 111C TT: 111C
Auto/Manu(4)

*/
void defaultDisplayInOpenedMode() {
  lcd.clear();
  if(heating == 1) {
    redValue = adjustRedColorWithTemperature(currentTemp, targetTemp);
    blueValue = adjustBlueColorWithTemperature(currentTemp, targetTemp);
    lcd.setRGB(redValue, 0, blueValue);
  } else {
    lcd.setRGB(122, 122, 122);
  }
  lcd.setCursor(0, 0);
  lcd.print("CT:     TT:");
  lcd.setCursor(4, 0);
  lcd.print(currentTemp);
  lcd.setCursor(12, 0);
  lcd.print(targetTemp);
  lcd.setCursor(15, 0);
  switch(temperatureSetting) {
    case CELSIUS_TEMP:
      lcd.print("C");
      break;
    case FAHRN_TEMP:
      lcd.print("F");
      break;
    case KELVIN_TEMP:
      lcd.print("K");
      break;
    default:
      break;
  }
  lcd.setCursor(0, 1);
  switch(operatingMode) {
    case AUTOMATIC_STATE:
      lcd.print("Auto");
      break;
    case MANUAL_STATE:
      lcd.print("Manual");
      if(reachedTargetTemperature == 1) {
        lcd.setCursor(8, 1);
        lcd.print("!HOT!");
      }
      break;
    default:
      break;
  }
  if(cantReachTargetTemperature == 1) {
    lcd.setCursor(8, 1);
    lcd.print("!COLD!");
  }

}

// resets target temp to the minimum value acceptable
void resetTargetTemp() {
  targetTemp = 1;
}
