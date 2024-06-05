#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "h_bridge.h"
#include "buzzer.h"
#include "ultrasone.h"
#include "infrared.h"
#include "PID.h"

// Flags and variables
volatile uint8_t emergency_stop_flag = 0;
volatile uint8_t infrared_detected_flag = 0;
volatile uint8_t cool_down_flag = 0;
volatile uint16_t cool_down_counter = 0;
volatile uint8_t obstacle_detected_flag = 0;

// Button detection functions
static int lat_detectie(void) { return ((PIN_BUTTONS & (1 << S1)) == 0); }
static int lat_detectie2(void) { return ((PIN_BUTTONS & (1 << S2)) == 0); }
static int boom_detectie(void) { return ((PIN_BUTTONS & (1 << S3)) == 0); }

void init(void) {
    // Initialize peripherals
    init_ultrasonic();
    initm();
    init_motors();
    init_infrared();
    init_ultrasoniclat();
    init_timer_buzzer();

    // Initialize External Interrupt for emergency stop
    EICRA |= (1 << ISC01); // Falling edge on INT0
    EIMSK |= (1 << INT0);  // Enable INT0

    // Initialize Timer0 for infrared delay and cool-down
    TCCR0A = (1 << WGM01); // CTC mode
    TCCR0B = (1 << CS02) | (1 << CS00); // Prescaler 1024
    OCR0A = 156; // 10ms interval (16000000/1024/100)
    TIMSK0 |= (1 << OCIE0A); // Enable Timer0 compare match A interrupt

    sei(); // Set 'enable global interrupts' flag

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

    if (obstacle_detected_flag) {
        obstacle_detected_flag = 0;
    }
}

int main(void) {
    init();
    uint16_t distance;
    uint16_t distancelat;

    // PID configuration
    PIDController lat_pid;
    PIDController_Init(&lat_pid, 2.0f, 0.1f, 0.01f, -255.0f, 255.0f);
    lat_pid.setpoint = 5.0f; // Desired lateral distance in cm

    while (1) {
        if (emergency_stop_flag) {
            agv_stoppen();
            while (1); // Stay here indefinitely for emergency stop
        }

        distance = measure_distance();
        distancelat = measure_distancelat();

        if (distance < 15) {
            agv_stoppen();
            buzzer_uit();  // Ensure buzzer is off
            obstacle_detected_flag = 1;
        } else if (!obstacle_detected_flag) {
            if (infrared_detect() && !cool_down_flag && !infrared_detected_flag) {
                agv_stoppen();
                infrared_detected_flag = 1;
                TCCR0B |= (1 << CS02) | (1 << CS00); // Start Timer0
            } else if (!infrared_detected_flag) {
                // PID control for lateral distance
                float correction = PIDController_Update(&lat_pid, (float)distancelat);

                // Calculate motor speeds based on correction
                int16_t base_speed = 200;
                int16_t left_motor_speed = base_speed - (int16_t)correction;
                int16_t right_motor_speed = base_speed + (int16_t)correction;

                // Constrain motor speeds to valid range
                left_motor_speed = (left_motor_speed < 0) ? 0 : (left_motor_speed > 255) ? 255 : left_motor_speed;
                right_motor_speed = (right_motor_speed < 0) ? 0 : (right_motor_speed > 255) ? 255 : right_motor_speed;

                // Set motor speeds
                set_motor_speed(1, (uint8_t)left_motor_speed);
                set_motor_speed(2, (uint8_t)right_motor_speed);

                buzzer_uit();
            }

            if (distance < 30 && distance > 15) {
                for (int i = 0; i < 2; i++) {
                    if (TIFR4 & (1 << TOV4)) {
                        TCNT4 = TCNT_INIT;
                        TIFR4 = (1 << TOV4);
                        buzzer_toggle();
                    }
                }
            }
        }
    }
    return 0;
}
