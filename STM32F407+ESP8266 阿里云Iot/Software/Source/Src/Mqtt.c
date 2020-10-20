#include "Mqtt.h"                        // Timer3处理的头文件
#include "DataMath.h"                    // 本程序的头部文件
#include "Mystring.h"                    // 本程序的头部文件
#include "utils_hmac.h"                  // 包含需要的头文件
#include "Usart1deal.h"                  // 串口1发送数据处理的头文件
#include "string.h"                      // 包含需要的头文件
#include "stdio.h"                       // 包含需要的头文件


/*==========================================================================================
下面这三条指令是用于区分编译系统为 C++ 或 C 的预处理指令。
"__cplusplus" 为 C++ 预处理器名字，这三条指令表明接在其下面的那些指令为 C 程序特征。
==========================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


const DeviceMsgStru cDeviceMsgDefault[] = 
{
  "a1vqTyIII6X",
  0x0B,
  "D001",
  0x04,
  "6f0e8a03fd4366b5954cf1897b287ea7",
  0x20,
  "/sys/a1vqTyIII6X/D001/thing/event/property/set",
  "/sys/a1vqTyIII6X/D001/thing/event/property/post",
};






/*==========================================================================================
                                        全局常量定义
==========================================================================================*/


/*==========================================================================================
                                        全局变量定义
==========================================================================================*/
/*--------------------------------------------------------------------------------------
MQTT状态标书全局变量声明:
--------------------------------------------------------------------------------------*/
MqttStateBits  gbitMqtt;

/*--------------------------------------------------------------------------------------
MQTT数据的全局数据结构的全局变量声明：
--------------------------------------------------------------------------------------*/
MqttDataStru gstvMqtt;

/*--------------------------------------------------------------------------------------
设备信息数据的全局数据结构的全局变量声明：
--------------------------------------------------------------------------------------*/
DeviceMsgStru gstvDevice;

/*--------------------------------------------------------------------------------------
网络信息数据的全局数据结构的全局变量声明：
--------------------------------------------------------------------------------------*/
UserNetMsgStru gstvUserNet;



/*==========================================================================================
                             本源程序文件内部使用的局部变量定义
==========================================================================================*/

/*
********************************************************************************************
                             　       程序指令代码清单
********************************************************************************************
*/
/*----------------------------------------------------------*/
/*函数名：初始化接收,发送,命令数据的 缓冲区 以及各状态参数  */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void MqttBuff_Init(void)
{
  MqttDataStru  *pMqtt;

  pMqtt = &gstvMqtt;

	MemClearSram(&gbitMqtt,     sizeof(MqttStateBits));
	MemClearSram2(&gstvMqtt,    sizeof(MqttDataStru));
	MemClearSram2(&gstvDevice,  sizeof(DeviceMsgStru));
	MemClearSram2(&gstvUserNet, sizeof(UserNetMsgStru));

  pMqtt->RxDataInPtr  = pMqtt->RxDataBuf[0];                // 指向发送缓冲区存放数据的指针归位
  pMqtt->RxDataOutPtr = pMqtt->RxDataInPtr;                  // 指向发送缓冲区读取数据的指针归位
  pMqtt->RxDataEndPtr = pMqtt->RxDataBuf[cMqttRxBufNum - 1]; // 指向发送缓冲区结束的指针归位

  pMqtt->TxDataInPtr  = pMqtt->TxDataBuf[0];                // 指向发送缓冲区存放数据的指针归位
  pMqtt->TxDataOutPtr = pMqtt->TxDataInPtr;                  // 指向发送缓冲区读取数据的指针归位
  pMqtt->TxDataEndPtr = pMqtt->TxDataBuf[cMqttTxBufNum - 1]; // 指向发送缓冲区结束的指针归位

  pMqtt->CmdDataInPtr  = pMqtt->CmdDataBuf[0];                 // 指向发送缓冲区存放数据的指针归位
  pMqtt->CmdDataOutPtr = pMqtt->CmdDataInPtr;                  // 指向发送缓冲区读取数据的指针归位
  pMqtt->CmdDataEndPtr = pMqtt->CmdDataBuf[cMqttTxBufNum - 1]; // 指向发送缓冲区结束的指针归位
}


