/*
  ThermalMqttasticPrinter.h - The counter part to my ThermalMqttastic npm library.
  Luis Beu
  http://luisbeu.de
*/

#ifndef ThermalMqttasticPrinter_h
#define ThermalMqttasticPrinter_h

#include <Stream.h>
#include "Arduino.h"

class ThermalMqttasticPrinter
{
public:
    /**
     * Constructor for the Printer class.
     * @param s Pointer to the stream object used for printing (default: Serial).
     * @param debugMode Flag indicating whether debug mode is enabled (default: false).
     */
    ThermalMqttasticPrinter(Stream *s = &Serial, bool debugMode = false);

    /**
     * Handles the MQTT topic based on the received topic and payload.
     * @param topic Pointer to the topic string.
     * @param payload Pointer to the payload data.
     * @param length Length of the payload data.
     */
    void handleTopic(char *topic, byte *payload, unsigned int length);

private:
    Stream *stream; // Pointer to the stream object for printing.
    bool debug;     // Flag indicating whether debug mode is enabled.

    /**
     * Handles the 'write' and 'writeBytes' topics payload by converting comma-separated numbers to integers and writing them to the stream.
     * @param payload Pointer to the payload data.
     * @param length Length of the payload data.
     */
    void handle(byte *payload, unsigned int length);
};

#endif
