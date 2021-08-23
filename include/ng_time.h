#pragma once

#include <avr/io.h>

namespace ng {
    /**
     * @brief Returns the number of milliseconds passed since the microcontroller
     *        began running the current program.
     *
     * @return uint32_t Number of milliseconds passed since the program started
     */
    uint32_t millis()
    {
        uint32_t m;
        asm volatile ("rcall _millis" : "=w" (m) :: "r30");
        return m;
    }
}