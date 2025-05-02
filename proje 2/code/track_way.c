#include "zf_common_headfile.h"
#include "user_code.h"


uint8   mid_weight[90]={
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //����81-90��                         
                        6 ,8 ,9 ,10 ,12 ,12 ,14 ,17 ,20 ,20 , //����71-80��
                        23 ,25 ,23 ,23 ,21 ,20 ,20 ,18 ,18 ,15 , //����61-70��                        
                        8 ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //����51-60��                      
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //����41-50��
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //����31-40��
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //����21-30��
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //����11-20��
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 };//����1-10��

uint8   mid_weight_zhijiao[90]={
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //����81-90��                         
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //����71-80��
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //����61-70��                        
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //����51-60��                      
                        1 ,1 ,2 ,5 ,8 ,10 ,13 ,15 ,16 ,19 , //����41-50��
                        25 ,25 ,22 ,20 ,17 ,16 ,15 ,15 ,13 ,10 , //����31-40��
                        8 ,6 ,5 ,2 ,1 ,1 ,1 ,1 ,1 ,1 , //����21-30��
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 , //����11-20��
                        1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 };//����1-10��

/*
�������ƣ�void get_start_point(uint8 start_row)
����˵����Ѱ�������߽�ı߽����Ϊ������ѭ������ʼ��
����˵����������������
�������أ���
�޸�ʱ�䣺2022��9��8��
��    ע��
example��  get_start_point(image_h-2)
 */
uint8 start_point_l[2] = { 0 };//�������x��yֵ
uint8 start_point_r[2] = { 0 };//�ұ�����x��yֵ\

uint8 border_max= image_w-1;
uint8 border_min= 0;
uint8 l_found = 0,r_found = 0;

uint8 get_start_point(uint8 start_row)
{
	uint8 i = 0;
	//����
	start_point_l[0] = 0;//x
	start_point_l[1] = 0;//y
        l_found=0;

	start_point_r[0] = 0;//x
	start_point_r[1] = 0;//y
        r_found=0;
		//���м�����ߣ��������
	for (i = image_w / 2; i > border_min; i--)
	{
		start_point_l[0] = i;//x
		start_point_l[1] = start_row;//y
		if (bin_image[start_row][i] == 255 && bin_image[start_row][i - 1] == 0)
		{
			//printf("�ҵ�������image[%d][%d]\n", start_row,i);
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
			//printf("�ҵ�������image[%d][%d]\n", start_row,i);
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
			//printf("�ҵ��ұ����image[%d][%d]\n",start_row, i);
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
			//printf("�������image[%d][%d]\n", start_row,i);
			r_found = 1;
			break;
		}
	}
        }
//        if(start_point_r[0]-start_point_l[0]>30||start_point_r[0]-start_point_l[0]<0)//��������������������Ҵ�����
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
		//printf("δ�ҵ����\n");
		return 0;
	} 
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ͼ�񻭺ڿ�Ϊ��������׼��
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
 ����    ����search_neighborhood
 ����    �ܡ��������ұ߽�
 ����    ������
 ���� �� ֵ����
 ��ע�����
 ----------------------------------------------------------------*/
struct LEFT_EDGE
{
    int16 row;  //������
    int16 col;  //������
    uint8 flag; //���ڱ߽�ı�־
    uint8 dir;
};
struct RIGHT_EDGE
{
    int16 row;  //������
    int16 col;  //������
    uint8 flag; //���ڱ߽�ı�־
    uint8 dir;
};

