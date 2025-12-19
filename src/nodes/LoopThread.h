#pragma once
#include "../utils/SimpleTimer.h"
#include "./CallbackData.h"
#include "./LoopTask.h"

// задача-поток, вызывается постоянно
class LoopThread : public LoopTask {
   public:
    LoopThread(TaskCallback callback) : LoopThread((hash_t)0, callback) {}
    LoopThread(const char* id, TaskCallback callback) : LoopThread(LPHr(id), callback) {}
    LoopThread(hash_t id, TaskCallback callback) : LoopTask(id, callback, TASK_TYPE_THREAD, false, true) {}

    // перезапустить поток
    void restart() {
        _case = 0;
    }

    uint16_t _case = 0;
    SimpleTimer _tmr;

   private:
};

// поток с данными
template <typename T>
class LoopThreadData : public LoopThread, public TaskCallbackData<T> {
    LP_MAKE_CALLBACK(DataCallback, void, T*);

   public:
    LoopThreadData(T* data, DataCallback callback) : LoopThreadData((hash_t)0, data, callback) {}
    LoopThreadData(const char* id, T* data, DataCallback callback) : LoopThreadData(LPHr(id), data, callback) {}
    LoopThreadData(hash_t id, T* data, DataCallback callback) : LoopThread(id, _exec), TaskCallbackData<T>(data, callback) {}

   private:
    using LoopTask::attach;
    using LoopTask::detach;

    static void _exec() {
        LP.thisTaskAs<LoopThreadData<T>>()->TaskCallbackData<T>::exec();
    }
};
