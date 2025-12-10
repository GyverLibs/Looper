#include <Arduino.h>
#include <Looper.h>

LP_TIMER(500, []() {
    Serial.println("hello 1");
});

LP_TIMER(1000, []() {
    Serial.println("hello 2");
    Looper.delay(5000);
});

void setup() {
    Serial.begin(115200);
}
void loop() {
    Looper.loop();
}