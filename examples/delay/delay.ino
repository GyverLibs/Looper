#include <Arduino.h>
#include <Looper.h>

LP_TIMER(500, []() {
    Serial.println("hello 1");
});

LP_TICKER([]() {
    Serial.println("hello 2");
    LP.delay(5000);
});

void setup() {
    Serial.begin(115200);
}
void loop() {
    LP.loop();
}