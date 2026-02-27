#pragma once


// 窗口信息结构体  
struct tagWndInfo {
	int		id;
	HWND	hWnd;			// 窗口句柄
	CRect	rtWnd;			// 窗口位置和大小   
	CString strTitle;		// 窗口标题

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
	int		iMode;  


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


class	CTaskThread; // 前向声明
struct	tagTaskInfo 
{
	int				id;
	tagWndInfo*		pWnd;
	CTaskThread*	pTask;

	tagTaskInfo()
	{
		id		= 0;
		pWnd	= NULL;
		pTask	= NULL;
	}
};


struct tagResItem 
{
	CString strName;		// 资源名称，用于标识该资源（例如控件、图片或文本的键）
	int     iType;			// 资源类型，建议用枚举或常量表示（例如 0=找图, 1=找色, 2=找字 等）
	CRect	rtArea;			// 资源在窗口中的位置和大小
	CString strRes;			// 资源内容，可以是文件路径、文本内容或其他资源标识符
	CString strDeltaClr;	// 资源的颜色偏移值，格式可以是 "R,G,B" 或其他适合的表示方法
	double	dSim;			// 资源相似度，范围从 0.0 到 1.0，表示资源与目标的匹配程度
	int		iDir;			// 资源的方向，建议用枚举或常量表示（例如 0=无, 1=向上, 2=向下, 3=向左, 4=向右 等）

	tagResItem()
	{
		strName		= _T("");
		iType		= 0;
		rtArea		= CRect(0, 0, 0, 0);
		strRes		= _T("");
		strDeltaClr = _T("");
		dSim		= 0.0;
		iDir		= 0;
	}
};
