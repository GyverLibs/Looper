#include "LooperClass.h"

#include <limits.h>

#include "./nodes/LoopThread.h"
#include "./nodes/LoopTimer.h"
#include "./platform.h"

LooperClass LP;
LooperClass& Looper = LP;

void LooperClass::loop(bool main) {
    // !!! main loop: _thisTask & _thisState not saved
    _thisTask = _tasks.getLast();

    while (_thisTask) {
        if (main) {
            _removed = false;

            switch (_thisTask->_getMask(TASK_SETUP | TASK_EXIT)) {
                case TASK_SETUP:
                case TASK_SETUP | TASK_EXIT:
                    _thisTask->_markSettled();
                    _execState(tState::Setup);
                    if (_removed) return;
                    break;

                case TASK_EXIT:
                    _thisTask->_markRemoved();
                    _tasks.remove(_thisTask);
                    _execState(tState::Exit);
                    return;
            }
        }

        switch (_thisTask->_getMask(TASK_TYPE_MASK | TASK_DISABLED | TASK_PAUSED | TASK_SETUP | TASK_EXIT)) {
            case TASK_IS_TICKER:
            case TASK_IS_THREAD:
                _thisTask->exec();
                break;

            case TASK_IS_TIMER:
                thisTimer()->poll();
                break;
        }
        _thisTask = _removed ? nullptr : _thisTask->getPrev();
        looper::yield();
    }
#if LOOPER_USE_EVENTS
    while (main && _events.length()) _sendEvent(_events.pop());
#endif
}

void LooperClass::_execState(tState state) {
    if (!_thisTask->hasStates()) return;
    _thisState = state;
    _thisTask->exec();
    _thisState = tState::Loop;
}

void LooperClass::onEvent(LooperCallback callback) {
#if LOOPER_USE_EVENTS
    _event_cb = callback;
#endif
}

void LooperClass::reset() {
    LoopTask* t = _tasks.getLast();
    while (t) {
        t->reset();
        t = t->getPrev();
    }
}

uint32_t LooperClass::nextTimerLeft() {
    uint32_t tmin = UINT32_MAX;
    LoopTask* t = _tasks.getLast();
    while (t) {
        uint32_t left = 0;
        if (t->_getMask(TASK_SETUP | TASK_EXIT)) return 0;

        switch (t->_getMask(TASK_TYPE_MASK | TASK_DISABLED)) {
            case TASK_IS_TICKER:
                return 0;

            case TASK_IS_TIMER:
                if (!static_cast<LoopTimer*>(t)->running()) goto next;
                left = static_cast<LoopTimer*>(t)->left();
                break;

            case TASK_IS_THREAD:
                left = static_cast<LoopThread*>(t)->_tmr.left();
                break;

            default:
                goto next;
        }
        if (!left) return 0;
        if (tmin > left) tmin = left;

    next:
        t = t->getPrev();
    }
    return (tmin == UINT32_MAX) ? 0 : tmin;
}

uint16_t LooperClass::length() {
    return _tasks.length();
}

void LooperClass::delay(uint32_t ms) {
    // !!! save stack for loop
    uint32_t tmr = looper::millis();
    LoopTask* taskT = _thisTask;
    tState stateT = _thisState;

    _thisState = tState::Loop;
    if (taskT) taskT->_pause();

    while (looper::millis() - tmr < ms) {
        loop(false);
        looper::yield();
    }

    if (taskT) taskT->_resume();
    _thisState = stateT;
    _thisTask = taskT;
}

void LooperClass::add(LoopTask* task) {
    if (task && !task->isAdded()) {
        task->_markAdded();
        task->reset();
        _tasks.add(task);
    }
}

void LooperClass::remove(LoopTask* task, bool) {
    if (task) task->_markExit();
}

void LooperClass::_removeNow(LoopTask* task) {
    if (task && task->isAdded()) {
        task->_markRemoved();
        _tasks.remove(task);
        _removed = true;
    }
}

void LooperClass::removeThis(bool callExit) {
    remove(_thisTask, callExit);
}

LoopTask* LooperClass::thisTask() { return _thisTask; }
LoopTimer* LooperClass::thisTimer() { return thisTaskAs<LoopTimer>(); }
LoopThread* LooperClass::thisThread() { return thisTaskAs<LoopThread>(); }
tState LooperClass::thisState() { return _thisState; }
bool LooperClass::thisSetup() { return _thisState == tState::Setup; }
bool LooperClass::thisExit() { return _thisState == tState::Exit; }
bool LooperClass::thisEvent() { return _thisState == tState::Event; }
bool LooperClass::thisLoop() { return _thisState == tState::Loop; }

bool LooperClass::eventBroadcast() {
#if LOOPER_USE_EVENTS
    return _thisBroad;
#else
    return 0;
#endif
}

LoopTask* LooperClass::eventSource() {
#if LOOPER_USE_EVENTS
    return _thisSource;
#else
    return nullptr;
#endif
}
void* LooperClass::eventData() {
#if LOOPER_USE_EVENTS
    return _thisData;
#else
    return nullptr;
#endif
}

void LooperClass::_sendEvent(EventData& evt) {
    sendEvent(evt.id, evt.data);
}

void LooperClass::sendEvent(hash_t id, void* data) {
#if LOOPER_USE_EVENTS
    LoopTask* sourceT = _thisSource;
    LoopTask* taskT = _thisTask;
    tState stateT = _thisState;
    bool broadT = _thisBroad;
    void* dataT = _thisData;

    _thisState = tState::Event;
    _thisSource = _thisTask;
    _thisBroad = (id == 0);
    _thisData = data;

    if (_event_cb) _event_cb(id);

#if LOOPER_USE_ID
    _thisTask = _tasks.getLast();
    while (_thisTask) {
        if (_thisTask->id() && _thisTask->canListen() && (_thisBroad || _thisTask->id() == id)) {
            _thisTask->exec();
            looper::yield();
        }
        _thisTask = _removed ? nullptr : _thisTask->getPrev();
    }
#endif

    _thisSource = sourceT;
    _thisBroad = broadT;
    _thisState = stateT;
    _thisData = dataT;
    _thisTask = taskT;
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
#if LOOPER_USE_ID
    if (!id) return nullptr;
    LoopTask* t = _tasks.getLast();
    while (t) {
        if (t->id() == id) return t;
        t = t->getPrev();
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

LoopThread* LooperClass::getThread(hash_t id) {
    return static_cast<LoopThread*>(getTask(id));
}

LoopThread* LooperClass::getThread(const char* id) {
    return getThread(LPHr(id));
}