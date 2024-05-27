#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 16000000ul
#endif
#include <util/delay.h>
#include "h_bridge.h"
#include "buzzer.h"
#include "ultrasone.h"
#include "infrared.h"

static int lat_detectie(void) { return ((PIN_BUTTONS & (1 << S1)) == 0); }
static int lat_detectie2(void) { return ((PIN_BUTTONS & (1 << S2)) == 0); }
static int boom_detectie(void) { return ((PIN_BUTTONS & (1 << S3)) == 0); }

void init(void) {
    // Initialize H-bridge, set motor pins as output
    init_ultrasonic();
    init_pin();
    init_timer();
    initm();
    init_motors();
    init_infrared();
    TCCR1A = 0;
    TCCR1B = 0;
    buzzer_uit();
}

int main(void) {
    init();
    uint16_t distance;

    while (1) {
        distance = measure_distance();

        if (distance < 30 && distance > 15) {
            TCNT4 = TCNT_INIT;
            TIFR4 = (1 << TOV4);
            buzzer_toggle();
            _delay_ms(100);
        }

        if (distance < 15) {
            agv_stoppen();
            buzzer_uit();
            _delay_ms(100);
        }

        if (lat_detectie()) {
            agv_rechts_bocht();
            _delay_ms(100);
        }

        if (lat_detectie2()) {
            agv_links_bocht();
            _delay_ms(100);
        }

        if (infrared_detect()) {
            agv_stoppen();
            for (int i = 0; i < 3; i++) {
                buzzer_toggle();
                _delay_ms(500);
                buzzer_toggle();
                _delay_ms(500);
            }
            _delay_ms(100);  // Wait for 3 seconds
            buzzer_uit();
        } else {
            agv_rechtdoor();
            buzzer_uit();
            _delay_ms(100);
        }
    }

    return 0;
}
