#ifndef ULTRASONETEST_H_INCLUDED
#define ULTRASONETEST_H_INCLUDED

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include <stdint.h>

// Definitions for three ultrasonic sensors
#define US1_TRIG_PIN PH0
#define US1_ECHO_PIN PH1

#define US2_TRIG_PIN PJ1
#define US2_ECHO_PIN PJ0

#define US3_TRIG_PIN PH4
#define US3_ECHO_PIN PH5

void init_ultrasonics(void);
float measure_distance1(void);
float measure_distance2(void);
float measure_distance3(void);

#endif // ULTRASONETEST_H_INCLUDED

