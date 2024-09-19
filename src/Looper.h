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

#define LP_MAKE(type, ...) type _LP_CONCAT(__loop_obj_, __COUNTER__)(__VA_ARGS__)
#define LP_MAKE_(id, type, ...) type _LP_CONCAT(__loop_obj_, __COUNTER__)(LPH(id), __VA_ARGS__)
#define LP_NEW(type, ...) new type(__VA_ARGS__)
#define LP_NEW_(id, type, ...) new type(LPH(id), __VA_ARGS__)

// LISTENER
#define LP_LISTENER_(id, ...) LP_MAKE_(id, LoopListener, __VA_ARGS__)

// TICKER
#define LP_TICKER(...) LP_MAKE(LoopTicker, __VA_ARGS__)
#define LP_TICKER_(id, ...) LP_MAKE_(id, LoopTicker, __VA_ARGS__)

// TIMER
#define LP_TIMER(ms, ...) LP_MAKE(LoopTimer, ms, __VA_ARGS__)
#define LP_TIMER_(id, ms, ...) LP_MAKE_(id, LoopTimer, ms, __VA_ARGS__)

// THREAD
#define _LP_THREAD_CASE Looper.thisThread()->_case
#define _LP_THREAD_RESET() _LP_THREAD_CASE = 0;

// начать поток
#define LP_THREAD_BEGIN()      \
    switch (_LP_THREAD_CASE) { \
        case 0:;

// завершить поток
#define LP_THREAD_END() \
    }                   \
    _LP_THREAD_RESET();

#define _LP_THREAD_INNER(body) \
    LP_THREAD_BEGIN();         \
    body                       \
    LP_THREAD_END();

#define LP_THREAD(body) static LoopThread _LP_CONCAT(__loop_obj_, __COUNTER__)([]() { _LP_THREAD_INNER(body) })
#define LP_THREAD_(id, body) static LoopThread _LP_CONCAT(__loop_obj_, __COUNTER__)(LPH(id), []() { _LP_THREAD_INNER(body) })
#define LP_THREAD_DATA(T, data, data_arg, body) static LoopThreadData<T> _LP_CONCAT(__loop_obj_, __COUNTER__)(data, [](data_arg) { _LP_THREAD_INNER(body) })
#define LP_THREAD_DATA_(id, T, data, data_arg, body) static LoopThreadData<T> _LP_CONCAT(__loop_obj_, __COUNTER__)(LPH(id), data, [](data_arg) { _LP_THREAD_INNER(body) })

// перезапустить поток (начать выполнение с начала)
#define LP_RESTART()        \
    do {                    \
        _LP_THREAD_RESET(); \
        return;             \
    } while (0);

// выйти из потока и потом вернуться в эту точку
#define LP_EXIT()                          \
    do {                                   \
        _LP_THREAD_CASE = __COUNTER__ + 1; \
        return;                            \
        case __COUNTER__:;                 \
    } while (0);

// асинхронно ждать условия
#define LP_WAIT(cond)                      \
    do {                                   \
        _LP_THREAD_CASE = __COUNTER__ + 1; \
        case __COUNTER__:                  \
            if (!(cond)) return;           \
    } while (0);

// асинхронно ждать события
#define LP_WAIT_EVENT() LP_WAIT(Looper.thisEvent());

// асинхронно ждать время в мс
#define LP_DELAY(ms)                                \
    do {                                            \
        Looper.thisThread()->_resetTmr();           \
        LP_WAIT(Looper.thisThread()->_elapsed(ms)); \
        Looper.thisThread()->_stopTmr();            \
    } while (0);

// устарело
#define LP_SLEEP(ms) LP_DELAY(ms)

// освободить семафор
#define LP_SEM_SIGNAL(sem) sem++;

// ждать семафор
#define LP_SEM_WAIT(sem) \
    do {                 \
        LP_WAIT(sem);    \
        sem--;           \
    } while (0);

// семафор
typedef uint16_t LP_SEM;