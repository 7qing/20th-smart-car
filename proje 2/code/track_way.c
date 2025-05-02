#include "zf_common_headfile.h"
#include "user_code.h"


uint8   mid_weight[90]={
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数81-90行                         
                        6 ,8 ,9 ,10 ,12 ,12 ,14 ,17 ,20 ,20 , //倒数71-80行
                        23 ,25 ,23 ,23 ,21 ,20 ,20 ,18 ,18 ,15 , //倒数61-70行                        
                        8 ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数51-60行                      
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数41-50行
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数31-40行
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数21-30行
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数11-20行
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 };//倒数1-10行

uint8   mid_weight_zhijiao[90]={
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数81-90行                         
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数71-80行
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数61-70行                        
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数51-60行                      
                        1 ,1 ,2 ,5 ,8 ,10 ,13 ,15 ,16 ,19 , //倒数41-50行
                        25 ,25 ,22 ,20 ,17 ,16 ,15 ,15 ,13 ,10 , //倒数31-40行
                        8 ,6 ,5 ,2 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数21-30行
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //倒数11-20行
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 };//倒数1-10行

/*
函数名称：void get_start_point(uint8 start_row)
功能说明：寻找两个边界的边界点作为八邻域循环的起始点
参数说明：输入任意行数
函数返回：无
修改时间：2022年9月8日
备    注：
example：  get_start_point(image_h-2)
 */
uint8 start_point_l[2] = { 0 };//左边起点的x，y值
uint8 start_point_r[2] = { 0 };//右边起点的x，y值\

uint8 border_max= image_w-1;
uint8 border_min= 0;
uint8 l_found = 0,r_found = 0;

