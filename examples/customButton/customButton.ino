#include <Arduino.h>
#include <Looper.h>

class LoopButton : public LoopTickerBase {
    LP_MAKE_CALLBACK(ClickCallback, void);

   public:
    LoopButton(uint8_t pin, ClickCallback callback) : _pin(pin), _cb(callback) {
        pinMode(pin, INPUT_PULLUP);
    }

    void exec() {
        if (!digitalRead(_pin)) {
            if (!_flag) {
                _flag = true;
                _cb();
            }
        } else {
            if (_flag) _flag = false;
        }
    }

   private:
    uint8_t _pin;
    ClickCallback _cb;
    bool _flag;
};

LoopButton btn(2, []() {
    Serial.println("click!");
});

void setup() {
    Serial.begin(115200);
}
void loop() {
    Looper.loop();
}
