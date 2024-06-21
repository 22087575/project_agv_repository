#ifndef INFRARED_H_
#define INFRARED_H_

#include <avr/io.h>

#define INFRARED_PIN PF5
#define INFRARED2_PIN PF6

void init_infrared(void);
int infrared_detect(void);
int infrared_detect2(void);

#endif // INFRARED_H_
