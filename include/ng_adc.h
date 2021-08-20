#pragma once

#include <avr/io.h>
#include "Arduino.h"

const uint8_t ADC_REF_VCC = 0;

namespace ng
{
    class Adc
    {
    public:
        static uint16_t read(uint8_t pin) {
            // MUX1 & MUX0 are 2 lowest bits in ADMUX
            ADMUX = (ADMUX & 0xFC) | pin; // select input pin

            initFrequency();
            while(ADCSRA & _BV(ADSC)); // Wait for conversion

            int16_t result = ADCW;
            off();

            return result;
        }

        static void initFrequency() {
            // Init frequenсy divider
            #if F_CPU <= 200000L               // Less or equal to 200 kHz
                ADCSRA = _BV(ADEN) |_BV(ADSC);  // Prescaler of 2 -> F_CPU / 2
            #elif F_CPU <= 1200000L            // Between 200 kHz and 1.2 MHz
                ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS1); // Prescaler to 4 -> F_CPU / 4
            #elif F_CPU <= 6400000L            // Between 1.2 MHz and 6.4 MHz
                ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2); // Prescaler to 16 -> F_CPU / 16
            #else                              // Greater than 6.4 MHz
                ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // Prescaler to 128 -> F_CPU / 128
            #endif
        }

        static void on() {
            ADCSRA |= _BV(ADEN);
        }

        static void off() {
            ADCSRA &= ~_BV(ADEN);
        }

        static void setReference(uint8_t mode) {
            if (mode == ADC_REF_VCC)
                ADMUX &= ~(1 << REFS0); // Vcc as reference
            else
                ADMUX |= (1 << REFS0); // Internal 1.1V as reference
        }
    };
}
