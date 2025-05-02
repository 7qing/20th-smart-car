#include "zf_common_headfile.h"
#include "user_code.h"

uint8 stop_otsu_flag=0;
uint8 image_thereshold;//图像分割阈值
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)//限定了对图像中心底部进行统计，节约运算量
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
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;
	
	
    for (Y = 40; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 42; X < Image_Width-42; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }




    for (MinValue = 0; MinValue < 255 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
    if(Threshold<45){Threshold=60;}
   return Threshold;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      图像压缩
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
//  @brief      图像二值化，这里用的是大津法二值化。
//  @since      v1.0 
//------------------------------------------------------------------------------------------------------------------
uint8 bin_image[image_h][image_w];//图像数组
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

//定义膨胀和腐蚀的阈值区间
#define threshold_max	255*5//此参数可根据自己的需求调节
#define threshold_min	255*2//此参数可根据自己的需求调节
void image_filter(uint8(*bin_image)[image_w])//形态学滤波，简单来说就是膨胀和腐蚀的思想
{
	uint16 i, j;
	uint32 num = 0;

        
	for (i = 1; i < image_h - 1; i++)
	{
		for (j = 1; j < (image_w - 1); j++)
		{
			//统计八个方向的像素值
			num =
				bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
				+ bin_image[i][j - 1] + bin_image[i][j + 1]
				+ bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];


			if (num >= threshold_max && bin_image[i][j] == 0)
			{

				bin_image[i][j] = 255;//白  可以搞成宏定义，方便更改

			}
			if (num <= threshold_min && bin_image[i][j] == 255)
			{

				bin_image[i][j] = 0;//黑

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

void yuansu_row_col_check()//通过行列元素跳变次数来判断元素，自行编写元素判断与处理
{
  uint8 i,j;
  jump_init();
  for(i=1;i<6;i++)//顶部元素行搜索  
  {
    check.top_jump=0;//每一行搜索时跳变次数重置
    for (j=0;j<image_w-1;j++)
    {
      if(bin_image[i][j]!=bin_image[i][j+1])
      {check.top_jump++;coord.top_jump_x=j;}
    }
    coord.top_jump_y=i;
    if(check.top_jump!=2){break;}//如果跳变次数异常跳出循环
  }
  
  for(i=image_h-1;i>image_h-6;i--)//底部元素行搜索
  {
    check.bottom_jump=0;//每一行搜索时跳变次数重置
    for (j=0;j<image_w-1;j++)
    {
      if(bin_image[i][j]!=bin_image[i][j+1])
      {check.bottom_jump++;coord.bottom_jump_x=j;}
    }
    coord.bottom_jump_y=i;
    if(check.bottom_jump!=2){break;}//跳变次数异常跳出循环
  }

  for(j=1;j<6;j++)//左部元素列搜索
  {
    check.left_jump=0;//每一列搜索时跳变次数重置
    for (i=image_h-1;i>1;i--)
    {
      if(bin_image[i][j]!=bin_image[i-1][j])
      {check.left_jump++;coord.left_jump_y=i;}
    }
    coord.left_jump_x=j;
    if(check.left_jump>=2){break;}//跳变次数异常跳出循环
  }

  for(j=image_w-1;j>image_w-6;j--)//右部元素列搜索
  {
    check.right_jump=0;//每一列搜索时跳变次数重置
    for (i=image_h-1;i>1;i--)
    {
      if(bin_image[i][j]!=bin_image[i-1][j])
      {check.right_jump++;coord.right_jump_y=i;}
    }
    coord.right_jump_x=j;
    if(check.right_jump>=2){break;}//跳变次数异常跳出循环
  }
 
}

void element()//元素的判断，通过元素行元素列的跳变次数来判断
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
//右圆环入环与出环
  if(yuansu.R_round_flag==1&&tuoluoyi_sum<-300)  
  {
   if(check.left_jump>=2&&check.right_jump>=2){yuansu.R_round_flag=2;}
  }
  if(yuansu.R_round_flag==2)  
  {
   if(check.left_jump==0&&check.right_jump==0){yuansu.R_round_flag=0;}
  }
//左圆环入环与出环
  if(yuansu.L_round_flag==1&&tuoluoyi_sum>300)
  {
    if(check.left_jump>=2&&check.right_jump>=2){yuansu.L_round_flag=2;}
  }
  if(yuansu.L_round_flag==2)
  {
    if(check.left_jump==0&&check.right_jump==0){yuansu.L_round_flag=0;}
  }
}

void connect_angle_points(uint8 x0,uint8 y0,uint8 x1,uint8 y1)//连线函数，x0，y0-底部跳变坐标   x1，y1-直角处跳变坐标   
{      
        int16 dx = my_abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int16 dy = -my_abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
        int16 error = dx + dy;
        
        // 计算最大可能的迭代次数（线长的2倍保证覆盖）
        int16 max_iter = 2 * (dx > -dy ? dx : -dy);
        
        // 使用for循环替代while
        for(int16 i = 0; i <= max_iter; i++) 
        {
            // 将线上的有效点存入中线数组
            if(y0 >= 0 && y0 < image_h && x0 >= 0 && x0 < image_w) 
            {
                center_line[y0] = x0;
            }
            
            // 到达终点则提前退出
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
  


  
  //image_filter(bin_image);//滤波,cyt2bl3上实测10ms左右，慎用 
  

}

void image_process_2()
{

  yuansu_row_col_check();
  element();
  if(yuansu.square_flag)//当十字时切换为八邻域
  {neighbor_track(1);}
  else
  {neighbor_track(0);}//其余情况扫线，有中线继承，一定程度上抗虚线
  if(yuansu.R_turning_flag&&coord.right_jump_y>10)//右直角拉线，底部跳变点拉到右部跳变点
  {connect_angle_points(coord.bottom_jump_x,coord.bottom_jump_y,coord.right_jump_x,coord.right_jump_y);}
  else if(yuansu.L_turning_flag&&coord.left_jump_y>10)//左直角拉线，底部跳变点拉到左部跳变点
  {connect_angle_points(coord.bottom_jump_x,coord.bottom_jump_y,coord.left_jump_x,coord.left_jump_y);}
  else if(yuansu.square_flag)//十字拉线，通过八邻域其实不需要拉线的，但是拉一下比较稳定
  {connect_angle_points(coord.bottom_jump_x,coord.bottom_jump_y,coord.top_jump_x,coord.top_jump_y);} 
  get_err();//误差获取

  ips200_show_uint(0, 300, image_thereshold, 3);
  ips200_show_gray_image(0,0,bin_image[0],image_w, image_h,image_w, image_h, 0);
  ips200_show_int(200,200,err,3);
  
}

