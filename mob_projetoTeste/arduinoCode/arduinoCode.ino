#define RELAY1  2
#define RELAY2  3
#define RELAY3  4
#define RELAY4  5
#define ONOFFSWITCH  6 //Switch ON OFF
#define MANUAL  7 //Manual
#define BUTTON1  8 //Butão 1
#define BUTTON2  9 //Butão 2
#define MANUAL2 10 //Manual

int tempDesejada = 0;
float reading = 0;

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
  pinMode(MANUAL, INPUT);
  pinMode(MANUAL2, INPUT);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
  digitalWrite(BUTTON1, INPUT);
  digitalWrite(BUTTON2, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

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

  //getRotation();

  //getTemp();

  systemOnOff();

  //butoes();

  delay(1000);
}

void getRotation() {
  int sensor_value = analogRead(A3);
  float voltage;
  voltage = (float)sensor_value * 5 / 1023;
  float degrees = (voltage * 300) / 5;
  //int brightness;
  //brightness = map(degrees, 0, 300, 0, 255);
  Serial.println(degrees);
}

void getTemp() {
  reading = (5.0 * analogRead(A2) * 100.0) / 1024; // Converts the analog voltage from sensor to digital reading where 5 is the supply voltage i.e. 5V
  // prints the data onto serial monitor
  Serial.print("Temperature is: "); //println prints next thing on a new line
  Serial.print((float)reading); // Prints current temperature on Monitor
  Serial.println(" *C");
}

void systemOnOff() {
  if (digitalRead(ONOFFSWITCH) == LOW) {
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
