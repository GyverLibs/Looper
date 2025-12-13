#pragma once
#include <inttypes.h>

// размер стека отложенных событий
#ifndef LOOPER_QUEUE_SIZE
#define LOOPER_QUEUE_SIZE 3
#endif

// использовать ID задач для событий
#ifndef LOOPER_USE_ID
#define LOOPER_USE_ID true
#endif

// использовать события
#ifndef LOOPER_USE_EVENTS
#define LOOPER_USE_EVENTS true
#endif

namespace looper {

// миллисекунды со старта МК
uint32_t millis();

// вызывается в цикле между задачами
void yield();

}