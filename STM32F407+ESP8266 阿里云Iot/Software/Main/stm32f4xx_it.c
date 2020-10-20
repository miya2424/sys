/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "Timer1.h"                        // Timer3处理的头文件
#include "Timer3.h"                        // Timer3处理的头文件
#include "Timer4.h"                        // Timer4处理的头文件
#include "Usart1deal.h"                    // 串口1发送数据处理的头文件
#include "Usart2deal.h"                    // 串口2处理的头文件
#include "Wifi.h"                          // Timer3处理的头文件
#include "main.h"
#include "Mqtt.h"                          // Timer3处理的头文件
#include "Leddeal.h"                       // LED处理的头文件
#include "DataMath.h"                         // 本程序的头部文件


/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
 
}


/*-------------------------------------------------*/
/*函数名：串口2接收中断函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{          
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    if(gbitMqtt.ConnectFlag == 0)  // 没有连接服务器
    {
      if(USART2->DR)   // 非0
      {
        gstvEspAt.RxBuff[gstvEspAt.RxCounter] = USART2->DR;
        gstvEspAt.RxCounter++;
      }
    }
    else
    {
      /* 连接服务器 */
      gstvEspAt.RxBuff[gstvEspAt.RxCounter] = USART2->DR;
      if(gstvEspAt.RxCounter == 0)  // 接收到第一个数据
      {
        /* 启动定时器 */
        TIM_Cmd(TIM4, ENABLE);
      }
      else
      {
        TIM_SetCounter(TIM4, 0); 
      }
      gstvEspAt.RxCounter ++;
    }
  }
} 

/*-------------------------------------------------*/
/*函数名：定时器4中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
  /* 每30ms处理一次数据 */
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    MemCopyFlashToSram(&gstvEspAt.RxBuff[0], &gstvMqtt.RxDataInPtr[1], gstvEspAt.RxCounter);
    gstvMqtt.RxDataInPtr[0] = gstvEspAt.RxCounter;
    gstvMqtt.RxDataInPtr += cMqttBuffSize;
    if(gstvMqtt.RxDataInPtr == gstvMqtt.RxDataEndPtr)
    {
      gstvMqtt.RxDataInPtr = gstvMqtt.RxDataBuf[0];
    }
    gstvEspAt.RxCounter = 0;
		TIM_SetCounter(TIM3, 0);                                 //清零定时器6计数器，重新计时ping包发送时间
		TIM_Cmd(TIM4, DISABLE);                        				  //关闭TIM4定时器
		TIM_SetCounter(TIM4, 0);                        			  //清零定时器4计数器
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			  //清除TIM4溢出中断标志 	
  }
}

/*-------------------------------------------------*/
/*函数名：定时器3中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    switch(gstvMqtt.PingFlag)
    {
      case 0:
        MqttSendPingReq();
        break;
      case 1:
        TIM3_ENABLE_2S();
        MqttSendPingReq();
        break;
      case 2:
      case 3:
      case 4:
        MqttSendPingReq();
        break;
      case 5:
        gbitMqtt.ConnectFlag = 0;
        TIM_Cmd(TIM3, DISABLE);
        break;
    }
    gstvMqtt.PingFlag++;
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}

/*-------------------------------------------------*/
/*函数名：定时器1中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM1_UP_TIM10_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
  {
    CDTime--;
    if(CDTime == 0)
    {
			LED1_OUT(!LED1_IN_STA);                 
			LED_CD_State();                           
			TIM_Cmd(TIM1,DISABLE);                
    }
    else
    {
      LED_CD_State();
    }
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 
  }
}



/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
