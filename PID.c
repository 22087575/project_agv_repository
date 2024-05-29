#include <avr/io.h>
#include "PID.h"

void PIDController_Init(PIDController* pid, float Kp, float Ki, float Kd, float output_min, float output_max) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->previous_error = 0;
    pid->integral = 0;
    pid->setpoint = 0;
    pid->output_min = output_min;
    pid->output_max = output_max;
    pid->output = 0;
}

float PIDController_Update(PIDController* pid, float measured_value) {
    float error = pid->setpoint - measured_value;
    pid->integral += error;
    float derivative = error - pid->previous_error;
    pid->output = (pid->Kp * error) + (pid->Ki * pid->integral) + (pid->Kd * derivative);

    // Clamp output to min/max
    if (pid->output > pid->output_max) {
        pid->output = pid->output_max;
    } else if (pid->output < pid->output_min) {
        pid->output = pid->output_min;
    }

    pid->previous_error = error;

    return pid->output;
}
