#ifndef INFRARED_H_
#define INFRARED_H_

#include <avr/io.h>

#define INFRARED_PIN PF4

void init_infrared(void);
int infrared_detect(void);

#endif // INFRARED_H_
