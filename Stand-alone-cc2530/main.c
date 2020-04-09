/**************************************









***************************************/
#include "ioCC2530.h"
#include <stdio.h>
#include <string.h>
#include "DHT11.h"
#include "BH1750.h"
#include "motor.h"
#include "remote.h"

#define MQ_PIN P0_6           //����P0.6��Ϊ�������������
#define Relay_PIN P0_5
#define LED P1_0

char is_serial_receive = 0;
void main(void)
{
  unsigned int temp_now,hum_now,Guang;
  unsigned char ltemp[3]; 
  unsigned char lhumidity[3];
  uchar MQ_data=1;
  uchar Window=0;
  uchar mode = 0;
  uchar j=0;
  CLKCONCMD &= ~0x40;      // ����ϵͳʱ��ԴΪ 32MHZ����
  while(CLKCONSTA & 0x40); // �ȴ������ȶ� 
  CLKCONCMD &= ~0x47;      // ����ϵͳ��ʱ��Ƶ��Ϊ 32MHZ
  
  P1DIR |= 0x01;                              // P1.0 ����LED
  LED = 1;
  
  P0DIR &= ~0x40;              //P0.6����Ϊ����� MQ-2
  P0DIR |= 0x20;          //P0.5����Ϊ�̵������
  //P0SEL &=0x7f;
  Relay_PIN = 1;          //�̵����͵�ƽ��������ʼ��Ϊ��
  Init_BH1750();          // ���նȴ�����
  InitMotor();            // ���������ʼ��
  
  EA = 0;
 
  InitKey();
  uart0_init();                               //115200
  timer1_init();
  rf_init();
 
  EA = 1;

  printf("Server start!\r\n");
  unsigned char senddata[16];
  while(1)
  {
    memset(ltemp, 0, 2);
    memset(lhumidity, 0, 2);
    memset(senddata, 0, 16);
    DHT11();             //��ȡ��ʪ��
    
    temp_now=wendu_shi*10+wendu_ge;
    hum_now=shidu_shi*10+shidu_ge;
      
    ltemp[0]=wendu_shi+0x30;
    ltemp[1]=wendu_ge+0x30;
    ltemp[2]='\0';
    lhumidity[0]=shidu_shi+0x30;
    lhumidity[1]=shidu_ge+0x30;
    lhumidity[2]='\0';
   
   //unsigned char *ntemp=(unsigned char*)temp_now;
   //unsigned char *nhum=(unsigned char*)hum_now;
   
    //��ʪ�ȷ���
   strcpy(senddata,"SD01");
   strcat(senddata,ltemp);
   strcat(senddata,lhumidity);
   strcat(senddata,"00000000");
   uart0_sendbuf(senddata,16);
   Delay_ms(200); 
   /*
   uart0_sendbuf("temp: ",6);
   uart0_sendbuf(ltemp,2);
   uart0_sendbuf(" hum: ",6);
   uart0_sendbuf(lhumidity,2);
   uart0_sendbuf("\n",1);
   */
   //Delay_ms(2000);  //��ʱ��2S��ȡ1�� 
   
   
   
      uart0_flush_rxbuf();
    }
  }


