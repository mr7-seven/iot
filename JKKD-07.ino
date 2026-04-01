#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

const char* serverUrl = "http://192.168.1.10:1880/api/led";

#define LED_PIN 2

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() - lastTime > timerDelay) {

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      http.begin(serverUrl);
      http.setTimeout(5000);

      int httpCode = http.GET();

      if (httpCode > 0) {
        String payload = http.getString();
        Serial.println("Response:");
        Serial.println(payload);

        StaticJsonDocument<200> doc;

        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
          const char* led = doc["led"];

          if (led != nullptr) {
            if (strcmp(led, "on") == 0) {
              digitalWrite(LED_PIN, HIGH);
              Serial.println("LED ON");
            } else {
              digitalWrite(LED_PIN, LOW);
              Serial.println("LED OFF");
            }
          } else {
            Serial.println("Key 'led' not found");
          }

        } else {
          Serial.print("JSON error: ");
          Serial.println(error.c_str());
        }

      } else {
        Serial.print("HTTP error: ");
        Serial.println(httpCode);
      }

      http.end();
    }

    lastTime = millis();
  }
}