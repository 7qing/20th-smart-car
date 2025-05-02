#include "zf_common_headfile.h"
#include "user_code.h"

uint8 v_time;
uint8 Time_5ms;
uint8 angle_1_time;
uint8 angle_2_time;
uint8 speed_time;
uint8 motor_time;

void time_para_init()
{
  v_time=0;
  Time_5ms=0;
  angle_1_time=0;
  angle_2_time=0;
  speed_time=0;
  motor_time=0;
}