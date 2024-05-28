#include <Arduino.h>
// wifi
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <PubSubClient.h>
#include <ArduinoJson.h>
// WiFi and MQTT settings
#define ssid "xxx"
#define password "xxx"
#define mqttServer "broker.hivemq.com"

#define requestTopic "rfid/request/C020322xxx"
#define responseTopic "rfid/response/C020322xxx"
//RFID
#include <MFRC522.h>

#define SDA_PIN D4
#define RST_PIN D3

MFRC522 mfrc522(SDA_PIN, RST_PIN);
#define BUZZER_PIN D2
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long previousMillis = 0;
const long interval = 500;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  // Convert payload to string
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  if (message.equals("success")) {
    // do something
  } else {
    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      delay(100);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected to MQTT");
      client.subscribe(responseTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial);
  SPI.begin();         // inisialisasi SPI bus
  mfrc522.PCD_Init();  // inisialisasi MFRC522
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    TapKartu();
  }
}

void TapKartu() {
  String uid = readUID();
  if (uid.length() > 0) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("UID tag: " + uid);
    JsonDocument doc;
    doc["uid"] = uid;
    String payload;
    serializeJson(doc, payload);
    // Publish data to MQTT topic
    client.publish(requestTopic, payload.c_str());
  }
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
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return "";
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
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
