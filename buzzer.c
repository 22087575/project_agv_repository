#include <avr/io.h>
#include "buzzer.h"

void init_buzzer(void) {
    // Stel de buzzer pin in als uitgang
    DDRB |= (1 << BUZZER_PIN);
    // Zet de buzzer uit om te beginnen
    PORTB &= ~(1 << BUZZER_PIN);
}

void buzzer_uit(void) {
    // Zet de buzzer pin hoog om de buzzer aan te zetten
    PORTB |= (1 << BUZZER_PIN);
}

void buzzer_aan(void) {
    // Zet de buzzer pin laag om de buzzer uit te zetten
    PORTB &= ~(1 << BUZZER_PIN);
}

void buzzer_toggle(void) {
    // Wissel de huidige toestand van de buzzer pin
    PORTB ^= (1 << BUZZER_PIN);
}

void init_timer(void) {
    TCCR4A = 0;
    TCCR4B = (1 << CS42) | (0 << CS41) | (0 << CS40);
    TCNT4 = 0;  // Reset timer
}
