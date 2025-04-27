/*
 * @Author: yyx-pc 3454523412@qq.com
 * @Date: 2025-04-27 20:17:38
 * @LastEditors: yyx-pc 3454523412@qq.com
 * @LastEditTime: 2025-04-27 20:36:26
 * @FilePath: \car\Seekfree_CYT4BB_Opensource_Library\project\code\image.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "image.h"

uint8 Find_Line_Image[IAMGE_H][IAMGE_W];  // 成功处理后的图像
uint8 left_line[IAMGE_H];                 // 左线
uint8 right_line[IAMGE_H];                // 右线
uint8 center_line[IAMGE_H];               // 中线
// bool Image_Count_Flag = false;  自定义开启图像计数标志位，挂1时开启图像计数
// uint16 Image_Count = 0;         图像计数
// bool Inverse_Flag = false;                逆透视标志位

/**
 * 函数功能：      复制并压缩图像，将 188 * 120 图像压缩为 80 * 60 大小
 * 特殊说明：      总钻风使用手册中说明：图像分辨率为  752 * 480， 376 *
 * 240， 188 * 120 这三种分辨率视野是一样的，三者呈整数倍关系
 *                其他分辨率是通过裁减得到的(这个裁减包含比188 *
 * 120小的任何分辨率，如 94 * 60)，如376 * 240 的视野反而比752 * 400
 * 的视野广 此处将总钻风传回图像 188 * 120 压缩为 80 * 60， 所以将 j
 * 乘系数 2.35（188 / 80） 经实际测试，当设置图像大小为 94 * 60
 * 时，传回的图像视野是 188 * 120 的四分之一，虽然也和 752 * 480
 * 呈整数倍关系，但和上述情况不同
 *
 *
 * 形  参：        无
 * 示例：          Copy_Zip_Image();
 * 返回值：        无
 */
void Copy_Zip_Image(void)  //*****
{
    uint8 i, j;
    if (mt9v03x_finish_flag ==
        1)  // mt9v03x_finish_flag：逐飞库定义摄像头采集标志位，采集完一帧图像会挂1
    {
        for (i = 0; i < IAMGE_H; i++) {
            for (j = 0; j < IAMGE_W; j++) {
                Find_Line_Image[i][j] = mt9v03x_image[i * 2][(
                    uint8)(j * 2.35)];  // 将188 * 120图像压缩为 80 *
                                        // 60大小，X轴比为2.35，Y轴比为2
            }
        }
        mt9v03x_finish_flag = 0;  // 注意清掉图像采集完成标志位
    }
}

/**
 * 函数功能：      搜索线
 * 特殊说明：      通过搜索图像的最下面一行的平均值作为参考点
 *                通过参考点加减一定值得到白线的最大值和最小值
 *
 * 形  参：        const uint8 *image
 * 返回值：        无
 */
void search_line(void) {}