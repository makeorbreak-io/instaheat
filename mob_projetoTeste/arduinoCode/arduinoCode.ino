#define RELAY1  2 //Relay Motor 1
#define RELAY2  3 //Relay Motor 2
#define RELAY3  4 //Válvula 1
#define RELAY4  5 //Válvula 2
#define SwitchAUTO  6 //Switch 1
#define SwitchONOFF  7 //Switch 2
#define SwitchMANUAL  10 //Switch 3
#define BUTTON1  8 //BUTTON 1 (Aberto quando carregado)
#define BUTTON2  9 //BUTTON 2 (Aberto quando carregado)
#define TERMO  11
#define BLUETOOTH  13
int tempDesejada = 0;
float reading = 0;

#include "OneWire.h"
#include "DallasTemperature.h"
OneWire oneWire(TERMO);
float tempMin = 999;
float tempMax = 0;
DallasTemperature sensors(&oneWire);
char data_received = 0;
String data_sent = "";

//rgb_lcd lcd;

/*int redValue = 0;           // red value lcd 0-255
  int greenValue = 255;         // green value lcd 0-255
  int blueValue = 0;          // blue value lcd 0-255*/

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(SwitchAUTO, INPUT);
  pinMode(SwitchONOFF, INPUT);
  pinMode(SwitchMANUAL, INPUT);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
  digitalWrite(BUTTON1, INPUT);
  digitalWrite(BUTTON2, INPUT);
  digitalWrite(TERMO, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(BLUETOOTH, OUTPUT);
  //digitalWrite(RELAY3, LOW);
  //digitalWrite(RELAY4, LOW);

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
  while (Serial.available() > 0) {
    data_received = Serial.read();
    Serial.print(data_received);
    Serial.print("\n");
    if (data_received == '1')
      digitalWrite(BLUETOOTH, HIGH);
    else if (data_received == '0')
      digitalWrite(BLUETOOTH, LOW);
  }
  double temp = getTemp();
  delay(1000);
  Serial.print(temp);
  Serial.print("\n");

  if (digitalRead(7) == HIGH) {
    openMotor1(true);
    openMotor2(true);
  } else {
    openMotor1(false);
    openMotor2(false);
  }
  if (digitalRead(6) == HIGH) {
    openValve1(true);
    openValve2(true);
  } else {
    openValve1(false);
    openValve2(false);
  }
  if (digitalRead(8) == HIGH) {}
  if (digitalRead(9) == HIGH) {}

  //getRotation();

  //getTemp();

  //systemOnOff();

  /*int state = HIGH;
    int reading;
    int previous = LOW;

    long time = 0;
    long debounce = 200;

    if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;

    time = millis();
    }

    Serial.println(digitalRead(ONOFFSWITCH));

    previous = reading;*/

  /*Serial.println(digitalRead(BUTTON1));
    Serial.println(digitalRead(BUTTON2));
    Serial.println();

    //butoes();*/

  //testeTermo();
}

/*// Get Rotation
  void getRotation() {
  int sensor_value = analogRead(A2);
  float voltage;
  voltage = (float)sensor_value * 5 / 1023;
  float degrees = (voltage * 300) / 5;
  //int brightness;
  //brightness = map(degrees, 0, 300, 0, 255);
  Serial.println(degrees);
  }*/

// Temp tm35
double getTemp() {
  reading = (5.0 * analogRead(A1) * 100.0) / 1024; // Converts the analog voltage from sensor to digital reading where 5 is the supply voltage i.e. 5V
  // prints the data onto serial monitor
  Serial.print("Temperature is: "); //println prints next thing on a new line
  Serial.print((float)reading); // Prints current temperature on Monitor
  Serial.println(" *C");
  return reading;
}

/*//Switch System On Off
  void systemOnOff() {
  if (digitalRead(ONOFFSWITCH) == HIGH) {
  } else {
  digitalWrite(RELAY1, HIGH);         // Motor 1 Desligado
  digitalWrite(RELAY2, HIGH);         // Motor 2 Desligado

  digitalWrite(RELAY3, LOW);          // Válvula 1 Aberta
  // delay(500);
  //digitalWrite(RELAY3, HIGH);         // Válvula 1 Fechada

  digitalWrite(RELAY4, LOW);          // Válvula 2 Aberta
  //delay(500);
  //digitalWrite(RELAY4, HIGH);         // Válvula 2 Fechada
  }
  }

  // Butões Definir Temp / Abrir Torneira
  void butoes() {
  if (digitalRead(BUTTON1) == HIGH) {
  Serial.println("1");
  }
  if (digitalRead(BUTTON2) == HIGH) {
  Serial.println("2");
  }

  if (digitalRead(MANUAL) == HIGH) {
  Serial.println("PIN7 ON");
  } else {
  Serial.println("PIN7 OFF");
  }
  if (digitalRead(MANUAL2) == HIGH) {
  Serial.println("PIN10 ON");
  } else {
  Serial.println("PIN10 OFF");
  }
  }
*/
/*void testeButo() {
  Serial.println(digitalRead(Switch1));
  Serial.println(digitalRead(Switch3));
  //Serial.println(digitalRead(Switch2));
  //Serial.println(digitalRead(BUTTON1));
  //Serial.println(digitalRead(BUTTON2));
  Serial.println();
  }*/
/*void testeTermo() {
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
  Serial.print("Temperature is: ");
  Serial.print(sensors.getTempCByIndex(0));
  delay(1000);
  }*/

void openMotor1(boolean state) {
  if (state == true) {
    digitalWrite(RELAY1, LOW);
  } else {
    digitalWrite(RELAY1, HIGH);
  }
}
void openMotor2(boolean state) {
  if (state == true) {
    digitalWrite(RELAY2, LOW);
  } else {
    digitalWrite(RELAY2, HIGH);
  }
}
void openValve1(boolean state) {
  if (state == true) {
    digitalWrite(RELAY3, LOW);
  } else {
    digitalWrite(RELAY3, HIGH);
  }
}
void openValve2(boolean state) {
  if (state == true) {
    digitalWrite(RELAY4, LOW);
  } else {
    digitalWrite(RELAY4, HIGH);
  }
}
