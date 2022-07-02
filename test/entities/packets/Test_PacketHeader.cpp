#include <Arduino.h>
#include <unity.h>

#include <LoraMesher.h>

void testPacketHeaderLength() {
    TEST_ASSERT_EQUAL(7, sizeof(PacketHeader));
}

void testTotalPacketHeaderLength() {
    PacketHeader header;
    TEST_ASSERT_EQUAL(7, header.getPacketLength());

    header.payloadSize = 1;
    TEST_ASSERT_EQUAL(8, header.getPacketLength());
}

void testPacketHeader() {
    Serial.println("- Test PacketHeader");
    RUN_TEST(testPacketHeaderLength);
    RUN_TEST(testTotalPacketHeaderLength);
}