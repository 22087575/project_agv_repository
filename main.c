#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "h_bridge.h"
#include "buzzer.h"
#include "ultrasone.h"
#include "infrared.h"
#include "PID.h"

// Flags and variables
volatile uint8_t boom_detected_flag = 0;
volatile uint8_t cool_down_flag = 0;
volatile uint16_t cool_down_counter = 0;
volatile uint16_t ultrason_obstacle_flag = 0;
volatile uint16_t ir_left_detected_flag = 0;
volatile uint16_t ir_right_detected_flag = 0;
volatile uint16_t path_exit_flag = 0;
// Initialize system
void init(void) {
    // Initialize peripherals
    initm();
    init_motors();
    init_infrared();
    init_ultrasonics();
    init_timer_buzzer();

    // Initialize Timer0 for ultrasonic delay and cool-down
    TCCR0A = (1 << WGM01); // CTC mode
    TCCR0B = (1 << CS02) | (1 << CS00); // Prescaler 1024
    OCR0A = 156; // 10ms interval (16000000/1024/100)
    TIMSK0 |= (1 << OCIE0A); // Enable Timer0 compare match A interrupt

    sei(); // Enable global interrupts
    init_buzzer(); // Initialize the buzzer
    buzzer_uit();  // Ensure the buzzer is off
}

// Timer0 compare match interrupt service routine
ISR(TIMER0_COMPA_vect) {
    static uint8_t tick_count = 0;

    if (boom_detected_flag) {
        tick_count++;

        if (tick_count % 75 == 0) { // Toggle buzzer every 500ms
            buzzer_toggle();
        }

        if (tick_count >= 150) { // 3 seconds
            boom_detected_flag = 0;
            cool_down_flag = 1;
            tick_count = 0;
            buzzer_uit();
        }
    }

    if (cool_down_flag) {
        cool_down_counter++;
        if (cool_down_counter >= 75) { // Cool-down period of 3 seconds
            cool_down_flag = 0;
            cool_down_counter = 0;
            disable_timer0();
        }
    }
}
void disable_timer0() {
    // Clear the Clock Select bits to stop the timer
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));

    // Disable Timer0 interrupts
   TIMSK0 &= ~((1 << TOIE0) | (1 << OCIE0A) | (1 << OCIE0B));
}
int main(void) {
    init();
    uint16_t distance1, distance2, distance3;

    while (1) {
        distance1 = measure_distance1();
        distance2 = measure_distance2();
        distance3 = measure_distance3();
        // IR Sensor readings and path correction
        ir_left_detected_flag = read_ir_left(); // Function to read left IR sensor
        ir_right_detected_flag = read_ir_right(); // Function to read right IR sensor

        // Handle obstacle detected by distance1 sensor
        if (distance1 < 15 && !cool_down_flag) {
            if (!boom_detected_flag) {
                agv_stoppen();
                boom_detected_flag = 1;
                TCCR0B |= (1 << CS02) | (1 << CS00); // Start Timer0
            }
        }

         // Handle obstacle detected by distance1 sensor
        if (distance2 < 15 && !cool_down_flag) {
            if (!boom_detected_flag) {
                agv_stoppen();
                boom_detected_flag = 1;
                TCCR0B |= (1 << CS02) | (1 << CS00); // Start Timer0
            }
        }
        // Handle obstacle detected by distance3 sensor
        if(!(distance3 < 15))
        {
            ultrason_obstacle_flag = 0;
        }
        if (distance3 < 15) {
            agv_stoppen();
            buzzer_uit();
            ultrason_obstacle_flag = 1;
        }

        // Warning signal for close obstacles detected by distance3
        if (distance3 < 30 && distance3 > 15) {
            if (!boom_detected_flag && !cool_down_flag) {
                for (int i = 0; i < 2; i++) {
                    if (TIFR4 & (1 << TOV4)) {
                        TCNT4 = TCNT_INIT;
                        TIFR4 = (1 << TOV4);
                        buzzer_toggle();
                        _delay_ms(100);
                    }
                }
            }
        }



        if (ir_left_detected_flag && ir_right_detected_flag && !boom_detected_flag && !ultrason_obstacle_flag) {
            agv_rechtdoor();
            path_exit_flag = 0; // Reset path exit flag if both sensors detect the wall
        } else if (!ir_left_detected_flag && ir_right_detected_flag && !boom_detected_flag && !ultrason_obstacle_flag) {
            agv_links_correctie(); // Correct to the left if right sensor sees the wall but left does not
        } else if (ir_left_detected_flag && !ir_right_detected_flag && !boom_detected_flag && !ultrason_obstacle_flag) {
            agv_rechts_correctie(); // Correct to the right if left sensor sees the wall but right does not
        } else {
            if (!path_exit_flag && !boom_detected_flag && !ultrason_obstacle_flag) {
                agv_stoppen();
                _delay_ms(1000); // Brief stop before turning
                path_exit_flag = 1; // Set flag to indicate AGV has exited the path
            }
    }
    }
    return 0;
}
