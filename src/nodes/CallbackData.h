#pragma once
#include "../utils/macro.h"

template <typename T>
class TaskCallbackData {
    LP_MAKE_CALLBACK(DataCallback, void, T*);

   public:
    TaskCallbackData(T* data, DataCallback callback) : _data(data), _cb(callback) {}
    
    // подключить новые данные
    void setData(T* data) {
        _data = data;
    }

    // получить данные
    T* getData() {
        return _data;
    }

    // вызвать обработчик
    void exec() {
        if (_cb) _cb(_data);
    }

   private:
    T* _data;
    DataCallback _cb;
};