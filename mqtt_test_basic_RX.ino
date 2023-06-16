#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Replace with your WiFi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Replace with your MQTT broker IP address
const char* mqttServer = "MQTT_BROKER_IP";
const int mqttPort = 1883;

// Replace with your MQTT server credentials
const char* mqttUsername = "your_MQTT_username";
const char* mqttPassword = "your_MQTT_password";

// MQTT topics
const char* topic = "your_topic";

// WiFi client instance
WiFiClient espClient;
PubSubClient client(espClient);

// Function to connect and reconnect to MQTT broker
void reconnect() {
  // Loop until we're connected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqttUsername, mqttPassword)) {
      Serial.println("connected");
      // Once connected, subscribe to the topic
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Callback function to handle MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  
  // Print the received message
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  // Start serial communication
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  
  // Set MQTT server and callback function
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  // If not connected to MQTT broker, reconnect
  if (!client.connected()) {
    reconnect();
  }
  
  // Maintain MQTT connection
  client.loop();
}
