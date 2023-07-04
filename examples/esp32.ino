#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <ThermalMqttasticPrinter.h>

const char* ssid = "Your WiFi SSID";
const char* password = "Your WiFi password";

const char* mqttServer = "Your MQTT server IP or hostname";
const int mqttPort = 1883;
const char* mqttUsername = "Your MQTT username";
const char* mqttPassword = "Your MQTT password";

HardwareSerial printerSerial(2); // Use Hardware Serial port, specify the appropriate serial port number (e.g., 1, 2, etc.)
WiFiClient espClient;
PubSubClient client(espClient);
ThermalMqttasticPrinter printer(&printerSerial, true); // True for debug mode. Writes to Serial

void logMessage(const char* message) {
  Serial.println(message);
}

void logError(const char* error) {
  Serial.print("Error: ");
  Serial.println(error);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  logMessage("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    logMessage("Connecting to WiFi...");
  }

  logMessage("Connected to WiFi");
}

void connectToMQTT() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback([](char* topic, byte* payload, unsigned int length) {
    printer.handleTopic(topic, payload, length);
  });

  logMessage("Connecting to MQTT...");

  while (!client.connected()) {
    if (client.connect("ThermalMqttasticPrinter", mqttUsername, mqttPassword)) {
      logMessage("Connected to MQTT");
      client.subscribe("write");
      client.subscribe("writeBytes");
    } else {
      logError("MQTT connection failed, retrying...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  printerSerial.begin(9600); // Initialize the thermal printer serial connection with the appropriate baud rate
  connectToWiFi();
  connectToMQTT();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }

  client.loop();
}
