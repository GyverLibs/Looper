#include "SimpleTimer.h"

#include "../platform.h"

void SimpleTimer::restart() {
    _tmr = looper::millis();
    if (!_tmr) _tmr--;
}

void SimpleTimer::restart(uint32_t ms, uint32_t sec, uint16_t min, uint16_t hour, uint16_t day) {
    _prd = ms;
    if (sec) _prd += sec * 1000ul;
    if (min) _prd += min * 60 * 1000ul;
    if (hour) _prd += hour * 60 * 60 * 1000ul;
    if (day) _prd += day * 24 * 60 * 60 * 1000ul;
    restart();
}

void SimpleTimer::stop() {
    _tmr = 0;
}

bool SimpleTimer::running() {
    return _tmr;
}

void SimpleTimer::force() {
    _tmr = looper::millis() - _prd;
    if (!_tmr) _tmr--;
}

bool SimpleTimer::ready() {
    return _tmr && (looper::millis() - _tmr >= _prd);
}

uint32_t SimpleTimer::getPeriod() {
    return _prd;
}

uint32_t SimpleTimer::left() {
    if (!_tmr) return 0;
    uint32_t dif = looper::millis() - _tmr;
    return dif >= _prd ? 0 : _prd - dif;
    // return _tmr ? ((looper::millis() - _tmr >= _prd) ? 0 : (_prd - (looper::millis() - _tmr))) : 0;
}

bool SimpleTimer::interval() {
    if (ready()) {
        restart();
        return true;
    }
    return false;
}

bool SimpleTimer::timeout() {
    if (ready()) {
        stop();
        return true;
    }
    return false;
}