struct LEFT_EDGE  L_edge[search_count];     //��߽�ṹ��
struct RIGHT_EDGE R_edge[search_count];    //�ұ߽�ṹ��
uint8 L_edge_count=0, R_edge_count = 0;                     //���ұߵ�ĸ���
uint8 dire_left,dire_right;                                 //��¼��һ��������λ��
uint8 L_search_amount = search_count, R_search_amount = search_count;  //���ұ߽��ѵ�ʱ�������ĵ�
void search_neighborhood(void)
{
    L_edge_count = 0;//��ߵ������0
    R_edge_count = 0;//�ұߵ������0
    
    if(l_found)//�����߽����ڲ��ҵ�,��ʼ����
    {
        //��������
        L_edge[0].row = start_point_l[1];
        L_edge[0].col = start_point_l[0];
        L_edge[0].flag = 1;
        int16 curr_row = start_point_l[1];//��ʼ��������
        int16 curr_col = start_point_l[0];//��ʼ��������
        dire_left = 0; //��ʼ���ϸ��߽�������
        //��ʼ���ߣ����ȡ150���㣬���������ѣ���7����λ
        for(int i = 1;i < L_search_amount; i++)    //�������150����
        {
            ////Խ���˳� ��Խ�����Խ�磨���������������ң�
            if(curr_row+1 < 1 || curr_row>image_h-1)  break;
            //���߹���
            if(dire_left != 2&&bin_image[curr_row-1][curr_col-1]==BLACK&&bin_image[curr_row-1][curr_col]==WHITE)   //���Ϻڣ�2���ұ߰�
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
            else if(dire_left!=3&&bin_image[curr_row-1][curr_col+1]==BLACK&&bin_image[curr_row][curr_col+1]==WHITE)    //���Ϻڣ�3���±߰�
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
            else if(bin_image[curr_row-1][curr_col]==BLACK&&bin_image[curr_row-1][curr_col+1]==WHITE)                  //���Ϻڣ�1���Ұ�
            {
                curr_row = curr_row - 1;
                L_edge_count = L_edge_count + 1;
                dire_left = 0;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
                L_edge[i-1].dir=0;
            }
            else if(dire_left!=5&&bin_image[curr_row][curr_col-1]==BLACK&&bin_image[curr_row-1][curr_col-1]==WHITE)     //����ڣ�5���ϰ�
            {
                curr_col = curr_col - 1;
                L_edge_count = L_edge_count +1;
                dire_left = 4;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
                L_edge[i-1].dir=4;
            }
            else if(dire_left!=4&&bin_image[curr_row][curr_col+1]==BLACK&&bin_image[curr_row+1][curr_col+1]==WHITE)  //���Һڣ�4���°�
            {
                curr_col = curr_col + 1;
                L_edge_count = L_edge_count +1;
                dire_left = 5;
                L_edge[i].row = curr_row;
                L_edge[i].col = curr_col;
                L_edge[i].flag = 1;
                L_edge[i-1].dir=5;
            }
            else if(dire_left!=6&&bin_image[curr_row+1][curr_col-1]==BLACK&&bin_image[curr_row][curr_col-1]==WHITE)    //���ºڣ�6���ϰ�
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
            else if(dire_left!=7&&bin_image[curr_row+1][curr_col+1]==BLACK&&bin_image[curr_row+1][curr_col]==WHITE)    //���ºڣ�7�����
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

    if(r_found)//����ұ߽���ڲ��ѵ�
             {
                 R_edge[0].row = start_point_r[1];
                 R_edge[0].col = start_point_r[0];
                 R_edge[0].flag = 1;
                 int16 curr_row = start_point_r[1];
                 int16 curr_col = start_point_r[0];
                 dire_right = 0;
                 for(int i = 1;i<R_search_amount;i++)
                 {
		////Խ���˳� ��Խ�����Խ�磨���������������ң�
                     if(curr_row < 1 || curr_row>image_h-1)  break;
                     //���߹���
                     if(curr_col<image_w&&dire_right!=3&&bin_image[curr_row-1][curr_col+1]==BLACK&&bin_image[curr_row-1][curr_col]==WHITE)    //���Ϻڣ�3�����
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
                     else if(dire_right!=2&&bin_image[curr_row-1][curr_col-1]==BLACK&&bin_image[curr_row][curr_col-1]==WHITE) //���Ϻڣ�2���°�
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
                     else if(bin_image[curr_row-1][curr_col]==BLACK&&bin_image[curr_row-1][curr_col-1]==WHITE)                  //���Ϻڣ�1�����
                     {
                         curr_row = curr_row - 1;
                         R_edge_count = R_edge_count + 1;
                         dire_right = 0;
                         R_edge[i].row = curr_row;
                         R_edge[i].col = curr_col;
                         R_edge[i].flag = 1;
                         R_edge[i-1].dir=0;
                     }
                     else if(dire_right!=4&&bin_image[curr_row][curr_col+1]==BLACK&&bin_image[curr_row-1][curr_col+1]==WHITE)   //���Һڣ�4���ϰ�
                     {
                         curr_col = curr_col + 1;
                         R_edge_count = R_edge_count + 1;
                         dire_right = 5;
                         R_edge[i].row = curr_row;
                         R_edge[i].col = curr_col;
                         R_edge[i].flag = 1;
                         R_edge[i-1].dir=5;
                     }
                     else if(dire_right!=5&&bin_image[curr_row][curr_col-1]==BLACK&&bin_image[curr_row+1][curr_col-1]==WHITE)   //����ڣ�5���°�
                     {
                         curr_col = curr_col-1;
                         R_edge_count = R_edge_count + 1;
                         dire_right = 4;
                         R_edge[i].row = curr_row;
                         R_edge[i].col = curr_col;
                         R_edge[i].flag = 1;
                         R_edge[i-1].dir=4;
                     }


                     else if(dire_right!=6&&bin_image[curr_row+1][curr_col-1]==BLACK&&bin_image[curr_row+1][curr_col]==WHITE)   //���ºڣ�6���Ұ�
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
                     else if(dire_right!=7&&bin_image[curr_row+1][curr_col+1]==BLACK&&bin_image[curr_row][curr_col+1]==WHITE)   //���ºڣ�7���ϰ�
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
 ����    ����clear_find_point
 ����    �ܡ�������߽��ʼ��
 ����    ������
 ���� �� ֵ��
 ��ע�����
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

uint8 l_border[image_h];//��������
uint8 r_border[image_h];//��������
uint8 center_line[image_h];//��������
void get_left()
{
	uint8 i = 0;
	uint16 j = 0;
	uint8 h = 0;
	//��ʼ��
	for (i = 0;i<image_h;i++)
	{
		l_border[i] = border_min;
	}
	h = image_h - 2;
	//���
	for (j = 0; j < search_count; j++)
	{
		//printf("%d\n", j);
		if (L_edge[j].row == h)
		{
			l_border[h] = L_edge[j].col+1;
		}
		else continue; //ÿ��ֻȡһ���㣬û����һ�оͲ���¼
		h--;
		if (h == 0) 
		{
			break;//�����һ���˳�
		}
	}
}

/*
�������ƣ�void get_right(uint16 total_R)
����˵�����Ӱ�����߽�����ȡ��Ҫ�ı���
����˵����
total_R  ���ҵ��ĵ������
�������أ���
�޸�ʱ�䣺2022��9��25��
��    ע��
example��get_right(data_stastics_r);
 */
void get_right()
{
	uint8 i = 0;
	uint16 j = 0;
	uint8 h = 0;
	for (i = 0; i < image_h; i++)
	{
		r_border[i] = border_max;//�ұ��߳�ʼ���ŵ����ұߣ�����߷ŵ�����ߣ�����������պ�����������߾ͻ����м䣬������ŵõ�������
	}
	h = image_h - 2;
	//�ұ�
	for (j = 0; j < search_count; j++)
	{
		if (R_edge[j].row == h)
		{
			r_border[h] = R_edge[j].col - 1;
		}
		else continue;//ÿ��ֻȡһ���㣬û����һ�оͲ���¼
		h--;
		if (h == 0)break;//�����һ���˳�
	}
}

void get_mid()
{
	uint8 i = 0;
	for (i = 0; i < image_h; i++)
	{
		center_line[i] = (r_border[i]+l_border[i])/2;//�ұ��߳�ʼ���ŵ����ұߣ�����߷ŵ�����ߣ�����������պ�����������߾ͻ����м䣬������ŵõ�������
	}
}

unsigned char in_flag = 0;
unsigned char zuoyou_xunxian(uint8(*bin_image)[image_w]) // ���м���������������
{
    int i, j;
    static int left_border = 0, right_border = image_w-1, mid = image_w / 2;
    
    static int last_left_border = 0, last_right_border = image_w-1;
    

    // ��һ��Ѳ�ߣ�����ԶС����������ɨ�跽ʽ
    for (i = image_h - 1; i >= image_h - 5; i--) // �����������ɨ��
    {
            // ����ɨ��
            for (j = 0; j < image_w - 2; j++)
            {
                if (bin_image[i][j] == 0 && bin_image[i][j + 1] == 255 )
                {
                    left_border = j;
                    break; // �������ҵ����б߽��û��Ҫѭ����ȥ��
                }
            }

            // û�ҵ��ұ߽�Ĵ���
            if (j == image_w - 3)
            {
                left_border = last_left_border;
            }

            // �����ɨ��
            for (j = image_w-1; j > 1; j--)
            {
                if (bin_image[i][j] == 0 && bin_image[i][j - 1] == 255 && bin_image[i][j - 2] == 255) // �ڰ���Ϊ������߽�
                {
                    right_border = j;
                    break; // �������ҵ����б߽��û��Ҫѭ����ȥ��
                }
            }

            // û�ҵ��ұ߽�Ĵ���
            if (j == 2)
            {
                right_border = last_right_border;
            }

            mid = (left_border + right_border) / 2; // ��������
            

            l_border[i] = (unsigned char)left_border; // �����������
            r_border[i] = (unsigned char)right_border; // �ұ���������
            center_line[i] = (unsigned char)mid; // ���������߼��㹫ʽ
    }


    for(i = image_h - 6;i>0;i--)
    {
        for (j = center_line[i+1]; j > 0; j--)//����һ�е�����������
        {
		if (bin_image[i][j] == 0xFF && bin_image[i][j - 1] == 0x00 )
		{
                    left_border = j;
                    break; // �������ҵ����б߽��û��Ҫѭ����ȥ��
		}
        }
            // û�ҵ���߽�Ĵ����̳���һ�е���߽�
            if (j == 1)
            {
                left_border = last_left_border;
            }


            // ���ұ�ɨ�裬����һ�е��������ұ���
        for (j = center_line[i+1]; j < image_w-2; j++)
        {
            if (bin_image[i][j] == 0xFF && bin_image[i][j + 1] == 0x00 ) // �ںڰ���Ϊ������߽�
            {
                right_border = j;
                break; // �������ҵ����б߽��û��Ҫѭ����ȥ��
            }
        }

            // û�ҵ��ұ߽�Ĵ����̳���һ�е��ұ߽�
            if (j == image_w-3)
            {
                right_border = last_right_border;
            }
            
        mid = (left_border + right_border) / 2; // ��������
            

        l_border[i] = (unsigned char)left_border; // �����������
        r_border[i] = (unsigned char)right_border; // �ұ���������
        center_line[i] = (unsigned char)mid; // ���������߼��㹫ʽ
        
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
    if(yuansu.R_round_flag==1){center_line[i]=r_border[i];}//��Բ���뻷��Ѳ�ұ߽�
    if(yuansu.R_round_flag==2){center_line[i]=l_border[i];}//��Բ��������Ѳ��߽�
    if(yuansu.L_round_flag==1){center_line[i]=l_border[i];}//��Բ���뻷
    if(yuansu.L_round_flag==2){center_line[i]=r_border[i];}//��Բ������
    if(yuansu.R_turning_flag)//���Ҳ�������Ϸ��ĺ�ɫ��������߷������ұߣ�������
    {
      if(i<coord.right_jump_y-1)center_line[i]=image_w-1;
    }
    if(yuansu.L_turning_flag)//ͬ��ֱ��
    {
      if(i<coord.left_jump_y-1)center_line[i]=0;
    }
    if(yuansu.R_turning_flag||yuansu.L_turning_flag)//ֱ��������Ѳ�ߵ�Ȩ�����鲻һ��
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
  
  if(!yuansu.L_turning_flag&&!yuansu.R_turning_flag&&!yuansu.R_round_flag&&!yuansu.L_round_flag)//�������ֱ����Բ���Ͷ��������˲�
  {
  err=moving_average_filter(history_err,ERR_SIZE,err);//����ƽ���˲�  
  }
}

void neighbor_track(uint8 flag)//1-������  0-����Ѳ�ߣ��̳����ߣ�
{
  uint8 i;
  if(flag)
  {
  clear_find_point();//��ʼ��
  for(i=1;i<5;i++)
  {
  if(get_start_point(image_h-i)){break;}
  }//�Ұ���������ҵ����˳�ѭ��
  image_draw_rectan(bin_image);//���߿�
  search_neighborhood();//������
  get_left();//��߽�
  get_right();//�ұ߽�
  get_mid();
  }
  
  else
  {
  zuoyou_xunxian(bin_image); // ���м���������������  
  }
}

