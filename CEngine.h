#pragma once

class CWndManager;	// 前向声明
class CTaskManager;

// 引擎
class CEngine
{
public:				
	CEngine();  
	~CEngine();  

public:
	void Init();							// 获取窗口信息配置
	int GetWndList();						// 获取窗口列表
	void Start();							// 启动引擎
	tagResItem* GetResItem( CString& strName);	// 获取资源项

public: 
	tagWndIni	m_WndIni;					// 窗口信息
	CString		m_strWorkPath;				// 工作路径
	int			m_iWidth;					// 窗口宽度 
	int			m_iHeight;					// 窗口高度

	CArray<tagResItem*>	   m_arrRes; 		// 资源列表
	CArray<tagWndInfo*>	   m_arrWnd;		// 窗口列表
	CArray<tagTaskInfo*>   m_arrTask;		// 任务列表

private:
	CWndManager* m_pWndMgr;					// 窗口管理器指针
	CTaskManager* m_pTaskMgr;				// 任务管理器指针
	
private:
	bool LoadRes();							// 加载资源配置
};



