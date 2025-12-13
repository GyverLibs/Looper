#pragma once
#include "../utils/SimpleTimer.h"
#include "./CallbackData.h"
#include "./LoopTask.h"

// задача-интервальный таймер
class LoopTimer : public LoopTask, public SimpleTimer {
   public:
    LoopTimer(uint32_t ms, TaskCallback callback, bool start = true, bool states = false) : LoopTimer((hash_t)0, ms, callback, start, states) {}
    LoopTimer(const char* id, uint32_t ms, TaskCallback callback, bool start = true, bool states = false) : LoopTimer(LPHr(id), ms, callback, start, states) {}
    LoopTimer(hash_t id, uint32_t ms, TaskCallback callback, bool start = true, bool states = false) : LoopTask(id, callback, TASK_IS_TIMER, states), SimpleTimer(ms, start) {}

    // опрос таймера
    void poll() {
        if (interval()) exec();
    }
};

// таймер для создания своих классов
class LoopTimerBase : public LoopTimer {
   public:
    LoopTimerBase(uint32_t ms, bool start = true, bool states = false) : LoopTimerBase((hash_t)0, ms, start, states) {}
    LoopTimerBase(const char* id, uint32_t ms, bool start = true, bool states = false) : LoopTimerBase(LPHr(id), ms, start, states) {}
    LoopTimerBase(hash_t id, uint32_t ms, bool start = true, bool states = false) : LoopTimer(id, ms, _exec, start, states) {}

    // выполняется при срабатывании таймера
    virtual void exec() = 0;

   private:
    using LoopTask::attach;
    using LoopTask::detach;
    using LoopTask::exec;

    static void _exec() {
        LP.thisTaskAs<LoopTimerBase>()->exec();
    }
};

// таймер с данными
template <typename T>
class LoopTimerData : public LoopTimer, public TaskCallbackData<T> {
    LP_MAKE_CALLBACK(DataCallback, void, T*);

   public:
    LoopTimerData(uint32_t ms, T* data, DataCallback callback, bool start = true, bool states = false) : LoopTimerData((hash_t)0, ms, data, callback, start, states) {}
    LoopTimerData(const char* id, uint32_t ms, T* data, DataCallback callback, bool start = true, bool states = false) : LoopTimerData(LPHr(id), ms, data, callback, start, states) {}
    LoopTimerData(hash_t id, uint32_t ms, T* data, DataCallback callback, bool start = true, bool states = false) : LoopTimer(id, ms, _exec, start, states), TaskCallbackData<T>(data, callback) {}

   private:
    using LoopTask::attach;
    using LoopTask::detach;

    static void _exec() {
        LP.thisTaskAs<LoopTimerData<T>>()->TaskCallbackData<T>::exec();
    }
};