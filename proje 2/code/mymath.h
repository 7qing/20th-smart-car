#ifndef _MYMATH_H_
#define _MYMATH_H_

int my_abs(int16 value);
float Slope_Calculate(uint8 begin, uint8 end, uint8 *border);
void calculate_s_i(uint8 start, uint8 end, uint8 *border, float *slope_rate, float *intercept);
void ips_show_round(uint8 x,uint8 y);
float k_get(uint8 y1,uint8 y2,uint8 *border);
void connect_point(int temp[], int x1, int x2, int y1, int y2);//x为行数，y为列数
void yanshen_to_up(int temp[], int x1, int x2, int y1, int y2);//x为行数，y为列数
void yanshen_to_down(int temp[], int x1, int x2, int y1, int y2);//x为行数，y为列数
int16 moving_average_filter(int16 *buffer, uint8 size, int16 new_value);

#define ERR_SIZE 5
extern int16 history_err[ERR_SIZE];//历史误差

#endif