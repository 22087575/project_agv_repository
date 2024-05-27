#include "buzzer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
  /* void init_buzzer(void)
{
 DDR_buzzer |= (1 << buzzer);

 }*/
/* void buzzer_aan(void) {PORTE &= ~(1 << buzzer);}*/
 void buzzer_uit(void)
 {
     PORTE |= (1 << buzzer);
}
 void buzzer_toggle(void)
 {
     PORTE ^= (1 << buzzer);
}

  void init_timer(void)
 {
 TCCR4A = 0;
 TCCR4B = (1<<CS42) | (0<<CS41) | (0<<CS40);
 TCNT4 = TCNT_INIT;
 }
