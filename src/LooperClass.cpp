#include "LooperClass.h"

#include <limits.h>

#include "./nodes/LoopThread.h"
#include "./nodes/LoopTimer.h"
#include "./platform.h"

LooperClass LP;
LooperClass& Looper = LP;

void LooperClass::loop(bool main) {
    // !!! main loop: _thisTask & _thisState not saved
    _thisTask = _tasks.getFirst();

    while (_thisTask) {
        switch (_thisTask->_getMask(TASK_MASK_STATE | TASK_DISABLED | TASK_SKIPPED)) {
            case TASK_STATE_SETUP:
            case TASK_STATE_SETUP | TASK_DISABLED:
                if (main) {
                    _thisTask->_setState(TASK_STATE_LOOP);
                    _execState(tState::Setup);
                }
                break;

            case TASK_STATE_EXIT:
            case TASK_STATE_EXIT | TASK_DISABLED:
                if (main) {
                    _thisTask->_setState(TASK_STATE_NONE);
                    LoopTask* prev = _tasks.remove(_thisTask);
                    _execState(tState::Exit);
                    _thisTask = prev;
                }
                break;

            case TASK_STATE_LOOP:
                switch (_thisTask->_getMask(TASK_MASK_TYPE)) {
                    case TASK_TYPE_TICKER:
                    case TASK_TYPE_THREAD:
                        _thisTask->exec();
                        break;

                    case TASK_TYPE_TIMER:
                        thisTimer()->poll();
                        break;
                }
                break;
        }
        if (_thisTask) _thisTask = _thisTask->getNext();
        looper::yield();
    }
#if LOOPER_USE_EVENTS && LOOPER_QUEUE_SIZE
    while (_events.length() && main) _sendEvent(_events.pop());
#endif
}

void LooperClass::_execState(tState state) {
    if (_thisTask->hasStates()) {
        _thisState = state;
        _thisTask->exec();
        _thisState = tState::Loop;
    }
}

void LooperClass::onEvent(LooperCallback callback) {
#if LOOPER_USE_EVENTS
    _event_cb = callback;
#endif
}

void LooperClass::reset() {
    LoopTask* t = _tasks.getFirst();
    while (t) {
        t->reset();
        if (t->isThread()) static_cast<LoopThread*>(t)->restart();
        else if (t->isTimer()) static_cast<LoopTimer*>(t)->restart();
        t = t->getNext();
    }
}

uint32_t LooperClass::nextTimerLeft() {
    uint32_t tmin = UINT32_MAX;
    LoopTask* t = _tasks.getFirst();
    while (t) {
        if (t->_getState() != TASK_STATE_LOOP) return 0;

        uint32_t left = 0;
        switch (t->_getMask(TASK_MASK_TYPE | TASK_DISABLED)) {
            case TASK_TYPE_TICKER:
                return 0;

            case TASK_TYPE_TIMER:
                if (!static_cast<LoopTimer*>(t)->running()) goto next;
                left = static_cast<LoopTimer*>(t)->left();
                break;

            case TASK_TYPE_THREAD:
                left = static_cast<LoopThread*>(t)->_tmr.left();
                break;

            default:
                goto next;
        }
        if (!left) return 0;
        if (tmin > left) tmin = left;

    next:
        t = t->getNext();
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
    if (taskT) taskT->_skip();

    while (looper::millis() - tmr < ms) {
        loop(false);
        looper::yield();
    }

    if (taskT) taskT->_unskip();
    _thisState = stateT;
    _thisTask = taskT;
}

void LooperClass::add(LoopTask* task) {
    if (task && !task->inLoop()) {
        task->_setState(task->hasStates() ? TASK_STATE_SETUP : TASK_STATE_LOOP);
        _tasks.add(task);
    }
}

void LooperClass::remove(LoopTask* task) {
    if (task && task->inLoop()) task->_setState(TASK_STATE_EXIT);
}

void LooperClass::removeThis() {
    remove(_thisTask);
}

void LooperClass::removeAll() {
    LoopTask* t = _tasks.getFirst();
    while (t) {
        remove(t);
        t = t->getNext();
    }
}

void LooperClass::_removeNow(LoopTask* task) {
    if (task && task->inLoop()) {
        task->_setState(TASK_STATE_NONE);
        LoopTask* prev = _tasks.remove(task);
        if (_thisTask == task) _thisTask = prev;
    }
}

tState LooperClass::thisState() { return _thisState; }
LoopTask* LooperClass::thisTask() { return _thisTask; }
LoopTimer* LooperClass::thisTimer() { return thisTaskAs<LoopTimer>(); }
LoopThread* LooperClass::thisThread() { return thisTaskAs<LoopThread>(); }
bool LooperClass::thisEvent() { return _thisState == tState::Event; }
bool LooperClass::thisSetup() { return _thisState == tState::Setup; }
bool LooperClass::thisExit() { return _thisState == tState::Exit; }
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
    _thisTask = _tasks.getFirst();
    while (_thisTask) {
        if (_thisTask->id() &&
            _thisTask->id() != _thisSource->id() &&
            (_thisBroad || _thisTask->id() == id) &&
            _thisTask->canListen()) {
            _thisTask->exec();
            looper::yield();
        }
        _thisTask = _thisTask ? _thisTask->getNext() : _tasks.getFirst();
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
#if LOOPER_USE_EVENTS && LOOPER_QUEUE_SIZE
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
    LoopTask* t = _tasks.getFirst();
    while (t) {
        if (t->id() == id) return t;
        t = t->getNext();
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