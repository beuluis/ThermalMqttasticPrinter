/*
  ThermalMqttasticPrinter.cpp - The counter part to my ThermalMqttastic npm library.
  Luis Beu
  http://luisbeu.de
*/

#include "ThermalMqttasticPrinter.h"

ThermalMqttasticPrinter::ThermalMqttasticPrinter(Stream *s, bool debugMode) : stream(s), debug(debugMode)
{
    // Constructor for ThermalMqttasticPrinter class.
    // Assigns the provided stream to the member variable 'stream'.
    // Sets the debug mode based on the provided flag.
}

void ThermalMqttasticPrinter::handle(byte *payload, unsigned int length)
{
    // Handles the 'write' and 'writeBytes' topics payload by converting comma-separated numbers to integers and writing them to the stream.

    // Convert the payload to a String
    String payloadString(reinterpret_cast<char *>(payload), length);

    String number; // String to store the number being read

    for (unsigned int i = 0; i < payloadString.length(); i++)
    {
        // When we encounter a comma ',', we convert and write the number
        if (payloadString[i] == ',')
        {
            int value = number.toInt();
            stream->write(value);
            number = ""; // Reset the number string

            if (debug)
            {
                Serial.print("Debug - wrote: ");
                Serial.println(value);
            }
        }
        else
        {
            number += payloadString[i]; // Append the digit to the number string
        }
    }

    // Write the remaining number if any
    if (number.length() > 0)
    {
        int value = number.toInt();
        stream->write(value);

        // TODO: common log function
        if (debug)
        {
            Serial.print("Debug - wrote: ");
            Serial.println(value);
        }
    }
}

void ThermalMqttasticPrinter::handleTopic(char *topic, byte *payload, unsigned int length)
{
    // Handles the MQTT topic based on the received topic and payload.

    // Convert topic to String for comparison
    String topicString = String(topic);

    if (topicString == "write")
    {
        handle(payload, length);
        return;
    }

    if (topicString == "writeBytes")
    {
        handle(payload, length);
        return;
    }
}
