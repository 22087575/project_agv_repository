/*
 * h_bridge.h - XvR 2020
 */

#ifndef _H_BRIDGE_H_
#define _H_BRIDGE_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define IN1 PL2
#define IN2 PL0
#define IN3 PB2
#define IN4 PB0
#define EN_A PH6 // OC2B
#define EN_B PE3 // OC3A

// Button pins
#define S1 PF1
#define S2 PF2
#define S3 PF3
#define DDR_BUTTONS DDRF
#define PIN_BUTTONS PINF

// LED pins (as in the example)
#define D1 PB7
#define D2 PB6
#define D3 PB5
#define D4 PB4
#define DDR_LEDS DDRB
#define PORT_LEDS PORTB


#endif /* _H_BRIDGE_H_ */
