/*
********************************************************************************************
源程序文件名：      DataMath.c
源程序名称：        算术运算及数制转换等数学算法的源程序文件
程序版本：　　      1.0
程序功能：
　　　　本程序是算术运算及数制转换等数学算法的源程序文件。
程序说明：
　　　　本源程序文件的所有函数都是可重入性函数。

编译工具软件：      IAR Embedded Workbench for Atmel AVR 版本：3.20 以上
链接子程序文件：

编作者：            
编作者 E-Mail：     @163.COM
编制日期：          2002年4月20日
----------------------------------------------------------------------------------------
适用器件芯片类型:   Atmel 公司生产的８位 RISC 单片机
器件芯片时钟频率:   没有要求
存储器模式：        大存储器模式
外部扩展存储器大小：0 【字节】
数据堆栈大小：      32 【字节】
----------------------------------------------------------------------------------------
源程序版本历史：
2002年4月20日 -------- 版本 1.0 ：发行版本
********************************************************************************************
*/

/*==========================================================================================
　　　　　　　　　　　     本源程序包括的头部文件和其他源程序文件
==========================================================================================*/
#include "DataMath.h"                         // 本程序的头部文件



/*==========================================================================================
下面这三条指令是用于区分编译系统为 C++ 或 C 的预处理指令。
"__cplusplus" 为 C++ 预处理器名字，这三条指令表明接在其下面的那些指令为 C 程序特征。
==========================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*
********************************************************************************************
                             　       程序指令代码清单
********************************************************************************************
*/


