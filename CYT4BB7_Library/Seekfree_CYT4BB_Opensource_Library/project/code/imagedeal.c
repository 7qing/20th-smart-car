/*
 * @Author: yyx-pc 3454523412@qq.com
 * @Date: 2025-03-07 20:52:17
 * @LastEditors: yyx-pc 3454523412@qq.com
 * @LastEditTime: 2025-03-07 21:27:52
 * @FilePath: \Seekfree_CYT4BB_Opensource_Library\project\code\imagedeal.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "imagedeal.h"

#define GrayScale 256

uint8 Find_Line_Image[IAMGE_H][IAMGE_W];



/**
* 函数功能：      复制并压缩图像，将 188 * 120 图像压缩为 80 * 60 大小
* 特殊说明：      总钻风使用手册中说明：图像分辨率为  752 * 480， 376 * 240， 188 * 120 这三种分辨率视野是一样的，三者呈整数倍关系
*                其他分辨率是通过裁减得到的(这个裁减包含比188 * 120小的任何分辨率，如 94 * 60)，如376 * 240 的视野反而比752 * 400 的视野广
*                此处将总钻风传回图像 188 * 120 压缩为 80 * 60， 所以将 j 乘系数 2.35（188 / 80）
*                经实际测试，当设置图像大小为 94 * 60 时，传回的图像视野是 188 * 120 的四分之一，虽然也和 752 * 480 呈整数倍关系，但和上述情况不同
*
*                
* 形  参：        无
* 示例：          Copy_Zip_Image();
* 返回值：        无
*/
 
void Copy_Zip_Image(void)               //*****
{
    uint8 i,j;
    if(mt9v03x_finish_flag == 1 && Inverse_Flag == 0)       //mt9v03x_finish_flag：逐飞库定义摄像头采集标志位，采集完一帧图像会挂1
                                                            //Inverse_Flag：自定义逆透视标志位，挂1时会得到一张逆透视图像，根据自己需求而定
    {
        for(i = 0; i < Image_Y; i++)
        {
            for(j = 0; j < Image_X; j++)
            {
                Find_Line_Image[i][j] = mt9v03x_image[i * 2][(uint8)(j * 2.35)];    //将188 * 120图像压缩为 80 * 60大小，X轴比为2.35，Y轴比为2
            }
        }
        if(Image_Count_Flag == 1)                 //Image_Count_Flag：自定义开启图像计数标志位，挂1时开启图像计数，即每采集一张图像计数+1；挂0时图像计数清零
        {
            Image_Count ++;
        }
        else if(Image_Count_Flag == 0)
        {
            Image_Count = 0;
        }
//        Image_Num ++;
        mt9v03x_finish_flag = 0;    //注意清掉图像采集完成标志位
    }
    else if(mt9v03x_finish_flag == 1 && Inverse_Flag == 1)
    {
        for(i = 0; i < Image_Y; i++)
        {
            for(j = 0; j < Image_X; j++)
            {
                Find_Line_Image[i][j] = mt9v03x_image[i * 2][(uint8)(j * 2.35f)];
            }
        }
        //Get_Inverse_Perspective_Image(Find_Line_Image, I_Perspective_Image);    //逆透视处理函数，在另处详细说明
        if(Image_Count_Flag == 1)
        {
            Image_Count ++;
        }
        else if(Image_Count_Flag == 0)
        {
            Image_Count = 0;
        }
//        Image_Num ++;
        mt9v03x_finish_flag = 0;
    }
}