uint8 get_start_point(uint8 start_row)
{
	uint8 i = 0;
	//清零
	start_point_l[0] = 0;//x
	start_point_l[1] = 0;//y
        l_found=0;

	start_point_r[0] = 0;//x
	start_point_r[1] = 0;//y
        r_found=0;
		//从中间往左边，先找起点
	for (i = image_w / 2; i > border_min; i--)
	{
		start_point_l[0] = i;//x
		start_point_l[1] = start_row;//y
		if (bin_image[start_row][i] == 255 && bin_image[start_row][i - 1] == 0)
		{
			//printf("找到左边起点image[%d][%d]\n", start_row,i);
			l_found = 1;
			break;
		}
	}
        
        if(l_found==0)
        {
        for (i = image_w/2; i < image_w-1; i++)
	{
		start_point_l[0] = i;//x
		start_point_l[1] = start_row;//y
		if (bin_image[start_row][i] == 255 && bin_image[start_row][i - 1] == 0)
		{
			//printf("找到左边起点image[%d][%d]\n", start_row,i);
			l_found = 1;
			break;
		}
	}
        } 
             
        for (i = image_w / 2; i < border_max-1; i++)
	{
		start_point_r[0] = i;//x
		start_point_r[1] = start_row;//y
		if (bin_image[start_row][i] == 255 && bin_image[start_row][i + 1] == 0)
		{
			//printf("找到右边起点image[%d][%d]\n",start_row, i);
			r_found = 1;
			break;
		}
	}
        
        if(r_found==0)
        {
        for (i = image_w/2; i > 1; i--)
	{
		start_point_r[0] = i;//x
		start_point_r[1] = start_row;//y
		if (bin_image[start_row][i] == 255 && bin_image[start_row][i + 1] == 0)
		{
			//printf("找右起点image[%d][%d]\n", start_row,i);
			r_found = 1;
			break;
		}
	}
        }
//        if(start_point_r[0]-start_point_l[0]>30||start_point_r[0]-start_point_l[0]<0)//如果两边起点相差过大或者找错，清零
//        {	
////        start_point_l[0] = 0;//x
////	start_point_l[1] = 0;//y
////        l_found=0;
////
////	start_point_r[0] = 0;//x
////	start_point_r[1] = 0;//y
////        r_found=0;
//        return 0;
//        }

	if(l_found&&r_found)return 1;
	else {
		//printf("未找到起点\n");
		return 0;
	} 
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      给图像画黑框为八邻域做准备
//  @return     void
//  @since      v1.0
//  Sample usage:   image_draw_rectan(Image_use);
//-------------------------------------------------------------------------------------------------------------------
void image_draw_rectan(uint8(*image)[image_w])
{
    uint8 i = 0;
    for (i = 0; i < image_h; i++)
    {
        image[i][0] = 0;
        image[i][1] = 0;
        image[i][image_w - 1] = 0;
        image[i][image_w - 2] = 0;
    }
    for (i = 0; i < image_w; i++)
    {
        image[0][i] = 0;
        image[1][i] = 0;
    }
}

/*---------------------------------------------------------------
 【函    数】search_neighborhood
 【功    能】八邻域找边界
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
struct LEFT_EDGE
{
    int16 row;  //行坐标
    int16 col;  //列坐标
    uint8 flag; //存在边界的标志
    uint8 dir;
};
struct RIGHT_EDGE
{
    int16 row;  //行坐标
    int16 col;  //列坐标
    uint8 flag; //存在边界的标志
    uint8 dir;
};

struct LEFT_EDGE  L_edge[search_count];     //左边界结构体
struct RIGHT_EDGE R_edge[search_count];    //右边界结构体
uint8 L_edge_count=0, R_edge_count = 0;                     //左右边点的个数
uint8 dire_left,dire_right;                                 //记录上一个点的相对位置
uint8 L_search_amount = search_count, R_search_amount = search_count;  //左右边界搜点时最多允许的点
void search_neighborhood(void)
{
    L_edge_count = 0;//左边点个数清0
    R_edge_count = 0;//右边点个数清0
    
    if(l_found)//如果左边界点存在并找到,则开始爬线
    {
        //变量声明
        L_edge[0].row = start_point_l[1];
        L_edge[0].col = start_point_l[0];
        L_edge[0].flag = 1;
        int16 curr_row = start_point_l[1];//初始化行坐标
        int16 curr_col = start_point_l[0];//初始化列坐标
        dire_left = 0; //初始化上个边界点的来向
        //开始搜线，最多取150个点，不会往下搜，共7个方位
        for(int i = 1;i < L_search_amount; i++)    //最多搜索150个点
        {
            ////越界退出 行越界和列越界（向上向下向左向右）
            if(curr_row+1 < 1 || curr_row>image_h-1)  break;
            //搜线过程
            if(dire_left != 2&&bin_image[curr_row-1][curr_col-1]==BLACK&&bin_image[curr_row-1][curr_col]==WHITE)   //左上黑，2，右边白
            {
                curr_row = curr_row -1;
                curr_col = curr_col -1;
                L_edge_count = L_edge_count +1;
                dire_left = 7;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
                L_edge[i-1].dir=7;
            }
            else if(dire_left!=3&&bin_image[curr_row-1][curr_col+1]==BLACK&&bin_image[curr_row][curr_col+1]==WHITE)    //右上黑，3，下边白
            {
                curr_row = curr_row -1;
                curr_col = curr_col + 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 6;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
                L_edge[i-1].dir=6;
            }
            else if(bin_image[curr_row-1][curr_col]==BLACK&&bin_image[curr_row-1][curr_col+1]==WHITE)                  //正上黑，1，右白
            {
                curr_row = curr_row - 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 0;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
                L_edge[i-1].dir=0;
            }
            else if(dire_left!=5&&bin_image[curr_row][curr_col-1]==BLACK&&bin_image[curr_row-1][curr_col-1]==WHITE)     //正左黑，5，上白
            {
                curr_col = curr_col - 1;
                L_edge_count = L_edge_count +1;
                dire_left = 4;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
                L_edge[i-1].dir=4;
            }
            else if(dire_left!=4&&bin_image[curr_row][curr_col+1]==BLACK&&bin_image[curr_row+1][curr_col+1]==WHITE)  //正右黑，4，下白
            {
                curr_col = curr_col + 1;
                L_edge_count = L_edge_count +1;
                dire_left = 5;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
                L_edge[i-1].dir=5;
            }
            else if(dire_left!=6&&bin_image[curr_row+1][curr_col-1]==BLACK&&bin_image[curr_row][curr_col-1]==WHITE)    //左下黑，6，上白
            {
                curr_row = curr_row + 1;
                curr_col = curr_col -1;
                L_edge_count = L_edge_count +1;
                dire_left = 3;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
                L_edge[i-1].dir=3;
            }
            else if(dire_left!=7&&bin_image[curr_row+1][curr_col+1]==BLACK&&bin_image[curr_row+1][curr_col]==WHITE)    //右下黑，7，左白
            {
                curr_row = curr_row + 1;
                curr_col = curr_col + 1;
                L_edge_count = L_edge_count +1;
                dire_left = 2;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
                L_edge[i-1].dir=2;
            }
            else
                break;
        }
    }

    if(r_found)//如果右边界存在并搜到
             {
                 R_edge[0].row = start_point_r[1];
                 R_edge[0].col = start_point_r[0];
                 R_edge[0].flag = 1;
                 int16 curr_row = start_point_r[1];
                 int16 curr_col = start_point_r[0];
                 dire_right = 0;
                 for(int i = 1;i<R_search_amount;i++)
                 {
		////越界退出 行越界和列越界（向上向下向左向右）
                     if(curr_row < 1 || curr_row>image_h-1)  break;
                     //爬线过程
                     if(curr_col<image_w&&dire_right!=3&&bin_image[curr_row-1][curr_col+1]==BLACK&&bin_image[curr_row-1][curr_col]==WHITE)    //右上黑，3，左白
                     {
                         curr_row = curr_row - 1;
                         curr_col = curr_col + 1;
                         R_edge_count = R_edge_count + 1;
                         dire_right = 6;
                         R_edge[i].row = curr_row;
                         R_edge[i].col = curr_col;
                         R_edge[i].flag = 1;
                         R_edge[i-1].dir=6;
                     }
                     else if(dire_right!=2&&bin_image[curr_row-1][curr_col-1]==BLACK&&bin_image[curr_row][curr_col-1]==WHITE) //左上黑，2，下白
                     {
                         curr_row = curr_row-1;
                         curr_col = curr_col-1;
                         R_edge_count = R_edge_count + 1;
                         dire_right = 7;
                         R_edge[i].row = curr_row;
                         R_edge[i].col = curr_col;
                         R_edge[i].flag = 1;
                         R_edge[i-1].dir=7;
                     }
                     else if(bin_image[curr_row-1][curr_col]==BLACK&&bin_image[curr_row-1][curr_col-1]==WHITE)                  //正上黑，1，左白
                     {
                         curr_row = curr_row - 1;
                         R_edge_count = R_edge_count + 1;
                         dire_right = 0;
                         R_edge[i].row = curr_row;
                         R_edge[i].col = curr_col;
                         R_edge[i].flag = 1;
                         R_edge[i-1].dir=0;
                     }
                     else if(dire_right!=4&&bin_image[curr_row][curr_col+1]==BLACK&&bin_image[curr_row-1][curr_col+1]==WHITE)   //正右黑，4，上白
                     {
                         curr_col = curr_col + 1;
                         R_edge_count = R_edge_count + 1;
                         dire_right = 5;
                         R_edge[i].row = curr_row;
                         R_edge[i].col = curr_col;
                         R_edge[i].flag = 1;
                         R_edge[i-1].dir=5;
                     }
                     else if(dire_right!=5&&bin_image[curr_row][curr_col-1]==BLACK&&bin_image[curr_row+1][curr_col-1]==WHITE)   //正左黑，5，下白
                     {
                         curr_col = curr_col-1;
                         R_edge_count = R_edge_count + 1;
                         dire_right = 4;
                         R_edge[i].row = curr_row;
                         R_edge[i].col = curr_col;
                         R_edge[i].flag = 1;
                         R_edge[i-1].dir=4;
                     }


                     else if(dire_right!=6&&bin_image[curr_row+1][curr_col-1]==BLACK&&bin_image[curr_row+1][curr_col]==WHITE)   //左下黑，6，右白
                     {
                         curr_row = curr_row + 1;
                         curr_col = curr_col - 1;
                         R_edge_count = R_edge_count + 1;
                         dire_right = 3;
                         R_edge[i].row = curr_row;
                         R_edge[i].col = curr_col;
                         R_edge[i].flag = 1;
                         R_edge[i-1].dir=3;
                     }
                     else if(dire_right!=7&&bin_image[curr_row+1][curr_col+1]==BLACK&&bin_image[curr_row][curr_col+1]==WHITE)   //右下黑，7，上白
                     {
                         curr_row = curr_row + 1;
                         curr_col = curr_col + 1;
                         R_edge_count = R_edge_count + 1;
                         dire_right = 2;
                         R_edge[i].row = curr_row;
                         R_edge[i].col = curr_col;
                         R_edge[i].flag = 1;
                         R_edge[i-1].dir=2;
                     }
                     else
                         break;
                 }
             }
}
/*---------------------------------------------------------------
 【函    数】clear_find_point
 【功    能】八邻域边界初始化
 【参    数】无
 【返 回 值】
 【注意事项】
 ----------------------------------------------------------------*/
void clear_find_point(void)
{
    for(int i = 0;i<L_edge_count;i++)
    {
        L_edge[i].row = 0;
        L_edge[i].col = 0;
        L_edge[i].flag = 0;
    }
    for(int i = 0;i<R_edge_count;i++)
    {
        R_edge[i].row = 0;
        R_edge[i].col = 0;
        R_edge[i].flag = 0;
    }
}

uint8 l_border[image_h];//左线数组
uint8 r_border[image_h];//右线数组
uint8 center_line[image_h];//中线数组
void get_left()
{
	uint8 i = 0;
	uint16 j = 0;
	uint8 h = 0;
	//初始化
	for (i = 0;i<image_h;i++)
	{
		l_border[i] = border_min;
	}
	h = image_h - 2;
	//左边
	for (j = 0; j < search_count; j++)
	{
		//printf("%d\n", j);
		if (L_edge[j].row == h)
		{
			l_border[h] = L_edge[j].col+1;
		}
		else continue; //每行只取一个点，没到下一行就不记录
		h--;
		if (h == 0) 
		{
			break;//到最后一行退出
		}
	}
}

/*
函数名称：void get_right(uint16 total_R)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_R  ：找到的点的总数
函数返回：无
修改时间：2022年9月25日
备    注：
example：get_right(data_stastics_r);
 */
void get_right()
{
	uint8 i = 0;
	uint16 j = 0;
	uint8 h = 0;
	for (i = 0; i < image_h; i++)
	{
		r_border[i] = border_max;//右边线初始化放到最右边，左边线放到最左边，这样八邻域闭合区域外的中线就会在中间，不会干扰得到的数据
	}
	h = image_h - 2;
	//右边
	for (j = 0; j < search_count; j++)
	{
		if (R_edge[j].row == h)
		{
			r_border[h] = R_edge[j].col - 1;
		}
		else continue;//每行只取一个点，没到下一行就不记录
		h--;
		if (h == 0)break;//到最后一行退出
	}
}

void get_mid()
{
	uint8 i = 0;
	for (i = 0; i < image_h; i++)
	{
		center_line[i] = (r_border[i]+l_border[i])/2;//右边线初始化放到最右边，左边线放到最左边，这样八邻域闭合区域外的中线就会在中间，不会干扰得到的数据
	}
}

unsigned char in_flag = 0;
unsigned char zuoyou_xunxian(uint8(*bin_image)[image_w]) // 从中间往两边搜索中线
{
    int i, j;
    static int left_border = 0, right_border = image_w-1, mid = image_w / 2;
    
    static int last_left_border = 0, last_right_border = image_w-1;
    

    // 第一次巡线，近大远小，近处按旧扫描方式
    for (i = image_h - 1; i >= image_h - 5; i--) // 从最底下往上扫描
    {
            // 往右扫描
            for (j = 0; j < image_w - 2; j++)
            {
                if (bin_image[i][j] == 0 && bin_image[i][j + 1] == 255 )
                {
                    left_border = j;
                    break; // 跳出，找到本行边界就没必要循环下去了
                }
            }

            // 没找到右边界的处理
            if (j == image_w - 3)
            {
                left_border = last_left_border;
            }

            // 往左边扫描
            for (j = image_w-1; j > 1; j--)
            {
                if (bin_image[i][j] == 0 && bin_image[i][j - 1] == 255 && bin_image[i][j - 2] == 255) // 黑白认为到达左边界
                {
                    right_border = j;
                    break; // 跳出，找到本行边界就没必要循环下去了
                }
            }

            // 没找到右边界的处理
            if (j == 2)
            {
                right_border = last_right_border;
            }

            mid = (left_border + right_border) / 2; // 中线坐标
            

            l_border[i] = (unsigned char)left_border; // 左边线线数组
            r_border[i] = (unsigned char)right_border; // 右边线线数组
            center_line[i] = (unsigned char)mid; // 修正中心线计算公式
    }


    for(i = image_h - 6;i>0;i--)
    {
        for (j = center_line[i+1]; j > 0; j--)//从上一行的中线往左找
        {
		if (bin_image[i][j] == 0xFF && bin_image[i][j - 1] == 0x00 )
		{
                    left_border = j;
                    break; // 跳出，找到本行边界就没必要循环下去了
		}
        }
            // 没找到左边界的处理，继承上一行的左边界
            if (j == 1)
            {
                left_border = last_left_border;
            }


            // 往右边扫描，从上一行的中线往右边找
        for (j = center_line[i+1]; j < image_w-2; j++)
        {
            if (bin_image[i][j] == 0xFF && bin_image[i][j + 1] == 0x00 ) // 黑黑白认为到达左边界
            {
                right_border = j;
                break; // 跳出，找到本行边界就没必要循环下去了
            }
        }

            // 没找到右边界的处理，继承上一行的右边界
            if (j == image_w-3)
            {
                right_border = last_right_border;
            }
            
        mid = (left_border + right_border) / 2; // 中线坐标
            

        l_border[i] = (unsigned char)left_border; // 左边线线数组
        r_border[i] = (unsigned char)right_border; // 右边线线数组
        center_line[i] = (unsigned char)mid; // 修正中心线计算公式
        
        last_left_border= left_border;
        last_right_border= right_border;

    }

    return in_flag;
}

int16 err;
int32 sum_mid;
uint16 count;
int16 average_mid;

void get_err()
{
  unsigned char i;
  err=0;
  sum_mid=0;
  count=0;
  average_mid=0;
  for(i=image_h-1;i>image_h-90;i--)
  {
    if(yuansu.R_round_flag==1){center_line[i]=r_border[i];}//右圆环入环，巡右边界
    if(yuansu.R_round_flag==2){center_line[i]=l_border[i];}//右圆环出环，巡左边界
    if(yuansu.L_round_flag==1){center_line[i]=l_border[i];}//左圆环入环
    if(yuansu.L_round_flag==2){center_line[i]=r_border[i];}//左圆环出环
    if(yuansu.R_turning_flag)//在右部跳变点上方的黑色区域把中线放在最右边，误差最大化
    {
      if(i<coord.right_jump_y-1)center_line[i]=image_w-1;
    }
    if(yuansu.L_turning_flag)//同右直角
    {
      if(i<coord.left_jump_y-1)center_line[i]=0;
    }
    if(yuansu.R_turning_flag||yuansu.L_turning_flag)//直角与正常巡线的权重数组不一样
    {
    sum_mid+=center_line[i]*mid_weight_zhijiao[i];
    count+=mid_weight_zhijiao[i];   
    }
    else
    {
    sum_mid+=center_line[i]*mid_weight[i];
    count+=mid_weight[i];
    }
  }
  average_mid=(int16)(sum_mid/count);
  err=(image_w/2)-average_mid;
  
  if(!yuansu.L_turning_flag&&!yuansu.R_turning_flag&&!yuansu.R_round_flag&&!yuansu.L_round_flag)//如果不在直角与圆环就对误差进行滤波
  {
  err=moving_average_filter(history_err,ERR_SIZE,err);//滑动平均滤波  
  }
}

void neighbor_track(uint8 flag)//1-八邻域  0-左右巡线（继承中线）
{
  uint8 i;
  if(flag)
  {
  clear_find_point();//初始化
  for(i=1;i<5;i++)
  {
  if(get_start_point(image_h-i)){break;}
  }//找八邻域起点找到就退出循环
  image_draw_rectan(bin_image);//画边框
  search_neighborhood();//八邻域
  get_left();//左边界
  get_right();//右边界
  get_mid();
  }
  
  else
  {
  zuoyou_xunxian(bin_image); // 从中间往两边搜索中线  
  }
}

