#define BLYNK_PRINT Serial
#include <OakOLED.h>
#define BLYNK_TEMPLATE_ID "TMPL3bOu9-ZKn"
#define BLYNK_TEMPLATE_NAME "Secure Latch"
#define BLYNK_AUTH_TOKEN "Ql0yPPYAElksixbxvOqtd3hErIla2zLB"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#define I2CADDR 0x20
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

OakOLED display;
const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6,7}; //connect to the column pinouts of the keypad

int trigPin = 14;   // Trigger D5
int echoPin = 12;  // Echo pin D6
Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574 );
long duration, cm;

void setup() {
   Wire.begin();
   pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
    Blynk.begin(BLYNK_AUTH_TOKEN, "NEGI", "NEGI#12345");

	  kpd.begin( makeKeymap(keys) );
  Serial.begin(9600);
  
  display.begin();
  display.clearDisplay();
  
  //Display Text
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(0, 0);
  display.println("Enter PIN(4 Digit):");
  display.display(); 
 

  // Turn the LED on.
}

String password = "";
String ogpass = "2516";
void loop() {
  Blynk.run();

   if (proxy() < 80) {
    char key = kpd.getKey();
    if (key) {
      Serial.println(key);
      display.setCursor(0, 0);
      if (key == '#') {
        if (password == ogpass) {

          display.clearDisplay();
          display.println("Welcome!!!");
          
          password = "";

        } else {
          display.clearDisplay();
          display.println("Intruder!!!");
          Blynk.logEvent("password", "intruder alert calling 100");
          
          password = "";
          
        }
      } else if (key == '*') {
        password.remove(password.length() - 1);
      } else {
        password = password + key;
      }

      display.print(password);
      display.display();
            Blynk.virtualWrite(V6, key);

      display.clearDisplay();

      Serial.println(key);
    
  }
   }
}

long proxy() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration / 2) / 29.1;  // Divide by 29.1 or multiply by 0.0343
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(250);
  return cm;
}

