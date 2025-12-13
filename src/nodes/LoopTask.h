#pragma once
#include <inttypes.h>
#include <stddef.h>

#include "../platform.h"
#include "../utils/flags.h"
#include "../utils/hash.h"
#include "../utils/list.h"
#include "../utils/macro.h"

#define TASK_IS_LISTENER 0
#define TASK_IS_TICKER 1
#define TASK_IS_THREAD 2
#define TASK_IS_TIMER 3

#define TASK_TYPE_MASK (TASK_IS_LISTENER | TASK_IS_TICKER | TASK_IS_THREAD | TASK_IS_TIMER)
#define TASK_DISABLED (1 << 2)
#define TASK_PAUSED (1 << 3)
#define TASK_STATES (1 << 4)
#define TASK_ADDED (1 << 5)
#define TASK_SETUP (1 << 6)
#define TASK_EXIT (1 << 7)

enum class tState : uint8_t {
    Loop,
    Setup,
    Exit,
    Event,
};

LP_MAKE_CALLBACK(TaskCallback, void);

class LoopTask : public looper::List<LoopTask>::Node {
   public:
    LoopTask(hash_t id, TaskCallback callback, uint8_t type, bool states);
    ~LoopTask();

    // добавить в loop
    void addLoop();

    // убрать из loop
    void removeLoop();

    // спровоцировать вызов со статусом Setup
    void reset();

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

    // переключить задачу
    void toggle();

    // задача запущена
    bool isEnabled();

    // задача - таймер
    bool isTimer();

    // задача - тикер
    bool isTicker();

    // задача - обработчик событий
    bool isListener();

    // задача - поток
    bool isThread();

    // тип задачи
    uint8_t getType();

    // добавлена в Looper
    bool isAdded();

    // имеет статусы Setup и Exit
    bool hasStates();

    // может принять Event
    bool canListen();

    uint8_t _getMask(uint8_t mask);
    void _markSettled();
    void _markExit();
    void _pause();
    void _resume();
    void _markAdded();
    void _markRemoved();

   private:
#if LOOPER_USE_ID
    hash_t _id;
#endif
    TaskCallback _cb;
    looper::Flags _f;
};