/*--------------------------------------------------------------------------------------
函数名称：　　把大小写数据转换为大写数据
函数功能：　　本函数是把大小写数据转换为大写数据函数。
函数入口参数：
              *pSrouce ------- 存储在 SRAM 中的源数据的首个地址指针。
函数返回值：　
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
void	ConvSramToBigCaps(void						*pSrouce,	 		// 存储在 SRAM 中的源数据的首个地址指针。
                      	unsigned char		DataLen	 			// 存储在 SRAM 中的源数据的长度
                     		)  
{
	unsigned char		*pSrc;
	unsigned char		Len;
	unsigned char		i;


	pSrc		= pSrouce;
	Len			= DataLen;

	for(i=0; i<Len; i++)
		{
			if((*pSrc >= 'a') && (*pSrc <= 'z'))
				{
					*pSrc	= *pSrc - 'a' + 'A';
				}
			pSrc++;
		}
}
												

/*--------------------------------------------------------------------------------------
函数名称：　　把4个字节高低位转为8个字节的数组
函数功能：　　本函数是把4个字节高低位转为8个字节的数组函数。
函数入口参数：
              *pSrouce ------- 存储在 SRAM 中的源数据的首个地址指针。
函数返回值：　ListVal -------- 列表值
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
void	Conv4bcdToArray(void		*pSrouce,	 		// 存储在 SRAM 中的源数据的首个地址指针。
                      void		*pObject	 		// 存储在 SRAM 中的目标数据的首个地址指针。
                     	)
{
	unsigned char		*pSrc;
	unsigned char		*pDst;
	unsigned char		i;
	unsigned char		temp;

	pSrc		= pSrouce;
	pDst		= pObject;

	for(i=0; i<4; i++)
		{
			temp		= *pSrc;
			*pDst		= (temp & 0xf0) >> 4;
			pDst++;
			*pDst		= temp & 0x0f;
			pDst++;
			pSrc++;
		}
}
															
/*--------------------------------------------------------------------------------------
函数名称：　　把4个字节高低位转为1个字节的列表
函数功能：　　本函数是把4个字节高低位转为1个字节的列表函数。
函数入口参数：
              *pSrouce ------- 存储在 SRAM 中的源数据的首个地址指针。
函数返回值：　ListVal -------- 列表值
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
unsigned char	Conv4bcdToList(void		*pSrouce	 		// 存储在 SRAM 中的目标数据的首个地址指针。
                     				)
{
	unsigned char		*pSrc;
	unsigned char		i;
	unsigned char		ListVal;
	unsigned char		temp;

	pSrc		= pSrouce;
	ListVal	=	0x00;

	for(i=0; i<4; i++)
		{
			temp	= *pSrc;

			ListVal >>= 0x01;
			
			if(temp & 0xf0)
				{
					ListVal |= 0x80;
				}
			
			ListVal >>= 0x01;

			if(temp & 0x0f)
				{
					ListVal |= 0x80;
				}
			pSrc++;
		}
	return(ListVal);
}
/*--------------------------------------------------------------------------------------
函数名称：　　判断某个值有在表格中
函数功能：　　本函数是判断判断某个值有在表格中的函数。
函数入口参数：
              *pObject ------- 存储在 SRAM 中的目标数据的首个地址指针。
              ListCnt  ------- 列表的长度
              ZoneNum -------- 防区号
函数返回值：　TRUE    -------- 此值有在表格中
              FALSE   -------- 此值不在表格中
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
bool	MemVerdictTableVal(const void					 *pObject,			// 存储在 Flash 中的目标数据的首个地址指针。
                         unsigned char			 DataVal,		   // 要判断的值
                         unsigned char TableLen)		 // 表格个数
{
	const unsigned char  *pTab;
	unsigned char				 Val;
	unsigned char				 Len;
	unsigned char				 i;

	pTab	= pObject;
	Val		= DataVal;
	Len		= TableLen;

	for(i=0; i<Len; i++)
		{
			if(*pTab == Val)
				{
					break;
				}
			pTab++;
		}

	if(i >= Len)
		{
			return FALSE;
		}
	else
		{
			return TRUE;
		}
}


												 
/*--------------------------------------------------------------------------------------
函数名称：　　uint型的数据高8位与低8位互换
函数功能：　　本函数是uint型的数据高8位与低8位互换函数。
函数入口参数：
              SwapVal    ------- 要互换的源值。
函数返回值：　ResultVal	 ------- 转换后的值
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
unsigned short	MemSwapU16Val(unsigned short SwapVal			// 要互换的源值。
                        			)		
{
	unsigned short ResultVal;
	unsigned short temp;

	temp	= SwapVal;

	ResultVal = (unsigned short)(temp >>8);
	ResultVal|= (unsigned short)(temp <<8);

	return ResultVal;
}

                        						  
/*--------------------------------------------------------------------------------------
函数名称：　　计算防区列表中标志位的个数
函数功能：　　本函数是计算防区列表中标志位的个数函数。
函数入口参数：
              *pObject ------- 存储在 SRAM 中的目标数据的首个地址指针。
              ListCnt  ------- 列表的长度(<32)
函数返回值：　MaskCnt	 ------- 列表中位为1的个数
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
unsigned char	MemComputeListMaskCnt(void							*pObject,			// 存储在 SRAM 中的目标数据的首个地址指针。
                        						unsigned char			ListCnt				// 列表的长度
                        						)
{
	unsigned char	*pDst;
	unsigned char MaskCnt;
	unsigned char i,j;
	unsigned char temp;

	pDst		= pObject;
	MaskCnt	= 0x00;


	for(i=0; i<ListCnt; i++)
		{
			for(j=0; j<8; j++)
				{
					temp	= *pDst;
					if(((temp >> j) & 0x01) != 0x00)
						{
							MaskCnt++;
						}
				}
			pDst++;
		}
	return	MaskCnt;
}
/*--------------------------------------------------------------------------------------
函数名称：　　判断防区列表中的所有的位全部为0
函数功能：　　本函数是判断防区列表中的某个位状态的函数。
函数入口参数：
              *pObject ------- 存储在 SRAM 中的目标数据的首个地址指针。
              ZoneNum -------- 防区号
函数返回值：　TRUE -- 列表中有不为0
              FALSE -- 列表中有全部为0
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
bool	MemVerdictListAllZero(void							*pObject,			// 存储在 SRAM 中的目标数据的首个地址指针。
                           unsigned char			ZoneNum)			// 防区号
{
	unsigned char		*pSrc;
	unsigned char		Num;
	unsigned char		i;


  
	pSrc		= pObject;
	Num			= ZoneNum;


  for(i=0; i<Num; i++)
    {
     if(*pSrc != 0)
      {
       return TRUE;
      }
     pSrc++;
    }

  return FALSE;
}                           
/*--------------------------------------------------------------------------------------
函数名称：　　判断防区列表中的某个位状态
函数功能：　　本函数是判断防区列表中的某个位状态的函数。
函数入口参数：
              *pObject ------- 存储在 SRAM 中的目标数据的首个地址指针。
              ListCnt  ------- 列表的长度
              ZoneNum -------- 防区号(01-128)
函数返回值：　没有
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
bool	MemVerdictListBit(void							*pObject,			// 存储在 SRAM 中的目标数据的首个地址指针。
                        unsigned char			ZoneNum)			// 防区号
{
	unsigned char		*pSrc;
	unsigned char		Num;
	unsigned char		temp;
	unsigned char		temp1;
	unsigned char		i;

  if(ZoneNum == 0x00)
    {
     return FALSE;
    }
  
	pSrc		= pObject;
	Num			= ZoneNum;
	temp		= Num / 8;
	temp1		= Num % 8;
	

	if(temp1 == 0x00)
		{
			pSrc		+= temp-1;
			i				 = 0x07;
		}
	else
		{
			pSrc		+= temp;
			i				 = temp1-1;
		}
	temp	= *pSrc;
	if(((temp >> i) & 0x01) == CLEAR)
		{
			return FALSE;
		}
	else
		{
			return TRUE;
		}
}


/*--------------------------------------------------------------------------------------
函数名称：　　置防区列表中的某个位状态
函数功能：　　本函数是置防区列表中的某个位状态的函数。
函数入口参数：
              *pObject ------- 存储在 SRAM 中的目标数据的首个地址指针。
              ListCnt  ------- 列表的长度
              ZoneNum -------- 防区号(01-128)
函数返回值：　没有
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
void	MemSetListBit(void							*pObject,			// 存储在 SRAM 中的目标数据的首个地址指针。
                    unsigned char			ZoneNum)			// 防区号
{
	unsigned char		*pSrc;
	unsigned char		Num;
	unsigned char		temp;
	unsigned char		temp1;
	unsigned char		i;

  if(ZoneNum == 0x00)
    {
     return;
    }
	pSrc		= pObject;
	Num			= ZoneNum;
	temp		= Num / 8;
	temp1		= Num % 8;
	

	if(temp1 == 0x00)
		{
			pSrc		+= temp-1;
			i				 = 0x07;
		}
	else
		{
			pSrc		+= temp;
			i				 = temp1-1;
		}
	temp				 = *pSrc;
	temp				|= (1 << i);
	*pSrc				 = temp;
	
}

/*--------------------------------------------------------------------------------------
函数名称：　　清除防区列表中的某个位状态
函数功能：　　本函数是清除防区列表中的某个位状态的函数。
函数入口参数：
              *pObject ------- 存储在 SRAM 中的目标数据的首个地址指针。
              ListCnt  ------- 列表的长度
              ZoneNum -------- 防区号(01-128)
函数返回值：　没有
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
void	MemClearListBit(void							*pObject,			// 存储在 SRAM 中的目标数据的首个地址指针。
                    	unsigned char			ZoneNum)			// 防区号
{
	unsigned char		*pSrc;
	unsigned char		Num;
	unsigned char		temp;
	unsigned char		temp1;
	unsigned char		i;

  if(ZoneNum == 0x00)
    {
     return;
    }
  
	pSrc		= pObject;
	Num			= ZoneNum;
	temp		= Num / 8;
	temp1		= Num % 8;
	

	if(temp1 == 0x00)
		{
			pSrc		+= temp-1;
			i				 = 0x07;
		}
	else
		{
			pSrc		+= temp;
			i				 = temp1-1;
		}
	temp				 = *pSrc;
	temp				&= ~(1 << i);
	*pSrc				 = temp;
		
}


											
/*--------------------------------------------------------------------------------------
函数名称：　　复制字节数据 到 Buff中
函数功能：　　本函数是将固定长度（个数）的字节数据从源 const 复制到目标 SRAM 中的函数。
函数入口参数：
              *pObject ------- 存储在 SRAM 中的目标数据的首个地址指针。
              value    ------- 数据值
              ByteLen -------- 缓存中有数据个数
函数返回值：　没有
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
void	MemCopyDataToBuff(void							*pObject,			// 存储在 SRAM 中的目标数据的首个地址指针。
                        unsigned char			value,				// 数据值
                        unsigned char			DataLen)			// 缓存中有数据个数			
{
	unsigned char       i;
  unsigned char       Val;
	unsigned char       Len;
  unsigned char       *pDst;

	pDst		= pObject;
	Val			= value;
	Len			= DataLen;

	if((Len%2) == 0x00)	
		{
			i				= Len/2;
			pDst 	 += i;
			*pDst  	= Val<<4;
		}
	else
		{
			i				= Len/2;
			pDst	 += i;
      *pDst  &= 0xf0;
			*pDst	 |= Val;
		}
}
/*==========================================================================================
函数名称：　　复制字节数据从 const 到 SRAM 中
函数功能：　　本函数是将固定长度（个数）的字节数据从源 const 复制到目标 SRAM 中的函数。
函数入口参数：*pSource ------- 存储在 const 中的源数据的首个地址指针。
              *pObject ------- 存储在 SRAM 中的目标数据的首个地址指针。
              ByteLen -------- 复制的数据字节长度（个数）。
函数返回值：　没有
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  MemCopyFlashToSram (void const          *pSource,
                          void                *pObject,
                          const unsigned char ByteLen)
{
  unsigned char       i;
  unsigned char       temp;
  unsigned char const *pSrc;
  unsigned char       *pDst;

  pSrc    =  pSource;
  pDst    =  pObject;
  for (i=0; i<ByteLen; i++)
  {
    temp  =  *pSrc;
    *pDst =  temp;
    pDst++;
    pSrc++;
  }
}

void MemCopyFlashToSram2(void const          *pSource,
                          void                *pObject,
                          const unsigned short ByteLen)
{
  unsigned short       i;
  unsigned char        temp;
  unsigned char const *pSrc;
  unsigned char       *pDst;

  pSrc    =  pSource;
  pDst    =  pObject;
  for (i=0; i<ByteLen; i++)
  {
    temp  =  *pSrc;
    *pDst =  temp;
    pDst++;
    pSrc++;
  }
}
/*==========================================================================================
函数名称：　　复制字节数据从 const 到 SRAM 中并相应增加存储在 SRAM 中的目标数据的地址值
函数功能：　　本函数是将固定长度（个数）的字节数据从源 const 复制到目标 SRAM 中，
　　　　　　　并相应增加存储在 SRAM 中的目标数据的地址值的函数。
函数入口参数：*pSource ------- 存储在 const 中的源数据的首个地址指针。
              **pObject ------ 指向存储在 SRAM 中的目标数据的首个地址指针的指针。
              ByteLen -------- 复制的数据字节长度（个数）。
函数返回值：　复制的数据字节长度（个数）
函数出口参数：*pObject ------- 存储在 SRAM 中的目标数据的地址值，出口值＝首个地址指针＋ByteLen。
调用函数：　　１．
备注：        １．
==========================================================================================*/
unsigned char  MemAddrFlashToSram (void const          *pSource,   // 源字符串
                                   unsigned char       **pObject,  // 指向目标字符串的指针
                                   const unsigned char ByteLen)    // 字符串个数（字节长度）
{
  unsigned char     i;                        // 循环变量
  unsigned char     temp;                     // 临时变量
  Uchar8 const      *pSrc;                    // 指向源字符串的指针变量
  unsigned char     *pDst;                    // 指向目标字符串的指针变量

  pSrc              =  pSource;
  pDst              =  *pObject;
  for (i=0; i<ByteLen; i++)
  {
    temp            =  *pSrc;
    *pDst           =  temp;
    pDst++;
    pSrc++;
  }
  *pObject         +=  ByteLen;

  return(ByteLen);
}

