#pragma once

#include "LooperClass.h"
#include "nodes/Listener.h"
#include "nodes/Thread.h"
#include "nodes/Ticker.h"
#include "nodes/Timer.h"

// ========= macro =========
// широковещательное событие
#define LP_BROADCAST (hash_t)0

// отправить событие
#define LP_SEND_EVENT(id, data) Looper.sendEvent(LPH(id), data)

// отложить отправку события
#define LP_PUSH_EVENT(id, data) Looper.pushEvent(LPH(id), data)

#define LP_MAKE(type, ...) static type _LP_CONCAT(__loop_obj_, __COUNTER__)(__VA_ARGS__)
#define LP_MAKE_(id, type, ...) static type _LP_CONCAT(__loop_obj_, __COUNTER__)(LPH(id), __VA_ARGS__)
#define LP_NEW(type, ...) new type(__VA_ARGS__)
#define LP_NEW_(id, type, ...) new type(LPH(id), __VA_ARGS__)

// LISTENER
#define LP_LISTENER_(id, func) LP_MAKE_(id, LoopListener, func)

// TICKER
#define LP_TICKER(func) LP_MAKE(LoopTicker, func)
#define LP_TICKER_(id, func) LP_MAKE_(id, LoopTicker, func)

// TIMER
#define LP_TIMER(ms, func) LP_MAKE(LoopTimer, ms, func)
#define LP_TIMER_(id, ms, func) LP_MAKE_(id, LoopTimer, ms, func)

// THREAD
#define LP_THREAD_BEGIN()                 \
    switch (Looper.thisThread()->_case) { \
        case 0:;

#define LP_THREAD_END() \
    }                   \
    Looper.thisThread()->_case = 0;

#define _LP_THREAD_INNER(body) \
    LP_THREAD_BEGIN();         \
    body;                      \
    LP_THREAD_END();

#define LP_THREAD(body) static LoopThread _LP_CONCAT(__loop_obj_, __COUNTER__)([]() { _LP_THREAD_INNER(body) })
#define LP_THREAD_(id, body) static LoopThread _LP_CONCAT(__loop_obj_, __COUNTER__)(LPH(id), []() { _LP_THREAD_INNER(body) })
#define LP_THREAD_DATA(T, data, data_arg, body) static LoopThreadData<T> _LP_CONCAT(__loop_obj_, __COUNTER__)(data, [](data_arg) { _LP_THREAD_INNER(body) })
#define LP_THREAD_DATA_(id, T, data, data_arg, body) static LoopThreadData<T> _LP_CONCAT(__loop_obj_, __COUNTER__)(LPH(id), data, [](data_arg) { _LP_THREAD_INNER(body) })

// асинхронно ждать условия
#define LP_WAIT(cond)                             \
    Looper.thisThread()->_case = __COUNTER__ + 1; \
    case __COUNTER__:                             \
        if (!(cond)) return;

// асинхронно ждать события
#define LP_WAIT_EVENT(cond) LP_WAIT(Looper.thisEvent());

// асинхронно ждать время в мс
#define LP_SLEEP(ms)                                \
    do {                                            \
        Looper.thisThread()->_resetTmr();           \
        LP_WAIT(Looper.thisThread()->_elapsed(ms)); \
        Looper.thisThread()->_stopTmr();            \
    } while (0);

// освободить семафор
#define LP_SEM_SIGNAL(sem) sem++;

// ждать семафор
#define LP_SEM_WAIT(sem) \
    do {                 \
        LP_WAIT(sem);    \
        sem--;           \
    } while (0);

// семафор
typedef uint8_t LP_SEM;