
#include <Arduino.h>
#include <FastLED.h>
#include <HTTPClient.h>

#include "common.hpp"
#include "ledCOM.hpp"
#include "wifiCom.hpp"

#define BAUD_RATE (9600u) // Experimentally derived

#define CORE_NETWORK (0u) // Core for networking thread
#define CORE_LEDS    (1u) // Core for LED thread

#define QUEUE_LENGTH (10u) // Num elements in threadmsg queue


// Entrypoint, called automatically by Arduino code
void setup() {

    // Configure serial so we can get logs out
    Serial.begin(BAUD_RATE);
    Serial.println("Initiaised serial connection");

    // Create a queue that the two threads can share
    QueueHandle_t msgQueue = xQueueCreate(QUEUE_LENGTH, sizeof(ThreadMsg_t));
    Serial.printf("Queue ID is %p\n", msgQueue);

    // Create two tasks: one for network and one for LEDs
    TaskHandle_t taskLed;
    TaskHandle_t taskNetwork;
    xTaskCreatePinnedToCore(
        LedMain,     // Function to run
        "LedThread", // Task name
        2000,        // Stack size in words
        msgQueue,    // Parameter to pass in
        0,           // Priority
        &taskLed,    // Task handle out
        CORE_LEDS);  // Core binding

    Serial.println("LED started");

    xTaskCreatePinnedToCore(
        NetworkMain,
        "NetworkThread",
        35000,  // Experimental, no idea if this is sensible
        msgQueue,
        1,
        &taskNetwork,
        CORE_NETWORK);

    Serial.println("NET started");
}

void loop() {
    delay(10000);
};