/*==========================================================================================
函数名称：　　复制字节数据从 SRAM 到 SRAM 中
函数功能：　　本函数是将固定长度（个数）的字节数据从源 SRAM 复制到目标 SRAM 中的函数。
函数入口参数：*pSource ------- 存储在 SRAM 中的源数据的首个地址指针。
              *pObject ------- 存储在 SRAM 中的目标数据的首个地址指针。
              ByteLen -------- 复制的数据字节长度（个数）。
函数返回值：　没有
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  MemCopySramToSram (const void          *pSource,
                         void                *pObject,
                         const unsigned char ByteLen)
{
  unsigned char       i;
  unsigned char       temp;
  const unsigned char *pSrc;
  unsigned char       *pDst;

  pSrc    =  pSource;
  pDst    =  pObject;
  for (i=0; i<ByteLen; i++)
  {
    temp  =  *pSrc;
    *pDst =  temp;
    pDst++;
    pSrc++;
  }
}
void  MemCopySramToSram2 (const void          *pSource,
                          void                *pObject,
                          const unsigned int  ByteLen)
{
  unsigned short       ii;
  unsigned char       temp;
  const unsigned char *pSrc;
  unsigned char       *pDst;

  pSrc    =  pSource;
  pDst    =  pObject;
  for (ii=0; ii<ByteLen; ii++)
  {
    temp  =  *pSrc;
    *pDst =  temp;
    pDst++;
    pSrc++;
  }
}

/*==========================================================================================
函数名称：　　清除 SRAM 中的字节数据
函数功能：　　本函数是将固定长度（个数）的字节数据从 SRAM 中的目标数据区清除操作函数。
函数入口参数：*pSection ------ 要清除的存储在 SRAM 中的目标数据区的首个地址指针。
              ByteLen -------- 要清除的的数据字节长度（个数）。
函数返回值：　没有
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  MemClearSram (void               *pSection,
                    const unsigned char ByteLen)
{
  unsigned char  i;
  unsigned char  *ptr;

  ptr     =  pSection;
  for (i=0; i<ByteLen; i++)
  {
    *ptr  =  0x00;
    ptr++;
  }
}


void  MemClearSram2 (void               *pSection,
                     const unsigned int  ByteLen)
{
  unsigned int   ii;
  unsigned char  *ptr;

  ptr     =  pSection;
  for (ii=0; ii<ByteLen; ii++)
  {
    *ptr  =  0x00;
    ptr++;
  }
}

/*==========================================================================================
函数名称：　　清除字数据在 SRAM 中
函数功能：　　本函数是将固定长度（个数）的字数据从 SRAM 中的目标数据区清除操作函数。
函数入口参数：*pSection ------ 要清除的存储在 SRAM 中的目标数据区的首个地址指针。
              WordLen -------- 要清除的的数据字长度（个数）。
函数返回值：　没有
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  MemClearWordSram (unsigned int        *pSection,
                        const unsigned char WordLen)
{
  unsigned char  i;

  for (i=0; i<WordLen; i++)
  {
    *pSection  =  0x0000;
    pSection++;
  }
}

/*==========================================================================================
函数名称：　　填充字节数据在 SRAM 中
函数功能：　　本函数是将 SRAM 中的固定长度（个数）目标数据区用１个字节的数据填充操作函数。
函数入口参数：*pSection ------ 要填充的存储在 SRAM 中的目标数据区的首个地址指针。
              ByteLen -------- 要填充的的数据字节长度（个数）。
              Byte ----------- 要填充的字节数据值。
函数返回值：　没有
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  MemFillSram (void               *pSection,
                   const unsigned char ByteLen,
                   const unsigned char Byte)
{
  unsigned char  i;
  unsigned char  *ptr;

  ptr     =  pSection;
  for (i=0; i<ByteLen; i++)
  {
    *ptr  =  Byte;
    ptr++;
  }
}

/*==========================================================================================
函数名称：　　填充字数据在 SRAM 中
函数功能：　　本函数是将 SRAM 中的固定长度（个数）目标数据区用１个字的数据填充操作函数。
函数入口参数：*pSection ------ 要填充的存储在 SRAM 中的目标数据区的首个地址指针。
              WordLen -------- 要填充的的数据字长度（个数）。
              Word ----------- 要填充的字数据值。
函数返回值：　没有
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  MemFillSramWord (unsigned int        *pSection,
                       const unsigned char WordLen,
                       const unsigned int  Word)
{
  unsigned char  i;

  for (i=0; i<WordLen; i++)
  {
    *pSection  =  Word;
    pSection++;
  }
}

/*==========================================================================================
函数名称：　　比较字节数据与 SRAM 中的数据函数
函数功能：　　本函数是将１个字节数据与固定长度（个数）的 SRAM 中目标数据区进行比较操作函数。
函数入口参数：*pSection ------ 要比较的存储在 SRAM 中的目标数据区的首个地址指针。
              ByteLen -------- 要比较的 SRAM 字节长度（个数）。
              Byte ----------- 要比较的字节数据值。
函数返回值：　如果全部的 *pSection ＝ Byte ，返回值 ＝ ０；
              如果有一个 *pSection ≠ Byte ，返回值 ＝ １。
调用函数：　　１．
备注：        １．
==========================================================================================*/
signed char  MemCompSramAndByte (const void         *pSection,
                                 const unsigned char ByteLen,
                                 const unsigned char Byte)
{
  unsigned char        i;
  const unsigned char  *ptr;

  ptr     =  pSection;
  for (i=0; i<ByteLen; i++)
  {
    if (*ptr  !=  Byte)
    {
      return(1);
    }
    ptr++;
  }
  return(0);
}
/*==========================================================================================
函数名称：　　比较字节数据与 Flash 中的数据函数
函数功能：　　本函数是将１个字节数据与固定长度（个数）的 SRAM 中目标数据区进行比较操作函数。
函数入口参数：*pSection ------ 要比较的存储在 SRAM 中的目标数据区的首个地址指针。
              ByteLen -------- 要比较的 SRAM 字节长度（个数）。
              Byte ----------- 要比较的字节数据值。
函数返回值：　如果有一个 *pSection ＝ Byte ，返回值 ＝ ０；
              如果全部的 *pSection ≠ Byte ，返回值 ＝ １。
调用函数：　　１．
备注：        １．
==========================================================================================*/
signed char  MemCompByteAndFlash (const void         *pSection,
                                 const unsigned char ByteLen,
                                 const unsigned char Byte)
{
  unsigned char        i;
  const unsigned char  *ptr;

  ptr     =  pSection;
  for (i=0; i<ByteLen; i++)
  {
    if (*ptr  ==  Byte)
    {
      return(0);
    }
    ptr++;
  }
  return(1);
}

