#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef F_CPU
#define F_CPU 16000000ul
#endif
#include "h_bridge.h"
#include "buzzer.h"
#include "ultrasone.h"
#include "infrared.h"

// Globale variabelen voor noodstop en infrarood detectie
volatile uint8_t emergency_stop_flag = 0;
volatile uint8_t infrared_detected_flag = 0;
volatile uint8_t cool_down_flag = 0;
volatile uint16_t cool_down_counter = 0;

// Functies voor detectie van knoppen
static int lat_detectie(void) { return ((PIN_BUTTONS & (1 << S1)) == 0); }
static int lat_detectie2(void) { return ((PIN_BUTTONS & (1 << S2)) == 0); }
static int boom_detectie(void) { return ((PIN_BUTTONS & (1 << S3)) == 0); }

// Initialisatie van verschillende modules en interrupt setup
void init(void) {
    init_ultrasonic(); // Initialiseer ultrasone sensor
    initm(); // Initialiseer motors
    init_motors(); // Initialiseer motor pinnen
    init_infrared(); // Initialiseer infrarood sensor

    // Configureer externe interrupt voor noodstop
    EICRA |= (1 << ISC01); //  Falling edge on INT0
    EIMSK |= (1 << INT0);  // Zet INT0 aan

    // Configureer Timer0 voor infrarood vertraging en cool-down periode
    TCCR0A = (1 << WGM01); // CTC modus
    TCCR0B = (1 << CS02) | (1 << CS00); // Prescaler 1024
    OCR0A = 156; // 10ms interval (16000000/1024/100)
    TIMSK0 |= (1 << OCIE0A); // Schakel Timer0 vergelijk match A interrupt in

    sei(); // Schakel globale interrupts in
}

// ISR voor noodstop interrupt
ISR(INT0_vect) {
    emergency_stop_flag = 1;
}

// ISR voor Timer0 vergelijk match interrupt
ISR(TIMER0_COMPA_vect) {
    static uint8_t tick_count = 0;
    static uint8_t cool_down_count = 0;

    // Als infrarood detectie vlag gezet is
    if (infrared_detected_flag) {
        tick_count++;
        if (tick_count % 50 == 0) { // Wissel buzzer elke 500ms
            buzzer_toggle();
        }
        if (tick_count >= 150) { // 3 seconden
            infrared_detected_flag = 0;
            cool_down_flag = 1;
            tick_count = 0;
            buzzer_uit();
            agv_rechtdoor(); // Laat AGV doorgaan na 3 seconden
        }
    }

    // Cool-down periode
    if (cool_down_flag) {
        cool_down_count++;
        if (cool_down_count >= 150) { // Cool-down periode van 3 seconden
            cool_down_flag = 0;
            cool_down_count = 0;
        }
    }
}

int main(void) {
    init();
    uint16_t distance;
    init_timer();
    buzzer_uit();

    while (1) {
        // Controleer noodstop
        if (emergency_stop_flag) {
            agv_stoppen();
            while (1); // Blijf hier oneindig voor noodstop
        }

        distance = measure_distance();
        if (distance < 15) {
            agv_stoppen();
            buzzer_uit();
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
            } else if (distance < 30 && distance > 15) {
                for (int i = 0; i < 10; i++) {
                    TCNT4 = TCNT_INIT;
                    TIFR4 = (1 << TOV4);
                    buzzer_toggle();
                    _delay_ms(100);
                }
                buzzer_uit();
                _delay_ms(100);
            }
        }
    }

    return 0;
}
