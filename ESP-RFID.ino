// wifi
#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#endif
// Konfigurasi jaringan Wi-Fi
#define SERVER_IP "192.168.192.222"

#ifndef STASSID
#define STASSID "IoT"
#define STAPSK "12345678"
#endif
#include <ArduinoJson.h>
StaticJsonDocument<200> doc;
#include <MFRC522.h>

#define SDA_PIN D4
#define RST_PIN D3

MFRC522 mfrc522(SDA_PIN, RST_PIN);
WiFiClient client;
HTTPClient http;
unsigned long previousMillis = 0;
const unsigned long interval = 2000; // Waktu interval dalam milidetik

bool checkError(DeserializationError error) {
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return true;
  }
  return false;
}
// Konfigurasi server untuk mengirimkan data RFID UID
//void kirimDataRFID(String rfid)
//{
//  // wait for WiFi connection
//  if ((WiFi.status() == WL_CONNECTED)) {
//
//    // configure traged server and url
//    http.begin(client, "http://" SERVER_IP "/rfid/get_saldo.php");  // HTTP
//    http.addHeader("Content-Type", "application/json");
//
//    // Buat objek JSON
//    doc["uid"] = rfid;
//
//    // Konversi objek JSON menjadi String
//    String json;
//    serializeJson(doc, json);
//    Serial.print("[HTTP] POST...\n");
//    // start connection and send HTTP header and body
//    int httpCode = http.POST(json);
//    Serial.println(httpCode);
//    // httpCode will be negative on error
//    if (httpCode > 0) {
//      // HTTP header has been send and Server response header has been handled
//      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
//
//      // file found at server
//      if (httpCode == HTTP_CODE_OK) {
//        const String& payload = http.getString();
//        Serial.println("received payload: ");
//        Serial.println(payload);
//
//        // mengurai data JSON
//        DeserializationError error = deserializeJson(doc, payload);
//        // memeriksa kesalahan penguraian JSON
//        if (checkError(error)) {
//          return;
//        }
//        // memproses data JSON dan mengontrol relay
//        const char* status = doc["status"];
//        const char* pesan = doc["pesan"];
//        Serial.print(status);
//        Serial.print(" : ");
//        Serial.println(pesan);
//
//      }
//    } else {
//      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
//    }
//
//    http.end();
//  }
//}

void kirimDataRFID(String rfid)
{
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    // configure traged server and url
    http.begin(client, "http://" SERVER_IP "/rfid/get_uid.php?uid=" + rfid);

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header and body
    int httpCode = http.GET();
    Serial.println(httpCode);
    // httpCode will be negative on error
    if (httpCode > 0) {


      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload: ");
        Serial.println(payload);

      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
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

void setup()
{
  Serial.begin(115200);
  SPI.begin();             // inisialisasi SPI bus
  mfrc522.PCD_Init();       // inisialisasi MFRC522
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Mengupdate waktu sekarang

    String uid = readUID();
    if (uid.length() > 0) {
      Serial.println("UID tag: " + uid);
      kirimDataRFID(uid);
    }
  }
}