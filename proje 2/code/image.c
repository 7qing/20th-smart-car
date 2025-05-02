#include "zf_common_headfile.h"
#include "user_code.h"

uint8 stop_otsu_flag=0;
uint8 image_thereshold;//ͼ��ָ���ֵ
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)//�޶��˶�ͼ�����ĵײ�����ͳ�ƣ���Լ������
{
#define GrayScale 256
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale] = {0};
	
	uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // ��䷽��;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;
	
	
    for (Y = 40; Y <Image_Height; Y++) //Y<Image_Height��ΪY =Image_Height���Ա���� �ж�ֵ��
    {
        //Y=Image_Height;
        for (X = 42; X < Image_Width-42; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //ͳ��ÿ���Ҷ�ֵ�ĸ�����Ϣ
        }
    }




    for (MinValue = 0; MinValue < 255 && HistGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)
    {
        return MaxValue;          // ͼ����ֻ��һ����ɫ
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // ͼ����ֻ�ж�����ɫ
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  ��������
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //ǰ�����ص���
          PixelFore = Amount - PixelBack;         //�������ص���
          OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
          OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
          PixelIntegralBack += HistGram[Y] * Y;  //ǰ���Ҷ�ֵ
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
          MicroBack = (double)PixelIntegralBack / PixelBack;//ǰ���ҶȰٷֱ�
          MicroFore = (double)PixelIntegralFore / PixelFore;//�����ҶȰٷֱ�
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//����������䷽��g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
    if(Threshold<45){Threshold=60;}
   return Threshold;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ͼ��ѹ��
//  @return     void
//  @since      v2.0
//  Sample usage:   void compressimage();
//-------------------------------------------------------------------------------------------------------------------

uint8 Image_use_zip[image_h][image_w];
void compressimage(void)
{
  int i, j;
  for (i = 0; i < image_h; i++)
  {
    for (j = 0; j < image_w; j++)
    {
      Image_use_zip[i][j] = mt9v03x_image[i+30][j+24];
    }
  }
}

//------------------------------------------------------------------------------------------------------------------
//  @brief      ͼ���ֵ���������õ��Ǵ�򷨶�ֵ����
//  @since      v1.0 
//------------------------------------------------------------------------------------------------------------------
uint8 bin_image[image_h][image_w];//ͼ������
void turn_to_bin(void)
{
  uint8 i,j;
  static uint8 last_thereshold;
  if(stop_otsu_flag==0){image_thereshold = otsuThreshold(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);}
  else{image_thereshold=last_thereshold;}
  last_thereshold=image_thereshold ;
  for(i = 0;i<image_h;i++)
  {
      for(j = 0;j<image_w;j++)
      {
          if(Image_use_zip[i][j]>image_thereshold)bin_image[i][j] = 255;
          else bin_image[i][j] = 0;
      }
  }
}

//�������ͺ͸�ʴ����ֵ����
#define threshold_max	255*5//�˲����ɸ����Լ����������
#define threshold_min	255*2//�˲����ɸ����Լ����������
void image_filter(uint8(*bin_image)[image_w])//��̬ѧ�˲�������˵�������ͺ͸�ʴ��˼��
{
	uint16 i, j;
	uint32 num = 0;

        
	for (i = 1; i < image_h - 1; i++)
	{
		for (j = 1; j < (image_w - 1); j++)
		{
			//ͳ�ư˸����������ֵ
			num =
				bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
				+ bin_image[i][j - 1] + bin_image[i][j + 1]
				+ bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];


			if (num >= threshold_max && bin_image[i][j] == 0)
			{

				bin_image[i][j] = 255;//��  ���Ը�ɺ궨�壬�������

			}
			if (num <= threshold_min && bin_image[i][j] == 255)
			{

				bin_image[i][j] = 0;//��

			}

		}
	}

}

yuansu_flag  yuansu;
void yuansu_init()
{
    yuansu.R_round_flag=0;
    yuansu.L_round_flag=0;
    yuansu.square_flag=0;
    yuansu.break_flag=0;
    yuansu.L_turning_flag=0;
    yuansu.R_turning_flag=0;
    yuansu.count_flag=0;
    yuansu.round_index=0;
}

yuansu_check  check;
jump  coord;
void jump_init()
{
  check.bottom_jump=0;
  check.left_jump=0;
  check.right_jump=0;
  check.top_jump=0;
}

void yuansu_row_col_check()//ͨ������Ԫ������������ж�Ԫ�أ����б�дԪ���ж��봦��
{
  uint8 i,j;
  jump_init();
  for(i=1;i<6;i++)//����Ԫ��������  
  {
    check.top_jump=0;//ÿһ������ʱ�����������
    for (j=0;j<image_w-1;j++)
    {
      if(bin_image[i][j]!=bin_image[i][j+1])
      {check.top_jump++;coord.top_jump_x=j;}
    }
    coord.top_jump_y=i;
    if(check.top_jump!=2){break;}//�����������쳣����ѭ��
  }
  
  for(i=image_h-1;i>image_h-6;i--)//�ײ�Ԫ��������
  {
    check.bottom_jump=0;//ÿһ������ʱ�����������
    for (j=0;j<image_w-1;j++)
    {
      if(bin_image[i][j]!=bin_image[i][j+1])
      {check.bottom_jump++;coord.bottom_jump_x=j;}
    }
    coord.bottom_jump_y=i;
    if(check.bottom_jump!=2){break;}//��������쳣����ѭ��
  }

  for(j=1;j<6;j++)//��Ԫ��������
  {
    check.left_jump=0;//ÿһ������ʱ�����������
    for (i=image_h-1;i>1;i--)
    {
      if(bin_image[i][j]!=bin_image[i-1][j])
      {check.left_jump++;coord.left_jump_y=i;}
    }
    coord.left_jump_x=j;
    if(check.left_jump>=2){break;}//��������쳣����ѭ��
  }

  for(j=image_w-1;j>image_w-6;j--)//�Ҳ�Ԫ��������
  {
    check.right_jump=0;//ÿһ������ʱ�����������
    for (i=image_h-1;i>1;i--)
    {
      if(bin_image[i][j]!=bin_image[i-1][j])
      {check.right_jump++;coord.right_jump_y=i;}
    }
    coord.right_jump_x=j;
    if(check.right_jump>=2){break;}//��������쳣����ѭ��
  }
 
}

