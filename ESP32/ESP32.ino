#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// 🟢 WiFi credentials
const char* ssid = "r";
const char* password = "12345678";

// 🟢 Flask server URL
String serverDataURL = "http://10.92.124.27:5000/data";
String serverActuatorURL = "http://10.92.124.27:5000/actuator";

// 🟢 Sensors
const int ldrPin = 34;
const int pirPin = 32;
#define DHTPIN 12
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// 🟢 Relays
const int relay1Pin = 2;  // GPIO2 connected to Relay 1
const int relay2Pin = 27; // GPIO14 connected to Relay 2
bool relay1State = true;
bool relay2State = true;

void setup() {
  Serial.begin(115200);

  pinMode(pirPin, INPUT);
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // --- Read Sensors ---
    int ldrValue = analogRead(ldrPin);
    int lightLevel = 4095 - ldrValue;
    float lightPercent = (lightLevel / 4095.0) * 100.0;

    int pirState = digitalRead(pirPin);

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    if (isnan(humidity) || isnan(temperature)) {
      humidity = -1;
      temperature = -1;
    }

    // --- Send sensor data to Flask ---
    HTTPClient http;
    http.begin(serverDataURL);
    http.addHeader("Content-Type", "application/json");
    String payload = "{";
    payload += "\"ldr\":" + String(lightPercent, 1) + ",";
    payload += "\"pir\":" + String(pirState) + ",";
    payload += "\"temperature\":" + String(temperature,1) + ",";
    payload += "\"humidity\":" + String(humidity,1);
    payload += "}";
    int httpResponseCode = http.POST(payload);
    Serial.print("Sent data -> ");
    Serial.println(payload);
    Serial.println("Response code: " + String(httpResponseCode));
    http.end();

    // --- Get actuator command from Flask ---
    HTTPClient httpAct;
    httpAct.begin(serverActuatorURL);
    int httpCode = httpAct.GET();
    if (httpCode == 200) {
      String command = httpAct.getString(); // Expected format: {"relay1": "ON", "relay2": "OFF"}
      Serial.println(command);
          if (command.indexOf("\"relay1\":\"ON\"") != -1) {
              relay1State = true;
            } else {
              relay1State = false;
              }

            if (command.indexOf("\"relay2\":\"ON\"") != -1) {
              relay2State = true;
            } else {
              relay2State = false;
            }
         digitalWrite(relay2Pin, relay2State ? HIGH : LOW);
        digitalWrite(relay1Pin, relay1State ? HIGH : LOW);
       
      
      Serial.println("Relay1: " + String(relay1State ? "ON" : "OFF") +
                     ", Relay2: " + String(relay2State ? "ON" : "OFF"));
    }
    httpAct.end();
  }
  delay(2000);
}
