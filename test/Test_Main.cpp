#include <Arduino.h>
#include <unity.h>

#include "entities/Test_Entities.cpp"

void setup() {
    Serial.begin(115200);

    UNITY_BEGIN();
    testEntities();

    UNITY_END();
}

void loop() {
    // UNITY_END(); // stop unit testing
}