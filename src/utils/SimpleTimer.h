#pragma once
#include <inttypes.h>
#include <stddef.h>

// базовый миллисекундный таймер
class SimpleTimer {
   public:
    SimpleTimer(uint32_t ms = 0, bool start = true);

    // перезапустить таймер
    void restart();

    // перезапустить таймер с новым периодом
    void restart(uint32_t ms, uint32_t sec = 0, uint16_t min = 0, uint16_t hour = 0, uint16_t day = 0);

    // остановить таймер
    void stop();

    // форсировать срабатывание таймера
    void force();

    // таймер запущен
    bool running();

    // время таймера вышло
    bool ready();

    // время таймера вышло
    explicit operator bool() {
        return ready();
    }

    // получить период таймера
    uint32_t getPeriod();

    // время вышло с перезапуском
    bool interval();

    // время вышло с остановкой
    bool timeout();

    // осталось времени таймера. 0 также если таймер остановлен
    uint32_t left();

   private:
    uint32_t _tmr = 0, _prd = 0;
};