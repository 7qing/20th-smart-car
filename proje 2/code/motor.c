#include "zf_common_headfile.h"
#include "motor.h"

void motor_set_l(int16 x)
{
  if(x>=0)
  {
    gpio_set_level(DIR1, GPIO_HIGH);                   // DIR输出低电平
    pwm_set_duty(PWM1, x );                 // 计算占空比
  }
  else
  {
    gpio_set_level(DIR1, GPIO_LOW);             // DIR输出高电平
    pwm_set_duty(PWM1, (-x));                    // 计算占空比
  }
}

void motor_set_r(int16 x)
{
  if(x>=0)
  {
     gpio_set_level(DIR2, GPIO_HIGH);                  // DIR输出低电平
     pwm_set_duty(PWM2, x);                 // 计算占空比
  }
  else
  {
    gpio_set_level(DIR2, GPIO_LOW);             // DIR输出高电平
    pwm_set_duty(PWM2, (-x));                    // 计算占空比
  }
}

void motor_init()
{
  gpio_init(DIR1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // GPIO 初始化为输出 默认上拉输出高
  gpio_init(DIR2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // GPIO 初始化为输出 默认上拉输出高
  pwm_init(PWM1, 17000, 0);                                                   // PWM 通道初始化频率 17KHz 占空比初始为 0
  pwm_init(PWM2, 17000, 0);                                                   // PWM 通道初始化频率 17KHz 占空比初始为 0 
}