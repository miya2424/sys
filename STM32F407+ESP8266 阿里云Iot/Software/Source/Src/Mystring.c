/*
********************************************************************************************
源程序文件名：      String.c
源程序名称：        字符串函数的源程序文件
程序版本：　　      1.4
程序功能：
　　　　本程序是字符串函数的源程序文件。

程序说明：
　　　　本文件的所有函数都是可重入性函数。

链接子程序文件：　　
　　　　１．算术运算及数制转换等数学算法的源程序文件（DataMath.c）
　　　　　　说明：调用此文件中的清除 SRAM 中字节数据函数以缩短程序代码。
　　　　２．主程序源程序文件的源程序文件（Main.c）
　　　　　　说明：使用此文件中的"gbitMcuFlag.SecUpd"标志位。

编译工具软件：      IAR Embedded Workbench for Atmel AVR 版本：5.20A 以上

编作者：            Hbyin
编制日期：          2009年07月09日
----------------------------------------------------------------------------------------
适用器件芯片类型:   嵌入式微处理器
适用器件芯片类型：  嵌入式微处理器
器件芯片时钟频率：  任意
存储器模式：        没有要求
外部扩展存储器大小：没有要求 【字节】
调用堆栈大小：      没有要求 【字节】
数据堆栈大小：      没有要求 【字节】
----------------------------------------------------------------------------------------
源程序版本历史：
2009年07月09日　〖Hbyin〗  -------- 版本 1.0 ：发行版本
********************************************************************************************
*/


/*==========================================================================================
                                        调试开关声明
==========================================================================================*/
//#define DEBUG


/*==========================================================================================
                                    本源程序包括的头文件
建议：包含本项目的文件使用 #include "文件名.扩展名" ，
      包含系统库的文件使用 #include <文件名.扩展名> 。
==========================================================================================*/
#include "Mystring.h"                           // 本程序的头部文件


/*==========================================================================================
下面这三条指令是用于区分编译系统为 C++ 或 C 的预处理指令。
"__cplusplus" 为 C++ 预处理器名字，这三条指令表明接在其下面的那些指令为 C 程序特征。
==========================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*==========================================================================================
                            本源程序文件内部使用的字符化常数定义
==========================================================================================*/



/*==========================================================================================
                             本源程序文件内部使用的函数原型声明
==========================================================================================*/



/*==========================================================================================
                                        全局常量定义
==========================================================================================*/



/*==========================================================================================
                                        全局变量定义
==========================================================================================*/



/*==========================================================================================
                             本源程序文件内部使用的局部常量定义
==========================================================================================*/



/*==========================================================================================
                        本源程序文件内部使用的局部动态和静态变量定义
==========================================================================================*/



/*
********************************************************************************************
                             　       程序指令代码清单
********************************************************************************************
*/
/*==========================================================================================
函数名称：　　复制字符串的字节数据从 SRAM 到 SRAM 中
函数功能：　　本函数是将字符串的字节数据从源 SRAM 复制到目标 SRAM 中的函数。
函数入口参数：*pSource ------ 存储在 SRAM 中的源数据的首个地址指针。
              *pObject ------ 存储在 SRAM 中的目标数据的首个地址指针。
函数返回值：　复制字符串的的数据字节长度（个数）。
调用函数：　　１．
备注：        １．注意：“*pSource”存储区长度必须大于等于“*pObject”存储区长度。
==========================================================================================*/
unsigned char  StrCopySramToSram (void *pSource, void *pObject)
{
  unsigned char       *pSrc;
  unsigned char       *pDst;
  unsigned char       cnt;

  pSrc    =  pSource;
  pDst    =  pObject;
  cnt     =  0;
  while (*pSrc)
  {
    *pDst =  *pSrc;
    pDst++;
    pSrc++;
    cnt++;
  }
  return(cnt);
}

unsigned int  StrCopySramToSram2 (void *pSource, void *pObject)
{
  unsigned char       *pSrc;
  unsigned char       *pDst;
  unsigned int        cnt;

  pSrc    =  pSource;
  pDst    =  pObject;
  cnt     =  0;
  while (*pSrc)
  {
    *pDst =  *pSrc;
    pDst++;
    pSrc++;
    cnt++;
  }
  return(cnt);
}


