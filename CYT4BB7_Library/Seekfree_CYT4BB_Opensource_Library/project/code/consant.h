/*
 * @Author: yyx-pc 3454523412@qq.com
 * @Date: 2025-03-05 19:59:50
 * @LastEditors: yyx-pc 3454523412@qq.com
 * @LastEditTime: 2025-03-07 21:23:28
 * @FilePath: \Seekfree_CYT4BB_Opensource_Library\project\code\consant.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef CONSANT_H
#define CONSANT_H

#define WHEEL_RADIUS 0.0325 //车轮半径
#define Lines_Per_Revolution 1024  // 编码器每圈线数
#define NAV_DT 0.01f    // 导航周期


#define MT9V03X_W               ( 188 )                                         // 图像宽度     (不可修改)
#define MT9V03X_H               ( 120 )                                         // 图像高度     (不可修改)
#define MT9V03X_IMAGE_SIZE      ( MT9V03X_W * MT9V03X_H )                       // 整体图像大小不能超过 65535
#define IAMGE_W                 ( 80 )
#define IAMGE_H                 ( 60 )
#define IMAGE_SIZE      ( IAMGE_W * IAMGE_H )   
#endif