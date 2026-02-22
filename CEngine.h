#pragma once

class CWndManager; // 前向声明
class CTaskManager;

// 引擎类
class CEngine
{
public: // 构造和析构
	CEngine();  
	~CEngine();  

public:
	void Init();   // 获取窗口信息配置
	int GetWndList(); // 获取窗口列表
	void Start();  // 启动引擎

public: 
	tagWndIni	m_WndIni;  // 窗口信息
	CString		m_strWorkPath;   // 工作路径
	int			m_iWidth;
	int			m_iHeight;

public:
	CArray<tagWndInfo*> m_arrWnd; 
	CArray<tagTaskInfo*> m_arrTask;

private:
	// 窗口管理
	CWndManager* m_pWndMgr;  // 窗口管理器指针
	CTaskManager* m_pTaskMgr; // 任务管理器指针
	
};



