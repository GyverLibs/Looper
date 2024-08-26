#include <Arduino.h>
#include <Looper.h>

// один таймер включает и выключает второй таймер

LP_TIMER(1000, []() {
    Looper["tmr"]->toggle();
});

LP_TIMER_("tmr", 100, []() {
    Serial.println(millis());
});

void setup() {
    Serial.begin(115200);
}

void loop() {
    Looper.loop();
}
