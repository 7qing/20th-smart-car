/*
 * @Author: yyx-pc 3454523412@qq.com
 * @Date: 2025-04-27 20:15:42
 * @LastEditors: yyx-pc 3454523412@qq.com
 * @LastEditTime: 2025-04-27 20:16:35
 * @FilePath: \car\Seekfree_CYT4BB_Opensource_Library\project\code\image.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 *
 */
#ifndef __IMAGE__H
#define __IMAGE__H

#include "zf_common_headfile.h"

#define MT9V03X_W (188)  // 图像宽度     (不可修改)
#define MT9V03X_H (120)  // 图像高度     (不可修改)
#define MT9V03X_IMAGE_SIZE \
    (MT9V03X_W * MT9V03X_H)  // 整体图像大小不能超过 65535

#define IAMGE_W (80)
#define IAMGE_H (60)
#define IMAGE_SIZE (IAMGE_W * IAMGE_H)

void image_use();
extern uint8 Find_Line_Image[IAMGE_H][IAMGE_W];

#endif