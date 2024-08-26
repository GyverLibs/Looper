#pragma once
#include "CallbackData.h"
#include "LoopTask.h"

// задача-тикер, вызывается постоянно
class LoopTicker : public LoopTask {
   public:
    LoopTicker(TaskCallback callback, bool states = true, bool events = true) : LoopTicker((hash_t)0, callback, states, events) {}
    LoopTicker(const char* id, TaskCallback callback, bool states = true, bool events = true) : LoopTicker(LPHr(id), callback, states, events) {}
    LoopTicker(hash_t id, TaskCallback callback, bool states = true, bool events = true) : LoopTask(id, callback, TASK_IS_TICKER, states, events) {}
};

// тикер для создания своих классов
class LoopTickerBase : public LoopTicker {
   public:
    LoopTickerBase() : LoopTickerBase((hash_t)0) {}
    LoopTickerBase(const char* id) : LoopTickerBase(LPHr(id)) {}
    LoopTickerBase(hash_t id) : LoopTicker(id, _exec) {}

    // выполняется в loop и при событиях
    virtual void exec() = 0;

   private:
    using LoopTask::attach;
    using LoopTask::detach;
    using LoopTask::exec;

    static void _exec() {
        Looper.thisTaskAs<LoopTickerBase>()->exec();
    }
};

// тикер с данными
template <typename T>
class LoopTickerData : public LoopTicker, public TaskCallbackData<T> {
    LP_MAKE_CALLBACK(DataCallback, void, T*);

   public:
    LoopTickerData(T* data, DataCallback callback) : LoopTickerData((hash_t)0, data, callback) {}
    LoopTickerData(const char* id, T* data, DataCallback callback) : LoopTickerData(LPHr(id), data, callback) {}
    LoopTickerData(hash_t id, T* data, DataCallback callback) : LoopTicker(id, _exec), TaskCallbackData<T>(data, callback) {}

   private:
    using LoopTask::attach;
    using LoopTask::detach;

    static void _exec() {
        Looper.thisTaskAs<LoopTickerData<T>>()->TaskCallbackData<T>::exec();
    }
};
