#include <avr/io.h>
#include <util/delay.h>
#include "h_bridge.h"

void agv_rechtdoor(void)
{
    set_motor_speed(1, 200);
    set_motor_speed(2, 200);
    _delay_ms(100);
}

void agv_rechts_bocht(void)
{
    set_motor_speed(1, 165);
    set_motor_speed(2, 200);
    _delay_ms(100);
}

void agv_links_bocht(void)
{
    set_motor_speed(1, 165);
    set_motor_speed(2, 200);
    _delay_ms(100);
}

void agv_stoppen(void)
{
    set_motor_speed(1, 0);
    set_motor_speed(2, 0);
    _delay_ms(100);
}

void agv_rechts_correctie(void)
{
    set_motor_speed(1, 180);
    set_motor_speed(2, 200);
    _delay_ms(100);
}

void agv_links_correctie(void) {
    set_motor_speed(1, 200);
    set_motor_speed(2, 180);
    _delay_ms(100);
}

void init_motors(void)
{
    DDRL |= (1 << IN1) | (1 << IN2);
    DDRB |= (1 << IN3) | (1 << IN4);

    DDRH |= (1 << EN_A);
    DDRE |= (1 << EN_B);

    TCCR2A |= (1 << WGM20) | (1 << WGM21);
    TCCR2A |= (1 << COM2B1);
    TCCR2B |= (1 << CS21);

    TCCR3A |= (1 << WGM30) | (1 << WGM32);
    TCCR3A |= (1 << COM3A1);
    TCCR3B |= (1 << CS31);

    OCR2B = 0;
    OCR3A = 0;
}

void set_motor_speed(uint8_t motor, uint8_t speed)
{
    if (motor == 1)
        {
            OCR2B = speed;
        }
    if (motor == 2)
        {
            OCR3A = speed;
        }
}

void set_motor_direction(uint8_t motor, uint8_t direction)
{
    if (motor == 1)
    {
        if (direction == 1)
        {
            PORTL |= (1 << IN1);
            PORTL &= ~(1 << IN2);
        }

    }
    else if (motor == 2)
    {
        if (direction == 1)
        {
            PORTB |= (1 << IN3);
            PORTB &= ~(1 << IN4);
        }
    }
}

void initm(void)
{
    set_motor_direction(1, 1);
    set_motor_direction(2, 1);
}
