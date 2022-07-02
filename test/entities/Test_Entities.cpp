#include <Arduino.h>
#include <unity.h>

#include "packets\Test_Packets.cpp"

void testEntities() {
    Serial.println("\t**** Test Entities ****");
    testPackets();
}