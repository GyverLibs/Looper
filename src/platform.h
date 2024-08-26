#pragma once
#include <inttypes.h>

// размер стека отложенных событий
#define LOOPER_QUEUE_SIZE 3

namespace looper {

// миллисекунды со старта МК
uint32_t millis();

// вызывается в цикле между задачами
void yield();

}