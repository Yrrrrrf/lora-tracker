#include "LoRaRadio.h"

// Broadcast address
const byte BROADCAST_ADDRESS = 0xFF;

LoRaRadio::LoRaRadio(int csPin, int resetPin, int irqPin)
    : _csPin(csPin), _resetPin(resetPin), _irqPin(irqPin), 
      _localAddress(0), _destinationAddress(0), _msgCount(0), _lastRssi(0) {
}

bool LoRaRadio::begin(long frequency, byte localAddr, byte destAddr) {
    _localAddress = localAddr;
    _destinationAddress = destAddr;

    LoRa.setPins(_csPin, _resetPin, _irqPin);
    
    if (!LoRa.begin(frequency)) {
        return false; // Initialization failed
    }
    
    // You can configure other LoRa parameters here if needed
    // LoRa.setSyncWord(0xF3); // To prevent interference with other networks
    // LoRa.setTxPower(20);    // Set transmit power, default is 17
    
    return true; // Success
}

void LoRaRadio::sendData(const String& payload) {
    LoRa.beginPacket();
    LoRa.write(_destinationAddress);      // Add destination address
    LoRa.write(_localAddress);            // Add sender address
    LoRa.write(_msgCount);                // Add message ID
    LoRa.write(payload.length());         // Add payload length
    LoRa.print(payload);                  // Add payload
    LoRa.endPacket();
    _msgCount++;
}

String LoRaRadio::receiveData() {
    int packetSize = LoRa.parsePacket();
    if (packetSize == 0) {
        return ""; // No packet available
    }

    // Read packet header
    byte recipient = LoRa.read();
    byte sender = LoRa.read();
    byte msgId = LoRa.read();
    byte length = LoRa.read();

    // Check if the packet is for this node (or broadcast)
    if (recipient != _localAddress && recipient != BROADCAST_ADDRESS) {
        return ""; // Not for us
    }

    String incoming = "";
    while (LoRa.available()) {
        incoming += (char)LoRa.read();
    }

    // Check if payload length matches header
    if (length != incoming.length()) {
        return ""; // Packet integrity error
    }

    // Store signal strength of the received packet
    _lastRssi = LoRa.packetRssi();

    return incoming;
}

int LoRaRadio::getLastRssi() {
    return _lastRssi;
}