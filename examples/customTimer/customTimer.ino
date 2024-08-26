#include <Arduino.h>
#include <Looper.h>

class LoopHello : public LoopTimerBase {
   public:
    LoopHello(uint32_t ms) : LoopTimerBase(ms) {}

    void exec() {
        Serial.println("hello!");
    }
};

LoopHello hello(300);

void setup() {
    Serial.begin(115200);
}
void loop() {
    Looper.loop();
}
