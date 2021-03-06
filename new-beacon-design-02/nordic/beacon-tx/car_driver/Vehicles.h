//=======================================================================
//.T		包含文件.
//=======================================================================
#include <stdbool.h>
#include <stdint.h>
//-----------------------
#ifndef		__JC_VEHICLES_H__
#define		__JC_VEHICLES_H__

//=======================================================================
//.T		代码表格定义.
//=======================================================================
const	static	uint8_t	gun_ID_range[6] =
{	//打枪最小值.
	0x35,
	0x41,
	0x51,
	0x61,
	0x77,
	0x91
};
//=======================================================================
//.T		全局函数声明.
//=======================================================================
//-----------------------
void REV_main_initialize(void);
void REV_main_Loop(void);
//=======================================================================
#endif	//__JC_VEHICLES_H__.

//********************************* end *********************************//
