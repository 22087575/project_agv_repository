#include "ultrasone.h"
#include <avr/io.h>
#include <util/delay.h>

void init_ultrasonic(void) {
    DDRH |= (1 << TRIG_PIN);
    DDRH &= ~(1 << ECHO_PIN);
    PORTH &= ~(1 << TRIG_PIN);
}

uint16_t measure_distance(void) {
    uint16_t duration;

    PORTH |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTH &= ~(1 << TRIG_PIN);

    while (!(PINH & (1 << ECHO_PIN)));

    TCNT1 = 0;
    TCCR1B |= (1 << CS11);
    while (PINH & (1 << ECHO_PIN)) {
        if (TCNT1 > 60000) {
            break;
        }
    }
    TCCR1B &= ~(1 << CS11);

    duration = TCNT1;

    uint16_t distance = duration / 58;

    return distance;
}
