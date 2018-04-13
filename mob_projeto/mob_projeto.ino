#include <Wire.h>    /* Communication with Arduino */
#include <rgb_lcd.h> /* LCD */
#include <WiFi.h>

// PIN's
//input
int tempPin = 1;            // temperature sensor
int moreTempButtonPin = 1;  // button to increase temperature
int lessTempButtonPin = 1;  // button to decrease temperature
int openTapPin = 1;         // open tap manually
int switchModePin = 1;      // switch operating mode
int resetButtonPin = 1;     // insert button pin
//output
int eletroValve2Pin = 1;    // eletrovalve 2
int eletroValve1Pin = 1;    // eletrovalve 1
int lcdPin = 1;             // lcd

// Variables
int redValue = 0;           // red value lcd 0-255
int greenValue = 0;         // green value lcd 0-255
int blueValue = 0;          // blue value lcd 0-255

int targetTemp = 0;         // target temperature the user wants, received from the APP
double currentTemp;			// current temperature, to be sent to the APP

// Timers

// States
int operatingMode = 0;      // 0 - Auto, 1 - Manual, to be sent to the APP

// LCD
rgb_lcd lcd;

void setup() {

  pinMode(tempPin, INPUT);
  pinMode(moreTempButtonPin, INPUT);
  pinMode(lessTempButtonPin, INPUT);
  pinMode(openTapPin, INPUT);
  pinMode(switchModePin, INPUT);
  pinMode(resetButtonPin, INPUT);

  pinMode(eletroValve1Pin, OUTPUT);
  pinMode(eletroValve2Pin, OUTPUT);
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

  delay(500);
}
