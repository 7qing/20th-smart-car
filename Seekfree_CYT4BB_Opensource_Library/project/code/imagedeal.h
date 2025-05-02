/*
 * @Author: yyx-pc 3454523412@qq.com
 * @Date: 2025-03-07 20:50:53
 * @LastEditors: yyx-pc 3454523412@qq.com
 * @LastEditTime: 2025-05-02 18:41:46
 * @FilePath: \Seekfree_CYT4BB_Opensource_Library\project\code\imagedeal.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef IMAGEDEAL_H
#define IMAGEDEAL_H

#include "consant.h"
#include "zf_common_headfile.h"

extern int16 centerline[MT9V03X_H];
extern int16 leftline[MT9V03X_H];
extern int16 rightline[MT9V03X_H];
extern uint8 leftline_num;   // 左线点数量
extern uint8 rightline_num;  // 右线点数量
extern int16 sar_thre;       // 差比和阈值
extern uint8 pix_per_meter;  // 每米的像素数

void image_boundary_process(void);
bool difsum_left(uint8 y, uint8 x);
bool difsum_right(uint8 y, uint8 x);
bool difsum_up(uint8 y, uint8 x);
#endif