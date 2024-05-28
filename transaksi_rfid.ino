#include <Arduino.h>
// wifi
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <PubSubClient.h>
// WiFi and MQTT settings
#define ssid  "xxx"
#define password  "xxx"
#define mqttServer  "xxx"
#define mqttPort 1883
const char* mqttUser = "admin";
const char* mqttPassword = "elka123";
#define mqttTopic  "rfid/request/C020322xxx"
//RFID
#include <MFRC522.h>

#define SDA_PIN D4
#define RST_PIN D3

MFRC522 mfrc522(SDA_PIN, RST_PIN);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

#include <ArduinoJson.h>
StaticJsonDocument<200> doc;

unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
const long interval = 5000;  // Interval between readings (milliseconds)
const long retryInterval = 5000;
void setup()
{
  Serial.begin(115200);
  while (!Serial);
  SPI.begin();             // inisialisasi SPI bus
  mfrc522.PCD_Init();       // inisialisasi MFRC522
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  mqttClient.setServer(mqttServer, mqttPort);
}

void loop()
{
  if (!mqttClient.connected()) {
    connectToMqtt();
  }
  mqttClient.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    kirimData();
  }
}

void kirimData() {
  char payload[100];

    String uid = readUID();
    if (uid.length() > 0) {
      Serial.println("UID tag: " + uid);
          // Create JSON payload
    doc["uid"] = uid;

    serializeJson(doc, payload);
    // Publish data to MQTT topic
    mqttClient.publish(mqttTopic, payload);
    }

}

void connectToMqtt() {
  if (millis() - previousMillis1 >= retryInterval) {
    previousMillis1 = millis();

    while (!mqttClient.connected()) {
      Serial.println("Connecting to MQTT...");
      // Create a random client ID
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      //if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword)) {
       if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected to MQTT");
        mqttClient.subscribe(mqttTopic);
      } else {
        Serial.print("MQTT connection failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" Retrying in 5 seconds...");
      }
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle MQTT messages if needed
    Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


bool checkError(DeserializationError error) {
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return true;
  }
  return false;
}

String readUID() {
  // mencari kartu RFID
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return "";
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return "";
  }

  // menampilkan UID kartu RFID yang dibaca
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  // membersihkan kartu RFID dari MFRC522
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  return uid;
}