/*==========================================================================================
函数名称：　　比较 SRAM 与 SRAM 中的数据函数
函数功能：　　本函数是将固定字节 SRAM 中的源数据区与 SRAM 中目标数据区进行比较操作函数。
函数入口参数：*pSource ------- 要比较的存储在 SRAM 中的源数据区的首个地址指针。
              *pObject ------- 要比较的存储在 SRAM 中的目标数据区的首个地址指针。
              ByteLen -------- 要比较的数据的字节长度（个数）。
函数返回值：　如果全部的 *pSource ＝ *pObject ，返回值 ＝ ０；
              如果有一个 *pSource ≠ *pObject ，返回值 ＝ １。
调用函数：　　１．
备注：        １．
==========================================================================================*/
signed char  MemCompSramAndSram (const void         *pSource,
                                 const void         *pObject,
                                 const unsigned char ByteLen)
{
  unsigned char       i;
  const unsigned char *pSrc;
  const unsigned char *pDst;

  pSrc    =  pSource;
  pDst    =  pObject;
  for (i=0; i<ByteLen; i++)
  {
    if (*pSrc != *pDst)
    {
      return(1);
    }
    pSrc++;
    pDst++;
  }
  return(0);
}
signed char MemCompSramAndSram2(const void         *pSource,
                               const void         *pObject,
                               const unsigned int  ByteLen)
{
  unsigned int       i;
  const unsigned char *pSrc;
  const unsigned char *pDst;

  pSrc    =  pSource;
  pDst    =  pObject;
  for (i=0; i<ByteLen; i++)
  {
    if (*pSrc != *pDst)
    {
      return(1);
    }
    pSrc++;
    pDst++;
  }
  return(0);
}
/*==========================================================================================
函数名称：　　比较 SRAM 与 const 中的数据函数
函数功能：　　本函数是将固定字节 SRAM 中的源数据区与 const 中目标数据区进行比较操作函数。
函数入口参数：*pSource ------- 要比较的存储在 SRAM 中的源数据区的首个地址指针。
              *pObject ------- 要比较的存储在 const 中的目标数据区的首个地址指针。
              ByteLen -------- 要比较的数据的字节长度（个数）。
函数返回值：　如果全部的 *pSource ＝ *pObject ，返回值 ＝ ０；
              如果有一个 *pSource ≠ *pObject ，返回值 ＝ １。
调用函数：　　１．
备注：        １．
==========================================================================================*/
signed char  MemCompSramAndFlash (const void         *pSource,
                                  void const         *pObject,
                                  const unsigned char ByteLen)
{
  unsigned char       i;
  const unsigned char *pSrc;
  unsigned char const *pDst;

  pSrc    =  pSource;
  pDst    =  pObject;
  for (i=0; i<ByteLen; i++)
  {
    if (*pSrc != *pDst)
    {
      return(1);
    }
    pSrc++;
    pDst++;
  }
  return(0);
}

/*==========================================================================================
函数名称：　　求取两个数的余数
函数功能：　　本函数是用减法，来求取两个数的余数函数。
函数入口参数：Divisor -------- 被除数。
              Dividend ------- 除数。
函数返回值：　余数。
调用函数：　　１．
备注：        １．
==========================================================================================*/
unsigned char  CalaDivideResidue (unsigned char Divisor, const unsigned char Dividend)
{
  while (Divisor >= Dividend)
  {
    Divisor  -=  Dividend;
  }
  return(Divisor);
}

/*==========================================================================================
函数名称：　　求取两个数的商数
函数功能：　　本函数是用减法，来求取两个数的商数函数。
函数入口参数：Divisor -------- 被除数。
              Dividend ------- 除数。
函数返回值：　商数。
调用函数：　　１．
备注：        １．
==========================================================================================*/
unsigned char  CalaDivideQuotient (unsigned char Divisor, const unsigned char Dividend)
{
  unsigned char  quotient;

  quotient  =  0;
  while (Divisor >= Dividend)
  {
    Divisor  -=  Dividend;
    quotient++;
  }
  return(quotient);
}

