#include <Wire.h>    /* Communication with Arduino */
#include <rgb_lcd.h> /* LCD */
#include <WiFi.h>
#include "calculations.h"
#include "settings.h"
#include "constants.h"

// PIN's
//input
const int tempPin = 1;            // temperature sensor
const int moreTempButtonPin = 1;  // button to increase temperature
const int lessTempButtonPin = 1;  // button to decrease temperature
const int openTapPin = 1;         // open tap manually
const int switchModePin = 1;      // switch operating mode
const int resetButtonPin = 1;     // insert button pin
//output
const int eletroValveHeaterPin = 1; // eletrovalve 2
const int eletroValveTapPin = 1;    // eletrovalve 1
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
  lcd.setRGB(redValue, greenValue, blueValue);


  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("Message");

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("MyPlant");
  delay(1000);

}

void loop() {
  /*
  lcd.setRGB(red_value, green_value, blue_value);
  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("Message");
  lcd.clear();
  */
  
  
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



