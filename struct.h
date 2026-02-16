#pragma once


// 窗口信息结构体  
struct tagWndInfo {
	HWND hWnd;  // 窗口句柄
	CRect rtWnd;  // 窗口位置和大小   
	CString strTitle;  // 窗口标题

	tagWndInfo() {

		hWnd = NULL;
		rtWnd = CRect(0, 0, 0, 0);
		strTitle = _T("");
	}

};

// 枚举窗口参数结构体
struct tagWndIni {
	CString strProc;  // 进程名
	CString strTitle;  // 窗口标题
	CString strClz;  // 窗口类名


	tagWndIni()
	{
		strProc = _T("");
		strTitle = _T("");
		strClz = _T("");
	}



};