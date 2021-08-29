#pragma once

#include "ng_defs.h"
#include <avr/io.h>

namespace ng {

    /**
     * @brief Configures the specified pin to be an input or output.
     *
     * @param pin The pin number to set the mode of
     * @param mode Pin mode. Valid options are:
     *        OUTPUT - set pin as output,
     *        INPUT - set pin as input,
     *        INPUT_PULLUP - set pin as input and enable internal pullup resistor
     */
    void pinMode(uint8_t pin, uint8_t mode)
    {
        if(mode == OUTPUT) { // Pin as output
            DDRB |= _BV(pin);
        } else { // Pin as input or input pullup
            DDRB &= ~_BV(pin); // Set pin as input
            if(mode == INPUT_PULLUP)
                PORTB |= _BV(pin); // Enable pullup resistors
        }
    }


    /**
     * @brief Writes a HIGH or a LOW value to a digital pin.
     *
     * @param pin The pin number to set the state of
     * @param val The state to set.
     */
    void digitalWrite(uint8_t pin, uint8_t val)
    {
        if(val)
            PORTB |= _BV(pin);  // Set pin high
        else
            PORTB &= ~_BV(pin); // Set pin low
    }


    /**
     * @brief Reads a digital pin
     *
     * @param pin The pin to read
     * @return uint8_t State of the pin
     */
    uint8_t digitalRead(uint8_t pin)
    {
        return PINB & _BV(pin);
    }
}