#include <Arduino.h>
#include <Looper.h>
// раскомментируй нужный блок

// === blink
LP_THREAD({
    pinMode(LED_BUILTIN, OUTPUT);  // выполнится один раз

    while (true) {
        digitalWrite(LED_BUILTIN, 1);
        LP_DELAY(500);
        digitalWrite(LED_BUILTIN, 0);
        LP_DELAY(500);
    }
});
// === blink

// === цикл
// LP_THREAD({
//     static int i;

//     for (i = 0; i < 10; i++) {
//         Serial.println(i);
//         LP_DELAY(500);
//     }
// });
// === цикл

// === семафоры
// LP_SEM sem = 1;
// int val;

// LP_THREAD({
//     LP_SEM_WAIT(sem);
//     Serial.println("thr 1: ");
//     Serial.println(val);
//     LP_SEM_SIGNAL(sem);
// });

// LP_THREAD({
//     LP_SEM_WAIT(sem);
//     Serial.println("thr 2: ");
//     Serial.println(val);
//     LP_SEM_SIGNAL(sem);
// });

// LP_THREAD({
//     LP_SEM_WAIT(sem);
//     LP_DELAY(500);
//     val = random(100);
//     LP_SEM_SIGNAL(sem);
// });
// === семафоры

// === event
// LP_THREAD_("thread", {
//     Serial.println("wait event");
//     LP_WAIT_EVENT();
//     Serial.println("event!");
//     LP_DELAY(500);
// });

// LP_TIMER(1000, []() {
//     Looper.pushEvent("thread");
// });

// LP_THREAD({
//     LP_DELAY(2000);
//     Looper.pushEvent("thread");
// });
// === event

// === data
// LP_THREAD_DATA(int, new int(3), int* data, {
//     LP_DELAY(500);
//     Serial.println(*data);
//     *data += 1;
// });

// LoopThreadData<int> thread_manual(new int(3), [](int* data) {
//     LP_THREAD_BEGIN();

//     LP_DELAY(500);
//     Serial.println(*data);
//     *data += 1;

//     LP_THREAD_END();
// });
// === data

void setup() {
    Serial.begin(115200);
}

void loop() {
    Looper.loop();
}