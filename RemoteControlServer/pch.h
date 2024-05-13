// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

#define STATUS_TIMER_INFO 1		///状态信息定时器

//***********************  协议   ****************************
typedef struct _MSGHEAD
{
	int msg;		//命令
	int size;		//数据大小
}MSGHEAD;

enum COMMAND
{
	CLIENT_START_MONITOR		= 0x1000,			///开始监控
	CLIENT_STOP_MONITOR			= 0x1001,			///停止监控

	CLIENT_IMAGE			= 0X10010,			///发送图像
};
typedef struct _IMAGEDATA
{

	BITMAP logbmp;
	char* data;
}IMAGEDATA;
#define SERVER_PORT 2023		///程序开放端口

#endif //PCH_H
