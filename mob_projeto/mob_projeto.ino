#include <Wire.h>    /* Communication with Arduino */
#include "rgb_lcd.h" /* LCD */
#include "calculations.h"
#include "settings.h"
#include "constants.h"
#include "writeToCSV.h"
#include "readFromCSV.h"
#include "OneWire.h"
#include "DallasTemperature.h"

// PIN's
//input
const int tempPin = 11;            // temperature sensor
const int openTapPin = 8;         // open tap manually
const int operatingModePin = 10;      // switch operating mode
const int defineTargetTemperatureButtonPin = 9;     // insert button pin
const int bluetoothPin = 13;
const int motorSwitch = 7;
//output
const int eletroValveHeaterPin = 4; // eletrovalve 2
const int eletroValveTapPin = 5;    // eletrovalve 1
const int motor1 = 3;
const int motor2 = 2;

// Variables
int redValue = 0;           // red value lcd 0-255
int greenValue = 0;         // green value lcd 0-255
int blueValue = 0;          // blue value lcd 0-255
char data_received = 0;
String data_sent = "";

int targetTemp = 0;         // target temperature the user wants, received from the APP
int currentTemp = 0;			// current temperature, to be sent to the APP

int temperatureSetting = CELSIUS_TEMP;	// pretended temperature unit, received from the APP

// Timers
int thermometerTimer = 0;
int cantReachTargetTemperatureTimer = 0;
int timerToReachTT;

// States
int operatingMode = AUTOMATIC_STATE;    // operating mode, to be sent to the APP
int systemState;

// Flags
int reachedTargetTemperature = 0; //0 no, 1 yes
int cantReachTargetTemperature = 0; //0 no, 1 yes
int heating = 0;

// LCD
rgb_lcd lcd;

OneWire oneWire(tempPin);
DallasTemperature sensors(&oneWire);

void setup() {

  pinMode(tempPin, INPUT);
  pinMode(openTapPin, INPUT);
  pinMode(operatingModePin, INPUT);
  pinMode(defineTargetTemperatureButtonPin, INPUT);
  pinMode(motorSwitch, INPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);

  pinMode(eletroValveTapPin, OUTPUT);
  pinMode(eletroValveHeaterPin, OUTPUT);
  pinMode(bluetoothPin, OUTPUT);

  Serial.begin(9600);
  sensors.begin();
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, HIGH);
  switchWaterFlowToTap();
  lcd.begin(16, 2);
  lcd.setRGB(0, 0, 255);

  delay(3000);

  lcd.setCursor(3, 0);
  lcd.print("Insta Heat");

  int i;
  for (i = 0; i < 100; i += 2) {
    blueValue = adjustBlueColorWithTemperature(i, 100);
    redValue = adjustRedColorWithTemperature(i, 100);
    lcd.setRGB(redValue, 0, blueValue);
    delay(25);
  }
  delay(3000);
  lcd.setRGB(0, 0, 0);
  lcd.clear();
  //resetDatabase();
}

