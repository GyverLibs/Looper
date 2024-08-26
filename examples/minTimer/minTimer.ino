#include <Arduino.h>
#include <Looper.h>

LP_TIMER(500, [](){
    Serial.println("hello!");
});

void setup() {
    Serial.begin(115200);
}
void loop() {
    Looper.loop();
}
