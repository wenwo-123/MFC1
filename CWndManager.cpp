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

int CWndManager::GetWndList(CArray<tagWndInfo>& arrWnd)  // 获取窗口列表
{
	CArray<tagEnumExeWndParam> arrEnumWnd;  // 临时数组
	
	// 调用全局引擎的窗口配置信息
	GetProcessWnd(
		g_pEngine->m_WndIni.strProc,   // 进程名
		g_pEngine->m_WndIni.strTitle,  // 窗口标题
		g_pEngine->m_WndIni.strClz,    // 窗口类名
		arrEnumWnd                      // 输出结果
	);
	for (int i = 0; i < arrEnumWnd.GetSize(); i++) 
	{
		if (arrEnumWnd[i].hWnds[0] == 0)
			continue; // 没找到，跳过

		tagWndInfo info;
		info.hWnd = arrEnumWnd[i].hWnds[0];

		TCHAR title[MAX_PATH] = { 0 }; // 窗口标题
		GetWindowText(info.hWnd, title, MAX_PATH);
		info.strTitle = title;

		::GetWindowRect(info.hWnd, info.rtWnd);

		bool bExist = false;  // 标记窗口是否已存在
		for(int j = 0; j < arrWnd.GetCount(); j++) 
		{  
			if (info.hWnd == arrWnd[j].hWnd)
			{  
				arrWnd[j].rtWnd = info.rtWnd;  // 更新窗口信息
				arrWnd[j].strTitle = info.strTitle;  
				bExist = true;    
			}
			
		}
		if(!bExist)
			arrWnd.Add(info);  // 添加到输出数组
	}

	
	// 打印找到的窗口
	for (int i = 0; i < arrEnumWnd.GetSize(); i++) {
		if (arrEnumWnd[i].hWnds[0] == 0)
			continue;
			
		TCHAR title[MAX_PATH] = { 0 };  // 在此循环中重新定义
		GetWindowText(arrEnumWnd[i].hWnds[0], title, MAX_PATH);
		LogD(_T("%p - %s"), (void*)arrEnumWnd[i].hWnds[0], title);
	}
	
	return arrWnd.GetCount();  // 建议返回找到的窗口数量
}
