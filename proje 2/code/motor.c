#include "zf_common_headfile.h"
#include "motor.h"

void motor_set_l(int16 x)
{
  if(x>=0)
  {
    gpio_set_level(DIR1, GPIO_HIGH);                   // DIR����͵�ƽ
    pwm_set_duty(PWM1, x );                 // ����ռ�ձ�
  }
  else
  {
    gpio_set_level(DIR1, GPIO_LOW);             // DIR����ߵ�ƽ
    pwm_set_duty(PWM1, (-x));                    // ����ռ�ձ�
  }
}

void motor_set_r(int16 x)
{
  if(x>=0)
  {
     gpio_set_level(DIR2, GPIO_HIGH);                  // DIR����͵�ƽ
     pwm_set_duty(PWM2, x);                 // ����ռ�ձ�
  }
  else
  {
    gpio_set_level(DIR2, GPIO_LOW);             // DIR����ߵ�ƽ
    pwm_set_duty(PWM2, (-x));                    // ����ռ�ձ�
  }
}

void motor_init()
{
  gpio_init(DIR1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // GPIO ��ʼ��Ϊ��� Ĭ�����������
  gpio_init(DIR2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // GPIO ��ʼ��Ϊ��� Ĭ�����������
  pwm_init(PWM1, 17000, 0);                                                   // PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
  pwm_init(PWM2, 17000, 0);                                                   // PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0 
}