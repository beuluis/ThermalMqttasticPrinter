#include <Arduino.h>

#include <Ethernet.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <ThermalMqttasticPrinter.h>

const char* mqttServer = "Your MQTT server IP or hostname";
const int mqttPort = 1883;
const char* mqttUsername = "Your MQTT username";
const char* mqttPassword = "Your MQTT password";

SoftwareSerial printerSerial(10, 11); // Software Serial pins (RX, TX)
EthernetClient ethClient;
PubSubClient client(ethClient);
ThermalMqttasticPrinter printer(&printerSerial, true); // True for debug mode. Writes to Serial

void logMessage(const char* message) {
  Serial.println(message);
}

void logError(const char* error) {
  Serial.print("Error: ");
  Serial.println(error);
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

  Ethernet.begin(12345678); // Replace 'test' with your Ethernet shield MAC address

  connectToMQTT();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }

  client.loop();
}
