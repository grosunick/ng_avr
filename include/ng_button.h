#pragma once

#include <avr/io.h>
#include "ng_io.h"
#include "ng_time.h"

namespace ng
{
    class Button
    {
       #pragma pack(push,1)
       struct opts {
            uint8_t state: 1; //  previous state of button
            uint8_t pullUpState: 1; // pull up type of button (1 - button pull upped to high, 0 - button pull upped to low)
            uint8_t isPressDebMode: 1; // button is in debounce handling mode while pressing
            uint8_t isUnpressDebMode: 1; // button is in debounce handling mode while unpressing
            uint8_t isChanged: 1; // button state change sign

            uint32_t tmr: 32;
       } opt;
       #pragma pack(pop)

        /**
         * pin wich is connected to button
         */
        uint8_t pin;

        /**
         * debounce tiomeout
         */
        uint8_t debounce = 30;
    public:
        Button(uint8_t _pin, uint8_t pullUpState = HIGH): pin(_pin) {
            pinMode(pin, INPUT_PULLUP);

            opt.state = digitalRead(pin);
            opt.pullUpState = HIGH;
            opt.isPressDebMode = 0;
        }

        uint8_t isUp() {
            return opt.state == opt.pullUpState;
        }

        uint8_t isDown() {
            return opt.state != opt.pullUpState;
        }

        // uint8_t isClicked() {

        // }

        void tick() {
            opt.isChanged = ng::digitalRead(pin) != opt.state;

            pressHandler();
            unpressHandler();
        }

        void pressHandler() {
            if(opt.isChanged && (opt.state =! opt.pullUpState)) {
                if (!opt.isPressDebMode) {
                    opt.isPressDebMode = 1;
                    opt.tmr = millis();
                } else if (millis() - opt.tmr > debounce) {
                    opt.state = !opt.state;
                    opt.isPressDebMode = 0;
                }
            } else {
                opt.isPressDebMode = 0;
            }
        }

        void unpressHandler() {
            if(opt.isChanged && (opt.state == opt.pullUpState)) {
                if (!opt.isUnpressDebMode) {
                    opt.isUnpressDebMode = 1;
                    opt.tmr = millis();
                } else if (millis() - opt.tmr > debounce) {
                    opt.state = !opt.state;
                    opt.isUnpressDebMode = 0;
                }
            } else {
                opt.isUnpressDebMode = 0;
            }
        }
    };
}