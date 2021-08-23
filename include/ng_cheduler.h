#pragma once

#include <avr/io.h>
#include "ng_time.h"

#define EVERY_MS(x) \
  static uint32_t tmr;\
  bool flag = ng::millis() - tmr >= (x);\
  if (flag) tmr = ng::millis();\
  if (flag)

namespace ng
{
    struct Task {
        void (*fn)(void) = 0;
        uint8_t period = 0; // period in milliseconds
        uint32_t tmr = 0;

        Task() {}

        Task(void (*fn)(), uint32_t period) {
            this->fn = fn;
            this->period = period;
            tmr = 0;
        }

        bool isReady() {
            return (millis() - tmr > period);
        }

        void excecute() {
            tmr = millis();
            fn();
        }
    };

    template<uint8_t SIZE>
    class Cheduler {
        int8_t cnt = 0;
        Task tasks[SIZE];

        uint32_t tmr = 0;
    public:
        Cheduler() {
            cnt = 0;
        }

        void tick() {
            for(int i = 0; i < cnt; i++) {
                if (tasks[i].isReady()) {
                    tasks[i].excecute();
                }
            }
        }

        uint8_t add(void (*fn)(), uint32_t period) {
            tasks[cnt] = Task(fn, period);
            return cnt++;
        }
    };
}