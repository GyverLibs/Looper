#include "LooperClass.h"

#include <limits.h>

#include "nodes/Thread.h"
#include "nodes/Timer.h"
#include "platform.h"

#define LP_LIST_AMOUNT 2

LooperClass Looper;

void LooperClass::loop() {
    if (_thisState != tState::None) return;
    _thisState = _setup ? (_setup = false, tState::Setup) : tState::Loop;
    _thisTask = _tasks.getLast();
    while (_thisTask) {
        looper::yield();
        switch (_thisTask->_tickMask()) {
            case (TASK_ENABLED_TICKER):
                _thisTask->exec();
                break;
            case (TASK_ENABLED_TIMER):
                static_cast<LoopTimer*>(_thisTask)->poll();
                break;
        }
        if (_removed) _removed = false;
        else _thisTask = _thisTask->getPrev();
#if LOOPER_USE_EVENTS
        while (_events.length()) _sendEvent(_events.pop());
#endif
    }
#if LOOPER_USE_EVENTS
    while (_events.length()) _sendEvent(_events.pop());
#endif
    _thisState = tState::None;
}

void LooperClass::onEvent(LooperCallback callback) {
#if LOOPER_USE_EVENTS
    _event_cb = callback;
#endif
}

void LooperClass::restart() {
    _setup = true;
}

uint32_t LooperClass::nextTimerLeft() {
    uint32_t next = UINT32_MAX;
    LoopTask* p = _tasks.getLast();
    while (p) {
        if (p->_tickMask() == TASK_ENABLED_TIMER) {
            uint32_t left = static_cast<LoopTimer*>(p)->left();
            if (left < next) next = left;
        }
        p = p->getPrev();
    }
    return next == UINT32_MAX ? 0 : next;
}

uint16_t LooperClass::length() {
#if LOOPER_USE_EVENTS
    return _tasks.length() + _lisns.length();
#else
    return _tasks.length();
#endif
}

void LooperClass::delay(uint32_t ms) {
    if (_thisState != tState::None) return;
    uint32_t tmr = looper::millis();
    while (looper::millis() - tmr < ms) loop();
}

void LooperClass::add(LoopTask* task) {
    if (!task) return;

#if LOOPER_USE_EVENTS
    task->isListener() ? _lisns.add(task) : _tasks.add(task);
#else
    if (!task->isListener()) _tasks.add(task);
#endif

    if (!_setup) _tickState(task, tState::Setup);
}

void LooperClass::remove(LoopTask* task, bool callExit) {
    if (!task) return;

    if (_thisTask == task) {
        if (!_removed) {
            _removed = true;
            if (callExit) _tickState(task, tState::Exit);
        }
    } else {
        if (callExit) _tickState(task, tState::Exit);
    }

#if LOOPER_USE_EVENTS
    task->isListener() ? _lisns.remove(task) : _tasks.remove(task);
#else
    if (!task->isListener()) _tasks.remove(task);
#endif

    if (_thisTask == task) _thisTask = _thisTask->getPrev();
}

void LooperClass::_tickState(LoopTask* task, tState state) {
    if (task->hasStates()) {
        tState stateTemp = _thisState;
        LoopTask* taskTemp = _thisTask;
        _thisState = state;
        _thisTask = task;
        task->exec();
        _thisState = stateTemp;
        _thisTask = taskTemp;
    }
}

void LooperClass::removeThis(bool callExit) {
    remove(thisTask(), callExit);
}

LoopTask* LooperClass::thisTask() { return _removed ? nullptr : _thisTask; }
LoopTimer* LooperClass::thisTimer() { return thisTaskAs<LoopTimer>(); }
LoopThread* LooperClass::thisThread() { return thisTaskAs<LoopThread>(); }
tState LooperClass::thisState() { return _thisState; }
bool LooperClass::thisSetup() { return _thisState == tState::Setup; }
bool LooperClass::thisExit() { return _thisState == tState::Exit; }
bool LooperClass::thisEvent() { return _thisState == tState::Event; }
bool LooperClass::thisLoop() { return _thisState == tState::Loop; }

bool LooperClass::eventBroadcast() {
#if LOOPER_USE_EVENTS
    return _broadcast;
#else
    return 0;
#endif
}

LoopTask* LooperClass::eventSource() {
#if LOOPER_USE_EVENTS
    return _source;
#else
    return nullptr;
#endif
}
void* LooperClass::eventData() {
#if LOOPER_USE_EVENTS
    return _thisState == tState::Event ? _data : nullptr;
#else
    return nullptr;
#endif
}

void LooperClass::_sendEvent(EventData& evt) {
    sendEvent(evt.id, evt.data);
}

void LooperClass::sendEvent(hash_t id, void* data) {
#if LOOPER_USE_EVENTS
    LoopTask* source = _thisTask;
    tState stateTemp = _thisState;
    _thisState = tState::Event;

    if (_event_cb) {
        _broadcast = (id == 0);
        _data = data;
        _source = _thisTask;
        _event_cb(id);
    }

#if LOOPER_USE_ID
    for (uint8_t i = 0; i < LP_LIST_AMOUNT; i++) {
        _thisTask = _getList(i)->getLast();
        while (_thisTask) {
            if (_thisTask->id() && _thisTask->canListen() && (!id || _thisTask->id() == id)) {
                _data = data;
                _broadcast = (id == 0);
                _source = source;
                _thisTask->exec();
                looper::yield();
            }
            if (_removed) _removed = false;
            else _thisTask = _thisTask->getPrev();
        }
    }
#endif

    _broadcast = false;
    _source = nullptr;
    _thisTask = source;
    _thisState = stateTemp;
#endif
}

void LooperClass::sendEvent(const char* id, void* data) {
    sendEvent(LPHr(id), data);
}

bool LooperClass::pushEvent(hash_t id, void* data) {
#if LOOPER_USE_EVENTS
    return _events.push(EventData{id, data});
#else
    return 0;
#endif
}

bool LooperClass::pushEvent(const char* id, void* data) {
    return pushEvent(LPHr(id), data);
}

LoopTask* LooperClass::getTask(hash_t id) {
#if LOOPER_USE_EVENTS
    if (!id) return nullptr;
    for (uint8_t i = 0; i < LP_LIST_AMOUNT; i++) {
        LoopTask* p = _getList(i)->getLast();
        while (p) {
            if (p->id() == id) return p;
            p = p->getPrev();
        }
    }
#endif
    return nullptr;
}

LoopTask* LooperClass::getTask(const char* id) {
    return getTask(LPHr(id));
}

LoopTask* LooperClass::operator[](hash_t id) {
    return getTask(id);
}
LoopTask* LooperClass::operator[](const char* id) {
    return getTask(id);
}

LoopTimer* LooperClass::getTimer(hash_t id) {
    return static_cast<LoopTimer*>(getTask(id));
}

LoopTimer* LooperClass::getTimer(const char* id) {
    return getTimer(LPHr(id));
}

#if LOOPER_USE_EVENTS
looper::List<LoopTask>* LooperClass::_getList(uint8_t idx) {
    return (looper::List<LoopTask>*[]){&_tasks, &_lisns}[idx];
}
#endif