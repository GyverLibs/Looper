#pragma once
#include <inttypes.h>
#include <stddef.h>

#include "../platform.h"
#include "../utils/flags.h"
#include "../utils/hash.h"
#include "../utils/list.h"
#include "../utils/macro.h"

#define TASK_ENABLED (1 << 0)
#define TASK_IS_LISTENER (1 << 1)
#define TASK_IS_TICKER (1 << 2)
#define TASK_IS_TIMER (1 << 3)
#define TASK_HAS_EVENTS (1 << 4)
#define TASK_HAS_STATES (1 << 5)

#define TASK_ENABLED_TICKER (TASK_ENABLED | TASK_IS_TICKER)
#define TASK_ENABLED_TIMER (TASK_ENABLED | TASK_IS_TIMER)

enum class tState : uint8_t {
    None,
    Setup,
    Loop,
    Exit,
    Event,
};

LP_MAKE_CALLBACK(TaskCallback, void);

class LoopTask : public looper::List<LoopTask>::Node {
   public:
    LoopTask(hash_t id, TaskCallback callback, uint8_t type, bool states, bool events) : _cb(callback) {
#if LOOPER_USE_ID
        _id = id;
#endif
        sreg.set(TASK_ENABLED | type);
        if (states) enableStates();
        if (events) enableEvents();
        addLoop();
    }
    ~LoopTask() {
        removeLoop();
    }

    // добавить в loop
    void addLoop();

    // убрать из loop
    void removeLoop();

    // вызвать обработчик
    void exec();

    // подключить обработчик
    void attach(TaskCallback callback);

    // отключить обработчик
    void detach();

    // id задачи
    hash_t id();

    // включить задачу
    void enable();

    // выключить задачу
    void disable();

    // задача запущена
    bool isEnabled();

    // включить если выключена и наоборот
    void toggle();

    // включить обработку событий
    void enableEvents();

    // отключить обработку событий
    void disableEvents();

    // включена обработка событий
    bool hasEvents();

    // включить статусы Setup Exit
    void enableStates();

    // выключить статусы Setup Exit
    void disableStates();

    // включены статусы Setup Exit
    bool hasStates();

    // задача - таймер
    bool isTimer();

    // задача - тикер
    bool isTicker();

    // задача - обработчик событий
    bool isListener();

    // запущен и слушает события
    bool canListen();

    uint8_t _tickMask();

   private:
#if LOOPER_USE_ID
    hash_t _id;
#endif
    TaskCallback _cb;
    looper::Flags sreg;
};