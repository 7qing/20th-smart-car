/*********************************************************************************************************************
* CYT2BL3 Opensourec Library ���� CYT2BL3 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� CYT2BL3 ��Դ���һ����
*
* CYT2BL3 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
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
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          main_cm4
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT2BL3
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-11-19       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "user_code.h"

// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������

// *************************** ����Ӳ������˵�� ***************************
// ���������Ҷ���������ͷ ��Ӧ��������ͷ�ӿ� ��ע������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      TXD                 �鿴 zf_device_mt9v03x.h �� MT9V03X_COF_UART_TX �궨��
//      RXD                 �鿴 zf_device_mt9v03x.h �� MT9V03X_COF_UART_RX �궨��
//      PCLK                �鿴 zf_device_mt9v03x.h �� MT9V03X_PCLK_PIN �궨��
//      VSY                 �鿴 zf_device_mt9v03x.h �� MT9V03X_VSYNC_PIN �궨��
//      D0-D7               �鿴 zf_device_mt9v03x.h �� MT9V03X_DATA_PIN �궨�� �Ӹö��忪ʼ�������˸�����
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ
// 
//      �������� SPI ������ Ӳ������
//      SCL                 �鿴 zf_device_ips200.h �� IPS200_SCL_PIN_SPI  �궨��  B0
//      SDA                 �鿴 zf_device_ips200.h �� IPS200_SDA_PIN_SPI  �궨��  B1
//      RST                 �鿴 zf_device_ips200.h �� IPS200_RST_PIN_SPI  �궨��  B2
//      DC                  �鿴 zf_device_ips200.h �� IPS200_DC_PIN_SPI   �궨��  C19
//      CS                  �鿴 zf_device_ips200.h �� IPS200_CS_PIN_SPI   �궨��  B3 
//      BL                  �鿴 zf_device_ips200.h �� IPS200_BLk_PIN_SPI  �궨��  C18
//      GND                 ���İ��Դ�� GND
//      3V3                 ���İ� 3V3 ��Դ

// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����� �����İ���������� �嵽��
// 
// 2.����ͷ�������������ͷ�ӿ� ע������1.8��TFTģ�����������Ļ�ӿ�
// 
// 3.�����ϵ� ���ߺ��İ�������Ϻ��ϵ� ���İ尴�¸�λ����
// 
// 4.��Ļ����ʾ��ʼ����ϢȻ����ʾ����ͷͼ��
// 
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�

// **************************** �������� ****************************
//wansai NEW PROJECT  compress 110 4.22 lvbo PINK


#define IPS200_TYPE     (IPS200_TYPE_SPI)                                // �������� SPI ������ ����궨����д IPS200_TYPE_SPI

#define PIT0                       		(PIT_CH0 )   
#define PIT1                       		(PIT_CH1 )     // ʹ�õ������жϱ��

uint8 yuzhi;

int main(void)
{
    clock_init(SYSTEM_CLOCK_160M); 	// ʱ�����ü�ϵͳ��ʼ��<��ر���>
    debug_info_init();                  // ���Դ�����Ϣ��ʼ��
    
    // �˴���д�û����� ���������ʼ�������
    time_para_init();
    
    //�����ʼ��
    motor_init();
    
    encoder_dir_init(ENCODER1, ENCODER1_QUADDEC_A, ENCODER1_QUADDEC_B);       // ��ʼ��������ģ�������� ����������������ģʽ
    encoder_dir_init(ENCODER2, ENCODER2_QUADDEC_A, ENCODER2_QUADDEC_B);       // ��ʼ��������ģ�������� ����������������ģʽ
    
    ips200_init(IPS200_TYPE);
    ips200_show_string(0, 0, "mt9v03x init.");
    // �˴���д�û����� ���������ʼ�������
    while(1)
    {
        if(imu660ra_init())
        {
           printf("\r\n imu660ra init error.");                                 // imu660ra ��ʼ��ʧ��
        }
        else
        {
           break;
        }       
    }
        
    while(1)
    {
        if(mt9v03x_init())
            ips200_show_string(0, 16, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // ���Ʊ�ʾ�쳣
    }

    ips200_show_string(0, 16, "init success.");
      
    yuansu_init();      

    pit_ms_init(PIT0,1);
    pit_ms_init(PIT1,1);
    while(true)
    {      
        // �˴���д��Ҫѭ��ִ�еĴ���  
        if(mt9v03x_finish_flag)
        {
//            timer_init(TC_TIME2_CH0, TIMER_US);             // ��ʱ��ʹ�� TC_TIME2_CH0 ʹ��΢�뼶����
//            timer_start(TC_TIME2_CH0);
            image_process();          
            mt9v03x_finish_flag = 0;           
        }
            image_process_2(); 
//            timer_stop(TC_TIME2_CH0);
//            ips200_show_uint(100,100,timer_get(TC_TIME2_CH0),6);
//            timer_clear(TC_TIME2_CH0);
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

// **************************** �������� ****************************
// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// 
// ����1����Ļ����ʾ
//      ���ʹ��������ԣ��������Ҫ�õ�ع��� �����Ļ�������ŵ�ѹ
//      �����Ļ�ǲ��ǲ��λ���� ������Ŷ�Ӧ��ϵ
//      �����Ӧ���Ŷ���ȷ ���һ���Ƿ������Ų��β��� ��Ҫ��ʾ����
//      �޷���ɲ��β�������һ��GPIO���̽���Ļ����IO��ʼ��ΪGPIO��ת��ƽ �����Ƿ��ܿ�
// 
// ����2����ʾ reinit ����
//      �������Ƿ�����
//      ���幩���Ƿ�ʹ�õ�������ĵ�ع���
// 
// ����2����ʾͼ������ ��λ
//      �������ͷ�ź����Ƿ����ɶ�
