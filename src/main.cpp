#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <creds.h>

#define LED_R_PIN 16
#define LED_G_PIN 14
#define RELAY_PIN 4
#define ECHO 12
#define TRIG 13
#define TEMP_DATA 5

const char* ssid = STASSID;
const char* password = STAPSK;

void setup() {
  Serial.begin(115200);

  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(LED_R_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_G_PIN, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

bool relayState = LOW;
bool ledRState = HIGH;
bool ledGState = LOW;

int switchDelay = 20 * 1000;
int ledDelay = 2 * 1000;
int switchTs = millis();
int ledTs = millis();

void loop() {
  ArduinoOTA.handle();
  
  int now = millis();

  if (now - ledTs >= ledDelay) {
    ledRState = !ledRState;
    ledGState = !ledGState;

    digitalWrite(LED_R_PIN, ledRState);
    digitalWrite(LED_G_PIN, ledGState);
    ledTs = now;
  }

  if (now - switchTs >= switchDelay) {
    relayState = !relayState;

    digitalWrite(RELAY_PIN, relayState);
    switchTs = now;
  }
}
