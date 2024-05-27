#include <avr/io.h>
#include <util/delay.h>
#include "h_bridge.h"

// Deze functie laat de AGV rechtdoor rijden
void agv_rechtdoor(void) {
    // Zet de snelheid van motor 1 en motor 2 op maximum (255)
    set_motor_speed(1, 255);
    set_motor_speed(2, 255);
    _delay_ms(100); // Wacht 100 ms
}

// Deze functie laat de AGV een bocht naar rechts maken
void agv_rechts_bocht(void) {
    // Zet de snelheid van motor 1 op 240 en motor 2 op 180
    set_motor_speed(1, 240);
    set_motor_speed(2, 180);
    _delay_ms(100); // Wacht 100 ms
}

// Deze functie laat de AGV een bocht naar links maken
void agv_links_bocht(void) {
    // Zet de snelheid van motor 1 op 180 en motor 2 op 240
    set_motor_speed(1, 180);
    set_motor_speed(2, 240);
    _delay_ms(100); // Wacht 100 ms
}

// Deze functie stopt de AGV
void agv_stoppen(void) {
    // Zet de snelheid van motor 1 en motor 2 op 0
    set_motor_speed(1, 0);
    set_motor_speed(2, 0);
    _delay_ms(100); // Wacht 100 ms
}

// Deze functie corrigeert de AGV naar rechts
void agv_rechts_correctie(void) {
    // Zet de snelheid van motor 1 op 255 en motor 2 op 230
    set_motor_speed(1, 255);
    set_motor_speed(2, 230);
    _delay_ms(100); // Wacht 100 ms
}

// Deze functie corrigeert de AGV naar links
void agv_links_correctie(void) {
    // Zet de snelheid van motor 1 op 230 en motor 2 op 255
    set_motor_speed(1, 230);
    set_motor_speed(2, 255);
    _delay_ms(100); // Wacht 100 ms
}

// Initialisatie van de motoren
void init_motors(void) {
    // Zet de pinnen voor de motorbesturing als uitgang
    DDRL |= (1 << IN1) | (1 << IN2);
    DDRB |= (1 << IN3) | (1 << IN4);

    // Zet de pinnen voor de motorenable als uitgang
    DDRH |= (1 << EN_A);
    DDRE |= (1 << EN_B);

    // Configureer Timer2 voor PWM op motor 1
    TCCR2A |= (1 << WGM20) | (1 << WGM21); // Fast PWM mode
    TCCR2A |= (1 << COM2B1); // Clear OC2B on Compare Match, set OC2B at BOTTOM
    TCCR2B |= (1 << CS21); // Prescaler op 8

    // Configureer Timer3 voor PWM op motor 2
    TCCR3A |= (1 << WGM30) | (1 << WGM32); // Fast PWM mode
    TCCR3A |= (1 << COM3A1); // Clear OC3A on Compare Match, set OC3A at BOTTOM
    TCCR3B |= (1 << CS31); // Prescaler op 8

    // Zet de PWM waarden voor beide motoren op 0 (uit)
    OCR2B = 0;
    OCR3A = 0;
}

// Zet de snelheid van een motor
void set_motor_speed(uint8_t motor, uint8_t speed) {
    if (motor == 1) {
        OCR2B = speed; // Zet de PWM waarde voor motor 1
    } else if (motor == 2) {
        OCR3A = speed; // Zet de PWM waarde voor motor 2
    }
}

// Zet de richting van een motor
void set_motor_direction(uint8_t motor, uint8_t direction) {
    if (motor == 1) {
        if (direction == 1) {
            PORTL |= (1 << IN1); // Zet IN1 hoog
            PORTL &= ~(1 << IN2); // Zet IN2 laag
        } else {
            PORTL &= ~(1 << IN1); // Zet IN1 laag
            PORTL |= (1 << IN2); // Zet IN2 hoog
        }
    } else if (motor == 2) {
        if (direction == 1) {
            PORTB |= (1 << IN3); // Zet IN3 hoog
            PORTB &= ~(1 << IN4); // Zet IN4 laag
        } else {
            PORTB &= ~(1 << IN3); // Zet IN3 laag
            PORTB |= (1 << IN4); // Zet IN4 hoog
        }
    }
}

// Initialiseer de motorrichtingen
void initm(void) {
    set_motor_direction(1, 1); // Zet motor 1 naar vooruit
    set_motor_direction(2, 1); // Zet motor 2 naar vooruit
}

