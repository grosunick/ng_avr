#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t period = 1;
uint8_t width = 10;

#ifndef PROGRAM_PWM_SIZE
    #define PROGRAM_PWM_SIZE 1
#endif

#ifndef FAST_PROGRAM_PWM
    #define FAST_PROGRAM_PWM 2
#endif

namespace ng
{
    /**
     * Structure describing PWM protocol
     **/
    struct PWMattr {
        uint8_t pin = 0;        // pin number
        uint8_t step = 10;    // pwm value
        uint8_t width = 20;     // pwm width
        uint8_t enable = 0;     // pwm status

        // TODO: need to hide the field from client code
        uint8_t acc = 0;        // pwm counter
    } pwmOptions[PROGRAM_PWM_SIZE];

    class ProgrmPWM
    {
        /**
         * Initialization of PWM Timer
         */
        void initTimer() {
            cli();
            // TCCR0A = _BV(WGM01); // CTC mode
            TCCR0B = _BV(CS00);  // frequency = (F_CPU/1)
            TIMSK0 = _BV(TOIE0) ; // Enable OCR0A value match interrupt on Timer0
            OCR0A = 0;

            sei(); // Turn on global interrupts
        }
    public:
        /**
         * @param pwmNum Program pwm number. The maximum number of software PWM's is set by the constant PROGRAM_PWM_SIZE
         * @param pin Program pwm pin
         * @param val PWM current value
         * @param width PWM width (default 255)
         */
        void setupPwm(uint8_t pwmNum, uint8_t pin, uint8_t val, uint8_t width = 255) {
            static uint8_t isInited = 0;

            if (isInited == 0) { // init timer
                initTimer();
                isInited = 1;
            }

            auto &opts = pwmOptions[pwmNum];
            opts.pin = pin;
            opts.step = val;
            opts.width = width;
            opts.acc = 0;
            opts.enable = 1;
        }

        void enable(uint8_t pwmNum) {
            if (pwmNum >=0 && pwmNum < PROGRAM_PWM_SIZE) {
                pwmOptions[pwmNum].enable = 1;
            }
        }

        void disable(uint8_t pwmNum) {
            if (pwmNum >=0 && pwmNum < PROGRAM_PWM_SIZE) {
                pwmOptions[pwmNum].enable = 0;
            }
        }
    } pwmManager;

    void pwmAlgo(PWMattr &opts) {
        if (!opts.enable) {
            return;
        }

        if (opts.acc == opts.width) {
            PORTB |= _BV(opts.pin);
            opts.acc = 0;
        }

        if (opts.acc == opts.step) {
            PORTB &= ~_BV(opts.pin);
        }

        opts.acc++;
        return;

        // __asm__ volatile
        // (
        //                 "cpi    %[enable], 0         \n\t"
        //                 "breq   ret_%=               \n\t"
        //                 "cp     %[a], %[period]      \n\t"
        //                 "breq   off_%=               \n\t"
        //                 "cp     %[a], %[width]       \n\t"
        //                 "breq   on_%=                \n\t"
        //                 "rjmp   incr_%=              \n\t"
        //     "on_%=:                                  \n\t"
        //                 "in     r16, %[port]         \n\t"
        //                 "mov    r17, %[pinOn]        \n\t"
        //                 "or     r16, r17             \n\t"
        //                 "out    %[port], r16         \n\t"
        //                 "mov    %[a], 0              \n\t"
        //                 "rjmp   incr_%=              \n\t"
        //     "off_%=:"
        //                 "in     r16, %[port]         \n\t"
        //                 "mov    r17, %[pinOff]       \n\t"
        //                 "and    r16, r17             \n\t"
        //                 "out    %[port], r16         \n\t"
        //     "incr_%=:                                \n\t"
        //                 "inc    %[a]                 \n\t"
        //     "ret_%=:                                 \n\t"
        //     : [a] "=&r" (opts.acc)
        //     : [port] "I" (_SFR_IO_ADDR(PORTB)),
        //     "[a]" (opts.acc),
        //     [width] "r" (opts.width),
        //     [period] "r" (opts.period),
        //     [enable] "r" (opts.enable),
        //     [pinOn] "r" (_BV(opts.pin)),
        //     [pinOff] "r" (~_BV(opts.pin))
        //     : "r16", "r17"
        // );
    }
}

ISR(TIM0_OVF_vect)
{
    for (uint8_t i = 0; i < PROGRAM_PWM_SIZE; i++) {
        auto &opts = ng::pwmOptions[i];

        ng::pwmAlgo(opts);
    }
}