/*==========================================================================================
函数名称：　　复制字符串的字节数据从 const 到 SRAM 中
函数功能：　　本函数是将字符串的字节数据从源 const 复制到目标 SRAM 中的函数。
函数入口参数：*pSource ------ 存储在 const 中的源数据的首个地址指针。
              *pObject ------ 存储在 SRAM 中的目标数据的首个地址指针。
函数返回值：　复制字符串的的数据字节长度（个数）。
调用函数：　　１．
备注：        １．注意：“*pSource”存储区长度必须大于等于“*pObject”存储区长度。
==========================================================================================*/
unsigned char  StrCopyFlashToSram (void const *pSource, void *pObject)
{
  unsigned char const *pSrc;
  unsigned char       *pDst;
  unsigned char       cnt;

  pSrc    =  pSource;
  pDst    =  pObject;
  cnt     =  0;
  while (*pSrc)
  {
    *pDst =  *pSrc;
    pDst++;
    pSrc++;
    cnt++;
  }
  return(cnt);
}

unsigned int  StrCopyFlashToSram2 (void const *pSource, void *pObject)
{
  unsigned char const *pSrc;
  unsigned char       *pDst;
  unsigned int        cnt;

  pSrc    =  pSource;
  pDst    =  pObject;
  cnt     =  0;
  while (*pSrc)
  {
    *pDst =  *pSrc;
    pDst++;
    pSrc++;
    cnt++;
  }
  return(cnt);
}

/*==========================================================================================
函数名称：　　在字符串 *pSource 中搜索与字符串 *pTarget 匹配的子字符串。
函数功能：    本函数是在字符串 *pSource 中搜索与字符串 *pTarget 匹配的子字符串。如果找到匹配
　　　　　　　的子字符串，返回 *pSource 中的子字符串的起始地址指针；否则返回 NULL。
函数入口参数：*pSource ------ 指向存储在 SRAM 中的源字符串的首个地址指针。
              *pTarget ------ 指向存储在 SRAM 中的目标字符串的首个地址指针。
函数返回值：　“*pSource 中的子字符串的起始地址指针”表示搜索到匹配的子字符串；
              “NULL”                               表示没有搜索到匹配的子字符串。
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  *StrStr (void *pSource, void *pTarget)
{
  unsigned char *pSrc;
  unsigned char *pDst;
  unsigned char *s;
  unsigned char *t;

  pSrc    =  pSource;
  pDst    =  pTarget;
  if (*pDst == '\0')                          // 如果目标字符串 pDst 为空
  {
    return(pSrc);                             // 则直接返回　pSrc 的起始地址指针
  }

  while (*pSrc)
  {
    s     =  pSrc;
    t     =  pDst;
    while (*t && *s == *t)
    {
      ++s;
      ++t;
    }

    if (*t == '\0')
    {
      return(pSrc);
    }
    ++pSrc;
    while (*pSrc != '\0' && *pSrc != *pDst)
    {
      ++pSrc;
    }
  }
  return(NULL);
}

/*==========================================================================================
函数名称：　　在字符串 *pSource 中搜索与 const 存储器中字符串 *pTarget 匹配的子字符串。
函数功能：    本函数是在字符串 *pSource 中搜索与字符串 *pTarget 匹配的子字符串。如果找到匹配
　　　　　　　的子字符串，返回 *pSource 中的子字符串的起始地址指针；否则返回 NULL。
函数入口参数：*pSource ------ 指向存储在 SRAM 中的源字符串的首个地址指针。
              *pTarget ------ 指向存储在 const 中的目标字符串的首个地址指针。
函数返回值：　“*pSource 中的子字符串的起始地址指针”表示搜索到匹配的子字符串；
              “NULL”                               表示没有搜索到匹配的子字符串。
调用函数：　　１．
备注：        １．
==========================================================================================*/
void  *StrStrFlash (void *pSource, void const *pTarget)
{
  unsigned char       *pSrc;
  unsigned char const *pDst;
  unsigned char       *s;
  unsigned char const *t;

  pSrc    =  pSource;
  pDst    =  pTarget;
  if (*pDst == '\0')                          // 如果目标字符串 pDst 为空
  {
    return(pSrc);                             // 则直接返回　pSrc 的起始地址指针
  }

  while (*pSrc)
  {
    s     =  pSrc;
    t     =  pDst;
    while (*t && *s == *t)
    {
      ++s;
      ++t;
    }

    if (*t == '\0')
    {
      return(pSrc);
    }
    ++pSrc;
    while (*pSrc != '\0' && *pSrc != *pDst)
    {
      ++pSrc;
    }
  }
  return(NULL);
}
extern void *StrAscFlash(void *pSource, void const *pTarget, unsigned short SrcLen)
{
  unsigned char       *pSrc;
  unsigned char const *pDst;
  unsigned char       *s;
  unsigned char const *t;
	unsigned short       SLen;

  pSrc    =  pSource;
  pDst    =  pTarget;
	SLen    =  SrcLen;

 if (*pDst == '\0')                          // 如果目标字符串 pDst 为空
  {
    return(pSrc);                             // 则直接返回　pSrc 的起始地址指针
  }

  while (SLen != 0x00)
  {
    s     =  pSrc;
    t     =  pDst;
    while (*s == *t)
    {
      ++s;
      ++t;
			if (*t == '\0')
		    {
		      return(pSrc);
		    }
    }

    
    ++pSrc;
		SLen--;
    while (SLen != 0x00 && *pSrc != *pDst)
    {
      ++pSrc;
			SLen--;
    }
  }
  return(NULL);
}	


