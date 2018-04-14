#include "constants.h"
#include <stdio.h>

int changeOperatingMode(int operatingMode) {
	switch(operatingMode) {
		case AUTOMATIC_STATE:
			return MANUAL_STATE;
		case MANUAL_STATE:
			return AUTOMATIC_STATE;
		default:
			//Serial.println("ERROR - appSettings.cpp : changeOperatingMode - no such operating mode setting found");
			return operatingMode;
	}
}

int adjustRedColorWithTemperature(int currentTemperature, int targetTemperature) {
	double ratio;

	// relative temperature
	ratio = (double)currentTemperature / targetTemperature;

	if(ratio < 0) return 0;
	if(ratio > 1) return 255;

	return ratio * 255;
}

int adjustBlueColorWithTemperature(int currentTemperature, int targetTemperature) {
	double ratio;

	// relative temperature
	ratio = (double)(targetTemperature - currentTemperature) / targetTemperature;

	if(ratio < 0) return 0;
	if(ratio > 1) return 255;

	return ratio * 255;
}
