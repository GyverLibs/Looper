#include "LoopTask.h"

#include "../LooperClass.h"

LoopTask::LoopTask(hash_t id, TaskCallback callback, uint8_t type, bool states) : _cb(callback) {
#if LOOPER_USE_ID
    _id = id;
#endif
    _f.set(type & TASK_TYPE_MASK);
    if (states) _f.set(TASK_STATES);
    LP.add(this);
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

void LoopTask::reset() {
    if (hasStates()) _f.set(TASK_SETUP);
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
    _f.clear(TASK_DISABLED);
}
void LoopTask::disable() {
    _f.set(TASK_DISABLED);
}
void LoopTask::toggle() {
    isEnabled() ? disable() : enable();
}
bool LoopTask::isEnabled() {
    return !_f.read(TASK_DISABLED);
}

bool LoopTask::isTimer() {
    return getType() == TASK_IS_TIMER;
}
bool LoopTask::isTicker() {
    return getType() == TASK_IS_TICKER;
}
bool LoopTask::isListener() {
    return getType() == TASK_IS_LISTENER;
}
bool LoopTask::isThread() {
    return getType() == TASK_IS_THREAD;
}

uint8_t LoopTask::getType() {
    return _f.mask(TASK_TYPE_MASK);
}

bool LoopTask::isAdded() {
    return _f.read(TASK_ADDED);
}

bool LoopTask::hasStates() {
    return _f.read(TASK_STATES);
}
bool LoopTask::canListen() {
    return !isTimer() && !_f.mask(TASK_PAUSED | TASK_DISABLED | TASK_SETUP | TASK_EXIT);
}

uint8_t LoopTask::_getMask(uint8_t mask) {
    return _f.mask(mask);
}
void LoopTask::_markSettled() {
    _f.clear(TASK_SETUP);
}
void LoopTask::_markExit() {
    _f.set(TASK_EXIT);
}
void LoopTask::_pause() {
    _f.set(TASK_PAUSED);
}
void LoopTask::_resume() {
    _f.clear(TASK_PAUSED);
}
void LoopTask::_markAdded() {
    _f.set(TASK_ADDED);
}
void LoopTask::_markRemoved() {
    _f.clear(TASK_ADDED);
}