/*----------------------------------------------------------*/
/*函数名：重新初始化接收,发送,命令缓冲区 以及各状态参数     */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void MqttBufRe_Init(void)
{
  MqttDataStru  *pMqtt;
  
  u8  *MqttTxDataTempPtr;       // 指向发送缓冲区位置的临时指针

  pMqtt = &gstvMqtt;

  if(pMqtt->TxDataOutPtr != pMqtt->TxDataInPtr) // if成立的话，说明发送缓冲区有数据了
  {
    pMqtt->TxDataOutPtr = pMqtt->TxDataInPtr;
    if(pMqtt->TxDataOutPtr == pMqtt->TxDataBuf[0])
    {
      pMqtt->TxDataOutPtr = pMqtt->TxDataBuf[cMqttTxBufNum - 4];
    }
    else if(pMqtt->TxDataOutPtr == pMqtt->TxDataBuf[1])
    {
      pMqtt->TxDataOutPtr = pMqtt->TxDataBuf[cMqttTxBufNum - 3];
    }
    else if(pMqtt->TxDataOutPtr == pMqtt->TxDataBuf[2])
    {
      pMqtt->TxDataOutPtr = pMqtt->TxDataBuf[cMqttTxBufNum - 2];
    }
    else
    {
      pMqtt->TxDataOutPtr -= cMqttBuffSize;  // OUT指针上移一个单元
      pMqtt->TxDataOutPtr -= cMqttBuffSize;  // OUT指针上移一个单元
      pMqtt->TxDataOutPtr -= cMqttBuffSize;  // OUT指针上移一个单元
    }

    MqttTxDataTempPtr = pMqtt->TxDataInPtr;
    pMqtt->TxDataInPtr = pMqtt->TxDataOutPtr;
    MqttSendConnectPack( );                           // 发送连接报文
    MqttSendSubscribe(gstvDevice.SubTopicName, 0);  //发送添加订阅topic，等级0			
    pMqtt->TxDataInPtr = MqttTxDataTempPtr;
  }
  else
  {
    MqttSendConnectPack( );                           // 发送连接报文
    MqttSendSubscribe(gstvDevice.SubTopicName, 0);  //发送添加订阅topic，等级0			
  }   
  gbitMqtt.ConnectPackFlag = CLEAR;
  gbitMqtt.SubcribePackFlag = CLEAR;
  gstvMqtt.PingFlag = 0;
}


/*----------------------------------------------------------*/
/*函数名：阿里云初始化参数，得到客户端ID，用户名和密码      */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void AliIotParameter_Init(void)
{
  u8 temp[128];
  
  UserNetMsgStru  *pUserNet;
  DeviceMsgStru   *pDevice;

  pUserNet = &gstvUserNet;
  pDevice  = &gstvDevice;
  
	MemClearSram2(&gstvUserNet, sizeof(UserNetMsgStru));
  MemCopyFlashToSram2(&cDeviceMsgDefault, &gstvDevice, sizeof(DeviceMsgStru));

  sprintf((char *)pUserNet->ClientId, "%s|securemode=3,signmethod=hmacsha1|", pDevice->DeviceName);
  pUserNet->ClientIdLen = strlen((char *)pUserNet->ClientId);

	sprintf((char *)pUserNet->UserName, "%s&%s", pDevice->DeviceName, pDevice->ProductKey);                     
	pUserNet->UserNameLen = strlen((char *)pUserNet->UserName);                                      
	
	sprintf((char *)temp, "clientId%sdeviceName%sproductKey%s", pDevice->DeviceName, pDevice->DeviceName, pDevice->ProductKey);   
	utils_hmac_sha1((char *)temp, strlen((char *)temp), (char *)pUserNet->Passward, (char *)pDevice->DeviceSecre, pDevice->DeviceSecreLen);               
	pUserNet->PasswardLen = strlen((char *)pUserNet->Passward);                                                    
	
	sprintf((char *)pUserNet->ServerIp, "%s.iot-as-mqtt.cn-shanghai.aliyuncs.com", pDevice->ProductKey);                  
	pUserNet->ServerPort = 1883;                          
	
	u1_printf("服 务 器：%s:%d\r\n", pUserNet->ServerIp,pUserNet->ServerPort); 
	u1_printf("客户端ID：%s\r\n", pUserNet->ClientId);          
	u1_printf("用 户 名：%s\r\n", pUserNet->UserName);             
	u1_printf("密    码：%s\r\n", pUserNet->Passward);         
}


/*----------------------------------------------------------*/
/*函数名：连接服务器报文                                    */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void MqttSendConnectPack(void)
{
  MqttDataStru    *pMqtt;
  UserNetMsgStru  *pUserNet;

  pUserNet = &gstvUserNet;
  pMqtt    = &gstvMqtt;
  
	pMqtt->FixedLen    = 2;                                                   
	pMqtt->VariableLen = 10;                                                   
	pMqtt->PayloadLen  = 2 + pUserNet->ClientIdLen+ 2 + pUserNet->UserNameLen+ 2 + pUserNet->PasswardLen; 
	
	pMqtt->TxBuff[0]  = 0x10;              //第1个字节 ：固定0x01	
	pMqtt->TxBuff[1]  = pMqtt->VariableLen + pMqtt->PayloadLen;        //第2个字节 ：可变报头+有效负荷的长度
	pMqtt->TxBuff[2]  = 0x00;          		//第3个字节 ：固定0x00	            
	pMqtt->TxBuff[3]  = 0x04;              //第4个字节 ：固定0x04
	pMqtt->TxBuff[4]  = 0x4D;							//第5个字节 ：固定0x4D
	pMqtt->TxBuff[5]  = 0x51;							//第6个字节 ：固定0x51
	pMqtt->TxBuff[6]  = 0x54;							//第7个字节 ：固定0x54
	pMqtt->TxBuff[7]  = 0x54;							//第8个字节 ：固定0x54
	pMqtt->TxBuff[8]  = 0x04;							//第9个字节 ：固定0x04
	pMqtt->TxBuff[9]  = 0xC2;						  //第10个字节：使能用户名和密码校验，不使用遗嘱，不保留会话
	pMqtt->TxBuff[10] = 0x00; 						  //第11个字节：保活时间高字节 0x00
	pMqtt->TxBuff[11] = 0xB4;						  //第12个字节：保活时间高字节 0x64   100s
	
	/* CLIENT_ID */
	pMqtt->TxBuff[12] = pUserNet->ClientIdLen/256;                			  			//客户端ID长度高字节
	pMqtt->TxBuff[13] = pUserNet->ClientIdLen%256;               			  			//客户端ID长度低字节
	memcpy(&pMqtt->TxBuff[14], pUserNet->ClientId, pUserNet->ClientIdLen);     //复制过来客户端ID字串	
	
	/* 用户名 */
	pMqtt->TxBuff[14 + pUserNet->ClientIdLen] = pUserNet->UserNameLen / 256; 				  		  //用户名长度高字节
	pMqtt->TxBuff[15 + pUserNet->ClientIdLen] = pUserNet->UserNameLen % 256; 				 		    //用户名长度低字节
	memcpy(&pMqtt->TxBuff[16+pUserNet->ClientIdLen], pUserNet->UserName, pUserNet->UserNameLen);             

	/* 密码 */
	pMqtt->TxBuff[16+pUserNet->ClientIdLen+pUserNet->UserNameLen] = pUserNet->PasswardLen/256;			    //密码长度高字节
	pMqtt->TxBuff[17+pUserNet->ClientIdLen+pUserNet->UserNameLen] = pUserNet->PasswardLen%256;			    //密码长度低字节
	memcpy(&pMqtt->TxBuff[18+pUserNet->ClientIdLen+pUserNet->UserNameLen],pUserNet->Passward,pUserNet->PasswardLen); 

	TxDataBuf_Deal(&pMqtt->TxBuff[0], pMqtt->FixedLen + pMqtt->VariableLen + pMqtt->PayloadLen);  
}


