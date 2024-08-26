#pragma once
#include "CallbackData.h"
#include "LoopTask.h"
#include "utils/SimpleTimer.h"

// задача-интервальный таймер
class LoopTimer : public LoopTask, public SimpleTimer {
   public:
    LoopTimer(uint32_t ms, TaskCallback callback, bool states = false, bool events = false) : LoopTimer((hash_t)0, ms, callback, states, events) {}
    LoopTimer(const char* id, uint32_t ms, TaskCallback callback, bool states = false, bool events = false) : LoopTimer(LPHr(id), ms, callback, states, events) {}
    LoopTimer(hash_t id, uint32_t ms, TaskCallback callback, bool states = false, bool events = false) : LoopTask(id, callback, TASK_IS_TIMER, states, events), SimpleTimer(ms) {}

    // опрос таймера
    void poll() {
        if (interval()) exec();
    }
};

// таймер для создания своих классов
class LoopTimerBase : public LoopTimer {
   public:
    LoopTimerBase(uint32_t ms) : LoopTimerBase((hash_t)0, ms) {}
    LoopTimerBase(const char* id, uint32_t ms) : LoopTimerBase(LPHr(id), ms) {}
    LoopTimerBase(hash_t id, uint32_t ms) : LoopTimer(id, ms, _exec) {}

    // выполняется при срабатывании таймера
    virtual void exec() = 0;

   private:
    using LoopTask::attach;
    using LoopTask::detach;
    using LoopTask::exec;

    static void _exec() {
        Looper.thisTaskAs<LoopTimerBase>()->exec();
    }
};

// таймер с данными
template <typename T>
class LoopTimerData : public LoopTimer, public TaskCallbackData<T> {
    LP_MAKE_CALLBACK(DataCallback, void, T*);

   public:
    LoopTimerData(uint32_t ms, T* data, DataCallback callback) : LoopTimerData((hash_t)0, ms, data, callback) {}
    LoopTimerData(const char* id, uint32_t ms, T* data, DataCallback callback) : LoopTimerData(LPHr(id), ms, data, callback) {}
    LoopTimerData(hash_t id, uint32_t ms, T* data, DataCallback callback) : LoopTimer(id, ms, _exec), TaskCallbackData<T>(data, callback) {}

   private:
    using LoopTask::attach;
    using LoopTask::detach;

    static void _exec() {
        Looper.thisTaskAs<LoopTimerData<T>>()->TaskCallbackData<T>::exec();
    }
};