/*==========================================================================================
函数名称：　　字节数据交换高低半位转换运算
函数功能：　　本函数是将１个字节数据的高半位转换为低半位，低半位转换为高半位，
              高低互相交换转换运算函数。
函数入口参数：*pByte --------- 要交换高低半位数据序列的首个地址指针。
              Len ------------ 要交换高低半位数据序列的的数据字节长度（个数）。
函数返回值：　没有
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  SwapNibbleByte (void *pByte, unsigned char Len)
{
  unsigned char *pSrc;
  unsigned char high;
  unsigned char low;

  pSrc          =  pByte;
  while (Len)
  {
    high        =  *pSrc;
    low         =  high;
    high      <<=  4;
    high       &=  0xF0;
    low       >>=  4;
    low        &=  0x0F;
    *pSrc       =  (high | low);
    pSrc++;
    Len--;
  }
}


/*==========================================================================================
函数名称：　　字节数据倒转位序转换运算
函数功能：　　本函数将１个字节数据的ＭＳＢ转为ＬＳＢ，第７位转为第１位……的位数倒序操作。
函数入口参数：byte ------ 要倒转位序的８位无符号数。
函数返回值：　已倒转位序的８位无符号数。
调用函数：　　１．
备注：        １．
==========================================================================================*/
unsigned char  ConvReverseByte (unsigned char byte)
{
  byte  =  ((byte >> 1) & 0x55) | ((byte << 1) & 0xAA);
  byte  =  ((byte >> 2) & 0x33) | ((byte << 2) & 0xCC);
  byte  =  ((byte >> 4) & 0x0F) | ((byte << 4) & 0xF0);
  return(byte);
}

/*==========================================================================================
函数名称：　　字数据倒转位序转换运算
函数功能：　　本函数将１个字数据的ＭＳＢ转为ＬＳＢ，第１５位转为第１位……的位数倒序操作。
函数入口参数：byte ------ 要倒转位序的１６位无符号数。
函数返回值：　已倒转位序的１６位无符号数。
调用函数：　　１．
备注：        １．
==========================================================================================*/
//unsigned int  ConvReverseWord (unsigned int word)
//{
//}


/*==========================================================================================
函数名称：　　将１个数字转换为位列表格式
函数功能：　　本函数将１个数字转换为１个字节的位列表格式操作。
函数入口参数：number ------ 要转换的１个数字。
函数返回值：　１个字节的位列表格式。
调用函数：　　１．
备注：        １．
==========================================================================================*/
unsigned char  DecodeNumberToBit (unsigned char number)
{
  unsigned char temp;

  temp  =  CalaDivideResidue(number, cSysDataBits);
  temp  =  (1 << temp);

  return(temp);
}

/*==========================================================================================
函数名称：　　将１个数字数值转换为多个相对应的位列表格式
函数功能：　　本函数将１个数字数值转换为多个相对应的位列表格式操作。
函数入口参数：Value ------- 要转换的１个数字数值。
　　　　　　　Start ------- 数字数值的起始计算值。
　　　　　　　Index ------- 参数数值的位序号（０≤Index≤７）。
　　　　　　　*pList ------ 指向参数数值对应位列表的首个地址指针。
　　　　　　　ListSize ---- 参数数值对应位列表的总个数。
函数返回值：　没有。
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  DecodeValueToList (unsigned char       Value,    // 参数数值
                         const unsigned char Start,    // 参数数值的起始计算值
                         const unsigned char Index,    // 参数数值的位序号（０≤Index≤７）
                         void               *pList,    // 指向参数数值对应位列表的指针
                         const unsigned char ListSize) // 参数数值对应位列表的总个数
{
  unsigned char     i;                        // 临时循环变量
  unsigned char     *ptr;                     // 指向参数数值对应位列表的指针

  // 计算参数数值对应的位列表值
  ptr               =  pList;
  Value            -=  Start;                 // 参数数值－计算起始值
  for (i=0; i<ListSize; i++)
  {
    *ptr           &= ~(1 << Index);          // 该位列表的相应位首先清０
    if (Value == i)                           // 如果要计算的参数数值为符合位列表对应的值？
    {
      *ptr         |=  (1 << Index);          // 那么，本位列表的相应位置位
    }
    ptr++;                                    // 指针指向要计算的参数数值的下一个位列表
  }
}

/*==========================================================================================
函数名称：　　将１串数字的数据序列转换为位列表格式序列
函数功能：　　本函数将１串数字的数据序列转换为位列表格式序列操作。
函数入口参数：*pBit --------- 存储转换结果的位列表格式序列的首个地址指针。
              *pNumber ------ 要转换的数字数据序列的首个地址指针（数据为十六进制）。
              NumLen -------- 要转换的数字数据序列的长度。
函数返回值：　没有。
调用函数：　　１．unsigned char DecodeNumberToBit(unsigned char number)
              　　（将１个数字转换为位列表格式）
备注：        １．注意：在调用本函数之前，必须将“*pBit”中的数值清０。
==========================================================================================*/
void  DecodeDataToBitList (void               *pBit,
                           const void         *pNumber,
                           const unsigned char NumLen)
{
  unsigned char       i;                      // 临时循环变量
  unsigned char       flg;                    // 临时位变量
  unsigned char       cnt;                    // 个数计数器变量
  const unsigned char *pSrc;
  unsigned char       *pDst;

  pSrc             =  pNumber;
  pDst             =  pBit;
  for (i=0; i<NumLen; i++)
  {
    cnt            =  0;
    flg            =  *pSrc;
    while (flg >= cSysDataBits)
    {
      flg         -=  cSysDataBits;
      cnt++;
    }
    *(pDst + cnt) |=  (1 << flg);
    pSrc++;
  }
}

void  DecodeData1ByteToBitList (unsigned char *pBit,
                                unsigned char *pNumber,
                                unsigned char NumLen)
{
  unsigned char i;                            // 临时循环变量

  for (i=0; i<NumLen; i++)
  {
    if (*pNumber < 8)
    {
      *pBit |=  (1 << *pNumber);
    }
    pNumber++;
  }
}

