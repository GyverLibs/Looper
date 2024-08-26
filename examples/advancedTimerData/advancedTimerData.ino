#include <Arduino.h>
#include <Looper.h>

// создадим таймер, который будет выводить текст заданное количество раз с заданным периодом

// функция для удобного запуска таймера
void makeTimer(int period, int count, const char* text) {
    struct tdata_t {
        int count;
        const char* text;
    };

    new LoopTimerData<tdata_t>(period, new tdata_t{count, text}, [](tdata_t* data) {
        Serial.println(data->text);

        if (!data->count--) {
            delete data;
            delete Looper.thisTask();
        }
    });
}

void setup() {
    Serial.begin(115200);
    makeTimer(100, 5, "hello 1");
    makeTimer(500, 3, "hello 2");
}

void loop() {
    Looper.loop();
}
