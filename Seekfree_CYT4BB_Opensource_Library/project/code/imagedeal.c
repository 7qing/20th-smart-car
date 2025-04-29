/*
 * @Author: yyx-pc 3454523412@qq.com
 * @Date: 2025-03-07 20:52:17
 * @LastEditors: yyx-pc 3454523412@qq.com
 * @LastEditTime: 2025-04-29 21:43:36
 * @FilePath: \Seekfree_CYT4BB_Opensource_Library\project\code\imagedeal.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "imagedeal.h"

#define GrayScale 256

uint8 Find_Line_Image[IAMGE_H][IAMGE_W];  // 定义一个 80 * 60 的图像数组

bool Inverse_Flag = false;      // 逆透视标志位
bool Image_Count_Flag = false;  // 图像计数标志位
uint16 Image_Count = 0;         // 图像计数

uint8 find_extreme_num(uint8 *val, uint8 num0, uint8 num2, uint8 mode);

/**
 * 函数功能：      寻找极值
 * 特殊说明：      通过传入的数组，找到最大值或者最小值
 *
 * 形  参：        uint8 *val
 *                uint8 num0   起始位置
 *                uint8 num2   结束位置
 *                uint8 mode   1 返回最大 0 返回最小
 * 返回值：        uint8
 */
inline uint8 find_extreme_num(uint8 *val, uint8 num0, uint8 num2, uint8 mode) {
    uint8 temp = val[num0];
    for (uint8 i = num0; i < num2; i++) {
        if (mode) {
            if (val[i] > temp) {
                temp = val[i];
            }
        } else {
            if (val[i] < temp) {
                temp = val[i];
            }
        }
    }

    return temp;
}

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
    if (mt9v03x_finish_flag == 1 &&
        Inverse_Flag ==
            0)  // mt9v03x_finish_flag：逐飞库定义摄像头采集标志位，采集完一帧图像会挂1
                // Inverse_Flag：自定义逆透视标志位，挂1时会得到一张逆透视图像，根据自己需求而定
    {
        for (i = 0; i < IAMGE_H; i++) {
            for (j = 0; j < IAMGE_W; j++) {
                Find_Line_Image[i][j] = mt9v03x_image[i * 2][(
                    uint8)(j * 2.35)];  // 将188 * 120图像压缩为 80 *
                                        // 60大小，X轴比为2.35，Y轴比为2
            }
        }
        if (Image_Count_Flag ==
            1)  // Image_Count_Flag：自定义开启图像计数标志位，挂1时开启图像计数，即每采集一张图像计数+1；挂0时图像计数清零
        {
            Image_Count++;
        } else if (Image_Count_Flag == 0) {
            Image_Count = 0;
        }
        //        Image_Num ++;
        mt9v03x_finish_flag = 0;  // 注意清掉图像采集完成标志位
    } else if (mt9v03x_finish_flag == 1 && Inverse_Flag == 1) {
        for (i = 0; i < IAMGE_H; i++) {
            for (j = 0; j < IAMGE_W; j++) {
                Find_Line_Image[i][j] =
                    mt9v03x_image[i * 2][(uint8)(j * 2.35f)];
            }
        }
        // Get_Inverse_Perspective_Image(Find_Line_Image, I_Perspective_Image);
        // 逆透视处理函数，在另处详细说明
        if (Image_Count_Flag == 1) {
            Image_Count++;
        } else if (Image_Count_Flag == 0) {
            Image_Count = 0;
        }
        //        Image_Num ++;
        mt9v03x_finish_flag = 0;
    }
}

#define SEARCH_IMAGE_H 60  // 搜索图像大小
#define SEARCH_IMAGE_W 80  // 搜索图像大小
#define REFERENCEROW 5     // 参考行数
#define WHITEMAXMUL 13     // 白线最大值倍数
#define WHITEMINMUL 7      // 白线最小值倍数
#define BLACLK_POINT 50    // 黑线点
uint8 reference_point, white_max_point, white_min_point;

/**
 * 函数功能：      获取参考点
 * 特殊说明：      通过搜索图像的最下面一行的平均值作为参考点
 *                通过参考点加减一定值得到白线的最大值和最小值
 *
 * 形  参：        const uint8 *image
 * 返回值：        无
 */
void get_reference_point(const uint8 *image) {
    uint8 *p = (uint8 *)&image[SEARCH_IMAGE_H +
                               REFERENCEROW * SEARCH_IMAGE_W];  // 需要
    uint16 temp = 0;
    uint32 temp2 = 0;
    temp = REFERENCEROW * 10;
    for (int i = 0; i < REFERENCEROW; i++) {
        for (int j = 35; j < 45; j++) {
            temp2 += *(p + i * SEARCH_IMAGE_W + j);
        }
    }
    reference_point = (uint8)(temp2 / temp);  // 平均值,作为参考点
    white_max_point = (uint8)func_limit_ab(
        (int32)reference_point * WHITEMAXMUL / 10, BLACLK_POINT, 255);
    white_min_point = (uint8)func_limit_ab(
        (int32)reference_point * WHITEMINMUL / 10, BLACLK_POINT, 255);
}

