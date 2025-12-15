#include "LoopTask.h"

#include "../LooperClass.h"

LoopTask::LoopTask(hash_t id, TaskCallback callback, uint8_t type, bool states) : _cb(callback) {
#if LOOPER_USE_ID
    _id = id;
#endif
    _reg.writeBits(TASK_MASK_TYPE, type);
    if (states) _reg.set(TASK_HAS_STATES);
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
    switch (_reg.read(TASK_MASK_TYPE | TASK_MASK_STATE | TASK_DISABLED | TASK_SKIPPED)) {
        case TASK_TYPE_LISTENER | TASK_STATE_LOOP:
        case TASK_TYPE_TICKER | TASK_STATE_LOOP:
        case TASK_TYPE_THREAD | TASK_STATE_LOOP:
            return true;
    }
    return false;
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