#include <Arduino.h>
#include "loramesher.h"
#include "display.h"
#include "WString.h"

#define BOARD_LED 4
#define LED_ON LOW
#define LED_OFF HIGH

LoraMesher& radio = LoraMesher::getInstance();

uint32_t dataCounter = 0;
struct dataPacket {
    uint32_t counter = {0};
};

dataPacket* helloPacket = new dataPacket;

/**
 * @brief Flash the lead
 *
 * @param flashes number of flashes
 * @param delaymS delay between is on and off of the LED
 */
void led_Flash(uint16_t flashes, uint16_t delaymS) {
    uint16_t index;
    for (index = 1; index <= flashes; index++) {
        digitalWrite(BOARD_LED, LED_ON);
        vTaskDelay(delaymS / portTICK_PERIOD_MS);
        digitalWrite(BOARD_LED, LED_OFF);
        vTaskDelay(delaymS / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Print the counter of the packet
 *
 * @param data
 */
void printPacket(dataPacket* data, uint16_t sourceAddress) {
    char text[32];
    snprintf(text, 32, ("%X-> %d" CR), sourceAddress, data->counter);

    Screen.changeLineThree(String(text));
    Log.verbose(F("Received data nº %d" CR), data->counter);
}

/**
 * @brief Iterate through the payload of the packet and print the counter of the packet
 *
 * @param packet
 */
void printDataPacket(LoraMesher::userPacket<dataPacket>* packet) {
    Log.trace(F("Packet arrived from %X with size %d bytes" CR), packet->src, packet->payloadSize);

    //Get the payload to iterate through it
    dataPacket* dPacket = packet->payload;
    size_t payloadLength = radio.getPayloadLength(packet);

    printPacket(&dPacket[0], packet->src);

    Log.trace(F("---- Payload ---- Payload length in dataP: %d " CR), payloadLength);
    Log.setShowLevel(false);

    for (size_t i = 0; i < payloadLength; i++) {
        Log.verbose(F("Received data nº %d" CR), i);
        Log.verbose(F("%d -- "), i);
        Log.verbose(F("%d, "), dPacket[i].counter);
        Log.verbose(F("" CR));
    }

    Log.setShowLevel(true);
    Log.trace(F("---- Payload Done ---- " CR));

}

/**
 * @brief Function that process the received packets
 *
 */
void processReceivedPackets(void*) {
    for (;;) {
        /* Wait for the notification of processReceivedPackets and enter blocking */
        ulTaskNotifyTake(pdPASS, portMAX_DELAY);
        led_Flash(1, 100); //one quick LED flashes to indicate a packet has arrived

        //Iterate through all the packets inside the Received User Packets FiFo
        while (radio.getReceivedQueueSize() > 0) {
            Log.trace(F("ReceivedUserData_TaskHandle notify received" CR));
            Log.trace(F("Queue receiveUserData size: %d" CR), radio.getReceivedQueueSize() > 0);

            //Get the first element inside the Received User Packets FiFo
            LoraMesher::userPacket<dataPacket>* packet = radio.getNextUserPacket<dataPacket>();

            //Print the data packet
            printDataPacket(packet);

            //Delete the packet when used. It is very important to call this function to release the memory of the packet.
            radio.deletePacket(packet);

        }
    }
}

/**
 * @brief Initialize the LoRaMesher
 *
 */
void setupLoraMesher() {
    //Create a loramesher with a processReceivedPackets function
    radio.init(processReceivedPackets);

    Log.verbose("Lora initialized" CR);
}

/**
 * @brief Displays the address in the first line
 *
 */
void printAddressDisplay() {
    char addrStr[15];
    snprintf(addrStr, 15, "Id: %X\r\n", radio.getLocalAddress());

    Screen.changeLineOne(String(addrStr));
}


void printRoutingTableToDisplay() {
    char text[15];
    for (int i = 0; i < radio.routingTableSize(); i++) {
        LoraMesher::networkNode node = radio.routingTable[i].networkNode;
        snprintf(text, 15, ("|%X(%d)->%X"), node.address, node.metric, radio.routingTable[i].via);
        Screen.changeRoutingText(text, i);
    }

    Screen.changeSizeRouting(radio.routingTableSize());
    Screen.changeLineFour();
}


/**
 * @brief Every 20 seconds it will send a counter to a position of the dataTable
 *
 */
void sendLoRaMessage(void*) {
    uint16_t dstAddr = 0x8C20;
    bool addrInDataTable = false;

    for (;;) {

        //Search if the dstAddress is inside the routing table
        for (int i = 0; i < radio.routingTableSize() && !addrInDataTable; i++) {
            LoraMesher::routableNode node = radio.routingTable[i];

            if (node.networkNode.address == dstAddr)
                addrInDataTable = true;
        }

        if (addrInDataTable) {
            Log.trace(F("Send data packet nº %d to %X" CR), dataCounter, dstAddr);

            //Create packet and send it.
            radio.createPacketAndSend(dstAddr, helloPacket, 1);

            //Print second line in the screen
            Screen.changeLineTwo("Send " + String(dataCounter));

            //Increment data counter
            helloPacket->counter = dataCounter++;
        }

        //Print routing Table to Display
        printRoutingTableToDisplay();

        //Wait 120 seconds to send the next packet
        vTaskDelay(120000 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Setup the Task to create and send periodical messages
 *
 */
void createSendMessages() {
    TaskHandle_t sendLoRaMessage_Handle = NULL;
    BaseType_t res = xTaskCreate(
        sendLoRaMessage,
        "Send LoRa Message routine",
        4098,
        (void*) 1,
        1,
        &sendLoRaMessage_Handle);
    if (res != pdPASS) {
        Log.error(F("Send LoRa Message task creation gave error: %d" CR), res);
        vTaskDelete(sendLoRaMessage_Handle);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(BOARD_LED, OUTPUT); //setup pin as output for indicator LED

    Screen.initDisplay();
    Log.verbose("Board Init" CR);

    led_Flash(2, 125);          //two quick LED flashes to indicate program start
    setupLoraMesher();
    printAddressDisplay();

    if (radio.getLocalAddress() == 0xDE9C)
        createSendMessages();
}

void loop() {
    Screen.drawDisplay();
}