#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define WIFI_SSID "NAMA_WIFI"
#define WIFI_PASS "PASSWORD"

#define MQTT_HOST "broker.emqx.io"
#define MQTT_PORT 1883

#define DHTPIN 4
#define DHTTYPE DHT11

#define RELAYPIN 2

const char* PubTopic = "trom/123/sensor";
const char* SubTopic = "trom/123/relay";

AsyncMqttClient mqttClient;
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastSend = 0;
const long interval = 30000;  // 10 detik

void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void connectToMqtt() {
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT");
  mqttClient.subscribe(SubTopic, 0);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties,
                   size_t len, size_t index, size_t total) {

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, payload, len);

  if (error) return;

  const char* status = doc["status"];

  if (strcmp(status, "ON") == 0) {
    digitalWrite(RELAYPIN, LOW);
  } else if (strcmp(status, "OFF") == 0) {
    digitalWrite(RELAYPIN, HIGH);
  }
}

// fungsi kirim DHT11
void sendDHT() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  StaticJsonDocument<256> doc;

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read DHT");
    doc["suhu"] = nullptr;
    doc["kelembapan"] = nullptr;
  } else {
    doc["suhu"] = temp;
    doc["kelembapan"] = hum;
  }

  char buffer[256];
  serializeJson(doc, buffer);

  mqttClient.publish(PubTopic, 0, false, buffer);

  Serial.println("DHT sent");
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, HIGH);

  dht.begin();

  WiFi.onEvent([](WiFiEvent_t event) {
    if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
      connectToMqtt();
    }
  });

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
}

void loop() {
  unsigned long now = millis();

  // kirim tiap 10 detik tanpa blocking
  if (now - lastSend >= interval) {
    lastSend = now;
    sendDHT();
  }
}