#define CONTRASTOFFSET 2                   // 对比度偏移量
#define STOPROW 0                          // 停止搜索的行数
#define REFERENCEROW 50                    // 参考行数
#define SEARCHRANCE 10                     // 搜索范围
uint8 center_line[SEARCH_IMAGE_W];         // 参考列
uint8 reference_contrast_radio = 20;       // 参考对比度
uint8 reference_col;                       // 动态参考列
uint8 reference_col_line[SEARCH_IMAGE_H];  // 参考列数组
/**
 * 函数功能：      搜索图像参考列
 * 特殊说明：
 *
 *
 * 形  参：        const uint8 *image   图像数据
 * 返回值：        无
 */
void search_reference_col(const uint8 *image) {
    int col, row;
    int16 temp1 = 0, temp2 = 0, temp3 = 0;
    memset(center_line, SEARCH_IMAGE_H - 1, sizeof(uint8) * SEARCH_IMAGE_W);
    for (int col = 0; col < SEARCH_IMAGE_W; col += CONTRASTOFFSET) {
        for (int row = SEARCH_IMAGE_H - 1; row > STOPROW;
             row -= CONTRASTOFFSET) {
            temp1 = *(image + row * SEARCH_IMAGE_W + col);  // 获取灰度值
            temp2 = *(image + (row - STOPROW) * SEARCH_IMAGE_W +
                      col);  // 获取对比度灰度值

            if (temp2 > white_max_point) {  // 判断是否是白点，是的话直接跳过
                continue;
            }

            if (temp1 < white_min_point) {  // 判断是否是黑点，是的话直接赋值
                center_line[col] = (uint8)row;
                break;
            }

            temp3 = (temp1 - temp2) * 200 / (temp1 + temp2);  // 计算对比度

            if (temp3 > reference_contrast_radio ||
                row == STOPROW) {  // 判断是否超过阈值,如果高于，就纪律
                center_line[col] = (uint8)row;
                break;
            }
        }
    }
    reference_col = find_extreme_num(center_line, 10, SEARCH_IMAGE_W - 10, 0) +
                    CONTRASTOFFSET;
    reference_col = (uint8)func_limit_ab(reference_col, 1, SEARCH_IMAGE_W - 2);

    for (int i = 0; i < SEARCH_IMAGE_H; i++) {
        reference_col_line[i] = reference_col;
    }
}

int l_border[SEARCH_IMAGE_H];  // 左线数组
int r_border[SEARCH_IMAGE_H];  // 右线数组

/**
 * 函数功能：      搜索线
 * 特殊说明：      通过搜索图像的最下面一行的平均值作为参考点
 *                通过参考点加减一定值得到白线的最大值和最小值
 *
 * 形  参：        const uint8 *image
 * 返回值：        无
 */
