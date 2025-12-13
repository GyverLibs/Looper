#include <Arduino.h>
#include <Looper.h>

LP_TIMER(500, []() {
    LP.pushEvent("timer");
});

void setup() {
    Serial.begin(115200);

    LP.onEvent([](hash_t id) {
        Serial.println(id);
    });

    LP.sendEvent("kek");
}

void loop() {
    LP.loop();
}
