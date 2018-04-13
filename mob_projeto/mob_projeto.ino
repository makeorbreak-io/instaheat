#include <Wire.h>    /* Communication with Arduino */
#include <rgb_lcd.h> /* LCD */
#include <WiFi.h>

// PIN's
//input
#define RELAY1  2
#define RELAY2  3
#define RELAY3  4
#define RELAY4  5
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
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

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
  digitalWrite(RELAY1, LOW);          // Turns ON Relays 1
  delay(2000);                                      // Wait 2 seconds
  digitalWrite(RELAY1, HIGH);         // Turns Relay Off

  digitalWrite(RELAY2, LOW);          // Turns ON Relays 2
  delay(2000);                                      // Wait 2 seconds
  digitalWrite(RELAY2, HIGH);         // Turns Relay Off

  digitalWrite(RELAY3, LOW);          // Turns ON Relays 3
  delay(2000);                                      // Wait 2 seconds
  digitalWrite(RELAY3, HIGH);         // Turns Relay Off

  digitalWrite(RELAY4, LOW);          // Turns ON Relays 4
  delay(2000);                                      // Wait 2 seconds
  digitalWrite(RELAY4, HIGH);         // Turns Relay Off
  /*
    lcd.setRGB(red_value, green_value, blue_value);
    lcd.newclear(10);
    lcd.setCursor(0, 0);
    lcd.print("Message");
    lcd.clear();
  */

  delay(500);
}
