#include "zf_common_headfile.h"
#include "mymath.h"

int my_abs(int16 value)
{
if(value>=0) return value;
else return -value;
}

/** 
* @brief 最小二乘法
* @param uint8 begin				输入起点
* @param uint8 end					输入终点
* @param uint8 *border				输入需要计算斜率的边界首地址
*  @see CTest		Slope_Calculate(start, end, border);//斜率
* @return 返回说明
*     -<em>false</em> fail
*     -<em>true</em> succeed
*/
float Slope_Calculate(uint8 begin, uint8 end, uint8 *border)
{
	float xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
	int16 i = 0;
	float result = 0;
	static float resultlast;

	for (i = begin; i < end; i++)
	{
		xsum += i;
		ysum += border[i];
		xysum += i * (border[i]); 
		x2sum += i * i;

	}
	if ((end - begin)*x2sum - xsum * xsum) //判断除数是否为零
	{
		result = ((end - begin)*xysum - xsum * ysum) / ((end - begin)*x2sum - xsum * xsum);
		resultlast = result;
	}
	else
	{
		result = resultlast;
	}
	return result;
}

/** 
* @brief 计算斜率截距
* @param uint8 start				输入起点
* @param uint8 end					输入终点
* @param uint8 *border				输入需要计算斜率的边界
* @param float *slope_rate			输入斜率地址
* @param float *intercept			输入截距地址
*  @see CTest		calculate_s_i(start, end, r_border, &slope_l_rate, &intercept_l);
* @return 返回说明
*     -<em>false</em> fail
*     -<em>true</em> succeed
*/
void calculate_s_i(uint8 start, uint8 end, uint8 *border, float *slope_rate, float *intercept)
{
	uint16 i, num = 0;
	uint16 xsum = 0, ysum = 0;
	float y_average, x_average;

	num = 0;
	xsum = 0;
	ysum = 0;
	y_average = 0;
	x_average = 0;
	for (i = start; i < end; i++)
	{
		xsum += i;
		ysum += border[i];
		num++;
	}

	//计算各个平均数
	if (num)
	{
		x_average = (float)(xsum / num);
		y_average = (float)(ysum / num);

	}

	/*计算斜率*/
	*slope_rate = Slope_Calculate(start, end, border);//斜率
	*intercept = y_average - (*slope_rate)*x_average;//截距
}

void ips_show_round(uint8 x,uint8 y)
{
  ips200_draw_point(x+3,y,RGB565_RED);
  ips200_draw_point(x+3,y+3,RGB565_RED);
  ips200_draw_point(x,y+3,RGB565_RED);
  ips200_draw_point(x-3,y+3,RGB565_RED);
  ips200_draw_point(x-3,y,RGB565_RED);
  ips200_draw_point(x-3,y-3,RGB565_RED);
  ips200_draw_point(x,y-3,RGB565_RED);
  ips200_draw_point(x+3,y-3,RGB565_RED);
}

float k_get(uint8 y1,uint8 y2,uint8 *border)
{
  float k;
  k=1.0*(y1-y2)/(border[y1]-border[y2]);
  return k;

}

//连接temp数组上的两点
//x1x2分别为两点的行数
//y1y2分别为两点所对应的列数
void connect_point(int temp[], int x1, int x2, int y1, int y2)//x为行数，y为列数
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  int ux;
  if (dx > 0)
    ux = 1;
  else
    ux = -1;//x的增量方向，取或-1
  int uy;
  if (dy > 0)
    uy = 1;
  else
    uy = -1;//y的增量方向，取或-1
  int x = x1, y = y1, eps;//eps为累加误差
  eps = 0; dx = my_abs(dx); dy = my_abs(dy);
  if (dx > dy)
  {
    for (x = x1; x != x2; x += ux)
    {
      temp[x] = y;
      eps += dy;
      if ((eps << 1) >= dx)
      {
        y += uy; eps -= dx;
      }
      
    }
  }
  else
  {
    for (y = y1; y != y2; y += uy)
    {
      //一些操作
      //if (src[y,x] == type)
      //num++;
      temp[x] = y;
      eps += dx;
      if ((eps << 1) >= dy)
      {
        x += ux; eps -= dy;
      }
      
    }
  }
}

