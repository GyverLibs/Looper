#include <Arduino.h>
#include <Looper.h>

LP_TIMER(500, []() {
    Looper.pushEvent("timer");
});

void setup() {
    Serial.begin(115200);

    Looper.onEvent([](hash_t id) {
        Serial.println(id);
    });

    Looper.sendEvent("kek");
}

void loop() {
    Looper.loop();
}
