#ifndef LORA_RADIO_H
#define LORA_RADIO_H

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Defines a simple packet structure:
// [DEST_ADDR][SENDER_ADDR][MSG_ID][PAYLOAD_LEN][PAYLOAD]

class LoRaRadio {
public:
    // Constructor takes the pin configuration for the LoRa module.
    LoRaRadio(int csPin, int resetPin, int irqPin);

    // Initializes the LoRa radio with frequency and node addresses.
    // Returns true on success, false on failure.
    bool begin(long frequency, byte localAddr, byte destAddr);

    // Sends a data payload to the destination address.
    void sendData(const String& payload);

    // Checks for and receives a data payload.
    // Returns the payload string if a valid packet for this node is received.
    // Returns an empty string if no valid packet is available.
    String receiveData();

    // Gets the RSSI (Received Signal Strength Indicator) of the last packet.
    int getLastRssi();

private:
    // Pin configuration
    int _csPin;
    int _resetPin;
    int _irqPin;

    // Node addressing
    byte _localAddress;
    byte _destinationAddress;

    // Packet metadata
    byte _msgCount;
    int _lastRssi;
};

#endif // LORA_RADIO_H