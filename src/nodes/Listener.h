#pragma once
#include "CallbackData.h"
#include "LoopTask.h"

// задача-обработчик событий, вызывается при событии
class LoopListener : public LoopTask {
   public:
    LoopListener(const char* id, TaskCallback callback, bool states = false) : LoopListener(LPHr(id), callback, states) {}
    LoopListener(hash_t id, TaskCallback callback, bool states = false) : LoopTask(id, callback, TASK_IS_LISTENER, states, true) {}
};

// обработчик событий для создания своих классов
class LoopListenerBase : public LoopListener {
   public:
    LoopListenerBase(const char* id) : LoopListenerBase(LPHr(id)) {}
    LoopListenerBase(hash_t id) : LoopListener(id, _exec) {}

    // выполняется при получении события
    virtual void exec() = 0;

   private:
    using LoopTask::attach;
    using LoopTask::detach;
    using LoopTask::exec;

    static void _exec() {
        Looper.thisTaskAs<LoopListenerBase>()->exec();
    }
};

// обработчик событий с данными
template <typename T>
class LoopListenerData : public LoopListener, public TaskCallbackData<T> {
    LP_MAKE_CALLBACK(DataCallback, void, T*);

   public:
    LoopListenerData(const char* id, T* data, DataCallback callback) : LoopListenerData(LPHr(id), data, callback) {}
    LoopListenerData(hash_t id, T* data, DataCallback callback) : LoopListener(id, _exec), TaskCallbackData<T>(data, callback) {}

   private:
    using LoopTask::attach;
    using LoopTask::detach;

    static void _exec() {
        Looper.thisTaskAs<LoopListenerData<T>>()->TaskCallbackData<T>::exec();
    }
};