void element()//Ԫ�ص��жϣ�ͨ��Ԫ����Ԫ���е�����������ж�
{
  uint8 i,j;
  if(yuansu.R_round_flag==0&&yuansu.break_flag==0&&yuansu.L_round_flag==0)
  {
  if(check.top_jump>2&&check.left_jump==0&&check.right_jump>=2&&check.bottom_jump>2){yuansu.R_round_flag=1;tuoluoyi_sum=0;}
  else if(check.top_jump>2&&check.left_jump>=2&&check.right_jump==0&&check.bottom_jump>2){yuansu.L_round_flag=1;tuoluoyi_sum=0;}
  else if(check.top_jump==0&&check.left_jump==0&&check.right_jump>=2&&check.bottom_jump==2){yuansu.L_turning_flag=0;yuansu.R_turning_flag=1;yuansu.square_flag=0;}
  else if(check.top_jump==0&&check.left_jump>=2&&check.right_jump==0&&check.bottom_jump==2){yuansu.L_turning_flag=1;yuansu.R_turning_flag=0;yuansu.square_flag=0;}
  else if(check.top_jump>=2&&check.left_jump>=2&&check.right_jump>=2&&check.bottom_jump>=2){yuansu.L_turning_flag=0;yuansu.R_turning_flag=0;yuansu.square_flag=1;}
  else{yuansu_init();}
  }
//��Բ���뻷�����
  if(yuansu.R_round_flag==1&&tuoluoyi_sum<-300)  
  {
   if(check.left_jump>=2&&check.right_jump>=2){yuansu.R_round_flag=2;}
  }
  if(yuansu.R_round_flag==2)  
  {
   if(check.left_jump==0&&check.right_jump==0){yuansu.R_round_flag=0;}
  }
//��Բ���뻷�����
  if(yuansu.L_round_flag==1&&tuoluoyi_sum>300)
  {
    if(check.left_jump>=2&&check.right_jump>=2){yuansu.L_round_flag=2;}
  }
  if(yuansu.L_round_flag==2)
  {
    if(check.left_jump==0&&check.right_jump==0){yuansu.L_round_flag=0;}
  }
}

void connect_angle_points(uint8 x0,uint8 y0,uint8 x1,uint8 y1)//���ߺ�����x0��y0-�ײ���������   x1��y1-ֱ�Ǵ���������   
{      
        int16 dx = my_abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int16 dy = -my_abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
        int16 error = dx + dy;
        
        // ���������ܵĵ����������߳���2����֤���ǣ�
        int16 max_iter = 2 * (dx > -dy ? dx : -dy);
        
        // ʹ��forѭ�����while
        for(int16 i = 0; i <= max_iter; i++) 
        {
            // �����ϵ���Ч�������������
            if(y0 >= 0 && y0 < image_h && x0 >= 0 && x0 < image_w) 
            {
                center_line[y0] = x0;
            }
            
            // �����յ�����ǰ�˳�
            if(x0 == x1 && y0 == y1) break;
            
            int16 e2 = 2 * error;
            if(e2 >= dy) { error += dy; x0 += sx; }
            if(e2 <= dx) { error += dx; y0 += sy; }
        }
    
}

void image_process()
{

            
  compressimage();
   
  

  
  turn_to_bin();
  


  
  //image_filter(bin_image);//�˲�,cyt2bl3��ʵ��10ms���ң����� 
  

}

void image_process_2()
{

  yuansu_row_col_check();
  element();
  if(yuansu.square_flag)//��ʮ��ʱ�л�Ϊ������
  {neighbor_track(1);}
  else
  {neighbor_track(0);}//�������ɨ�ߣ������߼̳У�һ���̶��Ͽ�����
  if(yuansu.R_turning_flag&&coord.right_jump_y>10)//��ֱ�����ߣ��ײ�����������Ҳ������
  {connect_angle_points(coord.bottom_jump_x,coord.bottom_jump_y,coord.right_jump_x,coord.right_jump_y);}
  else if(yuansu.L_turning_flag&&coord.left_jump_y>10)//��ֱ�����ߣ��ײ�����������������
  {connect_angle_points(coord.bottom_jump_x,coord.bottom_jump_y,coord.left_jump_x,coord.left_jump_y);}
  else if(yuansu.square_flag)//ʮ�����ߣ�ͨ����������ʵ����Ҫ���ߵģ�������һ�±Ƚ��ȶ�
  {connect_angle_points(coord.bottom_jump_x,coord.bottom_jump_y,coord.top_jump_x,coord.top_jump_y);} 
  get_err();//����ȡ

  ips200_show_uint(0, 300, image_thereshold, 3);
  ips200_show_gray_image(0,0,bin_image[0],image_w, image_h,image_w, image_h, 0);
  ips200_show_int(200,200,err,3);
  
}

