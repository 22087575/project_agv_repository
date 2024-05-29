#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED

#include <avr/io.h>


typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float previous_error;
    float integral;
    float setpoint;
    float output;
    float output_min;
    float output_max;
} PIDController;

void PIDController_Init(PIDController* pid, float Kp, float Ki, float Kd, float output_min, float output_max);
float PIDController_Update(PIDController* pid, float measured_value);





#endif // PID_H_INCLUDED
