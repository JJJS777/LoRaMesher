#include <Arduino.h>
#include <unity.h>

#include "Test_PacketService.cpp"

void testServices() {
    Serial.println("\t**** Test Services ****");
    testPacketService();
}