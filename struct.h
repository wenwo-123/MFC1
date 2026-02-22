#pragma once


// 窗口信息结构体  
struct tagWndInfo {
	int		id;
	HWND hWnd;  // 窗口句柄
	CRect rtWnd;  // 窗口位置和大小   
	CString strTitle;  // 窗口标题

	tagWndInfo() {
		id			= 0;
		hWnd		= NULL;
		rtWnd		= CRect(0, 0, 0, 0);
		strTitle	= _T("");
	}

};

// 枚举窗口参数结构体
struct tagWndIni {
	CString strProc;  // 进程名
	CString strTitle;  // 窗口标题
	CString strClz;  // 窗口类名
	CString strLDPath;  // LD路径

	CString strDisplay;
	CString strMouse;
	CString strKeypad;
	CString strPublic;
	int iMode;


	tagWndIni()
	{
		strProc		= _T("");
		strTitle	= _T("");
		strClz		= _T("");
		strLDPath	= _T("");

		strDisplay	= _T("");
		strMouse	= _T("");
		strKeypad	= _T("");
		strPublic	= _T("");
		iMode = 0;
	}



};


class CTaskThread; // 前向声明

struct tagTaskInfo 
{
	int		id;
	tagWndInfo* pWnd;
	CTaskThread* pTask;

	tagTaskInfo()
	{
		id		= 0;
		pWnd	= NULL;
		pTask	= NULL;
	}
};