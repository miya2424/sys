/*
******************************************************************************************
版权所有 (C)，2012－2015，泉州禾逸电子有限公司 
----------------------------------------------------------------------------------------  
 源程序文件名 ：      Main.c  
 源程序名称：        主程序的源程序文件 程序版本：　　      1.0  
 STM8库函数版本 ：　  V1.1.1  
 程序功能 ：  
 　　　　本程序是主程序的源程序文件。  
 程序说明 ：  
　　　　  
 编译工具软件 ：       
 链接子程序文件 ：  
 编作者 ：             
 编作者 E-Mail：      
 编制日期 ：           
----------------------------------------------------------------------------------------  
 适用器件芯片类型 :    
 器件芯片时钟频率 :   
 存储器模式：        大存储器模式 外部扩展存储器大小：0 【字节】 数据堆栈大小：      32 【?

----------------------------------------------------------------------------------------  
 源程序版本历史 ： 20年月日 --------  
 版本 1.0 ：发行版本  
******************************************************************************************
**  
*/ 

/*
==========================================================================================
 调试声明 ：  
==========================================================================================
*/  

/*
==========================================================================================
 　　　　　　　　　　　     本源程序包括的头文件和其他源程序文件  
==========================================================================================
*/  
#include "stm32f4xx.h"                           // STM32库函数的头文件       
#include "Main.h"                                // 主程序的头文件     
#include "String.h"                              // 字符串函数的头部文件  
#include <stdio.h>

#include "Timer1.h"                        // Timer3处理的头文件
#include "Timer3.h"                        // Timer3处理的头文件
#include "Timer4.h"                        // Timer4处理的头文件
#include "Usart1deal.h"                    // 串口1发送数据处理的头文件
#include "Usart2deal.h"                    // 串口2处理的头文件
#include "Wifi.h"                          // Timer3处理的头文件
#include "main.h"
#include "Mqtt.h"                          // Timer3处理的头文件
#include "Leddeal.h"                       // LED处理的头文件
#include "Keydeal.h"                           // 按键处理的头文件
#include "DataMath.h"                          // 本程序的头部文件
#include "delay.h"
#include "sys.h"

/*
==========================================================================================
                             本源程序文件内部使用的函数原型声明  
==========================================================================================
*/  

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/*
==========================================================================================
 主程序的全局变量定义 ：  
==========================================================================================
*/  
/*--------------------------------------------------------------------------------------  
 Tick延时全局变量定义  
--------------------------------------------------------------------------------------*/  
vu32        TimingDelay;  
int         CDTime= 0;               //记录开关倒计时时间

/*--------------------------------------------------------------------------------------
系统运行全局标志的全局变量定义：
--------------------------------------------------------------------------------------*/
char  *CMD1 = "APP+OneSW=C";           // 开关控制命令，状态翻转， 开->关 关->开
char  *CMD2 = "APP+OneSW=?";           // 开关查询命令，回复服务器当前开关状态
char  *CMD3 = "APP+OneSWCD";           // 设置开关倒计时

