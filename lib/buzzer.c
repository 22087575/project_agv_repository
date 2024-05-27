#include <avr/io.h>
#include "buzzer.h"

void init_pin(void) {
    DDR_buzzer |= (1 << buzzer);
}

void buzzer_aan(void) {
    PORTE &= ~(1 << buzzer);
}

void buzzer_uit(void) {
    PORTE |= (1 << buzzer);
}

void buzzer_toggle(void) {
    PORTE ^= (1 << buzzer);
}

void init_timer(void) {
    TCCR4A = 0;
    TCCR4B = (1 << CS42);
    TCNT4 = TCNT_INIT;
}
