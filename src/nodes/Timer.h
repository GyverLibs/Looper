#pragma once
#include "../utils/SimpleTimer.h"
#include "CallbackData.h"
#include "LoopTask.h"

// задача-интервальный таймер
class LoopTimer : public LoopTask, public SimpleTimer {
   public:
    LoopTimer(uint32_t ms, TaskCallback callback, bool start = true, bool states = false, bool events = false) : LoopTimer((hash_t)0, ms, callback, start, states, events) {}
    LoopTimer(const char* id, uint32_t ms, TaskCallback callback, bool start = true, bool states = false, bool events = false) : LoopTimer(LPHr(id), ms, callback, start, states, events) {}
    LoopTimer(hash_t id, uint32_t ms, TaskCallback callback, bool start = true, bool states = false, bool events = false) : LoopTask(id, callback, TASK_IS_TIMER, states, events), SimpleTimer(ms, start) {}

    // опрос таймера
    void poll() {
        if (interval()) exec();
    }
};

// таймер для создания своих классов
class LoopTimerBase : public LoopTimer {
   public:
    LoopTimerBase(uint32_t ms, bool start = true) : LoopTimerBase((hash_t)0, ms, start) {}
    LoopTimerBase(const char* id, uint32_t ms, bool start = true) : LoopTimerBase(LPHr(id), ms, start) {}
    LoopTimerBase(hash_t id, uint32_t ms, bool start = true) : LoopTimer(id, ms, _exec, start) {}

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
    LoopTimerData(uint32_t ms, T* data, DataCallback callback, bool start = true) : LoopTimerData((hash_t)0, ms, data, callback, start) {}
    LoopTimerData(const char* id, uint32_t ms, T* data, DataCallback callback, bool start = true) : LoopTimerData(LPHr(id), ms, data, callback, start) {}
    LoopTimerData(hash_t id, uint32_t ms, T* data, DataCallback callback, bool start = true) : LoopTimer(id, ms, _exec, start), TaskCallbackData<T>(data, callback) {}

   private:
    using LoopTask::attach;
    using LoopTask::detach;

    static void _exec() {
        Looper.thisTaskAs<LoopTimerData<T>>()->TaskCallbackData<T>::exec();
    }
};