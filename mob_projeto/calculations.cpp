#include "constants.h"
#include "readFromCSV.h"
#include "writeToCSV.h"
#include <stdio.h>

double convertAnalogSignalToTemperature(int analogSignal, int tempSetting) {
	double newTemperature = analogSignal * CELSIUS_RATIO;

	switch(tempSetting) {
		case CELSIUS_TEMP:
			return newTemperature;
		case FAHRN_TEMP:
			return newTemperature * 1.8 + 32;
		case KELVIN_TEMP:
			return newTemperature + 273.15;
		default:
			//Serial.println("ERROR - calculations.cpp : convertAnalogSignalToTemperature - no such preferred temperature setting found");
			return -1;
	}
}

bool validateTemperature(int temperature) {
	return (temperature < MAX_TEMP && temperature > MIN_TEMP);
}

int increaseTargetTemperature(int currentTemperature) {
  if(validateTemperature(currentTemperature+1)) return currentTemperature;
  return currentTemperature++;
}

int decreaseTargetTemperature(int currentTemperature) {
  if(validateTemperature(currentTemperature-1)) return currentTemperature;
  return currentTemperature--;
}

double convertNumberOfCyclesIntoSeconds(int numberOfCycles) {
	return numberOfCycles / DEFAULT_DELAY;
}
