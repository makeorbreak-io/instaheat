#include "rgb_lcd.h" /* LCD */
#include <Wire.h> /* LCD */

#define RELAY1  2
#define RELAY2  3
#define RELAY3  4
#define RELAY4  5

rgb_lcd lcd;

int redValue = 0;           // red value lcd 0-255
int greenValue = 255;         // green value lcd 0-255
int blueValue = 0;          // blue value lcd 0-255

void setup() {
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  /*pinMode(RELAY3, OUTPUT);
    pinMode(RELAY4, OUTPUT);*/

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
  delay(500);                                      // Wait 2 seconds
  digitalWrite(RELAY1, HIGH);         // Turns Relay Off

  digitalWrite(RELAY2, LOW);          // Turns ON Relays 2
  delay(500);                                      // Wait 2 seconds
  digitalWrite(RELAY2, HIGH);         // Turns Relay Off

  /*digitalWrite(RELAY3, LOW);          // Turns ON Relays 3
    delay(700);                                      // Wait 2 seconds
    digitalWrite(RELAY3, HIGH);         // Turns Relay Off

    digitalWrite(RELAY4, LOW);          // Turns ON Relays 4
    delay(700);                                      // Wait 2 seconds
    digitalWrite(RELAY4, HIGH);         // Turns Relay Off*/



  lcd.setRGB(redValue, greenValue, blueValue);
  lcd.newclear(10);
  lcd.setCursor(0, 0);
  lcd.print("Message");
  lcd.clear();
}