void search_line(const uint8 *image) {
    uint8 *p = (uint8 *)&image[0];
    uint8 row_max = SEARCH_IMAGE_H - 1;               // 行最大值
    uint8 row_min = STOPROW;                          // 行最小值
    uint8 col_max = SEARCH_IMAGE_W - CONTRASTOFFSET;  // 列最大值
    uint8 col_min = CONTRASTOFFSET;                   // 列最小值
    int16 leftstartcol = reference_col;               // 左边起始列
    int16 rightstartcol = reference_col;              // 右边起始列
    uint8 leftendcol = 0;                             // 左边结束列
    uint8 rightendcol = SEARCH_IMAGE_W - 1;           // 右边结束列
    uint8 search_time = 0;                            // 搜索次数

    uint8 temp1 = 0, temp2 = 0;                      // 图像数据
    int temp3 = 0;                                   // 对比度
    int leftstop = 0, rightstop = 0, stoppoint = 0;  // lock

    int col, row;
    for (row = row_max; row >= row_min; row--) {
        l_border[row] = col_min - CONTRASTOFFSET;
        r_border[row] = col_max + CONTRASTOFFSET;
    }

    for (row = row_max; row >= row_min; row--) {
        p = (uint8 *)&image[row * SEARCH_IMAGE_W];  // 获取行数据
        if (!leftstop) {
            search_time = 2;
            do {
                if (search_time == 1) {
                    leftstartcol = reference_col;
                    leftendcol = col_min;
                }
                search_time--;
                for (col = leftstartcol; col >= leftendcol; col--) {
                    temp1 = *(p + col);                   // 获取灰度值
                    temp2 = *(p + col - CONTRASTOFFSET);  // 获取对比度灰度值

                    if (temp1 < white_min_point && col == leftendcol &&
                        leftstartcol ==
                            reference_col) {  // 判断是否是黑点，是的话直接赋值
                        leftstop = 1;         // 锁定,不在左边搜索
                        for (stoppoint = row; stoppoint >= row_min;
                             stoppoint--) {
                            l_border[stoppoint] = col_min;
                        }
                        search_time = 0;
                        break;
                    }

                    if (temp1 <
                        white_min_point) {  // 判断是否是黑点，是的话则不进行对比度直接赋值
                        l_border[row] = (uint8)col;
                        break;
                    }

                    if (temp2 >
                        white_max_point) {  // 判断是否是白点，是的话直接跳过
                        continue;
                    }

                    temp3 =
                        (temp1 - temp2) * 200 / (temp1 + temp2);  // 计算对比度

                    if (temp3 > reference_contrast_radio ||
                        col == col_min) {  // 判断是否超过阈值,如果高于，就纪律
                        l_border[row] = col - CONTRASTOFFSET;
                        leftstartcol = (uint8)func_limit_ab(col + SEARCHRANCE,
                                                            col, col_max);
                        leftendcol = (uint8)func_limit_ab(col - SEARCHRANCE,
                                                          col_min, col);
                        search_time = 0;
                        break;
                    }
                }
            } while (search_time);
        }
        if (!rightstop) {
            search_time = 2;
            do {
                if (search_time == 1) {
                    rightstartcol = reference_col;
                    rightendcol = col_max;
                }
                search_time--;
                for (col = rightstartcol; col <= rightendcol; col++) {
                    temp1 = *(p + col);                   // 获取灰度值
                    temp2 = *(p + col + CONTRASTOFFSET);  // 获取对比度灰度值

                    if (temp1 < white_min_point && col == rightendcol &&
                        rightstartcol ==
                            reference_col) {  // 判断是否是黑点，是的话直接赋值
                        rightstop = 1;        // 锁定,不在右边搜索
                        for (stoppoint = row; stoppoint >= row_min;
                             stoppoint--) {
                            r_border[stoppoint] = col_max;
                        }
                        search_time = 0;
                        break;
                    }

                    if (temp1 <
                        white_min_point) {  // 判断是否是黑点，是的话则不进行对比度直接赋值
                        r_border[row] = (uint8)col;
                        break;
                    }

                    if (temp2 >
                        white_max_point) {  // 判断是否是白点，是的话直接跳过
                        continue;
                    }

                    temp3 =
                        (temp1 - temp2) * 200 / (temp1 + temp2);  // 计算对比度

                    if (temp3 > reference_contrast_radio ||
                        col == col_max) {  // 判断是否超过阈值,如果高于，就纪律
                        r_border[row] = col + CONTRASTOFFSET;
                        rightstartcol = (uint8)func_limit_ab(col - SEARCHRANCE,
                                                             col, col_min);
                        rightendcol = (uint8)func_limit_ab(col + SEARCHRANCE,
                                                           col_max, col);
                        search_time = 0;
                        break;
                    }
                }
            } while (search_time);
        }
    }
}

uint8 left_line[IAMGE_H];
uint8 right_line[IAMGE_H];
/**
 * 函数功能：      搜索线
 * 特殊说明：      通过搜索图像的最下面一行的平均值作为参考点
 *                通过参考点加减一定值得到白线的最大值和最小值
 *
 * 形  参：        const uint8 *image
 * 返回值：        无
 */
void search_line2(void) {
    for (int i = 10; i < IAMGE_H; i += 2) {
        left_line[i] = IAMGE_W - 1;  // 初始化左线
        right_line[i] = 0;           // 初始化右线
        for (int j = 2; j < IAMGE_W; j += 2) {
            float diff =
                (float)(Find_Line_Image[i][j] - Find_Line_Image[i][j - 2]);
            float sum =
                (float)(Find_Line_Image[i][j] + Find_Line_Image[i][j - 2]);
            if (diff / sum > 0.36) {
                left_line[i] = j - 1;
                break;
            }
        }
        for (int j = IAMGE_W - 10; j > 2; j -= 2) {
            float diff =
                (float)(Find_Line_Image[i][j - 2] - Find_Line_Image[i][j]);
            float sum =
                (float)(Find_Line_Image[i][j] + Find_Line_Image[i][j - 2]);
            if (diff / sum > 0.36) {
                right_line[i] = j - 1;
                break;
            }
        }
    }
}

void image_log() {
    Copy_Zip_Image();
    // get_reference_point(&Find_Line_Image[0][0]);
    // search_reference_col(&Find_Line_Image[0][0]);
    // search_line(&Find_Line_Image[0][0]);
    search_line2();
    for (int i = 0; i < IAMGE_H; i++) {
        // Find_Line_Image[i][left_line[i]] = 255;
        Find_Line_Image[i][(left_line[i] + right_line[i]) / 2] = 255;
        // Find_Line_Image[i][right_line[i]] = 255;
    }
    ips114_show_gray_image(0, 0, (Find_Line_Image[0]), IAMGE_W, IAMGE_H,
                           (IAMGE_W), (IAMGE_H), 0);
}
