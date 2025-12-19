#include "LoopTask.h"

#include "../LooperClass.h"

LoopTask::LoopTask(hash_t id, TaskCallback callback, uint8_t type, bool states, bool events) : _cb(callback) {
#if LOOPER_USE_ID
    _id = id;
#endif
    _reg.flags = (type & TASK_MASK_TYPE) |
                 (states ? TASK_HAS_STATES : 0) |
                 (events ? TASK_HAS_EVENTS : 0);
    addLoop();
}
LoopTask::~LoopTask() {
    LP._removeNow(this);
}

void LoopTask::addLoop() {
    LP.add(this);
}
void LoopTask::removeLoop() {
    LP.remove(this);
}

bool LoopTask::inLoop() {
    return _getState() != TASK_STATE_NONE;
}

void LoopTask::reset() {
    if (hasStates() && _getState() == TASK_STATE_LOOP) _setState(TASK_STATE_SETUP);
}

void LoopTask::exec() {
    if (_cb) _cb();
}
void LoopTask::attach(TaskCallback callback) {
    _cb = callback;
}
void LoopTask::detach() {
    _cb = nullptr;
}

void LoopTask::enableEvents() {
    _reg.set(TASK_HAS_EVENTS);
}

void LoopTask::disableEvents() {
    _reg.clear(TASK_HAS_EVENTS);
}

void LoopTask::enableStates() {
    _reg.set(TASK_HAS_STATES);
}

void LoopTask::disableStates() {
    _reg.clear(TASK_HAS_STATES);
}

hash_t LoopTask::id() {
#if LOOPER_USE_ID
    return _id;
#else
    return 0;
#endif
}

void LoopTask::enable() {
    _reg.clear(TASK_DISABLED);
}
void LoopTask::disable() {
    _reg.set(TASK_DISABLED);
}
void LoopTask::toggle() {
    isEnabled() ? disable() : enable();
}
bool LoopTask::isEnabled() {
    return !_reg.read(TASK_DISABLED);
}

bool LoopTask::isTimer() {
    return getType() == TASK_TYPE_TIMER;
}
bool LoopTask::isTicker() {
    return getType() == TASK_TYPE_TICKER;
}
bool LoopTask::isListener() {
    return getType() == TASK_TYPE_LISTENER;
}
bool LoopTask::isThread() {
    return getType() == TASK_TYPE_THREAD;
}

uint8_t LoopTask::getType() {
    return _reg.read(TASK_MASK_TYPE);
}

bool LoopTask::hasStates() {
    return _reg.read(TASK_HAS_STATES);
}
bool LoopTask::canListen() {
    return _reg.compare(TASK_MASK_STATE | TASK_HAS_EVENTS | TASK_SKIPPED | TASK_DISABLED,
                        TASK_STATE_LOOP | TASK_HAS_EVENTS);
}

void LoopTask::_setState(uint8_t status) {
    _reg.writeBits(TASK_MASK_STATE, status);
}
uint8_t LoopTask::_getState() {
    return _reg.read(TASK_MASK_STATE);
}
uint8_t LoopTask::_getMask(uint8_t mask) {
    return _reg.read(mask);
}

void LoopTask::_skip() {
    _reg.set(TASK_SKIPPED);
}
void LoopTask::_unskip() {
    _reg.clear(TASK_SKIPPED);
}