/*==========================================================================================
函数名称：　　将双ＢＣＤ码的数值串转换为位列表格式处理
函数功能：    本函数用于将双ＢＣＤ码的数值串转换为位列表格式处理操作函数。
函数入口参数：*pBcd --------- 指向存储要转换的双ＢＣＤ码的数值串的首个地址指针。
　　　　　　　Len ----------- 要转换的双ＢＣＤ码的数值串的字节长度。
　　　　　　　*pBits -------- 指向存储转换为位列表格式后的数值的首个地址指针。
　　　　　　　MaxVal -------- 双ＢＣＤ码的数值串的允许最大数值。
函数返回值：　没有
函数返回值：　“ 1”表示要转换的双ＢＣＤ码的数值串转换为位列表格式操作成功；
　　　　　　　“ 0”表示要转换的双ＢＣＤ码数值串长度为０错误；
　　　　　　　“-1”表示要转换的双ＢＣＤ码数值串数值为０错误；
　　　　　　　“-2”表示要转换的双ＢＣＤ码数值串长度为奇数错误；
　　　　　　　“-3”表示要转换的双ＢＣＤ码数值超出允许最大数值错误。
调用函数：　　１．
备注：        １．注意：双ＢＣＤ码的数值串表示的数值在转换运算时已自动减１。
==========================================================================================*/
signed char  DecodeTwoBcdToBitList (void               *pBcd,
                                    const unsigned char Len,
                                    void               *pBits,
                                    const unsigned char MaxVal)
{
  unsigned char     i;                        // 临时循环变量
  unsigned char     len;                      // 临时双ＢＣＤ码的数值串长度变量
  unsigned char     *ptr;                     // 键盘编码状态变量的指针

  len               =  MaxVal;
  len             >>=  3;                     // 数值串的允许最大数值÷８＝位列表序列的长度
  MemClearSram(pBits, len);                   // 首先将位列表序列中的数值清０
  len               =  Len;                   // 取要转换的双ＢＣＤ码的数值串的字节长度
  if (len == 0)                               // 检查要转换的双ＢＣＤ码数值串长度是否为０？
  {
    return(0);                                // 是，返回要转换的双ＢＣＤ码数值串长度为０错误
  }
  if (len & BIT0)                             // 检查要转换的双ＢＣＤ码数值串长度是否为奇数？
  {
    return(-2);                               // 是，返回要转换的双ＢＣＤ码数值串格式错误
  }
  CompressByte2To1(pBcd, pBcd, len);
  len             >>=  1;                     // 双ＢＣＤ码的数值串长度减半为二进制数长度
  ptr               =  pBcd;                  // 指针指向要转换的双ＢＣＤ码的数值串首个地址
  ConvBcdToBin(ptr, ptr, len);
  if ((*ptr == 0) && (len == 1))
  {
    return(-1);                               // 返回要转换的双ＢＣＤ码数值串为０
  }
  for (i=0; i<len; i++)                       // 将双ＢＣＤ码的数值串转换为二进制数值串
  {
    *ptr           -=  1;
    if (*ptr >= MaxVal)                       // 检查要转换的二进制的数值是否超出允许最大数值？
    {
      return(-3);                             // 是，返回要转换的二进制的数值溢出错误
    }
    ptr++;
  }
  DecodeDataToBitList(pBits, pBcd, len);

  return(1);
}


/*==========================================================================================
函数名称：　　将２字节数据压缩为１个字节数据
函数功能：　　本函数是将固定长度（个数）的将２字节数据压缩为１个字节数据，压缩方式为将２字节
              数据的各自高半位丢弃，分别把２字节数据的低半位压缩合并为１个字节数据函数。
函数入口参数：*pByte2 -------- 要压缩的２字节数据序列的首个地址指针。
              *pByte1 -------- 存储压缩后的１个字节数据序列的首个地址指针。
              Len2 ----------- 要压缩的２字节数据序列的的数据字长度（个数）。
函数返回值：　没有
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  CompressByte2To1 (const void   *pByte2,
                        void         *pByte1,
                        unsigned char Len2)
{
  const unsigned char *pSrc;
  unsigned char       *pDst;

  pSrc          =  pByte2;
  pDst          =  pByte1;
  Len2        >>=  1;
  while (Len2)
  {
    *pDst       =  (*pSrc << 4);
    pSrc++;
    *pDst      +=  (*pSrc & 0x0F);
    pSrc++;
    pDst++;
    Len2--;
  }
}

/*==========================================================================================
函数名称：　　将１字节数据解压为２个字节数据
函数功能：　　本函数是将固定长度（个数）的将１字节数据解压为２个字节数据，解压方式为分别将
              １字节数据解压扩展为２个字节数据的低半位函数。
函数入口参数：*pByte1 -------- 要解压的１字节数据序列的首个地址指针。
              *pByte2 -------- 存储解压后的２个字节数据序列的首个地址指针。
              Len1 ----------- 要解压的１字节数据序列的的数据字长度（个数）。
函数返回值：　没有
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  DecompressByte1To2 (const void   *pByte1,
                          void         *pByte2,
                          unsigned char Len1)
{
  const unsigned char *pSrc;
  unsigned char       *pDst;
  unsigned char       temp;

  pSrc          =  pByte1;
  pDst          =  pByte2;
  while (Len1)
  {
    temp   =  *pSrc;
    *pDst  =  (temp >> 4) & 0x0F;
    pDst++;
    *pDst  =  (temp & 0x0F);
    pDst++;
    pSrc++;
    Len1--;
  }
}

/*==========================================================================================
函数功能：    本函数用于８位二进制数到２个ＢＣＤ码转换运算。
函数入口参数：binary_value --- 要转换的８位二进制数值（其值必需在０～９９之间）。
函数出口参数：转换后的２个ＢＣＤ码数字（分别放高低各半字节中；高位在前，低位在后）。
备注：        ▲．
==========================================================================================*/
void  ConvBinToBcd (const void         *pBin,
                    void               *pBcd,
                    const unsigned char Len)
{
  unsigned char       i;                       // 临时循环变量
  const unsigned char *pSrc;
  unsigned char       *pDst;
	unsigned char       temp;

  pSrc          =  pBin;
  pDst          =  pBcd;
  for (i=0; i<Len; i++)
  {
 		temp 	= (*pSrc /10) << 4;
		temp |= *pSrc % 10;
    pDst++;
    pSrc++;
  }
}
/*
unsigned char  ConvBin8ToBcd2 (unsigned char binary_value)
{
 unsigned char temp ;               // 临时寄存器变量

 
}
*/
/*--------------------------------------------------------------------------------------
函数功能：    本函数用于16位二进制数到4个ＢＣＤ码转换运算。
函数入口参数：WordVal --- 要转换的16位二进制数值（其值必需在０～9999之间）。
函数出口参数：转换后的4个ＢＣＤ码数字（分别放高低各半字节中；高位在前，低位在后）。
备注：        ▲．
--------------------------------------------------------------------------------------*/
unsigned short ConvWordTo4Bcd(unsigned short	WordVal)
{
	unsigned short		WordTemp;
	unsigned short 		BcdVal = 0;

	WordTemp		= WordVal;

	BcdVal			= (WordTemp/1000)<<12;
	WordTemp	 %= 1000;
	BcdVal		 |= (WordTemp/100)<<8;
	WordTemp	 %= 100;
	BcdVal		 |= (WordTemp/10)<<4;
	WordTemp	 %= 10;
	BcdVal		 |= WordTemp;

	return(BcdVal);
}