/*--------------------------------------------------------------------------------------  
 中央处理器运行任务标志的全局变量定义 ：  
--------------------------------------------------------------------------------------*/  
/*  
******************************************************************************************
**  
                             　       程序指令代码清单  
******************************************************************************************
**  
*/  
/*
==========================================================================================
 函数功能：    本函数是主程序函数。  
==========================================================================================
*/  
int main(void)
{
  
/*--------------------------------------------------------------------------------------  
在下面添加所有的初始化函数  
--------------------------------------------------------------------------------------*/  
  SystemInit();
  
  NVIC_Configuration();                       // nvic的初始化
  
  delay_init(180);
 
  Delay1ms(1000);
  Delay1ms(1000);  
/*
==========================================================================================
 函数功能：   初始化一次
==========================================================================================
*/  
 
	
  
  PrintUsart1_Init();    // 串口1 初始化 用于打印调试
 
  EspAtUsart2_Init();   // 串口2初始化，用于AT指令发送
  u1_printf("系统开始运行，初始化参数，请等候...\r\n"); 

  Timer4_Init(300,8400-1);                    // TIM4初始化，定时时间 30ms

  LED_Init( );               // LED初始化
 
  Key_Init( );               // 按键初始化

  WiFiResetIO_Init( );      // 初始化WIFI的复位IO

  MqttBuff_Init( );          // MQTT绥存区初始化

  AliIotParameter_Init( );  // 初始化连接阿里云IoT平台MQTT服务器的参数		

  Delay1ms(1000);
  u1_printf("系统初始化成功...\r\n"); 

  
/*--------------------------------------------------------------------------------------  
 打开看门狗定时器和设置看门狗定时器间隔定时时间 ：  
--------------------------------------------------------------------------------------*/  
  /* LSI的启动 */  
  RCC_LSICmd(ENABLE); //打开 LSI  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET); //等待直到 LSI稳定  

  /*
  IWDG timeout equal to 480 ms (the timeout may varies due to LSI frequencydispersion) */  
#ifdef  cSysWdtEnable  
  /* Enable write access to IWDG_PR and IWDG_RLR registers */  
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  
 
  /* IWDG counter clock: 40KHz(LSI) / 64 = 0.625 KHz = 1.6ms*/  
  IWDG_SetPrescaler(IWDG_Prescaler_128);  
 
  /* Set counter reload value to 300*1.6ms = 480ms */  
  IWDG_SetReload(2000);  
 
  /* Reload IWDG counter */  
  IWDG_ReloadCounter();  
 
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */  
  IWDG_Enable();  
#endif   

/*--------------------------------------------------------------------------------------  
 在下面添加所有的初始化后需要打开中断 ，且只运行１次的函数  
--------------------------------------------------------------------------------------*/  
 
  while(1)
  {
/*--------------------------------------------------------------------------------------  
在下面添加所有需不断循环运行的函数  
--------------------------------------------------------------------------------------*/  
#ifdef  cSysWdtEnable  
  IWDG_ReloadCounter();  
#endif  

		/*--------------------------------------------------------------------*/
		/*   Connect_flag=1同服务器建立了连接,我们可以发布数据和接收推送了    */
		/*--------------------------------------------------------------------*/
    if(gbitMqtt.ConnectFlag == 1)
    {
			/*-------------------------------------------------------------*/
			/*                     处理发送缓冲区数据                      */
			/*-------------------------------------------------------------*/
      if(gstvMqtt.TxDataOutPtr != gstvMqtt.TxDataInPtr)
      {
        /*
         * 0x10: 连接报文 
         * 0x82: 订阅报文 
         * SubcribePack_flag置位，说明连接和订阅成功
         */
        if((gstvMqtt.TxDataOutPtr[1] == 0x10) 
          ||((gstvMqtt.TxDataOutPtr[1] == 0x82)
          &&(gbitMqtt.ConnectPackFlag == SET))
          ||(gbitMqtt.SubcribePackFlag ==SET))
        {
					u1_printf("发送数据:0x%x\r\n",gstvMqtt.TxDataOutPtr[1]);  //串口提示信息
          MqttTxData(gstvMqtt.TxDataOutPtr);  // 发送数据
          gstvMqtt.TxDataOutPtr += cMqttBuffSize;
          if(gstvMqtt.TxDataOutPtr == gstvMqtt.TxDataEndPtr)
            gstvMqtt.TxDataOutPtr = gstvMqtt.TxDataBuf[0];
        }
      }

			/*-------------------------------------------------------------*/
			/*                     处理接收缓冲区数据                      */
			/*-------------------------------------------------------------*/
      if(gstvMqtt.RxDataOutPtr != gstvMqtt.RxDataInPtr)
      {
        u1_printf("接收到数据:");
				/*-----------------------------------------------------*/
				/*                    处理CONNACK报文                  */
				/*-----------------------------------------------------*/		
        /*
         * 如果接收了4个字节，第一个字节是0x20，表示收到的是CONNACK报文
         */
        if((gstvMqtt.RxDataOutPtr[0] == 4) 
          &&(gstvMqtt.RxDataOutPtr[1] == 0x20))
        {
          switch(gstvMqtt.RxDataOutPtr[4])
          {
            case 0x00:
              u1_printf("CONNECT报文成功\r\n");
              gbitMqtt.ConnectPackFlag = 1;
              break;
            case 0x01:
              u1_printf("连接已拒绝，不支持的协议版本，准备重启\r\n");
              gbitMqtt.ConnectPackFlag = CLEAR;
              break;
            case 0x02:
              u1_printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n");
              gbitMqtt.ConnectPackFlag = CLEAR;
              break;
            case 0x03:
              u1_printf("连接已拒绝，服务端不可用，准备重启\r\n");
              gbitMqtt.ConnectPackFlag = CLEAR;
              break;
            case 0x04:
              u1_printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");
              gbitMqtt.ConnectPackFlag = CLEAR;
              break;
            case 0x05:
              u1_printf("连接已拒绝，未授权，准备重启\r\n"); 
              gbitMqtt.ConnectPackFlag = CLEAR;
              break;
            default:
              u1_printf("连接已拒绝，未知状态，准备重启\r\n"); 
              gbitMqtt.ConnectPackFlag = CLEAR;
              break;
          }
        }    
        /* 如果一共接收到了5个字节，第一个字节是0x90,表示收到的是suback报文 */
        else if((gstvMqtt.RxDataOutPtr[0] == 5) 
               &&(gstvMqtt.RxDataOutPtr[1] == 0x90))
        {
          switch(gstvMqtt.RxDataOutPtr[5])
          {
            case 0x00:
            case 0x01:
              u1_printf("订阅成功\r\n");
              gbitMqtt.SubcribePackFlag = SET;
              gstvMqtt.PingFlag = 0;
              TIM3_ENABLE_30S( );   // 启动30S的定时器
              break;
            default:
              u1_printf("订阅失败，准备重启\r\n");
              gbitMqtt.ConnectFlag = CLEAR;
          }
        } 
        /* 如果接收到了2个字节，第一个字节是0xd0，表示收到的PINGRESP报文 */
        else if((gstvMqtt.RxDataOutPtr[0] == 0X02)
          &&(gstvMqtt.RxDataOutPtr[1] == 0xd0))
        {
          u1_printf("PING报文回复\r\n");
          if(gstvMqtt.PingFlag == 1)
          {
            gstvMqtt.PingFlag = 0;
          }
          else
          {
            gstvMqtt.PingFlag = 0;
            TIM3_ENABLE_30S( );
          }
        }
        /* 如果接收到的第一个字节是0x30,表示收到的是服务器发来的推送数据 */
        else if(gstvMqtt.RxDataOutPtr[1] == 0x30)
        {
          u1_printf("服务器等级0推送\r\n");
          MQTTRecDealPushdataQs0(gstvMqtt.RxDataOutPtr);
        }

        gstvMqtt.RxDataOutPtr += cMqttBuffSize;
        if(gstvMqtt.RxDataOutPtr == gstvMqtt.RxDataEndPtr)
        {
          gstvMqtt.RxDataOutPtr = gstvMqtt.RxDataBuf[0];
        }
      }
      
			/*-------------------------------------------------------------*/
			/*                     处理命令缓冲区数据                      */
			/*-------------------------------------------------------------*/
      if(gstvMqtt.CmdDataOutPtr != gstvMqtt.CmdDataInPtr)
      {
        gstvMqtt.CmdDataOutPtr[gstvMqtt.CmdDataOutPtr[0]+1] = '\0';
        u1_printf("命令:%s\r\n", &gstvMqtt.CmdDataOutPtr[1]);
        if(!memcmp(&gstvMqtt.CmdDataOutPtr[1], CMD1, strlen(CMD1)))  //判断指令，如果是CMD1
        {
          LED1_OUT(!LED1_IN_STA);
          LED_CD_State( );
        }
        else if(!memcmp(&gstvMqtt.CmdDataOutPtr[1], CMD2, strlen(CMD1)))   //判断指令，如果是CMD2，回复开关状态
        {
          LED_CD_State( );
        }
        else if(!memcmp(&gstvMqtt.CmdDataOutPtr[1], CMD3, strlen(CMD1)))   //判断指令，如果是CMD3，设置倒计时时间 
        {
          CDTime = gstvMqtt.CmdDataOutPtr[12];   // 记录倒计时时间
          if(CDTime != 0)
          {
            u1_printf("倒计时:%d分钟\r\n",CDTime);   
            TIM1Enable60s( );
          }
          else
          {
            u1_printf("取消倒计时\r\n");
            TIM_Cmd(TIM1, DISABLE);
          }
          LED_CD_State();
        }
        else
        {
          u1_printf("未知指令\r\n");
        }

        gstvMqtt.CmdDataOutPtr += cMqttBuffSize;
        if(gstvMqtt.CmdDataOutPtr == gstvMqtt.CmdDataEndPtr)
        {
          gstvMqtt.CmdDataOutPtr = gstvMqtt.CmdDataBuf[0];
        }
      }
    }
    else
    {
      u1_printf("需要连接服务器...\r\n");
      TIM_Cmd(TIM4, DISABLE);
      TIM_Cmd(TIM3, DISABLE);

      Wifi_RxCounter = 0;
      memset(Wifi_RxBuf, 0, Wifi_RxBuff_Size);
      if(WifiConnectIoTServer( ) == 0)
      {
        u1_printf("建立TCP连接成功...\r\n");   
        gbitMqtt.ConnectFlag = SET;

        Wifi_RxCounter = 0;
        memset(Wifi_RxBuf, 0, Wifi_RxBuff_Size);
        MqttBufRe_Init( );       // 重新初始化
      }
    }    
  }
}