void *StrUscFlash(void *pSource, void const *pTarget, unsigned char TarLen, unsigned short SrcLen)
{
  unsigned char       *pSrc;
  unsigned char const *pDst;
  unsigned char       *s;
  unsigned char const *t;
	unsigned char       Len;
	unsigned short       SLen;

  pSrc    =  pSource;
  pDst    =  pTarget;
	Len     =  TarLen;
	SLen    =  SrcLen;

  if (Len == 0x00)                           // 如果目标字符串 pDst 为空
  {
    return(pSrc);                             // 则直接返回　pSrc 的起始地址指针
  }

  while (SLen != 0x00)
  {
    s     =  pSrc;
    t     =  pDst;
		Len   =  TarLen;
    while (*s == *t)
    {
      ++s;
      ++t;
			Len--;
			if (Len == 0x00)
		    {
		      return(pSrc);
		    }
    }

    
    ++pSrc;
		SLen--;
    while (SLen != 0x00 && *pSrc != *pDst)
    {
      ++pSrc;
			SLen--;
    }
  }
  return(NULL);
}	
/*--------------------------------------------------------------------------------------
函数名称：　　在字符串 *pSource 中搜索与 const 存储器中字符串 *pTarget 匹配的子字符串。
函数功能：    本函数是在字符串 *pSource 中搜索与字符串 *pTarget 匹配的子字符串。如果找到匹配
　　　　　　　的子字符串，返回 *pSource 中的子字符串的起始地址指针；否则返回 NULL。
函数入口参数：*pSource ------ 指向存储在 SRAM 中的源字符串的首个地址指针。
              *pTarget ------ 指向存储在 const 中的目标字符串的首个地址指针。
函数返回值：　“*pSource 中的子字符串的起始地址指针”表示搜索到匹配的子字符串；
              “NULL”                               表示没有搜索到匹配的子字符串。
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
extern unsigned char StrStrFlashLen(void *pSource, void const *pTarget)
{
  unsigned char       *pSrc;
  unsigned char const *pDst;
  unsigned char       *s;
  unsigned char const *t;
	unsigned char				Len;
	
	Len			=  0x00;
  pSrc    =  pSource;
  pDst    =  pTarget;
	
  if (*pDst == '\0')                          // 如果目标字符串 pDst 为空
  {
    return(Len);                             // 则直接返回　pSrc 的起始地址指针
  }

  Len		=  0x00;

  while (*pSrc)
  {
    s     =  pSrc;
    t     =  pDst;
		
		
    while (*t && *s == *t)
    {
      ++s;
      ++t;
			
    }

    if (*t == '\0')
    {
      return(Len);
    }
    ++pSrc;
		Len++;
    while (*pSrc != '\0' && *pSrc != *pDst)
    {
      ++pSrc;
			Len++;
    }
  }
  return(Len);
}
extern unsigned char StrAscFlashLen(void *pSource, void const *pTarget, unsigned short SrcLen)
{
  unsigned char       *pSrc;
  unsigned char const *pDst;
  unsigned char       *s;
  unsigned char const *t;
	unsigned char				Len;
	unsigned short       SLen;
	
	Len			=  0x00;
  pSrc    =  pSource;
  pDst    =  pTarget;
 
  SLen    =  SrcLen;

	
  Len		=  0x00;

  if (*pDst == '\0')                          // 如果目标字符串 pDst 为空
  {
    return(Len);                             // 则直接返回　pSrc 的起始地址指针
  }
	
  while (SLen != 0x00)
  {
    s     =  pSrc;
    t     =  pDst;
		
    while (*s == *t)
    {
      ++s;
      ++t;
			if (*t == '\0')
		    {
		      return(Len);
		    }
    }
    ++pSrc;
		Len++;
		SLen--;
    while (SLen != 0x00  && *pSrc != *pDst)
    {
      ++pSrc;
			Len++;
			SLen--;
    }
  }
  return(Len);
}

extern unsigned char StrUscFlashLen(void *pSource, void const *pTarget, unsigned char TarLen, unsigned short SrcLen)
{
  unsigned char       *pSrc;
  unsigned char const *pDst;
  unsigned char       *s;
  unsigned char const *t;
	unsigned char				Len;
  unsigned char       tLen;
	unsigned short       SLen;
	
	Len			=  0x00;
  pSrc    =  pSource;
  pDst    =  pTarget;
  tLen    =  TarLen;
  SLen    =  SrcLen;

	
  Len		=  0x00;

  if (tLen == 0)                          // 如果目标字符串 pDst 为空
  {
    return(Len);                             // 则直接返回　pSrc 的起始地址指针
  }
	
  while (SLen != 0x00)
  {
    s     =  pSrc;
    t     =  pDst;
		tLen    =  TarLen;
		
    while (*s == *t)
    {
      ++s;
      ++t;
			tLen--;
			if (tLen == 0)
		    {
		      return(Len);
		    }
    }
    ++pSrc;
		Len++;
		SLen--;
    while (SLen != 0x00  && *pSrc != *pDst)
    {
      ++pSrc;
			Len++;
			SLen--;
    }
  }
  return(Len);
}
/*==========================================================================================
函数名称：　　Unicode 码的字符串转换为十六进制时钟格式。
函数功能：    本函数用于将 Unicode 码的时间格式字符串转换为十六进制时钟格式。
函数入口参数：*Ucs2Str ------- 指向存储 Unicode 码的时间格式字符串的指针。
              Separator ------ 时间格式的分隔符号，一般为“:”(0x003A)。
              *pstDst -------- 指向存储十六进制时钟格式的指针。
函数返回值：　“TRUE ”表示转换操作成功；
　　　　　　　“FALSE”表示转换操作失败。
调用函数：　　１．
备注：        １．Unicode 字符串时钟时间格式：[Ｘ][Ｘ]:[Ｘ][Ｘ]:[Ｘ][Ｘ]，分隔符为:(0x003A)
==========================================================================================*/
unsigned char  Ucs2StrDecodeClock (Uchar8           *Ucs2Str,
                                   Uint16           Separator,
                                   TimeClock_Stru   *pstDst)
{
  unsigned char i;
  unsigned char strlen;
  unsigned char len;

  i       =  0;
  while (i < 16)                              // 求取 Unicode 码的时间格式字符串的长度
  {
    if ((*Ucs2Str != 0x00) && (*Ucs2Str != (Uchar8)(Separator >> 8)))
    {
      break;
    }
    Ucs2Str++;
    if (((*Ucs2Str < '0') || (*Ucs2Str > '9'))  && \
        (*Ucs2Str != (Uchar8)(Separator & 0x00FF)) )
    {
      Ucs2Str--;
      break;
    }
    Ucs2Str++;
    i    +=  2;
  }

  strlen  = i;
  Ucs2Str = Ucs2Str - i;
  i       = 0;
  len     = 0;
  for (i=0; i<5; i+=2)
  {
    // 搜索第一个分隔符
    if ((*Ucs2Str       == (Uchar8)(Separator >> 8)) && \
        (*(Ucs2Str + 1) == (Uchar8)(Separator & 0x00FF)))
    {
      len      = i + 2;
      Ucs2Str  = Ucs2Str - i;
      Ucs2Str += 1;
      if (i > 2)                              // 小时时间为两位
      {
        i              = *Ucs2Str - '0';
        i            <<= 4;
        Ucs2Str       += 2;
        pstDst->Hou    = ((*Ucs2Str - '0') & 0x0F) + i;
        Ucs2Str       += 1;
      }
      else
      {
        if (i)                                // 检查分隔符前是否有数据？
        {
          pstDst->Hou  = ((*Ucs2Str - '0') & 0x0F);
          Ucs2Str     += 1;
        }
        else
        {
          Ucs2Str     -= 1;
          pstDst->Hou  = 0x00;
        }
      }

      Ucs2Str += 2;                           // 跳过第一个分隔符
      // 搜索第二个分隔符
      for (i=0; i<5; i+=2)
      {
        if ((*Ucs2Str       == (Uchar8)(Separator >> 8)) && \
            (*(Ucs2Str + 1) == (Uchar8)(Separator & 0x00FF)))
        {
          len      = len + i + 2;
          Ucs2Str  = Ucs2Str - i;
          Ucs2Str += 1;
          if (i > 2)                          // 分钟时间为两位
          {
            i                = *Ucs2Str - '0';
            i              <<= 4;
            Ucs2Str         += 2;
            pstDst->Min      = ((*Ucs2Str - '0') & 0x0F) + i;
            Ucs2Str         += 1;
          }
          else
          {
            if (i)                            // 检查分隔符前是否有数据？
            {
              pstDst->Min    = ((*Ucs2Str - '0') & 0x0F);
              Ucs2Str       += 1;
            }
            else
            {
              Ucs2Str       -= 1;
              pstDst->Min    = 0x00;
            }
          }

          Ucs2Str += 2;                       // 跳过第二个分隔符
          if (len >= strlen)
          {
            pstDst->Sec = 0x00;
          }
          else
          {
            Ucs2Str += 1;
            i        = strlen - len;
            if (i > 2)                        // 秒时间为两位
            {
              i              = *Ucs2Str - '0';
              i            <<= 4;
              Ucs2Str       += 2;
              pstDst->Sec    = ((*Ucs2Str - '0') & 0x0F) + i;
            }
            else
            {
              pstDst->Sec    = ((*Ucs2Str - '0') & 0x0F);
            }
          }

          return(TRUE);
        }

        Ucs2Str += 2;
      }
      return(FALSE);
    }

    Ucs2Str += 2;
  }

  return(FALSE);
}


