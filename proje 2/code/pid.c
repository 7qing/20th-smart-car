#include "zf_common_headfile.h"
#include "user_code.h"

int16 pwm_l,pwm_r;

int16 Speed_Ctl_l(int16 encoder_l,float_t p,float_t i,float_t d)//����ʽpid
{
  int16 speed_err;
  static int16 speed;
  static int16 speed_err_last1,speed_err_last2;
  static int16 speed_bb;
  speed_err = encoder_l - encoder_v_l;    //�ٶ�ƫ��=Ŀ���ٶ�-��ǰ�ٶ�
  
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
  speed_err = encoder_r - encoder_v_r;    //�ٶ�ƫ��=Ŀ���ٶ�-��ǰ�ٶ�
 
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

//int16 double_pd(int chazhi,float_t p1,float_t p2,float_t  d1,float_t d2)//˫pd���Ʋ���
//{
//  int16 out;
//  static int16 last_chazhi;
//  out=(int)(chazhi*p1+chazhi*my_abs(chazhi)*p2+(chazhi-last_chazhi)*d1+gyro_z*d2);
//  last_chazhi=chazhi;
//  return out;
//}
//
//int16 initial_speed(int chazhi,float_t p,float_t d)//��ʼ�ٶȼ��㣨ֱ������������٣�
//{
//  int16 out;
//  static int16 last_chazhi;
//  out=(int)(my_abs(chazhi)*p);
//  last_chazhi=chazhi;
//  return out;
//}


int angle(int chazhi,float_t p,float_t p2,float_t d)//�⻷
{
	int16 out_dw;
	static int16 last_chazhi;
        
        float_t dynamic_p2 = 0.0f;
    if (abs(chazhi) > 30)
    {
        // ���>30ʱ���÷��������������������ǿ
        dynamic_p2 = p2 * (abs(chazhi) - 30) / 10.0f;  // ���Թ���
        // ��ʹ��ƽ�����ɣ���������: dynamic_p2 = p2 * pow((abs(chazhi) - 30)/30.0f, 2);
    }
	out_dw=(int)(chazhi*p+chazhi*my_abs(chazhi)*dynamic_p2/900+(chazhi-last_chazhi)*d);//�⻷��ƫ��������ͷѲ�ߵ�err
	last_chazhi=chazhi;
    return out_dw;
}

int jiaosudu(int out_dw, float_t p,float_t d)//�ڻ�
{
	int16 out_dn,error;
	static int16 last_error;
	error=(out_dw-(int16)gyro_z);//�ڻ���ƫ�����⻷�����������z��Ľ��ٶ�֮��
	out_dn=(int)(error*p+(error-last_error)*d);
	last_error=error;
	return out_dn;
}

int16 nei_out;//���ٶȻ�
int16 wai_out;//�ǶȻ����
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
    wai_out=angle(err,6.0,12.0,1.5);  //�ǶȻ�,p,����p��d  
    angle_1_time=0;
  }
  if(angle_2_time==5)
  {
    nei_out=jiaosudu(wai_out,1.0,0.1);//���ٶȻ���p,d
    angle_2_time=0;
  }
  if(speed_time==5)
  {
    speed_time=0;
    speed_l=Speed_Ctl_l(50-nei_out,37.0,1.5,0.1);//110�ǻ����ٶȣ������Լ�������������������
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
       