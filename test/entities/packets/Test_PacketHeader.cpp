#include <Arduino.h>
#include <unity.h>

#include <LoraMesher.h>

PacketHeader header;

void testPacketHeaderLength() {
    TEST_ASSERT_EQUAL(7, sizeof(PacketHeader));
}

void testPacketHeader() {
    Serial.println("- Test PacketHeader");
    RUN_TEST(testPacketHeaderLength);

}