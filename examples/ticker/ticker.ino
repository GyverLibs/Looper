#include <Arduino.h>
#include <Looper.h>

LP_TICKER_("tick0", []() {
    switch (Looper.thisState()) {
        case tState::Setup:
            Serial.println("SETUP");
            break;

        case tState::Loop:
            Serial.println("LOOP");
            Looper.thisTask()->removeLoop();
            break;

        case tState::Exit:
            Serial.println("EXIT");
            break;

        case tState::Event:
            Serial.println("EVENT");
            break;

        default: break;
    }
});

void setup() {
    Serial.begin(115200);

    LP_PUSH_EVENT("tick0", 0);
}

void loop() {
    Looper.loop();
}
