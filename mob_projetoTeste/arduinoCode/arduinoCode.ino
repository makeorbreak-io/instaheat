//#include "rgb_lcd.h" /* LCD */
//#include <Wire.h> /* LCD */

#define RELAY1  2
#define RELAY2  3
#define RELAY3  4
#define RELAY4  5
#define ONOFFSWITCH  6 //Switch ON OFF
#define TEMPMAIS  7 //TEMP+
#define TEMPMENOS  8 //TEMP-

int tempDesejada = 0;

//rgb_lcd lcd;

/*int redValue = 0;           // red value lcd 0-255
  int greenValue = 255;         // green value lcd 0-255
  int blueValue = 0;          // blue value lcd 0-255*/

void setup() {
  Serial.begin(9600);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(ONOFFSWITCH, INPUT);
  pinMode(TEMPMAIS, INPUT);
  pinMode(TEMPMENOS, INPUT);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);

  /*Serial.begin(9600);

    lcd.begin(16, 2);
    lcd.setRGB(redValue, greenValue, blueValue);


    lcd.newclear(10);
    lcd.setCursor(0, 0);
    lcd.print("Message");

    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("MyPlant");
    delay(1000);*/

}

void loop() {
  Serial.println(tempDesejada );
  if (digitalRead(TEMPMAIS ) == LOW) {
    tempDesejada++;
  }
  if (digitalRead(TEMPMENOS ) == LOW) {
    tempDesejada--;
  }
  if (digitalRead(ONOFFSWITCH) == LOW) {
  } else {
    digitalWrite(RELAY1, HIGH);         // Motor 1 Desligado
    digitalWrite(RELAY2, HIGH);         // Motor 2 Desligado

    digitalWrite(RELAY3, LOW);          // Válvula 1 Aberta
    delay(500);
    digitalWrite(RELAY3, HIGH);         // Válvula 1 Fechada

    digitalWrite(RELAY4, LOW);          // Válvula 2 Aberta
    delay(500);
    digitalWrite(RELAY4, HIGH);         // Válvula 2 Fechada
  }
  delay(1000);

  /* lcd.setRGB(redValue, greenValue, blueValue);
    lcd.newclear(10);
    lcd.setCursor(0, 0);
    lcd.print("Message");
    lcd.clear();*/
}
