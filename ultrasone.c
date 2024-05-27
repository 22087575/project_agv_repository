#include "ultrasone.h"
#include <avr/io.h>
#include <util/delay.h>

// Initialiseer de ultrasonische sensor
void init_ultrasonic(void) {
    // Stel de trigger pin in als uitgang
    DDRH |= (1 << TRIG_PIN);
    // Stel de echo pin in als ingang
    DDRH &= ~(1 << ECHO_PIN);
    // Zet de trigger pin laag
    PORTH &= ~(1 << TRIG_PIN);
}

// Meet de afstand met de ultrasonische sensor
uint16_t measure_distance(void) {
    uint16_t duration;

    // Stuur een trigger puls van 10 microseconden
    PORTH |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTH &= ~(1 << TRIG_PIN);

    // Wacht tot de echo pin hoog wordt
    while (!(PINH & (1 << ECHO_PIN)));

    // Start de timer
    TCNT1 = 0;
    TCCR1B |= (1 << CS11); // Prescaler van 8
    while (PINH & (1 << ECHO_PIN)) {
        // Controleer of de timer te lang loopt om een te grote afstand te voorkomen
        if (TCNT1 > 60000) {
            break;
        }
    }
    // Stop de timer
    TCCR1B &= ~(1 << CS11);

    // Lees de timer waarde
    duration = TCNT1;

    // Converteer de duur naar afstand in centimeters
    uint16_t distance = duration / 58;

    return distance;
}