/*----------------------------------------------------------*/
/*函数名：SUBSCRIBE订阅topic报文                            */
/*参  数：QoS：订阅等级                                     */
/*参  数：topic_name：订阅topic报文名称                     */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void MqttSendSubscribe(u8  *topic_name, int Qos)
{
  MqttDataStru    *pMqtt;

  pMqtt    = &gstvMqtt;

  pMqtt->FixedLen    = 2;
  pMqtt->VariableLen = 2;
  pMqtt->PayloadLen  = 2 + strlen((char *)topic_name) + 1;

	pMqtt->TxBuff[0] = 0x82;                                        //第1个字节 ：固定0x82                      
	pMqtt->TxBuff[1] = pMqtt->VariableLen + pMqtt->PayloadLen;       //第2个字节 ：可变报头+有效负荷的长度	
	pMqtt->TxBuff[2] = 0x00;                                        //第3个字节 ：报文标识符高字节，固定使用0x00
	pMqtt->TxBuff[3] = 0x01;		                                      //第4个字节 ：报文标识符低字节，固定使用0x01
	pMqtt->TxBuff[4] = strlen((char *)topic_name)/256;                  //第5个字节 ：topic_name长度高字节
	pMqtt->TxBuff[5] = strlen((char *)topic_name)%256;		               //第6个字节 ：topic_name长度低字节
	memcpy(&pMqtt->TxBuff[6], topic_name, strlen((char *)topic_name));  //第7个字节开始 ：复制过来topic_name字串		
	pMqtt->TxBuff[6 + strlen((char *)topic_name)] = Qos;                  //最后1个字节：订阅等级
	
	TxDataBuf_Deal(&pMqtt->TxBuff[0], pMqtt->FixedLen + pMqtt->VariableLen + pMqtt->PayloadLen);  //加入发送数据缓冲区
}

/*----------------------------------------------------------*/
/*函数名：PING报文，心跳包                                  */
/*参  数：无                                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void MqttSendPingReq(void)
{
  MqttDataStru    *pMqtt;
  
  pMqtt    = &gstvMqtt;
  
	pMqtt->TxBuff[0]=0xC0;              //第1个字节 ：固定0xC0                      
	pMqtt->TxBuff[1]=0x00;              //第2个字节 ：固定0x00 

	TxDataBuf_Deal(&pMqtt->TxBuff[0], 2);   //加入数据到缓冲区
}


/*----------------------------------------------------------*/
/*函数名：等级0 发布消息报文                                */
/*参  数：topic_name：topic名称                             */
/*参  数：data：数据                                        */
/*参  数：data_len：数据长度                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void MqttSendPublishQs0(u8  *topic, char *data, u16 data_len)
{
  MqttDataStru    *pMqtt;
  
  pMqtt    = &gstvMqtt;

	pMqtt->FixedLen    = 2;                                     //固定报头长度：2字节
	pMqtt->VariableLen = 2 + strlen((char *)topic);                //可变报头长度：2字节(topic长度)+ topic字符串的长度
	pMqtt->PayloadLen  = data_len;                             //有效负荷长度：就是data_len
	
	pMqtt->TxBuff[0]=0x30;                                     //第1个字节 ：固定0xC0                
	pMqtt->TxBuff[1]=pMqtt->VariableLen + pMqtt->PayloadLen;    //第2个字节 ：可变报头+有效负荷的长度	
	pMqtt->TxBuff[2]=strlen((char *)topic)/256;                    //第3个字节 ：topic长度高字节
	pMqtt->TxBuff[3]=strlen((char *)topic)%256;		                 //第4个字节 ：topic长度低字节
	memcpy(&pMqtt->TxBuff[4],topic,strlen((char *)topic));         //第5个字节开始 ：拷贝topic字符串	
	memcpy(&pMqtt->TxBuff[4+strlen((char *)topic)],data,data_len);//拷贝data数据
	
	TxDataBuf_Deal(&pMqtt->TxBuff[0], pMqtt->FixedLen + pMqtt->VariableLen + pMqtt->PayloadLen);  //加入发送数据缓冲区
}

/*----------------------------------------------------------*/
/*函数名：处理服务器发来的等级0的推送                       */
/*参  数：redata：接收的数据                                */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void MQTTRecDealPushdataQs0(unsigned char *redata)
{
	int  re_len;               	            //定义一个变量，存放接收的数据总长度
	int  pack_num;                         //定义一个变量，当多个推送一起过来时，保存推送的个数
  int  temp_len;                         //定义一个变量，暂存数据
  int  totle_len;                        //定义一个变量，存放已经统计的推送的总数据量
	int  topic_len;              	        //定义一个变量，存放推送中主题的长度
	int  cmd_len;                          //定义一个变量，存放推送中包含的命令数据的长度
	int  cmd_loca;                         //定义一个变量，存放推送中包含的命令的起始位置
	int  i;                                 //定义一个变量，用于for循环
	unsigned char temp[cMqttBuffSize];      //临时缓冲区
	unsigned char *data;                    //redata过来的时候，第一个字节是数据总量，data用于指向redata的第2个字节，真正的数据开始的地方
		
	re_len   = redata[0];                                  //获取接收的数据总长度
	data     = &redata[1];                                 //data指向redata的第2个字节，真正的数据开始的 
	pack_num = temp_len = totle_len = 0;                	  //各个变量清零
	do
  {
		pack_num++;                                     			//开始循环统计推送的个数，每次循环推送的个数+1
		temp_len  = data[1+totle_len]+2;                 			//计算本次统计的推送的数据长度
		totle_len += temp_len;                          			//累计统计的总的推送的数据长度
		re_len    -= temp_len ;                            	  //接收的数据总长度 减去 本次统计的推送的总长度      
	}while(re_len!=0);                                  		//如果接收的数据总长度等于0了，说明统计完毕了
	if(pack_num!=1)
    u1_printf("本次接收了%d个推送数据\r\n",pack_num);    //串口输出信息
	temp_len = totle_len = 0;                		           	//各个变量清零
	for(i=0;i<pack_num;i++)                                //已经统计到了接收的推送个数，开始for循环，取出每个推送的数据 
  {                            
		topic_len = data[2+totle_len]*256+data[3+totle_len] + 2;  //计算本次推送数据中主题占用的数据量
		cmd_len = data[1+totle_len]-topic_len;                      //计算本次推送数据中命令数据占用的数据量
		cmd_loca = totle_len + 2 +  topic_len;                      //计算本次推送数据中命令数据开始的位置
		memcpy(temp,&data[cmd_loca],cmd_len);                       //命令数据拷贝出来		                 
		CMDBuf_Deal(temp, cmd_len);                                 //加入命令到缓冲区
		temp_len = data[1+totle_len]+2;                             //记录本次推送的长度
		totle_len += temp_len;                                      //累计已经统计的推送的数据长度
	}
}


