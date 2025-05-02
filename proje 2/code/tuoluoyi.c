#include "zf_common_headfile.h"
#include "user_code.h"


float_t tuoluoyi_sum=0;
uint16 tuoluoyi_time=0;
float_t gyro_z;

void gyro_z_get()
{
  imu660ra_get_acc();
  imu660ra_get_gyro();
}

void tuoluoyi_jifen()   
{  
  static float_t last_gyro_z;
  gyro_z_get();
  gyro_z=imu660ra_gyro_transition(imu660ra_gyro_z);
  gyro_z=0.4*gyro_z+0.6*last_gyro_z;
  tuoluoyi_sum+=(int16)gyro_z*0.00102;
  last_gyro_z=gyro_z;
}
             