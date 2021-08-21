#pragma once

#include <avr/io.h>

namespace ng {
    class Button {
        uint8_t state;
    public:
        Button(uint8_t st = 0): state(st) {}

        
    };
}