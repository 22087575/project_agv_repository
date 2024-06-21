#include <avr/io.h>
#include "infrared.h"

void init_infrared(void)
{
    DDRF &= ~(1 << INFRARED_PIN); // Set as input
    DDRF &= ~(1 << INFRARED2_PIN);
}

int read_ir_left(void)
{
    return (PINF & (1 << INFRARED_PIN)) == 0;
}
int read_ir_right(void)
{
    return (PINF & (1 << INFRARED2_PIN)) == 0;
}
