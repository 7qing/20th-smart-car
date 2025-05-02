#ifndef _image_h_
#define _image_h_

#define image_h    90//图像高度
#define image_w    140//图像宽度
#define WHITE  255
#define BLACK    0

uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row);
void compressimage(void);
void turn_to_bin(void);
void compressimage(void);
void yuansu_init();
void jump_init();
void yuansu_row_col_check();
void connect_angle_points(uint8 x0,uint8 y0,uint8 x1,uint8 y1);//连线函数，用于直角，x0，y0-底部跳变坐标   x1，y1-直角处跳变坐标   
void image_process();
void image_process_2();

typedef struct//元素标志位
{
    uint8 R_round_flag;  
    uint8 L_round_flag;
    uint8 square_flag;
    uint8 break_flag; 
    uint8 R_turning_flag;
    uint8 L_turning_flag;
    uint8 count_flag;
    uint8 round_index;
}yuansu_flag;

extern yuansu_flag  yuansu;

typedef struct//元素行元素列跳变次数
{
    uint8 top_jump;  
    uint8 bottom_jump;
    uint8 left_jump; 
    uint8 right_jump;
}yuansu_check;

extern yuansu_check  check;

typedef struct//跳变坐标
{  
    uint8 bottom_jump_x;
    uint8 bottom_jump_y;
    uint8 left_jump_x; 
    uint8 left_jump_y; 
    uint8 right_jump_x;
    uint8 right_jump_y;    
    uint8 top_jump_x;
    uint8 top_jump_y;        
}jump;

extern jump  coord;

extern uint8 bin_image[image_h][image_w];//图像数组
extern int16 err;


#endif