#ifndef _PID_H_
#define _PID_H_

#define speed_l_p                       40.0       
#define speed_l_i                       1.2
#define speed_l_d                       0.1 

#define speed_r_p                       50.0
#define speed_r_i                       2.0         
#define speed_r_d                       0.1


int16 Speed_Ctl_l(int16 encoder_l,float_t p,float_t i,float_t d);
int16 Speed_Ctl_r(int16 encoder_r,float_t p,float_t i,float_t d);
int16 double_pd(int chazhi,float_t p1,float_t p2,float_t  d1,float_t d2);
int16 initial_speed(int chazhi,float_t p,float_t d);//初始速度计算（直道加速弯道减速）
int angle(int chazhi,float_t p,float_t p2,float_t d);//外环
int jiaosudu(int out_dw, float_t p,float_t d);
void pid();
void turn_left(float_t x , int16 v_l,int16 v_r);
void turn_right(float_t x , int16 v_l,int16 v_r);
void pid_proc();

#endif

