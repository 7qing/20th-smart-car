#include "zf_common_headfile.h"
#include "mymath.h"

int my_abs(int16 value)
{
if(value>=0) return value;
else return -value;
}

/** 
* @brief ��С���˷�
* @param uint8 begin				�������
* @param uint8 end					�����յ�
* @param uint8 *border				������Ҫ����б�ʵı߽��׵�ַ
*  @see CTest		Slope_Calculate(start, end, border);//б��
* @return ����˵��
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
	if ((end - begin)*x2sum - xsum * xsum) //�жϳ����Ƿ�Ϊ��
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
* @brief ����б�ʽؾ�
* @param uint8 start				�������
* @param uint8 end					�����յ�
* @param uint8 *border				������Ҫ����б�ʵı߽�
* @param float *slope_rate			����б�ʵ�ַ
* @param float *intercept			����ؾ��ַ
*  @see CTest		calculate_s_i(start, end, r_border, &slope_l_rate, &intercept_l);
* @return ����˵��
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

	//�������ƽ����
	if (num)
	{
		x_average = (float)(xsum / num);
		y_average = (float)(ysum / num);

	}

	/*����б��*/
	*slope_rate = Slope_Calculate(start, end, border);//б��
	*intercept = y_average - (*slope_rate)*x_average;//�ؾ�
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

//����temp�����ϵ�����
//x1x2�ֱ�Ϊ���������
//y1y2�ֱ�Ϊ��������Ӧ������
void connect_point(int temp[], int x1, int x2, int y1, int y2)//xΪ������yΪ����
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  int ux;
  if (dx > 0)
    ux = 1;
  else
    ux = -1;//x����������ȡ��-1
  int uy;
  if (dy > 0)
    uy = 1;
  else
    uy = -1;//y����������ȡ��-1
  int x = x1, y = y1, eps;//epsΪ�ۼ����
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
      //һЩ����
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

//���������б�ʴ�����������
//x1x2�ֱ�Ϊ���������
//y1y2�ֱ�Ϊ��������Ӧ������
void yanshen_to_up(int temp[], int x1, int x2, int y1, int y2)//xΪ������yΪ����
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  int ux;
  if (dx > 0)
    ux = 1;
  else
    ux = -1;//x����������ȡ��-1
  int uy;
  if (dy > 0)
    uy = 1;
  else
    uy = -1;//y����������ȡ��-1
  int x = x1, y = y1, eps;//epsΪ�ۼ����
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
    //һЩ����
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

//���������б�ʴ�����������
//x1x2�ֱ�Ϊ���������
//y1y2�ֱ�Ϊ��������Ӧ������
void yanshen_to_down(int temp[], int x1, int x2, int y1, int y2)//xΪ������yΪ����
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  int ux;
  if (dx > 0)
    ux = 1;
  else
    ux = -1;//x����������ȡ��-1
  int uy;
  if (dy > 0)
    uy = 1;
  else
    uy = -1;//y����������ȡ��-1
  int x = x1, y = y1, eps;//epsΪ�ۼ����
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
    //һЩ����
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

int16 history_err[ERR_SIZE]={0,0,0,0,0};//��ʷ���
//����ƽ���˲�
/**
 * @brief ����ƽ���˲�����������������ݽ��л���ƽ���˲�����
 * 
 * �ú���ʹ��һ���̶���С�Ļ��������洢������������ݣ�ÿ������һ����ֵʱ��
 * ����»������е����ݲ����¼���ƽ��ֵ��
 * 
 * @param buffer ָ��洢���ݵĻ�������ָ�롣
 * @param size �������Ĵ�С��
 * @param new_value �����������ֵ��
 * @return int16 ��������ƽ���˲���Ľ����
 */
int16 moving_average_filter(int16 *buffer, uint8 size, int16 new_value)
{
    // ��̬���������ڼ�¼��ǰҪ���µĻ���������
    static uint8 index = 0;
    // ��̬���������ڴ洢��ǰ���������������ݵ��ܺ�
    static int16 sum = 0;
    // ��̬���������ڴ洢��ǰ�����������ݵ�ƽ��ֵ
    static int16 avg = 0;
    // ��̬���������ڱ�ǻ������Ƿ��Ѿ���ʼ��
    static uint8 is_initialized = 0;

    // ��ʼ��sum��avg
    if (!is_initialized) {
        // �������������ۼ��������ݵ�sum
        for (uint8 i = 0; i < size; i++) {
            sum += buffer[i];
        }
        // ���㻺���������ݵĳ�ʼƽ��ֵ
        avg = sum / size;
        // ��ǻ������Ѿ���ʼ��
        is_initialized = 1;
    }

    // ������ͺ�����
    // ���ܺ��м�ȥ��ǰ����λ�õľ�ֵ
    sum -= buffer[index];
    // ����ֵ�洢����ǰ����λ��
    buffer[index] = new_value;
    // ����ֵ�ۼӵ��ܺ���
    sum += buffer[index];
    // ����������ʹ��ȡģ����ȷ�������ڻ�������Χ��ѭ��
    index = (index + 1) % size;

    // �����µ�ƽ��ֵ
    avg = (int16)(sum / size);

    return avg;
}