/*----------------------------------------------------------*/
/*函数名：处理发送缓冲区                                    */
/*参  数：data：数据                                        */
/*参  数：size：数据长度                                    */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void TxDataBuf_Deal(unsigned char *data, int size)
{
  MqttDataStru    *pMqtt;
  
  pMqtt    = &gstvMqtt;
  
  MemCopyFlashToSram(&data[0], &pMqtt->TxDataInPtr[1], size);

	pMqtt->TxDataInPtr[0] = size;                  //记录数据长度
  
	pMqtt->TxDataInPtr += cMqttBuffSize;            //指针下移
	if(pMqtt->TxDataInPtr == pMqtt->TxDataEndPtr)   //如果指针到缓冲区尾部了
	{
		pMqtt->TxDataInPtr = pMqtt->TxDataBuf[0];         //指针归位到缓冲区开头
	}
}


/*----------------------------------------------------------*/
/*函数名：处理命令缓冲区                                    */
/*参  数：data：数据                                        */
/*参  数：size：数据长度                                    */
/*返回值：无                                                */
/*----------------------------------------------------------*/
void CMDBuf_Deal(unsigned char *data, int size)
{
  MqttDataStru    *pMqtt;
  
  pMqtt    = &gstvMqtt;
  
  MemCopyFlashToSram(&data[0], &pMqtt->CmdDataInPtr[1], size);

	pMqtt->CmdDataInPtr[0] = size;                   //记录数据长度
	pMqtt->CmdDataInPtr[size+1] = '\0';              //加入字符串结束符
	pMqtt->CmdDataInPtr += cMqttBuffSize;             //指针下移
	if(pMqtt->CmdDataInPtr == pMqtt->CmdDataEndPtr)   //如果指针到缓冲区尾部了
	{
		pMqtt->CmdDataInPtr = pMqtt->CmdDataBuf[0];                 //指针归位到缓冲区开头
	}
}


/*==========================================================================================
下面这三条指令是用于与上面三条区分编译系统为 C++ 或 C 的预处理指令相对应。用于指定
 extern "C" 链接指示符作用域，此处的"}"与上面的"{"相对应，为链接指示符作用结束符。
==========================================================================================*/
#ifdef __cplusplus
}
#endif


/*
********************************************************************************************
                                 本Ｃ语言源程序文件到此结束
********************************************************************************************
*/










