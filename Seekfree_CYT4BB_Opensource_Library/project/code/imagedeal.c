/*
 * @Author: yyx-pc 3454523412@qq.com
 * @Date: 2025-03-18 16:33:43
 * @LastEditors: yyx-pc 3454523412@qq.com
 * @LastEditTime: 2025-05-02 19:59:05
 * @FilePath: \car\Seekfree_CYT4BB_Opensource_Library\project\code\imagedeal.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "imagedeal.h"

int16 centerline[MT9V03X_H];
int16 leftline[MT9V03X_H];
int16 rightline[MT9V03X_H];
uint8 leftline_num;   // 左线点数量
uint8 rightline_num;  // 右线点数量
int16 sar_thre = 50;  // 差比和阈值

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// 逐行寻找边界点
void image_boundary_process(void) {
    uint8 row = MT9V03X_H - 1;  // 行
    // uint8 col = MT9V03X_W/2;//列
    uint8 start_col = MT9V03X_W / 2;  // 各行起点的列坐标,默认为MT9V03X_W / 2
    // 清零之前的计数
    leftline_num = 0;
    rightline_num = 0;

    difsum_right(row, MT9V03X_W - 1);
    difsum_left(row, 0);
    for (row = MT9V03X_H - 2; row > 0; row--) {
        // 选用上一行的中点作为下一行计算起始点，节省速度，同时防止弯道的左右两边均出现与画面一侧
        start_col =
            (uint8)(0.4 * centerline[row] + 0.3 * start_col +
                    0.1 *
                        MT9V03X_W);  // 一阶低通滤波，防止出现噪点影响下一行的起始点

        // 逐行作差比和
        difsum_right(row,
                     MIN(start_col + 0.6 * centerline[row], MT9V03X_W - 1));
        difsum_left(row, start_col);
        centerline[row] = 0.5 * (rightline[row] + leftline[row]);
        mt9v03x_image[row][rightline[row]] = 255;  // 画出中心线
    }
}
// 差比和寻找右侧边界点
bool difsum_right(uint8 y, uint8 x) {
    float sum, dif, sar;  // 和，差，比
    uint8 col;            // 列
    uint8 mov = 2;        // 每次作差后的移动量,默认为2，可以根据画面分辨率调整
    // 计算第x行的右边界
    rightline[y] = MT9V03X_W - 1;  // 未找到右边界时输出为0
    for (col = x; col >= mov + 1; col -= mov) {
        dif = (float)((mt9v03x_image[y][col - mov - 1] - mt9v03x_image[y][col])
                      << 8);  // 左移8位即乘256，可避免浮点数乘，加快速度
        sum =
            (float)((mt9v03x_image[y][col] + mt9v03x_image[y][col - mov - 1]));
        sar = fabs(dif / sum);  // 求取差比和
        if (sar > sar_thre) {   // 差比和大于阈值代表深浅色突变
            rightline[y] = (int16)(col - mov);
            rightline_num++;  // 右线点计数+
            return true;      // 找到边界后退出
        }
    }
    return false;  // 没有找到边界
}
// 差比和寻找左侧边界点
bool difsum_left(uint8 y, uint8 x) {
    float sum, dif, sar;  // 和，差，比
    uint8 col;            // 列
    uint8 mov = 2;        // 每次作差后的移动量,默认为2，可以根据画面分辨率调整
    // 计算第x行的左边界
    leftline[y] = 0;  // 未找到左边界时输出为187
    for (col = x; col <= MT9V03X_W - mov - 1; col += mov) {
        dif = (float)((mt9v03x_image[y][col] - mt9v03x_image[y][col + mov + 1])
                      << 8);  // 左移8位即乘256，可避免浮点数乘，加快速度
        sum =
            (float)((mt9v03x_image[y][col] + mt9v03x_image[y][col + mov + 1]));
        sar = fabs(dif / sum);  // 求取差比和
        if (sar > sar_thre) {   // 差比和大于阈值代表深浅色突变
            leftline[y] = (int16)(col + mov);
            leftline_num++;  // 左线点计数+
            return true;     // 找到边界后退出
        }
    }
    return false;  // 没有找到边界
}

// 差比和寻找上侧边界点
bool difsum_up(uint8 y, uint8 x) {
    float sum, dif, sar;  // 和，差，比
    uint8 col;            // 列
    uint8 mov = 2;        // 每次作差后的移动量,默认为2，可以根据画面分辨率调整
    for (int row = y; row >= mov + 1; row -= mov) {
        dif = (float)((mt9v03x_image[row][x] - mt9v03x_image[row + mov + 1][x])
                      << 8);  // 左移8位即乘256，可避免浮点数乘，加快速度
        sum =
            (float)((mt9v03x_image[row][x] + mt9v03x_image[row + mov + 1][x]));
        sar = fabs(dif / sum);  // 求取差比和
        if (sar > sar_thre) {   // 差比和大于阈值代表深浅色突变
            return true;        // 找到边界后退出
        }
        return false;
    }
}

uint16 imgerr;
// 圆弧处理函数
int roundabout_process() {
    uint8 row = MT9V03X_H - 1;
    uint8 col = MT9V03X_W - 1;
    int i, j = 0;
    bool left_up_flag = difsum_up(row, MT9V03X_W / 2 - 40);
    bool right_up_flag = difsum_up(row, MT9V03X_W / 2 + 40);
    if (!left_up_flag && !right_up_flag) {
        return 0;
    }
    if (left_up_flag && !right_up_flag) {
        if (difsum_left(10, 0))

            return 1;
        else
            return 0;
    } else {
        if (difsum_right(10, MT9V03X_W - 1))
            return 2;
        else
            return 0;
    }
    // 理论上不会到这里
    return 0;
}

int in_roundabout_process(int x) {
    // 行驶一段时间
    // Error = (中点坐标+X) -（右边界）
}

int Circuit Breaker {}
// 角点处理函数
int corner_process() {
    uint8 row = MT9V03X_H - 1;
    uint8 col = MT9V03X_W - 1;
    int i, j = 0;
    bool left_up_flag = difsum_up(row, 10);  // 第一次搜线
    bool right_up_flag = difsum_up(row, MT9V03X_W - 10);
    if (!left_up_flag && !right_up_flag)
        return 0;
    if (left_up_flag) {
        bool left_flag = difsum_left(row / 2, 0);
        if (left_flag) {
            return 1;  // 左边的弯
        }
    } else {
        bool right_flag = difsum_right(row / 2, MT9V03X_W - 1);
        if (right_flag) {
            return 2;  // 右边的弯
        }
    }
    // 理论上不会到这里
    return 0;
}

// 线处理函数
void line_process() {
    uint8 row = MT9V03X_H - 1;
    // 清零之前的计数
    leftline_num = 0;
    rightline_num = 0;
    imgerr = 0;
    bool left_flag = difsum_left(row, 0);  // 第一次搜线
    int i = 0;
    while (i < MT9V03X_H - 10 && !left_flag) {
        i += 10;
        left_flag = difsum_left(row - i, 0);
    }
    if (!left_flag) {
        return;
    }
    // 搜索到线
    bool right_flag = difsum_right(row - i, MT9V03X_W - 1);  // 第一次搜线
    imgerr += MT9V03X_W / 2 - (leftline[row - i] + rightline[row - i]) / 2;
    uint8 start_col = (leftline[row - i] + rightline[row - i]) / 2;
}