//依据两点的斜率从下往上延申
//x1x2分别为两点的行数
//y1y2分别为两点所对应的列数
void yanshen_to_up(int temp[], int x1, int x2, int y1, int y2)//x为行数，y为列数
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  int ux;
  if (dx > 0)
    ux = 1;
  else
    ux = -1;//x的增量方向，取或-1
  int uy;
  if (dy > 0)
    uy = 1;
  else
    uy = -1;//y的增量方向，取或-1
  int x = x1, y = y1, eps;//eps为累加误差
  eps = 0; dx = my_abs(dx); dy = my_abs(dy);
  
  if (ux == 1)
  {
    ux = -1;
    int temper = 0;
    temper = x1;
    x1 = x2;
    x2 = temper;
    temper = y1;
    y1 = y2;
    y2 = temper;
  }
  
  for (x = x1; x >= 0; x += ux)
  {
    //一些操作
    //if (src[y,x] == type)
    //num++;
    if (y < 1) y = 1;
    if (y > 186) y = 186;
    temp[x] = y;
    eps += dy;
    if ((eps << 1) >= dx)
    {
      y += uy; eps -= dx;
    }
  }
}

//依据两点的斜率从上往下延申
//x1x2分别为两点的行数
//y1y2分别为两点所对应的列数
void yanshen_to_down(int temp[], int x1, int x2, int y1, int y2)//x为行数，y为列数
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  int ux;
  if (dx > 0)
    ux = 1;
  else
    ux = -1;//x的增量方向，取或-1
  int uy;
  if (dy > 0)
    uy = 1;
  else
    uy = -1;//y的增量方向，取或-1
  int x = x1, y = y1, eps;//eps为累加误差
  eps = 0; dx = my_abs(dx); dy = my_abs(dy);
  
  if (ux == -1)
  {
    ux = 1;
    int temper = 0;
    temper = x1;
    x1 = x2;
    x2 = temper;
    temper = y1;
    y1 = y2;
    y2 = temper;
  }
  
  for (x = x1; x <120; x += ux)
  {
    //一些操作
    //if (src[y,x] == type)
    //num++;
    if (y < 1) y = 1;
    if (y > 186) y = 186;
    temp[x] = y;
    eps += dy;
    if ((eps << 1) >= dx)
    {
      y += uy; eps -= dx;
    }
  }
}

int16 history_err[ERR_SIZE]={0,0,0,0,0};//历史误差
//滑动平均滤波
/**
 * @brief 滑动平均滤波函数，对输入的数据进行滑动平均滤波处理。
 * 
 * 该函数使用一个固定大小的缓冲区来存储最近的输入数据，每次输入一个新值时，
 * 会更新缓冲区中的数据并重新计算平均值。
 * 
 * @param buffer 指向存储数据的缓冲区的指针。
 * @param size 缓冲区的大小。
 * @param new_value 新输入的数据值。
 * @return int16 经过滑动平均滤波后的结果。
 */
int16 moving_average_filter(int16 *buffer, uint8 size, int16 new_value)
{
    // 静态变量，用于记录当前要更新的缓冲区索引
    static uint8 index = 0;
    // 静态变量，用于存储当前缓冲区中所有数据的总和
    static int16 sum = 0;
    // 静态变量，用于存储当前缓冲区中数据的平均值
    static int16 avg = 0;
    // 静态变量，用于标记缓冲区是否已经初始化
    static uint8 is_initialized = 0;

    // 初始化sum和avg
    if (!is_initialized) {
        // 遍历缓冲区，累加所有数据到sum
        for (uint8 i = 0; i < size; i++) {
            sum += buffer[i];
        }
        // 计算缓冲区中数据的初始平均值
        avg = sum / size;
        // 标记缓冲区已经初始化
        is_initialized = 1;
    }

    // 更新求和和索引
    // 从总和中减去当前索引位置的旧值
    sum -= buffer[index];
    // 将新值存储到当前索引位置
    buffer[index] = new_value;
    // 将新值累加到总和中
    sum += buffer[index];
    // 更新索引，使用取模运算确保索引在缓冲区范围内循环
    index = (index + 1) % size;

    // 计算新的平均值
    avg = (int16)(sum / size);

    return avg;
}

