#include "constants.h"
#include <stdio.h>

double convertAnalogSignalToTemperature(int analogSignal, int tempSetting) {
	double newTemperature = analogSignal * CELSIUS_RATIO;
	
	switch(tempSetting) {
		case CELSIUS_TEMP:
			return newTemperature;
		case FAHRN_TEMP:
			return newTemperature * 1.8 + 32;
		case KELVIN_TEMP:
			return newTemperature - 273.15;
		default:
			perror("ERROR - calculations.cpp : convertAnalogSignalToTemperature - no such preferred temperature setting found");
			return -1;
	}
}

int increaseTargetTemperature(int currentTemperature) {
	if(validateTemperature(currentTemperature++)) return currentTemperature;
	return currentTemperature++;
}

int decreaseTargetTemperature(int currentTemperature) {
	if(validateTemperature(currentTemperature--)) return currentTemperature;
	return currentTemperature--;
}

bool validateTemperature(int temperature) {
	return (temperature < MAX_TEMP && temperature > MIN_TEMP);
}
