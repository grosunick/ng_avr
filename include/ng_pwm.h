#pragma once

#include <avr/io.h>

namespace ng
{
  /**
   * @brief Turns of PWM functionality on a pin
   *
   * @param pin Pin to disable PWM on
   */
  void offPWM(uint8_t pin)
  {
    if(pin == 0)
        TCCR0A &= ~_BV(COM0A1);
    else
        TCCR0A &= ~_BV(COM0B1);
  }

  /**
   * @brief Writes a PWM value to a PWM compatible pin with PWM frequency = (F_CPU/256) / 1
   *
   * @param pin PWM pin (digital pin 0 or 1)
   * @param val PWM value to write (8-bit, 0 - 255)
   */
  void fastPWM(uint8_t pin, uint8_t val)
  {
      TCCR0B = _BV(CS00);  // PWM frequency = (F_CPU/256) / 1
      DDRB |= _BV(pin); // Set pin to output

      // Otherwise setup the appropriate timer compare
      if(pin == 0) {
          TCCR0A |= _BV(WGM00) | _BV(WGM01) | _BV(COM0A1); // (PWM_FAST)
          OCR0A = val;
      } else {
          TCCR0A |= _BV(WGM00) | _BV(WGM01) | _BV(COM0B1); // (PWM_FAST)
          OCR0B = val;
      }
  }
}