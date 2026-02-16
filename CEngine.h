#pragma once

class CWndManager; // 前向声明

// 引擎类
class CEngine
{
public: // 构造和析构
	CEngine();  
	~CEngine();  

public:
	void Init();   // 获取窗口信息配置

public: 
	tagWndIni m_WndIni;  // 窗口信息
	CString m_strWorkPath;   // 工作路径

private:
	// 窗口管理
	CWndManager* m_wndMgr;  // 窗口管理器指针
	
};



