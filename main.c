#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "h_bridge.h"
#include "buzzer.h"
#include "ultrasonetest.h"
#include "infrared.h"
#include "PID.h"

// Flags and variables
volatile uint8_t emergency_stop_flag = 0;
volatile uint8_t ultrasonic_detected_flag = 0;
volatile uint8_t cool_down_flag = 0;
volatile uint16_t cool_down_counter = 0;
volatile uint8_t obstacle_detected_flag = 0;

// Variables to store the last state before emergency stop
volatile uint8_t last_left_motor_speed = 0;
volatile uint8_t last_right_motor_speed = 0;
volatile float last_distance2 = 0;
volatile uint8_t was_following_line = 0;

void init(void) {
    // Initialize peripherals
    initm();
    init_motors();
    init_infrared();
    init_ultrasonics();
    init_timer_buzzer();

    // Initialize External Interrupt for emergency stop
    EICRA |= (1 << ISC01) | (1 << ISC00); // Both edges on INT0
    EIMSK |= (1 << INT0);  // Enable INT0

    // Initialize Timer0 for ultrasonic delay and cool-down
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
    if ((PIND & (1 << PD0)) == 0) { // Check if the button is pressed (falling edge)
        emergency_stop_flag = 1;

        // Save the current state
        last_left_motor_speed = OCR2A;
        last_right_motor_speed = OCR2B;
        was_following_line = 1;

    } else { // Button is released (rising edge)
        emergency_stop_flag = 0;
    }
}

// Timer0 compare match interrupt service routine
ISR(TIMER0_COMPA_vect) {
    static uint8_t tick_count = 0;
    static uint8_t cool_down_count = 0;

    if (ultrasonic_detected_flag) {
        tick_count++;
        if (tick_count % 25== 0) { // Toggle buzzer every 500ms
            buzzer_toggle();
             set_motor_speed(1, 0);
                set_motor_speed(2, 0);
        }
        buzzer_aan();
        if (tick_count >= 150) { // 3 seconds
            ultrasonic_detected_flag = 0;
            cool_down_flag = 1;
            tick_count = 0;
            buzzer_uit();
            was_following_line = 1;
        }
    }

    if (cool_down_flag) {
        cool_down_count++;
        if (cool_down_count >= 150) { // Cool-down period of 3 seconds
            cool_down_flag = 0;
            cool_down_count = 0;
        if (was_following_line) {
                set_motor_speed(1, last_left_motor_speed);
                set_motor_speed(2, last_right_motor_speed);
                was_following_line = 0; // Reset the flag

    }
        }
    }
}
int main(void) {
    init();
    uint16_t distance1, distance2, distance3;

    // PID configuration
    PIDController lat_pid;
    PIDController_Init(&lat_pid, 2.0f, 0.1f, 0.01f, -255.0f, 255.0f);
    lat_pid.setpoint = 10.0f; // Desired lateral distance in cm

    while (1) {
        if (emergency_stop_flag) {
            agv_stoppen();
            buzzer_aan(); // Turn on the buzzer to indicate emergency stop
            continue; // Skip the rest of the loop until emergency stop is released
        } else {
            buzzer_uit(); // Ensure the buzzer is off when emergency stop is released

            // Restore the previous state if the AGV was following the line
            if (was_following_line) {
                set_motor_speed(1, last_left_motor_speed);
                set_motor_speed(2, last_right_motor_speed);
                was_following_line = 0; // Reset the flag
                distance2 = last_distance2;
            }
        }

        distance1 = measure_distance1();
        distance2 = measure_distance2();
        distance3 = measure_distance3();

        if (distance1 < 15 && !cool_down_flag) {
            agv_stoppen();
            ultrasonic_detected_flag = 1;
            TCCR0B |= (1 << CS02) | (1 << CS00); // Start Timer0
        }

        if (distance3 < 15) {
            agv_stoppen();
            buzzer_uit();
        } else {
            // PID control for lateral distance
            float correction = PIDController_Update(&lat_pid, (float)distance2);

            // Calculate motor speeds based on correction
            int16_t base_speed = 180;
            int16_t left_motor_speed = base_speed - (int16_t)correction;
            int16_t right_motor_speed = base_speed - (int16_t)correction;

            // Constrain motor speeds to valid range
            if (left_motor_speed < 0) left_motor_speed = 0;
            if (left_motor_speed > 200) left_motor_speed = 200;
            if (right_motor_speed < 0) right_motor_speed = 0;
            if (right_motor_speed > 200) right_motor_speed = 200;

            // Set motor speeds
            set_motor_speed(1, (uint8_t)left_motor_speed);
            set_motor_speed(2, (uint8_t)right_motor_speed);

            buzzer_uit();
        }

        if (distance3 < 30 && distance3 > 15) {
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

    return 0;
}
