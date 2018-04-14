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

int adjustRedColorWithTemperature(double currentTemperature, int targetTemperature) {
	double ratio;

	// absolute temperature
	//ratio = currentTemperature / MAX_TEMP;

	// OR

	// relative temperature
	ratio = currentTemperature / targetTemperature;

	if(ratio < 0) return 0;
	if(ratio > 1) return 255;

	return ratio * 255;
}

int adjustBlueColorWithTemperature(double currentTemperature, int targetTemperature) {
	double ratio;

	// absolute temperature
	//ratio = (MAX_TEMP - currentTemperature) / MAX_TEMP;

	// OR

	// relative temperature
	ratio = (targetTemperature - currentTemperature) / targetTemperature;

	if(ratio < 0) return 0;
	if(ratio > 1) return 255;

	return ratio * 255;
}
