#include "constants.h"
#include <stdio.h>

int changeOperatingMode(int operatingMode) {
	switch(operatingMode) {
		case AUTOMATIC_STATE:
			return MANUAL_STATE;
		case MANUAL_STATE:
			return AUTOMATIC_STATE;
		default:
			perror("ERROR - appSettings.cpp : changeOperatingMode - no such operating mode setting found");
			return operatingMode;
	}
}