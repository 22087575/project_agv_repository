#include "ultrasone.h"
#include <avr/io.h>
#include <util/delay.h>

// Initialize all three ultrasonic sensors
void init_ultrasonics(void) {
    // Sensor 1
    DDRH |= (1 << US1_TRIG_PIN);  // Set trigger pin as output
    DDRH &= ~(1 << US1_ECHO_PIN); // Set echo pin as input
    PORTH &= ~(1 << US1_TRIG_PIN);

    // Sensor 2
    DDRJ |= (1 << US2_TRIG_PIN);  // Set trigger pin as output
    DDRJ &= ~(1 << US2_ECHO_PIN); // Set echo pin as input
    PORTJ &= ~(1 << US2_TRIG_PIN);

    // Sensor 3
    DDRH |= (1 << US3_TRIG_PIN);  // Set trigger pin as output
    DDRH &= ~(1 << US3_ECHO_PIN); // Set echo pin as input
    PORTH &= ~(1 << US3_TRIG_PIN);

       // Sensor 4
    DDRD |= (1 << US4_TRIG_PIN);  // Set trigger pin as output
    DDRD &= ~(1 << US4_ECHO_PIN); // Set echo pin as input
    PORTD &= ~(1 << US4_TRIG_PIN);
}

// Measure distance with sensor 1
float measure_distance1(void) {
    uint16_t duration;

    // Trigger pulse
    PORTH |= (1 << US1_TRIG_PIN);
    _delay_us(10);
    PORTH &= ~(1 << US1_TRIG_PIN);

    // Wait for echo pin to go high
    while (!(PINH & (1 << US1_ECHO_PIN)));

    // Start timer
    TCNT1 = 0;
    TCCR1B |= (1 << CS11); // Prescaler of 8

    // Wait for echo pin to go low
    while (PINH & (1 << US1_ECHO_PIN)) {
        if (TCNT1 > 60000) {
            break;
        }
    }

    // Stop timer
    TCCR1B &= ~(1 << CS11);

    // Read timer value
    duration = TCNT1;

    // Convert duration to distance in cm
    float distance = duration / 58.0 / 2.0;

    return distance;
}

// Measure distance with sensor 2
float measure_distance2(void) {
    uint16_t duration;

    // Trigger pulse
    PORTJ |= (1 << US2_TRIG_PIN);
    _delay_us(10);
    PORTJ &= ~(1 << US2_TRIG_PIN);

    // Wait for echo pin to go high
    while (!(PINJ & (1 << US2_ECHO_PIN)));

    // Start timer
    TCNT1 = 0;
    TCCR1B |= (1 << CS11); // Prescaler of 8

    // Wait for echo pin to go low
    while (PINJ & (1 << US2_ECHO_PIN)) {
        if (TCNT1 > 60000) {
            break;
        }
    }

    // Stop timer
    TCCR1B &= ~(1 << CS11);

    // Read timer value
    duration = TCNT1;

    // Convert duration to distance in cm
    float distance = duration / 58.0 / 2.0;

    return distance;
}

// Measure distance with sensor 3
float measure_distance3(void) {
    uint16_t duration;

    // Trigger pulse
    PORTH |= (1 << US3_TRIG_PIN);
    _delay_us(10);
    PORTH &= ~(1 << US3_TRIG_PIN);

    // Wait for echo pin to go high
    while (!(PINH & (1 << US3_ECHO_PIN)));

    // Start timer
    TCNT1 = 0;
    TCCR1B |= (1 << CS11); // Prescaler of 8

    // Wait for echo pin to go low
    while (PINH & (1 << US3_ECHO_PIN)) {
        if (TCNT1 > 60000) {
            break;
        }
    }

    // Stop timer
    TCCR1B &= ~(1 << CS11);

    // Read timer value
    duration = TCNT1;

    // Convert duration to distance in cm
    float distance = duration / 58.0 / 2.0;

    return distance;
}

// Measure distance with sensor 3
float measure_distance4(void) {
    uint16_t duration;

    // Trigger pulse
    PORTD |= (1 << US4_TRIG_PIN);
    _delay_us(10);
    PORTD &= ~(1 << US4_TRIG_PIN);

    // Wait for echo pin to go high
    while (!(PINH & (1 << US4_ECHO_PIN)));

    // Start timer
    TCNT1 = 0;
    TCCR1B |= (1 << CS11); // Prescaler of 8

    // Wait for echo pin to go low
    while (PINH & (1 << US4_ECHO_PIN)) {
        if (TCNT1 > 60000) {
            break;
        }
    }

    // Stop timer
    TCCR1B &= ~(1 << CS11);

    // Read timer value
    duration = TCNT1;

    // Convert duration to distance in cm
    float distance = duration / 58.0 / 2.0;

    return distance;
}
