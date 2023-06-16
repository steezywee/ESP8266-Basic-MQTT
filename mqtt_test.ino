#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Replace with your WiFi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// MQTT broker details
const char* mqttServer = "your_MQTT_broker_IP";
const int mqttPort = 1883;
const char* mqttUser = "your_MQTT_username";
const char* mqttPassword = "your_MQTT_password";

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// Timer variables
unsigned long previousMillis = 0;
const long interval = 5000;  // 5 seconds

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to MQTT broker
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  // Reconnect to MQTT if connection is lost
  if (!client.connected()) {
    reconnect();
  }

  // Send MQTT message every 5 seconds
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sendMessage();
  }

  // Process MQTT messages
  client.loop();
}

void sendMessage() {
  // Create a message with topic and payload
  char message[50];
  snprintf(message, sizeof(message), "Hello World");

  // Publish the message
  client.publish("ESPTest", message);
  Serial.println("Message sent");
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages (if needed)
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Reconnecting to MQTT broker...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
