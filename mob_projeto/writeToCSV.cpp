/*#include <SD.h>
#include "constants.h"
#include <String.h>

const double DEFAULT_TIME = 0;
const double DEFAULT_INDEX = 0;

void resetDatabase() {
  if(!SD.begin(SD_CARD)) {
    Serial.println("ERROR - writeToCSV.cpp : resetDatabase - SD Card failed or non existant");
    return;
  }

  File csvFile = SD.open("database.csv", FILE_WRITE);

  if(!csvFile) {
    Serial.println("ERROR - writeToCSV.cpp : resetDatabase - Failed to open selected file");
    return;
  }

  int i;
  for(i = 0; i <= 75; i=i+5) { // i = temperatures 1-75
    writeLine(csvFile, i, DEFAULT_TIME, DEFAULT_INDEX);
  }

  csvFile.close();
}

void updateTemperatureValue(int targetTemperature, int numberOfCycles) {
  if(!SD.begin(SD_CARD)) {
    Serial.println("ERROR - writeToCSV.cpp : resetDatabase - SD Card failed or non existant");
    return;
  }

  File csvFile = SD.open("database.csv", FILE_WRITE);

  if(!csvFile) {
    Serial.println("ERROR - writeToCSV.cpp : resetDatabase - Failed to open selected file");
    return;
  }

  double newTime = numberOfCycles / DEFAULT_DELAY;
  String line = "";
  String textTemp = "", textTime, textIndex;
  int temp, index;
  double time, totalTime;

  while (csvFile.available()) {
    line = myFile.read();
    textTemp = line.substring(0,1);
    textTemp.trim();
    temp = textTemp.toInt();

    if(targetTemperature <= temp) {
      textTime = line.substring(6,9);
      textTime.trim();
      textIndex = line.substring(12,17);
      textIndex.trim();

      time = textTime.toDouble();
      index = textIndex.toInt();

      totalTime = time * index;
      index++;
      totalTime += newTime;
      time = newTime / index;
      break;
    }
  }

  writeLine(csvFile, temp, time, index);

  csvFile.close();
}

void writeLine(File file, int temp, double avgTime, int index) {
  file.print("%2d, %7.2f, %6d\n", temp, avgTime, index);
}*/