void loop() {
  while (Serial.available() > 0) {
   data_received = Serial.read();
   Serial.println(data_received); // :^)
   if (data_received == '1'){
     digitalWrite(bluetoothPin, HIGH);
   } else if (data_received == '0') {
     digitalWrite(bluetoothPin, LOW);
   } else if(data_received == '2'){
     targetTemp = 25;
     reachedTargetTemperature = 0; //0 no, 1 yes
     cantReachTargetTemperature = 0; //0 no, 1 yes
   } else if(data_received == '3'){
     targetTemp = 27;
     reachedTargetTemperature = 0; //0 no, 1 yes
     cantReachTargetTemperature = 0; //0 no, 1 yes
   } else if(data_received == '4'){
     targetTemp = 29;
     reachedTargetTemperature = 0; //0 no, 1 yes
     cantReachTargetTemperature = 0; //0 no, 1 yes
   } else if(data_received == '5'){
     targetTemp = 31;
     reachedTargetTemperature = 0; //0 no, 1 yes
     cantReachTargetTemperature = 0; //0 no, 1 yes
   } else if(data_received == '6'){
     targetTemp = 33;
     reachedTargetTemperature = 0; //0 no, 1 yes
     cantReachTargetTemperature = 0; //0 no, 1 yes
   } else if(data_received == '7'){
     targetTemp = 35;
     reachedTargetTemperature = 0; //0 no, 1 yes
     cantReachTargetTemperature = 0; //0 no, 1 yes
   } else if(data_received == '8'){
     targetTemp = 37;
     reachedTargetTemperature = 0; //0 no, 1 yes
     cantReachTargetTemperature = 0; //0 no, 1 yes
   } else if(data_received == '9'){
     targetTemp = 39;
     reachedTargetTemperature = 0; //0 no, 1 yes
     cantReachTargetTemperature = 0; //0 no, 1 yes
   }
 }

  if (digitalRead(motorSwitch) == LOW) {
    digitalWrite(motor1, HIGH);
    digitalWrite(motor2, HIGH);
  } else if (digitalRead(motorSwitch) == HIGH) {
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, LOW);
  }

  // measures the temperature registered by the thermometer every second
  if (thermometerTimer == 0) {
    //currentTemp = convertAnalogSignalToTemperature((5.0 * analogRead(A1) * 100.0) / 1024, temperatureSetting);
    sensors.requestTemperatures();
    currentTemp = sensors.getTempCByIndex(0);
    startThermometerTimer();
  }
  delay(50);
  Serial.println(currentTemp);
  if (digitalRead(defineTargetTemperatureButtonPin) == LOW) {
    reachedTargetTemperature = 0; //0 no, 1 yes
    cantReachTargetTemperature = 0; //0 no, 1 yes
    targetTemp = getRotation();
  }

  if (digitalRead(operatingModePin) == HIGH) {
    operatingMode = AUTOMATIC_STATE;
  } else {
    operatingMode = MANUAL_STATE;
  }

  if (currentTemp < targetTemp) {
    if (heating == 0) { // the first time the TT > CT, this will happen
      switchWaterFlowToHeater();
      startTimer();
      startCantReachTargetTemperatureTimer();
      heating = 1;
    }
    if (cantReachTargetTemperatureTimer == ONE_SEC) {
      cantReachTargetTemperature = 1;
    }
  } else {
    if (heating == 1) {
      //updateTemperatureValue(targetTemp, timerToReachTT);
      heating = 0;
      if (operatingMode == MANUAL_STATE) {
        reachedTargetTemperature = 1;
      }
    }
    if (operatingMode == AUTOMATIC_STATE) {
      switchWaterFlowToTap();
      resetTargetTemp();
    }
  }
  
  if (operatingMode == MANUAL_STATE) {
    if (digitalRead(openTapPin) == LOW)  {
      if (digitalRead(eletroValveHeaterPin) == LOW) {
        switchWaterFlowToHeater();
      } else {
        switchWaterFlowToTap();
      }
    }
  }

  defaultDisplayInOpenedMode();

  updateTimers();
  delay(DEFAULT_DELAY);
}

void updateTimers() {
  if (heating = 0) timerToReachTT++;
  if (thermometerTimer > 0) thermometerTimer--;
  if (cantReachTargetTemperatureTimer > 0) cantReachTargetTemperatureTimer--;
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

void startTimer() {
  timerToReachTT = 0;
}

// default display
/*
  CT: 111C TT: 111C
  Auto/Manu(4)

*/
void defaultDisplayInOpenedMode() {
  lcd.clear();
  if (heating == 1) {
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
  switch (temperatureSetting) {
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
  switch (operatingMode) {
    case AUTOMATIC_STATE:
      lcd.print("Auto");
      break;
    case MANUAL_STATE:
      lcd.print("Manual");
      if (reachedTargetTemperature == 1) {
        lcd.setCursor(8, 1);
        lcd.print("!HOT!");
      }
      break;
    default:
      break;
  }
  if (cantReachTargetTemperature == 1) {
    lcd.setCursor(8, 1);
    lcd.print("!COLD!");
  }

}

// resets target temp to the minimum value acceptable
void resetTargetTemp() {
  targetTemp = 1;
}

// Get Rotation
int getRotation() {
  int sensor_value = analogRead(A2);
  double voltage;
  voltage = (double)sensor_value * 5 / 1023;
  voltage = (voltage * 300) / 5;
  int brightness;
  return brightness = map(voltage, 0, 300, 0, 75);
}
