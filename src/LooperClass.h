#pragma once
#include <inttypes.h>

#include "nodes/LoopTask.h"
#include "platform.h"
#include "utils/hash.h"
#include "utils/list.h"
#include "utils/macro.h"
#include "utils/stack.h"

class LoopTimer;
class LoopThread;

// ========= LooperClass =========
class LooperClass {
    LP_MAKE_CALLBACK(LooperCallback, void, hash_t);

   public:
    // ========= SYSTEM =========
    // вызывать в loop
    void loop();

    // вызвать сигнал setup у всех задач
    void restart();

    // количество задач в стеке
    uint16_t length();

    // время до срабатывания следующего таймера
    uint32_t nextTimerLeft();

    // аналог delay, внутри него вызывается loop
    void delay(uint32_t ms);

    // добавить задачу
    void add(LoopTask* task);

    // убрать задачу и вызвать обработчик выхода (опционально)
    void remove(LoopTask* task, bool callExit = true);

    // получить указатель на задачу по id
    LoopTask* getTask(hash_t id);
    LoopTask* getTask(const char* id);

    LoopTask* operator[](hash_t id);
    LoopTask* operator[](const char* id);

    // получить указатель на задачу по id
    LoopTimer* getTimer(hash_t id);
    LoopTimer* getTimer(const char* id);

    // ========= THIS =========

    // указатель на текущую задачу
    LoopTask* thisTask();

    // указатель на текущую задачу-таймер
    LoopTimer* thisTimer();

    // указатель на текущую задачу-поток
    LoopThread* thisThread();

    // указатель на текущую задачу, кастуется в указанный тип
    template <typename T>
    T* thisTaskAs() {
        return static_cast<T*>(thisTask());
    }

    // убрать текущую задачу из loop и вызвать обработчик выхода (опционально)
    void removeThis(bool callExit = true);

    // статус текущей задачи
    tState thisState();

    // статус текущей задачи - setup
    bool thisSetup();

    // статус текущей задачи - exit
    bool thisExit();

    // статус текущей задачи - loop
    bool thisLoop();

    // статус текущей задачи - событие
    bool thisEvent();

    // ========= EVENT =========

    // данные события текущей задачи
    void* eventData();

    // задача, которая отправила событие
    LoopTask* eventSource();

    // событие отправлено всем
    bool eventBroadcast();

    // отправить событие сейчас
    void sendEvent(hash_t id, void* data = nullptr);
    void sendEvent(const char* id, void* data = nullptr);

    // отложить событие (отправить позже из loop)
    bool pushEvent(hash_t id, void* data = nullptr);
    bool pushEvent(const char* id, void* data = nullptr);

    // подключить перехватчик событий вида void f(hash_t id, void* data)
    void onEvent(LooperCallback callback);

    // ============ PRIVATE ============
   private:
    struct EventData {
        hash_t id;
        void* data;
    };

    LoopTask* _thisTask = nullptr;
    looper::List<LoopTask> _tasks;

#if LOOPER_USE_EVENTS
    LoopTask* _source = nullptr;
    LooperCallback _event_cb = nullptr;
    void* _data = nullptr;
    looper::List<LoopTask> _lisns;
    looper::Stack<EventData, LOOPER_QUEUE_SIZE> _events;
    
    looper::List<LoopTask>* _getList(uint8_t idx);
#endif

    tState _thisState = tState::None;
    bool _setup = true;
    bool _removed = false;
    bool _broadcast = false;

    void _sendEvent(EventData& evt);
    void _tickState(LoopTask* task, tState state);
};

extern LooperClass Looper;