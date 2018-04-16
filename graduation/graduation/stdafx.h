// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN  
#include "targetver.h"
           // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <WinSock2.h>
#include <windows.h>
#include <objbase.h>
#include <Shlwapi.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "ole32.lib")

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <string>

using namespace std;

#define BUFSIZE 1024

#include "base64.h"
#include "CSmtp.h"

// TODO: 在此处引用程序需要的其他头文件
