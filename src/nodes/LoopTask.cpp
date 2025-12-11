#include "LoopTask.h"

#include "LooperClass.h"

LoopTask::LoopTask(hash_t id, TaskCallback callback, uint8_t type, bool states, bool events) : _cb(callback) {
#if LOOPER_USE_ID
    _id = id;
#endif
    sreg.set(TASK_ENABLED | type);
    if (states) enableStates();
    if (events) enableEvents();
    addLoop();
}

void LoopTask::addLoop() {
    Looper.add(this);
    restart();
}
void LoopTask::removeLoop() {
    Looper.remove(this);
}

void LoopTask::restart() {
    sreg.set(TASK_SETUP);
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
    sreg.set(TASK_ENABLED);
}
void LoopTask::disable() {
    sreg.clear(TASK_ENABLED);
}
bool LoopTask::isEnabled() {
    return sreg.read(TASK_ENABLED);
}
void LoopTask::toggle() {
    sreg.write(TASK_ENABLED, !sreg.read(TASK_ENABLED));
}

void LoopTask::enableEvents() {
    sreg.set(TASK_HAS_EVENTS);
}
void LoopTask::disableEvents() {
    sreg.clear(TASK_HAS_EVENTS);
}
bool LoopTask::hasEvents() {
    return sreg.read(TASK_HAS_EVENTS);
}

void LoopTask::enableStates() {
    sreg.set(TASK_HAS_STATES);
}
void LoopTask::disableStates() {
    sreg.clear(TASK_HAS_STATES);
}
bool LoopTask::hasStates() {
    return sreg.read(TASK_HAS_STATES);
}

bool LoopTask::isTimer() {
    return sreg.read(TASK_IS_TIMER);
}
bool LoopTask::isTicker() {
    return sreg.read(TASK_IS_TICKER);
}
bool LoopTask::isListener() {
    return sreg.read(TASK_IS_LISTENER);
}
bool LoopTask::isThread() {
    return sreg.read(TASK_IS_THREAD);
}

bool LoopTask::canListen() {
    return sreg.isSet(TASK_ENABLED | TASK_HAS_EVENTS);
}

uint8_t LoopTask::_tickMask() {
    return sreg.mask(TASK_ENABLED | TASK_IS_TICKER | TASK_IS_TIMER | TASK_IS_THREAD | TASK_SETUP);
}
void LoopTask::_settle() {
    sreg.clear(TASK_SETUP);
}