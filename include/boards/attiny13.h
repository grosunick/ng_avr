/*
This file contains ATtiny13/A
specific hardware definitions.
               __ __
(D5/A0) PB5  1|     |8  VCC
(D3/A3) PB3  2|     |7  PB2 (D2/A1)
(D4/A2) PB4  3|     |6  PB1 (D1/PWM)
        GND  4|_____|5  PB0 (D0/PWM)
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