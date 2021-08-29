#pragma once

#include <avr/io.h>
#include "ng_io.h"
#include "ng_time.h"

namespace ng
{
    class Button
    {
       struct opts {
            uint8_t prevState: 1; // previous state of the button
            uint8_t state: 1; // state of the button
            uint8_t pullUpState: 1; // pull up type of button (1 - button pull upped to high, 0 - button pull upped to low)
            uint8_t isPressDebMode: 1; // button is in debounce handling mode while pressing
            uint8_t isUnpressDebMode: 1; // button is in debounce handling mode while unpressing
            uint8_t isChanged: 1; // button state change sign
            uint8_t isClicked: 1; // clicked sign
       } opt;

        /**
         * pin wich is connected to button
         */
        uint8_t pin;

        /**
         * debounce tiomeout
         */
        uint8_t debounce = 30;

        // button timer
        uint32_t tmr;

        uint8_t _readPin() {
            return digitalRead(pin)? 1: 0;
        }

    public:
        Button(uint8_t _pin, uint8_t pullUpState = HIGH): pin(_pin) {
            if (pullUpState == HIGH) {
                pinMode(pin, INPUT_PULLUP);
            }

            opt.state = _readPin(); // init button state
            opt.pullUpState = pullUpState; // pull up state: (HIGH|LOW)
            opt.isPressDebMode = 0;
            opt.isUnpressDebMode = 0;
            opt.isChanged = 0; // state changed sign
            opt.isClicked = 0; // is clicked sign
        }

        uint8_t isUp() {
            return opt.state == opt.pullUpState;
        }

        uint8_t isDown() {
            return opt.state != opt.pullUpState;
        }

        uint8_t isClicked() {
            if (opt.isClicked) {
                opt.isClicked = 0;
                return TRUE;
            }

            return FALSE;
        }

        void tick() {
            opt.isChanged = _readPin() != opt.state;

            pressHandler();
            unpressHandler();
            clickHandler();
        }

        void pressHandler() {
            if(opt.isChanged && (opt.state == opt.pullUpState)) {
                if (!opt.isPressDebMode) {
                    opt.isPressDebMode = 1;
                    tmr = ng::millis();
                } else if (ng::millis() - tmr > debounce) {
                    opt.state = !opt.state;
                    opt.isPressDebMode = 0;
                }
            } else {
                opt.isPressDebMode = 0;
            }
        }

        void unpressHandler() {
            if(opt.isChanged && (opt.state != opt.pullUpState)) {
                if (!opt.isUnpressDebMode) {
                    opt.isUnpressDebMode = 1;
                    tmr = ng::millis();
                } else if (ng::millis() - tmr > debounce) {
                    opt.state = !opt.state;
                    opt.isUnpressDebMode = 0;
                }
            } else {
                opt.isUnpressDebMode = 0;
            }
        }

        void clickHandler() {
            if (opt.prevState != opt.pullUpState && opt.state == opt.pullUpState) {
                opt.isClicked = 1;
            } else if (opt.state != opt.pullUpState) {
                opt.isClicked = 0;
            }

            opt.prevState = opt.state;
        }
    };
}