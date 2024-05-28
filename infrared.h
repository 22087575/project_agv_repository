#include <avr/io.h>
#include "infrared.h"

void init_infrared(void)
{
    DDRF &= ~(1 << INFRARED_PIN); // Set as input
}

int infrared_detect(void)
{
    return (PINF & (1 << INFRARED_PIN)) == 0;
}
