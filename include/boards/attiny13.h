#pragma once

/*
This file contains ATtiny13/A
specific hardware definitions.
                              __ __
(PCINT5/RESET/ADC0/dW) PB5  1|     |8  VCC
    (PCINT3/CLKI/ADC3) PB3  2|     |7  PB2 (SCK/ADC1/T0/PCINT2)
         (PCINT4/ADC2) PB4  3|     |6  PB1 (MISO/AIN1/OC0B/INT0/PCINT1)
                       GND  4|_____|5  PB0 (MOSI/AIN0/OC0A/PCINT0)
*/

// Boards.txt defines ATtiny13A. This ensures compatibility
#if !defined(__AVR_ATtiny13__)
  #define __AVR_ATtiny13__
#endif

// PIN macros
#define PIN_PB0 0
#define PIN_PB1 1
#define PIN_PB2 2
#define PIN_PB3 3
#define PIN_PB4 4
#define PIN_PB5 5

// Definitions for the analog pins
typedef enum
{
  A0 = 0,
  A1 = 1,
  A2 = 2,
  A3 = 3
} analog_pin_t;