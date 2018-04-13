#include <Wire.h>    /* Communication with Arduino */
#include <rgb_lcd.h> /* LCD */
#include <WiFi.h>    /* WiFi? */

/**
   Declaring the 16x2 lcd
*/
rgb_lcd lcd;

void setup() {
  /*
  pinMode(pin_number, OUTPUT); // Set a pin to output
  pinMode(pin_number, INPUT); // Set a pin to input
  */
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setRGB(red_value, green_value, blue_value);


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
