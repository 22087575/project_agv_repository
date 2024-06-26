#ifndef BUZZER_H_INCLUDED
#define BUZZER_H_INCLUDED

#include <avr/io.h>

 // D1 = 13 = PB7
 // D2 = 12 = PB6
 // D3 = 11 = PB5
 // D4 = 10 = PB4
 // S1 = A1 = PF1
 // S2 = A2 = PF2
 // S3 = A3 = PF3

 #define D1 PB7
 #define D2 PB6
 #define D3 PB5
 #define D4 PB4
 #define DDR_LEDS DDRB
#define PORT_LEDS PORTB

 #define S1 PF1
 #define S2 PF2
 #define S3 PF3
 #define DDR_BUTTONS DDRF
 #define PIN_BUTTONS PINF

 #define buzzer PE5
 #define DDR_buzzer DDRE

 // T = 1.5 s -> gebruik timer om 0.75 s af te wachten:
 // 0.75 * 16000000 = 12000000 cycles
 // prescaler selectie:
 // 1 -> 12000000
 // 8 -> 1500000
 // 64 -> 187500
 // 256 -> 46875
 // 1024 -> 11719
 // Kies hier 256: CS2-0: 1 0 0

 #define TCNT_INIT (65636ul - 62500u)
void buzzer_aan(void);
void buzzer_uit(void);
void buzzer_toggle(void);
void init_pin(void);
void init_timer(void);
#endif // BUZZER_H_INCLUDED