/*--------------------------------------------------------------------------------------
函数功能：    本函数用于4个ＢＣＤ码到16位二进制数转换运算。
函数入口参数：BcdVal --- 要转换的4个ＢＣＤ值（分别放高低各半字节中；高位在前，低位在后）。
函数出口参数：转换的16位二进制数值（其值必需在０～9999之间）
备注：        ▲．
--------------------------------------------------------------------------------------*/
unsigned short Conv4BcdToWord(unsigned short	BcdVal)
{
	unsigned short		BcdTemp;
	unsigned short 		WordVal = 0;
	unsigned short		temp;

	BcdTemp				= BcdVal;

	temp					= ((BcdTemp & 0xf000)>>12) *1000;
	WordVal				= temp;
	temp					= ((BcdTemp & 0x0f00)>>8) *100;
	WordVal			 += temp;
	temp					= ((BcdTemp & 0x00f0)>>4) *10;
	WordVal			 += temp;
	temp					= (BcdTemp & 0x000f);
	WordVal			 += temp;

	return(WordVal);
}

/*--------------------------------------------------------------------------------------
函数功能：    本函数用于８位二进制数到２个ＢＣＤ码转换运算。
函数入口参数：binary_value --- 要转换的８位二进制数值（其值必需在０～９９之间）。
函数出口参数：转换后的２个ＢＣＤ码数字（分别放高低各半字节中；高位在前，低位在后）。
备注：        ▲．
--------------------------------------------------------------------------------------*/
unsigned char ConvByteToBcd(unsigned char	Byte)
{
 unsigned char		TempByte;

 TempByte		= ((Byte/10)<<4)|(Byte%10);

 return TempByte;
}

unsigned char ConvBcdToByte(unsigned char	Bcd)
{
 unsigned char		TempBcd;

 TempBcd		= ((Bcd>>4) * 10 + ((Bcd) & 0x0f)) ;

 return TempBcd;
 
}
unsigned char ConvBcd2ToByte(void    *pBcd)
{
 unsigned char		TempBcd;
 unsigned char    *pSrc;

 pSrc		= pBcd;
 
 TempBcd	 = (*pSrc << 0x04);
 pSrc++;
 TempBcd	|= *pSrc & 0x0f;

 return TempBcd;
 
}
/*==========================================================================================
函数功能：    本函数用于８位二进制数到３个ＢＣＤ码转换运算。
函数入口参数：binary_value --- 要转换的３位二进制数值。
函数出口参数：转换后的３个ＢＣＤ码数字（分别存放在高字节的后半字节中和低字节中；低字节中为
              高位在前，低位在后）。
备注：        ▲．
==========================================================================================*/

unsigned int  ConvBin8ToBcd3 (unsigned char binary_value)
{
	unsigned char			ByteTemp;
	unsigned short 		BcdVal = 0;

	ByteTemp		= binary_value;

	BcdVal		 |= (ByteTemp/100)<<8;
	ByteTemp	 %= 100;
	BcdVal		 |= (ByteTemp/10)<<4;
	ByteTemp	 %= 10;
	BcdVal		 |= ByteTemp;

	return(BcdVal);
}

/*==========================================================================================
函数功能：    本函数用于１６位二进制数到５个ＢＣＤ码转换运算。
函数入口参数：binary_value --- 要转换的１６位二进制数值。
函数出口参数：转换后的５位压缩ＢＣＤ码数字（分别放在第１字节、第２字节和第３字节的低半字节中；
              高位在前，低位在后）。
备注：        ▲．
==========================================================================================*/
//unsigned long int  ConvBin16ToBcd5Ret (unsigned int binary_value)
//{
//  register unsigned long int temp ;           // 临时寄存器变量
//}


/*==========================================================================================
函数功能：    本函数用于１６位二进制数到５个ＢＣＤ码数据序列转换运算。
函数入口参数：*bcd_addr ----- 用于存储首个转换结果的ＢＣＤ码数值在ＳＲＡＭ中的地址指针。
              bin_value ----- 要转换的１６位二进制数值。
函数出口参数：转换后的ＢＣＤ码数据的个数。
备注：        ▲．转换后的ＢＣＤ码数据序列从大到小排列（ＭＳＢ在前），每个ＢＣＤ码单独一位。
==========================================================================================*/
unsigned char  ConvBin16ToBcd5 (void *bcd_addr, unsigned int bin_value)
{
  register unsigned char i;                   // 临时寄存器变量
  register unsigned char j;
  register unsigned char bcd_len;
  unsigned char  *ptr;

  ptr  = bcd_addr;

  *ptr       = bin_value / 10000;             // 指向ＢＣＤ码数值数据序列的个位数
  bin_value -= (*ptr) * 10000;
  ptr++;
  *ptr       = bin_value / 1000;              // 指向ＢＣＤ码数值数据序列的个位数
  bin_value -= (*ptr) * 1000;
  ptr++;
  *ptr       = bin_value / 100;               // 指向ＢＣＤ码数值数据序列的个位数
  bin_value -= (*ptr) * 100;
  ptr++;
  *ptr       = bin_value / 10;                // 指向ＢＣＤ码数值数据序列的个位数
  bin_value -= (*ptr) * 10;
  ptr++;
  *ptr       = bin_value;                     // 指向ＢＣＤ码数值数据序列的个位数
  j = 0;
  ptr -= 4;
  while ((! (*ptr)) && (j < 4))
  {
    ptr++;
    j++;
  }
  i = j;
  bcd_len = 5 - j;
  j       = bcd_len;
  while(j)
  {
    *(ptr - i) = *ptr;
    ptr++;
    j--;
  }
  return(bcd_len);
}

unsigned char  ConvBin16ToBcd51 (unsigned char *bcd_addr, unsigned int bin_value)
{

	unsigned char	 bcd_len=0;

  *bcd_addr  = bin_value / 10000;             // 指向ＢＣＤ码数值数据序列的个位数
  bin_value -= (*bcd_addr) * 10000;
  bcd_addr++;
  *bcd_addr  = bin_value / 1000;              // 指向ＢＣＤ码数值数据序列的个位数
  bin_value -= (*bcd_addr) * 1000;
  bcd_addr++;
  *bcd_addr  = bin_value / 100;               // 指向ＢＣＤ码数值数据序列的个位数
  bin_value -= (*bcd_addr) * 100;
  bcd_addr++;
  *bcd_addr  = bin_value / 10;                // 指向ＢＣＤ码数值数据序列的个位数
  bin_value -= (*bcd_addr) * 10;
  bcd_addr++;
  *bcd_addr  = bin_value;                     // 指向ＢＣＤ码数值数据序列的个位数
  /*
  j = 0;
  bcd_addr -= 4;
  while ((! (*bcd_addr)) && (j < 4))
  {
    bcd_addr++;
    j++;
  }
  i = j;
  bcd_len = 5 - j;
  j       = bcd_len;
  while(j)
  {
    *(bcd_addr - i) = *bcd_addr;
    bcd_addr++;
    j--;
  }
	*/
  return(bcd_len);
  
}