/*--------------------------------------------------------------------------------------
函数名称：　　在字符串 *pSource 中搜索与字符串 *pTarget 匹配的子字符串。
函数功能：    本函数是在字符串 *pSource 中搜索与字符串 *pTarget 匹配的子字符串。如果找到匹配
　　　　　　　的子字符串，返回 *pSource 中的子字符串的起始地址指针；否则返回 NULL。
函数入口参数：*pSource ------ 指向存储在 SRAM 中的源字符串的首个地址指针。
              *pTarget ------ 指向存储在 SRAM 中的目标字符串的首个地址指针。
函数返回值：　“*pSource 中的子字符串的起始地址指针”表示搜索到匹配的子字符串；
              “NULL”                               表示没有搜索到匹配的子字符串。
调用函数：　　１．
备注：        １．
--------------------------------------------------------------------------------------*/
unsigned short StringLen(void const *pSource)
{
  unsigned const short       *pSrc;

	unsigned short       SLen;
	
	SLen			=  0x00;
  pSrc    =  pSource;


	while (*pSrc != 0x0000)
    {
     SLen++;
     pSrc++;
    }

  
  return(SLen);
}

/*--------------------------------------------------------------------------------------
函数名称：　　两个8进制组合成Unicode 码
函数功能：　　本函数两个8进制组合成Unicode 码
函数入口参数：*pSrc  --------  指向存储源字符串缓冲区的指针。
  　　　　　　*pDest --------- 指向存储目标数据（十六进制数）缓冲区的指针。
　　　　　　　strLen -------   源字符串的数据字节长度（长度数值必需为偶数）。
函数返回值：　没有
调用函数：　　１.
备注：        １．
--------------------------------------------------------------------------------------*/
void CharHexConvUsc(const void *pSource, void *pTarget, const unsigned char strLen)
{
  unsigned char        i;
  const unsigned char  *pSrc;
  unsigned short       *pDst;
  unsigned char        temp1, temp2;

  pSrc      =  pSource;
  pDst      =  pTarget;
  if(strLen != 0xff && strLen < 0xff)
  {
    for(i=0; i<strLen; i+=2)
    {
      temp1 = *pSrc++;
      temp2 = *pSrc++;
      *pDst++ = (temp2 << 8) | temp1++;
    }
  }
}

/*--------------------------------------------------------------------------------------
函数名称：　　对两个字符串进行比较，然后返回比较结果
函数功能：　　本函数用于对两个字符串进行比较，然后返回比较结果
函数入口参数：*pSrc  --------  指向存储源字符串缓冲区的指针。
  　　　　　　*pDest --------- 指向存储目标数据（十六进制数）缓冲区的指针。
函数返回值：　没有
调用函数：　　１.
备注：        １．
当s1<s2时，返回为负数；
当s1==s2时，返回值= 0；
当s1>s2时，返回正数。
--------------------------------------------------------------------------------------*/
int StrCmp(const unsigned char* str1, const unsigned char* str2)
{
	int ret = 0;
	while(!(ret=*(unsigned char*)str1-*(unsigned char*)str2) && *str1)
	{
		str1++;
		str2++;
	} 
	if (ret < 0)
	{
		return -1;
	}
	else if (ret > 0)
	{
		return 1;
	}
	return 0;
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
