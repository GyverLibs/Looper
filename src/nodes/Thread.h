#pragma once
#include "Ticker.h"

// задача-поток, вызывается постоянно
class LoopThread : public LoopTicker {
   public:
    using LoopTicker::LoopTicker;

    void _resetTmr() {
        if (!_tmr) {
            _tmr = looper::millis();
            if (!_tmr) _tmr--;
        }
    }
    void _stopTmr() {
        _tmr = 0;
    }
    bool _elapsed(uint32_t ms) {
        return looper::millis() - _tmr >= ms;
    }

    uint16_t _case = 0;

   private:
    uint32_t _tmr = 0;
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
        Looper.thisTaskAs<LoopThreadData<T>>()->TaskCallbackData<T>::exec();
    }
};
