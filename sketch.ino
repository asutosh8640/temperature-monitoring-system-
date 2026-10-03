#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 15
#define DHTTYPE DHT22
#define LED_PIN 2
#define BUZZER_PIN 4

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long highTempStartTime = 0;
bool alarmActive = false;
unsigned long lastMqttPublish = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);

  dht.begin();
  WiFi.begin(ssid, password);
  client.setServer(mqtt_server, 1883);
}

void reconnectMQTT() {
  if (!client.connected()) {
    if (client.connect("ESP32_Temp_Client")) {
      Serial.println("MQTT Connected");
    }
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      reconnectMQTT();
    }
    client.loop();
  }

  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("Sensor Error!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" deg C");

    if (temp > 8.0) {
      if (highTempStartTime == 0) {
        highTempStartTime = millis();
      } else if (millis() - highTempStartTime >= 10000) {
        alarmActive = true;
      }
    } else {
      highTempStartTime = 0;
      alarmActive = false;
    }

    if (alarmActive) {
      digitalWrite(LED_PIN, HIGH);
      tone(BUZZER_PIN, 1000);
    } else {
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZER_PIN);
    }

    if (millis() - lastMqttPublish > 5000) {
      lastMqttPublish = millis();
      if (client.connected()) {
        char tempStr[8];
        dtostrf(temp, 6, 2, tempStr);
        client.publish("superhealth/room/temp", tempStr);
      }
    }
  }

  delay(1000); 
}
