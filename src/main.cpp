#define PROGRAM_PWM_SIZE 3

#include "ng_cheduler.h"
#include "ng_program_pwm.h"
#include "ng_adc.h"
#include "ng_pwm.h"

#include <avr/interrupt.h>

uint8_t WIDTH = 30;

void toggle() {
    attachInterrupt

    static uint8_t val[3] = {0, (uint8_t)(WIDTH/2U), WIDTH};
    static uint8_t dir[3] = {1, 1, 0};
    static uint8_t port[3] = {PORTB3, PORTB2, PORTB1};

    for (int i = 0; i < 3; i++) {
        if (dir[i]) val[i]++;
        else val[i]--;

        if (val[i] >= WIDTH || val[i] <= 0) dir[i] = !dir[i]; // change direction

        ng::pwmManager.setupPwm(i, port[i], val[i], WIDTH);
    }
}

void readVoltage() {
    uint16_t val = ng::Adc::read(2);

    ng::fastPWM(0, val / 4);
    // ng::pwmManager.setupPwm(0, PORTB0, val / 4, 255);
    // ng::pwmManager.setupPwm(1, PORTB1, val / 4, 255);
    // ng::pwmManager.setupPwm(2, PORTB2, val / 4, 255);
}

int main() {
    DDRB |= 0xFF;

    DDRB &= ~_BV(PORTB4);
    PORTB |= _BV(PORTB4);

    for(;;) {
        {EVERY_MS(1) {
            toggle();
        }}

        {EVERY_MS(1) {
            readVoltage();
        }}
    }

    return 0;
}