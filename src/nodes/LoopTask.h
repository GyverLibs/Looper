#pragma once
#include <inttypes.h>
#include <stddef.h>

#include "../platform.h"
#include "../utils/flags.h"
#include "../utils/hash.h"
#include "../utils/list.h"
#include "../utils/macro.h"

#define TASK_SHIFT_TYPE 0
#define TASK_TYPE_LISTENER (0 << TASK_SHIFT_TYPE)
#define TASK_TYPE_TICKER (1 << TASK_SHIFT_TYPE)
#define TASK_TYPE_THREAD (2 << TASK_SHIFT_TYPE)
#define TASK_TYPE_TIMER (3 << TASK_SHIFT_TYPE)
#define TASK_MASK_TYPE (TASK_TYPE_LISTENER | TASK_TYPE_TICKER | TASK_TYPE_THREAD | TASK_TYPE_TIMER)

#define TASK_SHIFT_STATE 2
#define TASK_STATE_NONE (0 << TASK_SHIFT_STATE)
#define TASK_STATE_SETUP (1 << TASK_SHIFT_STATE)
#define TASK_STATE_LOOP (2 << TASK_SHIFT_STATE)
#define TASK_STATE_EXIT (3 << TASK_SHIFT_STATE)
#define TASK_MASK_STATE (TASK_STATE_NONE | TASK_STATE_SETUP | TASK_STATE_LOOP | TASK_STATE_EXIT)

#define TASK_HAS_STATES (1 << 4)
#define TASK_DISABLED (1 << 5)
#define TASK_SKIPPED (1 << 6)

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

    // добавлена в loop
    bool inLoop();

    // вернуть на стадию Setup (только для добавленных задач)
    void reset();

    // вызвать обработчик
    void exec();

    // подключить обработчик
    void attach(TaskCallback callback);

    // отключить обработчик
    void detach();

    // id задачи
    hash_t id();

    // включить задачу (статусы Loop и Event)
    void enable();

    // выключить задачу (статусы Loop и Event)
    void disable();

    // переключить задачу
    void toggle();

    // задача запущена (статусы Loop и Event)
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

    // имеет статусы Setup и Exit
    bool hasStates();

    // может принять Event
    bool canListen();

    // ========== PRIVATE ==========
    void _setState(uint8_t status);
    uint8_t _getState();
    uint8_t _getMask(uint8_t mask);
    void _skip();
    void _unskip();

   private:
#if LOOPER_USE_ID
    hash_t _id;
#endif
    TaskCallback _cb;
    looper::Flags _reg;
};