/*
 * @Author: yyx-pc 3454523412@qq.com
 * @Date: 2025-03-05 19:36:03
 * @LastEditors: yyx-pc 3454523412@qq.com
 * @LastEditTime: 2025-03-07 21:02:46
 * @FilePath: \Seekfree_CYT4BB_Opensource_Library\project\code\Inertial_nav.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "inertial_nav.h"
#include "consant.h"

NavState inertial_nav_state;

void inertial_nav_dead_reckoning(){
    int16_t curr_left_encoder = encoder_get_count(TC_CH27_ENCODER);
    int16_t error_left_value = curr_left_encoder - inertial_nav_state.last_left_encoder;
    inertial_nav_state.last_left_encoder = curr_left_encoder;

    int16_t curr_right_encoder = encoder_get_count(TC_CH07_ENCODER);
    int16_t error_right_value = curr_right_encoder - inertial_nav_state.last_right_encoder;
    inertial_nav_state.last_right_encoder = curr_right_encoder;

    float delta_distance_left = (float)error_left_value * (2*PI*WHEEL_RADIUS/WHEEL_RADIUS);
    float delta_distance_right = (float)error_right_value * (2*PI*WHEEL_RADIUS/WHEEL_RADIUS);

    float omega_z =imu660ra_gyro_transition(imu660ra_gyro_z);
    
    //定期矫准
    inertial_nav_state.theta += omega_z * NAV_DT;

  
    inertial_nav_state.x += (delta_distance_left + delta_distance_right) * cosf(inertial_nav_state.theta) / 2;
    inertial_nav_state.y += (delta_distance_left + delta_distance_right) * sinf(inertial_nav_state.theta) / 2;

}