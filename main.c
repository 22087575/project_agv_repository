#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef F_CPU
#define F_CPU 16000000ul
#endif
#include "h_bridge.h"
#include "buzzer.h"
#include "ultrasone.h"
#include "infrared.h"
/*#include "ultrasonelat.h"*/

volatile uint8_t emergency_stop_flag = 0;
volatile uint8_t infrared_detected_flag = 0;
volatile uint8_t cool_down_flag = 0;
volatile uint16_t cool_down_counter = 0;

static int lat_detectie(void) { return ((PIN_BUTTONS & (1 << S1)) == 0); }
static int lat_detectie2(void) { return ((PIN_BUTTONS & (1 << S2)) == 0); }
static int boom_detectie(void) { return ((PIN_BUTTONS & (1 << S3)) == 0); }

void init(void) {
    // Initialize H-bridge, set motor pins as output
    init_ultrasonic();
    initm();
    init_motors();
    init_infrared();
    init_ultrasoniclat();

    // Initialize External Interrupt for emergency stop
    EICRA |= (1 << ISC01); // Falling edge on INT0
    EIMSK |= (1 << INT0);  // Enable INT0

    // Initialize Timer0 for infrared delay and cool-down
    TCCR0A = (1 << WGM01); // CTC mode
    TCCR0B = (1 << CS02) | (1 << CS00); // Prescaler 1024
    OCR0A = 156; // 10ms interval (16000000/1024/100)
    TIMSK0 |= (1 << OCIE0A); // Enable Timer0 compare match A interrupt

    sei(); // Enable global interrupts

    init_buzzer(); // Initialize the buzzer
    buzzer_uit();  // Ensure the buzzer is off
}

// Emergency stop interrupt service routine
ISR(INT0_vect) {
    emergency_stop_flag = 1;
}

// Timer0 compare match interrupt service routine
ISR(TIMER0_COMPA_vect) {
    static uint8_t tick_count = 0;
    static uint8_t cool_down_count = 0;

    if (infrared_detected_flag) {
        tick_count++;
        if (tick_count % 50 == 0) { // Toggle buzzer every 500ms
            buzzer_toggle();
        }
        if (tick_count >= 150) { // 3 seconds
            infrared_detected_flag = 0;
            cool_down_flag = 1;
            tick_count = 0;
            buzzer_uit();
            agv_rechtdoor();
        }
    }

    if (cool_down_flag) {
        cool_down_count++;
        if (cool_down_count >= 150) { // Cool-down period of 3 seconds
            cool_down_flag = 0;
            cool_down_count = 0;
        }
    }
}

int main(void) {
    init();
    uint16_t distance;
    uint16_t distancelat;

    if (emergency_stop_flag) {
        agv_stoppen();
        while (1); // Stay here indefinitely for emergency stop
    }

    while (1) {
        distance = measure_distance();
        distancelat = measure_distancelat();

        if (distance < 15) {
            agv_stoppen();
            buzzer_uit();  // Ensure buzzer is off
            _delay_ms(100);
        } else {
            if (lat_detectie()) {
                agv_rechts_bocht();
                _delay_ms(100);
            } else if (lat_detectie2()) {
                agv_links_bocht();
                _delay_ms(100);
            } else if (infrared_detect() && !cool_down_flag && !infrared_detected_flag) {
                agv_stoppen();
                infrared_detected_flag = 1;
                TCCR0B |= (1 << CS02) | (1 << CS00); // Start Timer0
            } else if (!infrared_detected_flag) {
                agv_rechtdoor();
                buzzer_uit();
            }
            if (distance < 30 && distance > 15) {
                for (int i = 0; i < 10; i++) {
                    buzzer_toggle();
                    _delay_ms(100);
                }
                buzzer_uit();
                _delay_ms(100);
            }
            if (distancelat > 50) {
                agv_links_bocht();
                _delay_ms(100);
            }
            if (distancelat < 13) {
                agv_rechts_correctie();
                _delay_ms(100);
            }
            if (distancelat > 17) {
                agv_links_correctie();
                _delay_ms(100);
            }
        }
    }
    return 0;
}