/*-------------------------------------------------*/
/*函数名：判断开关和倒计时状态，并发布给服务器     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_CD_State(void)
{
  char temp[20];

  MemClearSram(&temp, 20);
 
  sprintf(temp, "+OneSW:");     // 构建回复数据
 
  if(LED1_IN_STA) 
   temp[7] = '0';
  else
   temp[7] = '1';
  temp[8] = CDTime;
 
  MqttSendPublishQs0(gstvDevice.PubTopicName, temp, 9);
}


/*--------------------------------------------------------------------------------------  
 函数名称：    nvic的初始化。  
 函数功能：    本函数用于nvic的初始化操作。  
 函数入口参数 ：没有  
 函数返回值 ：  没有  
 调用函数：　　１．  
　　　　　　　　　  
 备注：        NVIC_PriorityGroup_2   
               本函数仅在主函数中调用一次即可 。  
--------------------------------------------------------------------------------------*/  
void NVIC_Configuration(void)
{
  /* Configure the NVIC Preemption Priority Bits */   
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}


/*--------------------------------------------------------------------------------------  
 函数名称：    延时100us基数  
 函数功能：    本函数用于系统延时100us基数。  
 函数入口参数 ：没有  
 函数返回值 ：  没有  
 调用函数：　　１．  
　　　　　　　　　  
 备注：         
--------------------------------------------------------------------------------------*/  
void Delay50us(vu32 Timing100usCnt)
{  
  u32 i;
  for(i=0; i<Timing100usCnt; i++)
  {
    delay_us(50);
    #ifdef  cSysWdtEnable  
    IWDG_ReloadCounter();  
    #endif  
  }
}  

/*--------------------------------------------------------------------------------------
函数名称：　　延时100us程序
函数功能：　　本函数用于延时100us程序。
函数入口参数：
函数返回值：　没有
函数出口参数：?
调用函数：　　１．
备注：        
--------------------------------------------------------------------------------------*/
void  Delay1ms(u32 DelayCnt)
{
  u32 i;
  for(i=0; i<DelayCnt; i++)
  {
    delay_us(1000);
    #ifdef  cSysWdtEnable  
    IWDG_ReloadCounter();  
    #endif  
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {
  }
  return ch;
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/  


