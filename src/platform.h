#pragma once
#include <inttypes.h>

// размер стека отложенных событий
#define LOOPER_QUEUE_SIZE 3

// использовать ID задач для событий
#define LOOPER_USE_ID true

// использовать события
#define LOOPER_USE_EVENTS true

namespace looper {

// миллисекунды со старта МК
uint32_t millis();

// вызывается в цикле между задачами
void yield();

}