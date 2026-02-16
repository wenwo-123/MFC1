#include "pch.h"
#include "CWndManager.h"
#include "CEngine.h"

extern CEngine* g_pEngine;  // 声明全局指针，指向CEngine实例

CWndManager::CWndManager() // 构造函数
{

}

CWndManager::~CWndManager()  // 析构函数
{

}

bool CWndManager::GetWndList(CArray<tagWndInfo>& arrWnd)  // 获取窗口列表
{
	CArray<tagEnumExeWndParam> arrEnumWnd;  // 临时数组
	
	// 调用全局引擎的窗口配置信息
	GetProcessWnd(
		g_pEngine->m_WndIni.strProc,   // 进程名
		g_pEngine->m_WndIni.strTitle,  // 窗口标题
		g_pEngine->m_WndIni.strClz,    // 窗口类名
		arrEnumWnd                      // 输出结果
	);
	
	// 打印找到的窗口
	for (int i = 0; i < arrEnumWnd.GetSize(); i++) {
		LogD(_T("%x - %s - %s"), 
			arrEnumWnd[i].hWnds[0],      // 窗口句柄
			arrEnumWnd[i].szTitleWord,   // 标题
			arrEnumWnd[i].szClzWord);    // 类名
	}
	
	return false;
}
