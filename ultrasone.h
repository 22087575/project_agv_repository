#ifndef ULTRASONETEST_H_
#define ULTRASONETEST_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include <stdint.h>

// Ports initialization
#define US_DDR		DDRH
#define US_PORT		PORTH
#define US_PIN		PINH
#define US_TRIGER	PH6
#define US_ECHO		PH3

#define TRIG_PIN PH0
#define ECHO_PIN PH1
void init_ultrasonic(void);
uint16_t measure_distance(void);
			// Return the distance in centimeters

#endif // ULTRASONETEST_H_INCLUDED
