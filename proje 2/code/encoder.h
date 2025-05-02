#ifndef _ENCODER_H_
#define _ENCODER_H_

#define ENCODER1                     	(TC_CH09_ENCODER)                      // �������ӿ�       
#define ENCODER1_QUADDEC_A             	(TC_CH09_ENCODER_CH1_P05_0)            // A����                      
#define ENCODER1_QUADDEC_B            	(TC_CH09_ENCODER_CH2_P05_1)            // B����                        
                                                                                
#define ENCODER2                     	(TC_CH07_ENCODER)                      // �������ӿ�   
#define ENCODER2_QUADDEC_A            	(TC_CH07_ENCODER_CH1_P02_0)            // A����                  
#define ENCODER2_QUADDEC_B            	(TC_CH07_ENCODER_CH2_P02_1)            // B����          

#define  speed_size         (20)

int16 smooth_speed_real(int16 current_speed,int16 *history_speed,int size);
void encoder_jifen();
void v_get();

extern int16 encoder_data_dir[2];
extern int16 v_r;
extern int16 v_l;
extern int16 encoder_sum_r;
extern int16 encoder_sum_l;
extern int32 encoder_sum;
extern int16 encoder_v_r;
extern int16 encoder_v_l;
extern int16 encoder_v;

#endif

