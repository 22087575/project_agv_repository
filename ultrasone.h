#ifndef ULTRASONETEST_H_
#define ULTRASONETEST_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include <stdint.h>

#define USl_DDR      DDRJ
#define USl_PORT     PORTJ
#define USl_PIN      PINJ
#define USl_TRIGER   PJ1
#define USl_ECHO     PJ0

#define TRIGl_PIN PJ1
#define ECHOl_PIN PJ0

// Ports initialization
#define US_DDR		DDRH
#define US_PORT		PORTH
#define US_PIN		PINH
#define US_TRIGER	PH6
#define US_ECHO		PH3

#define TRIG_PIN PH0
#define ECHO_PIN PH1
void init_ultrasonic(void);
float measure_distance(void);
			// Return the distance in centimeters


void init_ultrasoniclat(void);
float measure_distancelat(void);
#endif // ULTRASONETEST_H_INCLUDED
