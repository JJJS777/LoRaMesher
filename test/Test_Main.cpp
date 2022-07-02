#include <Arduino.h>
#include <unity.h>

//Define LoRaMesher god mode, to test private functions too.
#define LM_GOD_MODE

#include "entities/Test_Entities.cpp"
#include "services/Test_Services.cpp"

void setup() {
    Serial.begin(115200);

    UNITY_BEGIN();
    testEntities();
    testServices();

    UNITY_END();
}

void loop() {
    // UNITY_END(); // stop unit testing
}