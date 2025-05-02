#include "zf_common_headfile.h"
#include "user_code.h"

int16 pwm_l,pwm_r;

int16 Speed_Ctl_l(int16 encoder_l,float_t p,float_t i,float_t d)//增量式pid
{
  int16 speed_err;
  static int16 speed;
  static int16 speed_err_last1,speed_err_last2;
  static int16 speed_bb;
  speed_err = encoder_l - encoder_v_l;    //速度偏差=目标速度-当前速度
  
//  if(speed_err>80) speed_bb=80;
//  else if(speed_err<-80) speed_bb=-80;
//  else speed_bb=0;
  /**********************************/
  speed+=  p*(speed_err-speed_err_last1)
                +i* speed_err
                +d*(speed_err-2*speed_err_last1+speed_err_last2)
                +speed_bb;
  
  speed_err_last1=speed_err;
  speed_err_last2=speed_err_last1;
  
  if(speed> 8000) speed= 8000;
  if(speed<-8000) speed=-8000;
  
  return speed;
}

int16 Speed_Ctl_r(int16 encoder_r,float_t p,float_t i,float_t d)
{
  int16 speed_err;
  static int16 speed;
  static int16 speed_err_last1,speed_err_last2;
  static int16 speed_bb;
  speed_err = encoder_r - encoder_v_r;    //速度偏差=目标速度-当前速度
 
//  if(speed_err>80) speed_bb=80;
//  else if(speed_err<-80) speed_bb=-80;
//  else speed_bb=0;
  /**********************************/
  speed+=  p*(speed_err-speed_err_last1)
                +i* speed_err
                +d*(speed_err-2*speed_err_last1+speed_err_last2)
                +speed_bb;
  
  speed_err_last1=speed_err;
  speed_err_last2=speed_err_last1;
  
  if(speed> 8000) speed= 8000;
  if(speed<-8000) speed= -8000;
  
  return speed;
}

//int16 double_pd(int chazhi,float_t p1,float_t p2,float_t  d1,float_t d2)//双pd控制差速
//{
//  int16 out;
//  static int16 last_chazhi;
//  out=(int)(chazhi*p1+chazhi*my_abs(chazhi)*p2+(chazhi-last_chazhi)*d1+gyro_z*d2);
//  last_chazhi=chazhi;
//  return out;
//}
//
//int16 initial_speed(int chazhi,float_t p,float_t d)//初始速度计算（直道加速弯道减速）
//{
//  int16 out;
//  static int16 last_chazhi;
//  out=(int)(my_abs(chazhi)*p);
//  last_chazhi=chazhi;
//  return out;
//}


int angle(int chazhi,float_t p,float_t p2,float_t d)//外环
{
	int16 out_dw;
	static int16 last_chazhi;
        
        float_t dynamic_p2 = 0.0f;
    if (abs(chazhi) > 30)
    {
        // 误差>30时启用非线性项，并随误差增大而增强
        dynamic_p2 = p2 * (abs(chazhi) - 30) / 10.0f;  // 线性过渡
        // 或使用平方过渡（更激进）: dynamic_p2 = p2 * pow((abs(chazhi) - 30)/30.0f, 2);
    }
	out_dw=(int)(chazhi*p+chazhi*my_abs(chazhi)*dynamic_p2/900+(chazhi-last_chazhi)*d);//外环的偏差是摄像头巡线的err
	last_chazhi=chazhi;
    return out_dw;
}

int jiaosudu(int out_dw, float_t p,float_t d)//内环
{
	int16 out_dn,error;
	static int16 last_error;
	error=(out_dw-(int16)gyro_z);//内环的偏差是外环输出与陀螺仪z轴的角速度之差
	out_dn=(int)(error*p+(error-last_error)*d);
	last_error=error;
	return out_dn;
}

int16 nei_out;//角速度环
int16 wai_out;//角度环输出
int16 speed;
int16 speed_l;
int16 speed_r;
int16 target=0;
uint16 target_time;
void pid()
{
  speed_time++;
  angle_1_time++;
  angle_2_time++;
  if(angle_1_time==10)
  {
    wai_out=angle(err,6.0,12.0,1.5);  //角度环,p,二次p，d  
    angle_1_time=0;
  }
  if(angle_2_time==5)
  {
    nei_out=jiaosudu(wai_out,1.0,0.1);//角速度环，p,d
    angle_2_time=0;
  }
  if(speed_time==5)
  {
    speed_time=0;
    speed_l=Speed_Ctl_l(50-nei_out,37.0,1.5,0.1);//110是基础速度，随你自己调整，慢慢加来提速
    speed_r=Speed_Ctl_r(50+nei_out,35.0,1.3,0.1);
    
  }
}

int16 duty_l;
int16 duty_r;
uint8 time=0;
uint16 two_s=0;
uint16 time_200=0;
int16 duty;

void pid_proc()
{
  if(++time==5)
  {  
  time=0;
  motor_set_l(speed_l);
  motor_set_r(speed_r);
  
  }
}
       