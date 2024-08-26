#pragma once

#include "LooperClass.h"
#include "nodes/Listener.h"
#include "nodes/Ticker.h"
#include "nodes/Timer.h"

// ========= macro =========
#define LP_BROADCAST (hash_t)0
#define LP_SEND_EVENT(id, data) Looper.sendEvent(LPH(id), data)
#define LP_PUSH_EVENT(id, data) Looper.pushEvent(LPH(id), data)

#define LP_MAKE(type, ...) static type _LP_CONCAT(__loop_obj_, __COUNTER__)(__VA_ARGS__)
#define LP_MAKE_(id, type, ...) static type _LP_CONCAT(__loop_obj_, __COUNTER__)(LPH(id), __VA_ARGS__)
#define LP_NEW(type, ...) new type(__VA_ARGS__)
#define LP_NEW_(id, type, ...) new type(LPH(id), __VA_ARGS__)

#define LP_LISTENER_(id, func) LP_MAKE_(id, LoopListener, func)

#define LP_TICKER(func) LP_MAKE(LoopTicker, func)
#define LP_TICKER_(id, func) LP_MAKE_(id, LoopTicker, func)

#define LP_TIMER(ms, func) LP_MAKE(LoopTimer, ms, func)
#define LP_TIMER_(id, ms, func) LP_MAKE_(id, LoopTimer, ms, func)