#ifndef _MOTOR_H_
#define _MOTOR_H_

#define DIR1                (P00_2)
#define PWM1                (TCPWM_CH13_P00_3)
#define DIR2                (P18_6)
#define PWM2                (TCPWM_CH50_P18_7)

void motor_set_l(int16 x);
void motor_set_r(int16 x);
void motor_init();

#endif

