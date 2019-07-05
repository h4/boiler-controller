#include <Arduino.h>

#define LED_R 16
#define LED_G D5
#define ECHO 12
#define TRIG 13
#define TEMP_DATA 5
#define RELAY 4

void setup() {
  Serial.begin(115200);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(RELAY, OUTPUT);
  digitalWrite(LED_R, LOW);
  digitalWrite(RELAY, LOW);
  digitalWrite(LED_G, LOW);
}

int s = 0;
bool relayOn = false;

void loop() {
  Serial.println(millis());
  s++;
  delay(1000);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_R, LOW);
  delay(1000);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_R, HIGH);
  if (s % 4 == 0) {
    relayOn = !relayOn;
    digitalWrite(RELAY, relayOn);
    s = 0;
  }
}
