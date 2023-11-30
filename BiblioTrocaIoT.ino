#include "SevSeg.h"
#include "HTTPClient.h"
SevSeg unitDisplay;
SevSeg tenUnitDisplay;
#define sensor 26
#define reset 25
#define wait 33
#define httpLED 32
int i = 0;
int x = 0;
String server = "https://serverbibliotroca-production.up.railway.app/api/v1/bibliotroca/livros/ponto-de-coleta";
const char* ssid = "FATECZL";
const char* password = "DSM@2023";
 
void setup() {
  pinMode(sensor, INPUT);
  pinMode(reset, INPUT);
  pinMode(wait, OUTPUT);
  pinMode(httpLED, OUTPUT);
  digitalWrite(httpLED, LOW);
  digitalWrite(wait, LOW);
  byte displays = 1;
  byte CommonPins[] = {};
  byte LEDsegmentPins[] = {22, 23, 14, 12, 13, 21, 19};
  bool resistors = true;
  unitDisplay.begin(COMMON_CATHODE, displays, CommonPins, LEDsegmentPins, resistors);
  unitDisplay.setBrightness(80);
  byte LEDsegmentPins2[] = {4, 16, 18, 5, 17, 2, 15};
  tenUnitDisplay.begin(COMMON_CATHODE, displays, CommonPins, LEDsegmentPins2, resistors);
  tenUnitDisplay.setBrightness(80);
  unitDisplay.setNumber(i);
  tenUnitDisplay.setNumber(x);
  unitDisplay.refreshDisplay();
  tenUnitDisplay.refreshDisplay();
  Serial.begin(9600);
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}
 
void loop() {
 
  if(digitalRead(reset) == 1) {
    i = 0;
    x = 0;
    unitDisplay.setNumber(i);
    tenUnitDisplay.setNumber(x);
    unitDisplay.refreshDisplay();
    tenUnitDisplay.refreshDisplay();
  }
 
  if(digitalRead(sensor) == 0 && digitalRead(wait) == LOW) {
    digitalWrite(wait, HIGH);
    i++;
    if(i>9) {
      x++;
      i = 0;
      unitDisplay.setNumber(i);
      tenUnitDisplay.setNumber(x);
      unitDisplay.refreshDisplay();
      tenUnitDisplay.refreshDisplay();
    }
    if(x>9) {
      i = 0;
      x = 0;
      unitDisplay.setNumber(i);
      tenUnitDisplay.setNumber(x);
      unitDisplay.refreshDisplay();
      tenUnitDisplay.refreshDisplay();
    }
    unitDisplay.setNumber(i);
    unitDisplay.refreshDisplay();
 
 
 
    HTTPClient http;
    http.begin(server.c_str());
    int response = http.GET();
    Serial.print(response);
    if (response>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(response);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(response);
      }
    if (response>0) {
      digitalWrite(httpLED, HIGH);
      delay(1000);
      digitalWrite(httpLED, LOW);
    }
    http.end();
 
 
    delay(500);
  }
 
  if(digitalRead(sensor) == 1 && digitalRead(wait) == HIGH) {
    digitalWrite(wait, LOW);
    delay(500);
  }
}
