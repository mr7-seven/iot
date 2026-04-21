#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

const char* ws_host = "192.168.65.223"; // Ganti dengan ip server
const uint16_t ws_port = 1880;
const char* ws_path = "/ws/data";

WebSocketsClient webSocket;

// LED
const int ledPin = 2;
bool ledState = false;

// timing kirim data
unsigned long lastSend = 0;
const long interval = 10000;

// ======================
// SIMULASI SENSOR
// ======================
float readTemp() {
  return random(240, 320) / 10.0; // 24.0 - 32.0
}

float readHum() {
  return random(400, 800) / 10.0; // 40.0 - 80.0
}

// ======================
// HANDLE JSON MASUK
// ======================
void handleIncoming(uint8_t * payload, size_t length) {
  StaticJsonDocument<200> doc;

  DeserializationError err = deserializeJson(doc, payload, length);
  if (err) {
    Serial.println("[JSON] Parse failed");
    return;
  }

  if (doc["led"]) {
    String cmd = doc["led"];

    if (cmd == "ON") {
      digitalWrite(ledPin, HIGH);
      ledState = true;
      Serial.println("LED ON");
    } 
    else if (cmd == "OFF") {
      digitalWrite(ledPin, LOW);
      ledState = false;
      Serial.println("LED OFF");
    }
  }
}

// ======================
// EVENT WEBSOCKET
// ======================
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("[WS] Disconnected");
      break;

    case WStype_CONNECTED:
      Serial.println("[WS] Connected");
      break;

    case WStype_TEXT:
      Serial.printf("[WS] Received: %s\n", payload);
      handleIncoming(payload, length);
      break;

    default:
      break;
  }
}

// ======================
// WIFI NON-BLOCKING
// ======================
void connectWiFi() {
  static unsigned long lastAttempt = 0;
  const long retryInterval = 5000;

  if (WiFi.status() == WL_CONNECTED) return;

  if (millis() - lastAttempt > retryInterval) {
    Serial.println("[WiFi] Connecting...");
    WiFi.begin(ssid, password);
    lastAttempt = millis();
  }
}

// ======================
// KIRIM JSON
// ======================
void sendData() {
  StaticJsonDocument<200> doc;

  doc["temp"] = readTemp();
  doc["hum"] = readHum();
  doc["led"] = ledState ? "ON" : "OFF";

  String output;
  serializeJson(doc, output);

  webSocket.sendTXT(output);

  Serial.println("[WS] Sent:");
  Serial.println(output);
}

// ======================
// SETUP
// ======================
void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  webSocket.begin(ws_host, ws_port, ws_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

// ======================
// LOOP NON-BLOCKING
// ======================
void loop() {
  connectWiFi();

  if (WiFi.status() == WL_CONNECTED) {
    webSocket.loop();
  }

  if (millis() - lastSend > interval) {
    lastSend = millis();

    if (webSocket.isConnected()) {
      sendData();
    }
  }
}
