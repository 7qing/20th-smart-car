/*
 * @Author: yyx-pc 3454523412@qq.com
 * @Date: 2025-03-05 19:35:57
 * @LastEditors: yyx-pc 3454523412@qq.com
 * @LastEditTime: 2025-03-07 21:00:56
 * @FilePath: \Seekfree_CYT4BB_Opensource_Library\project\code\Inertial_nav.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AEin
 */
/*
 * @Author: yyx-pc 3454523412@qq.com
 * @Date: 2025-03-05 19:35:57
 * @LastEditors: yyx-pc 3454523412@qq.com
 * @LastEditTime: 2025-03-05 19:47:20
 * @FilePath: \Seekfree_CYT4BB_Opensource_Library\project\code\Inertial_nav.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef INERTIAL_NAV_H
#define INERTIAL_NAV_H

#include "zf_common_headfile.h"


typedef struct
{
    float x;
    float y;
    float theta;
    int32_t last_left_encoder;
    int32_t last_right_encoder;
}NavState;

extern NavState inertial_nav_state;

//航位推算法
void inertial_nav_dead_reckoning();

#endif