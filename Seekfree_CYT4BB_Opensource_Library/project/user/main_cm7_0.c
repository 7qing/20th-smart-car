/*********************************************************************************************************************
 * CYT4BB Opensourec Library ���� CYT4BB ��Դ�⣩��һ�����ڹٷ� SDK
 *�ӿڵĵ�������Դ�� Copyright (c) 2022 SEEKFREE ��ɿƼ�
 *
 * ���ļ��� CYT4BB ��Դ���һ����
 *
 * CYT4BB ��Դ�� ��������
 * �����Ը��������������ᷢ���� GPL��GNU General Public License����
 *GNUͨ�ù������֤�������� �� GPL �ĵ�3�棨��
 *GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
 *
 * ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
 * ����û�������������Ի��ʺ��ض���;�ı�֤
 * ����ϸ����μ� GPL
 *
 * ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
 * ���û�У������<https://www.gnu.org/licenses/>
 *
 * ����ע����
 * ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
 * �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt
 *�ļ��� ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
 * ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
 *
 * �ļ�����          main_cm7_0
 * ��˾����          �ɶ���ɿƼ����޹�˾
 * �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
 * ��������          IAR 9.40.1
 * ����ƽ̨          CYT4BB
 * ��������          https://seekfree.taobao.com/
 *
 * �޸ļ�¼
 * ����              ����                ��ע
 * 2024-1-4       pudding            first version
 ********************************************************************************************************************/

#include "Inertial_nav.h"
#include "imagedeal.h"
#include "zf_common_headfile.h"
// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

// **************************** �������� ****************************
// init
void ALL_init(bool encoderflag, bool imuflag, bool wirelessflag,
              bool ips114flag, bool mt9v03xflag) {
    if (encoderflag) {
        encoder_dir_init(TC_CH58_ENCODER, TC_CH58_ENCODER_CH1_P17_3,
                         TC_CH58_ENCODER_CH2_P17_4);  // ������
        encoder_dir_init(TC_CH27_ENCODER, TC_CH27_ENCODER_CH1_P19_2,
                         TC_CH27_ENCODER_CH2_P19_3);
    }
    if (imuflag) {
        imu660ra_init();  // ������
    }
    if (wirelessflag) {
        wireless_uart_init();  // ���ߴ���
    }
    if (ips114flag) {
        ips114_init();
        ips114_set_font(IPS114_8X16_FONT);
    }
    if (mt9v03xflag) {
        mt9v03x_init();
    }
}

int main(void) {
    clock_init(SYSTEM_CLOCK_250M);  // ʱ�����ü�ϵͳ��ʼ��<��ر���>
    debug_init();                   // ���Դ�����Ϣ��ʼ��
    // �˴���д�û����� ���������ʼ�������
    ALL_init(true  // encoderflag
             ,
             true  // imuflag
             ,
             false  // wirelessflag
             ,
             true,   // ips114flag
             true);  // mt9v03xflag
    // char strff[21];
    // sprintf(strff,"%d",imu660ra_gyro_x );
    ips114_show_string(0, 0, "seekfree");
    // �˴���д�û����� ���������ʼ�������
    while (true) {
        // �˴���д��Ҫѭ��ִ�еĴ���
        image_log();
        // ips114_show_float(0,0,(double)imu660ra_gyro_transition(imu660ra_gyro_x),2,2);
        // ips114_show_uint                (0,0,
        // encoder_get_count(TC_CH58_ENCODER), 4); ips114_show_uint (0,17,
        // encoder_get_count(TC_CH27_ENCODER), 4);
        //  �˴���д��Ҫѭ��ִ�еĴ���
    }
}

// **************************** �������� ****************************
