#include "zf_common_headfile.h"
#include "user_code.h"

int16 encoder_sum_r=0;
int16 encoder_sum_l=0;
int32 encoder_sum=0;
int16 encoder_v_r=0;
int16 encoder_v_l=0;
int16 v_r=0;
int16 v_l=0;
int16 encoder_v=0;
int16 encoder_data_dir[2] = {0};

void encoder_jifen()
{
    static int16 last_r,last_l;
    
    encoder_data_dir[0] = -(encoder_get_count(ENCODER1));
    encoder_clear_count(ENCODER1);
    v_r=(int16)(0.4*encoder_data_dir[0]+0.6*last_r);
    last_r=encoder_data_dir[0];
    
    encoder_data_dir[1] = encoder_get_count(ENCODER2);
    encoder_clear_count(ENCODER2);
    v_l=(int16)(0.4*encoder_data_dir[1]+0.6*last_l);
    last_l=encoder_data_dir[1];
    
    encoder_sum_r+=v_r ;    
    encoder_sum_l+=v_l ;
}

uint16 time_10s;

void v_get()
{  
    encoder_jifen();
      if(++v_time==5)
    {
      v_time=0;
      encoder_sum+=(v_r+v_l)*0.5;
      encoder_v_r=encoder_sum_r;
      encoder_sum_r=0;
      encoder_v_l=encoder_sum_l;
      encoder_sum_l=0;
    }
    encoder_v=(encoder_v_l+encoder_v_r)/2;
    if(++time_10s<2000) {encoder_sum=0;}
    if(time_10s>2001){time_10s=2001;}
}




             