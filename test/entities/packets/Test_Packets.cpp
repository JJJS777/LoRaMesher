#include <Arduino.h>
#include <unity.h>

#include "Test_PacketHeader.cpp"
#include "Test_Packet.cpp"

void testPackets() {
    Serial.println("\t-- Test Packets --");
    testPacketHeader();
    testPacket();
}