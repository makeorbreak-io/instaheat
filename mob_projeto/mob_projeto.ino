#include <Wire.h>    /* Communication with Arduino */
#include "rgb_lcd.h" /* LCD */
#include "calculations.h"
#include "settings.h"
#include "constants.h"
#include "writeToCSV.h"
#include "readFromCSV.h"

// PIN's
//input
const int tempPin = 1;            // temperature sensor
const int moreTempButtonPin = 1;  // button to increase temperature
const int lessTempButtonPin = 1;  // button to decrease temperature
const int openTapPin = 1;         // open tap manually
const int switchModePin = 1;      // switch operating mode
const int resetButtonPin = 1;     // insert button pin
//output
const int eletroValveHeaterPin = 5; // eletrovalve 2
const int eletroValveTapPin = 4;    // eletrovalve 1
const int lcdPin = 1;             	// lcd

// Variables
int redValue = 0;           // red value lcd 0-255
int greenValue = 0;         // green value lcd 0-255
int blueValue = 0;          // blue value lcd 0-255

int targetTemp = MIN_TEMP;         // target temperature the user wants, received from the APP
double currentTemp;			// current temperature, to be sent to the APP

int temperatureSetting = CELSIUS_TEMP;	// pretended temperature unit, received from the APP

// Timers

// States
int operatingMode = AUTOMATIC_STATE;    // operating mode, to be sent to the APP
int systemState = OPENED;

// LCD
rgb_lcd lcd;

void setup() {

  pinMode(tempPin, INPUT);
  pinMode(moreTempButtonPin, INPUT);
  pinMode(lessTempButtonPin, INPUT);
  pinMode(openTapPin, INPUT);
  pinMode(switchModePin, INPUT);
  pinMode(resetButtonPin, INPUT);

  pinMode(eletroValveTapPin, OUTPUT);
  pinMode(eletroValveHeaterPin, OUTPUT);
  pinMode(lcdPin, OUTPUT);

  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setRGB(0, 0, 255);

  lcd.setCursor(3, 0);
  lcd.print("Insta Heat");

  int i;
  for(i = 0; i < 100; i++) {
    blueValue = adjustBlueColorWithTemperature(i, 100);
    redValue = adjustRedColorWithTemperature(i, 100);
    lcd.setRGB(redValue, 0, blueValue);
    delay(25);
  }
  delay(1000);
  lcd.setRGB(0, 0, 0);
  lcd.clear();
}

void loop() {
  if(systemState == CLOSED) {
    currentTemp = convertAnalogSignalToTemperature(analogRead(tempPin), temperatureSetting);

  } else {
    waitingNewTempDisplay();
  }

  if(operatingMode == MANUAL_STATE) {
    if(digitalRead(resetButtonPin) == HIGH) {
      systemState = OPENED;
      resetTargetTemp();
      switchWaterFlowToTap();
      tapOpenManuallyDisplay();
      delay(1000);
    }
  }


  delay(DEFAULT_DELAY);
}

void updateTimers() {
	// if(timer_name > 0) timer_name--; fazer isto para todos os timers
}

// one per timer
void startTimerName() {
	// timer_name = defined_value;
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

// when current temperature > target temperature
void waitingNewTempDisplay() {
  lcd.clear();
  lcd.setRGB(122, 122, 122);
  lcd.setCursor(0, 0);
  lcd.print("Waiting New Temp");
  lcd.setCursor(0, 1);
  lcd.print("New Temp:");
  lcd.setCursor(10, 1);
  lcd.print(targetTemp);
  lcd.setCursor(14, 1);
  switch(temperatureSetting) {
    case CELSIUS_TEMP:
      lcd.print("C");
      break;
    case KELVIN_TEMP:
      lcd.print("K");
      break;
    case FAHRN_TEMP:
      lcd.print("F");
      break;
  }

}

// resets target temp to the minimum value acceptable
void resetTargetTemp() {
  targetTemp = 1;
}