/*==========================================================================================
函数功能：    本函数用于１６位二进制数到１０个 Unicode 字符码数据序列转换运算。
函数入口参数：*usc2_addr ----- 用于存储首个转换结果的 Unicode 字符码在ＳＲＡＭ中的地址指针。
              bin_value ------ 要转换的１６位二进制数值。
函数出口参数：转换后的 Unicode 字符码的个数。
备注：        ▲．转换后的 Unicode 字符码数据序列从大到小排列（ＭＳＢ在前）。
==========================================================================================*/
unsigned char  ConvBin16ToUsc2 (unsigned char *usc2_addr, unsigned int bin_value)
{
  unsigned char i;
  unsigned char len;
  unsigned char buf[5];

  len = ConvBin16ToBcd5(buf, bin_value);
  for (i=0; i<len; i++)
  {
    *usc2_addr = 0x00;
    usc2_addr++;
    *usc2_addr = buf[i] + '0';
    usc2_addr++;
  }

  return(len << 1);
}

unsigned char  ConvBin16ToUscc (unsigned char *usc2_addr, unsigned int bin_value)
{
  unsigned char i;
  unsigned char len;
  unsigned char buf[5];

  len = ConvBin16ToBcd5(buf, bin_value);
  for (i=0; i<len; i++)
  {
    *usc2_addr = 0xFF;
    usc2_addr++;
    *usc2_addr = buf[i] + 0x10;
    usc2_addr++;
  }

  return(len << 1);
}

unsigned char ConvAddrToUsc2(unsigned char *usc2_addr, unsigned int bin_value)
{
  unsigned char i;
  unsigned char len;
  unsigned char buf[5];

  len = ConvBin16ToBcd51(buf, bin_value);

  len = 3;
  
  for (i=0; i<len; i++)
  {
    *usc2_addr = 0x00;
    usc2_addr++;
    *usc2_addr = buf[i+2] + '0';
    usc2_addr++;
  }

  return(len<<1);
}


/*==========================================================================================
函数功能：    本函数用于２个ＢＣＤ码到８位二进制数转换运算。
函数入口参数：Bcd ----------- 要转换的２个ＢＣＤ码数值。
函数出口参数：转换后的８位二进制数。
备注：        ▲．
==========================================================================================*/
void  ConvBcdToBin (const void         *pBcd,
                    void               *pBin,
                    const unsigned char Len)
{
  unsigned char       i;                       // 临时循环变量
  const unsigned char *pSrc;
  unsigned char       *pDst;
	unsigned char       temp;

  pSrc          =  pBcd;
  pDst          =  pBin;
  for (i=0; i<Len; i++)
  {

		temp	= ((*pSrc & 0xf0) >> 4) * 4;
		temp += *pSrc & 0x0f;
		*pDst	= temp;
    pDst++;
    pSrc++;
  }
}

void  ConvBcdToBin2(const void         *pBcd,
                    void               *pBin,
                    const unsigned char Len)
{
  unsigned char       i;                       // 临时循环变量
  const unsigned char *pSrc;
  unsigned char       *pDst;
	unsigned char       temp;

  pSrc          =  pBcd;
  pDst          =  pBin;
  for (i=0; i<Len; i++)
  {
		temp	= (*pSrc & 0xf0) >> 4;
		*pDst	= temp;
		pDst++;
		temp  = *pSrc & 0x0f;
		*pDst	= temp;
    pDst++;
    pSrc++;
  }
}
/*==========================================================================================
函数功能：    本函数用于５个ＢＣＤ码数据序列到１６位二进制数转换运算。
函数入口参数：*bcd_addr --- 首个要转换的ＢＣＤ码数值在ＳＲＡＭ中的地址指针。
              bcd_len ----- 要转换的ＢＣＤ码数值的数据序列总长度。
函数出口参数：转换后的１６位二进制数。
备注：        ▲．ＢＣＤ码数据序列应从大到小排列，每个ＢＣＤ码数据为单独一位。
==========================================================================================*/
unsigned int  ConvBcd5ToBin16 (unsigned char *bcd_addr, unsigned char bcd_len)
{
  register unsigned char i;                   // 临时寄存器变量
  register unsigned char j;
  register unsigned int  temp;
  register unsigned int  bin;

  if (bcd_len)
  {
    bcd_addr += bcd_len;                      // 指向ＢＣＤ码数值数据序列的个位数
    bcd_addr--;
    bin       = *bcd_addr;
    bcd_addr--;
    for (i=1; i<bcd_len; i++)
    {
      j    = i;
      temp = 0x0001;
      while(j)
      {
        temp *= 10;
        j--;
      }
      bin += (*bcd_addr) * temp;
      bcd_addr--;
    }

    return(bin);
  }

  return(0x0000);
}

/*==========================================================================================
函数功能：    本函数用于１０个ＢＣＤ码数据序列到１６位二进制数转换运算。
函数入口参数：*bcd_addr --- 首个要转换的ＢＣＤ码数值在ＳＲＡＭ中的地址指针。
              bcd_len ----- 要转换的ＢＣＤ码数值的数据序列总长度。
函数出口参数：转换后的１６位二进制数。
备注：        ▲．ＢＣＤ码数据序列应从大到小排列，每个ＢＣＤ码数据为单独一位。
==========================================================================================*/
long unsigned int  CONV_10_BCD_to_BIN_16 (unsigned char *bcd_addr, unsigned char bcd_len)
{
  register unsigned char     i;               // 临时寄存器变量
  register unsigned char     j;
  register long unsigned int temp;
  register long unsigned int bin;

  if (bcd_len)
  {
    bcd_addr += bcd_len;                      // 指向ＢＣＤ码数值数据序列的个位数
    bcd_addr--;
    bin       = *bcd_addr;
    bcd_addr--;
    for (i=1; i<bcd_len; i++)
    {
      j    = i;
      temp = 0x00000001;
      while(j)
      {
        temp *= 10;
        j--;
      }
      bin += (*bcd_addr) * temp;
      bcd_addr--;
    }

    return(bin);
  }

  return(0x00000000);
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










