#include <Arduino.h>
#include <unity.h>

#include <LoraMesher.h>

//Define LoRaMesher god mode, to test private functions too.
#define LM_GOD_MODE

//TODO: If is ControlPacket -> Is DataPacket.

void testHasDataPacket() {
    TEST_ASSERT_TRUE(PacketService::hasDataPacket(DATA_P | NEED_ACK_P | ACK_P | XL_DATA_P | LOST_P | SYNC_P));
}

void testHasNotDataPacket() {
    TEST_ASSERT_FALSE(PacketService::hasDataPacket(HELLO_P));
}

void testHasControlPacket() {
    TEST_ASSERT_TRUE(PacketService::hasControlPacket(NEED_ACK_P | ACK_P | XL_DATA_P | LOST_P | SYNC_P));
}

void testHasNotControlPacket() {
    TEST_ASSERT_FALSE(PacketService::hasDataPacket(HELLO_P | DATA_P));
}

void testGetExtraLengthToPayloadDataPacket() {
    TEST_ASSERT_EQUAL(2, PacketService::getExtraLengthToPayload(DATA_P));
}

void testGetExtraLengthToPayloadControlPacketAndDataPacket() {
    TEST_ASSERT_EQUAL(5, PacketService::getExtraLengthToPayload(NEED_ACK_P | ACK_P | XL_DATA_P | LOST_P | SYNC_P));
}

void testDataPacketLength() {
    uint8_t dataPLen = sizeof(DataPacket<uint8_t>) + sizeof(PacketHeader);
    TEST_ASSERT_EQUAL(9, dataPLen);
}

void testControlPacketLength() {
    uint8_t controlPLen = sizeof(ControlPacket<uint8_t>) + sizeof(PacketHeader);
    TEST_ASSERT_EQUAL(10, controlPLen);
}

void testPacketService() {
    Serial.println("- Test Packet Service");
    RUN_TEST(testHasDataPacket);
    RUN_TEST(testHasNotDataPacket);
    RUN_TEST(testHasControlPacket);
    RUN_TEST(testHasNotControlPacket);
    RUN_TEST(testGetExtraLengthToPayloadDataPacket);
    RUN_TEST(testGetExtraLengthToPayloadControlPacketAndDataPacket);
    RUN_TEST(testDataPacketLength);
    RUN_TEST(testControlPacketLength);
}