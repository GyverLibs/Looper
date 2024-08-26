#include <Arduino.h>
#include <Looper.h>

// таймер на одно отложенное срабатывание
LP_TIMER(1000, []() {
    Serial.println("timer stop!");
    Looper.thisTimer()->stop();
});

void setup() {
    Serial.begin(115200);
}

void loop() {
    Looper.loop();
}
