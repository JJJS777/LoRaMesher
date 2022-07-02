#include <Arduino.h>
#include <unity.h>

#include <LoraMesher.h>

void testPacketLengthEqualPacketHeaderLength() {
    TEST_ASSERT_EQUAL(sizeof(PacketHeader), sizeof(Packet<uint8_t>));
}

void testPacket() {
    Serial.println("- Test Packet");
    RUN_TEST(testPacketLengthEqualPacketHeaderLength);
}