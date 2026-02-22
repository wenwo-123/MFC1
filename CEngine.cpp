#include "pch.h"
#include "CEngine.h"
#include "CWndManager.h"
#include "CTaskManager.h"
#include "CTaskThread.h"


CEngine::CEngine() 
{
	m_pWndMgr = new CWndManager();
	m_pTaskMgr = new CTaskManager();
}

CEngine::~CEngine() 
{
	for (int i = 0; i < m_arrWnd.GetCount(); i++)
	{
		tagWndInfo* pInfo = m_arrWnd[i];
		delete pInfo;
	}

	m_arrWnd.RemoveAll();

	if (m_pWndMgr)
		delete m_pWndMgr;

	if (m_pTaskMgr)
		delete m_pTaskMgr;
}




void CEngine::Init()  // 获取窗口信息配置
{
	TCHAR szPath[256] = { 0 }; 
	CWHService::GetWorkDirectory(szPath, 256);  // 获取工作路径
	m_strWorkPath = szPath;

	CWHIniData ini;
	ini.SetIniFilePath(m_strWorkPath+_T("/全局配置.ini"));

	m_WndIni.strProc = ini.ReadString(_T("窗口"), _T("进程"));
	m_WndIni.strTitle = ini.ReadString(_T("窗口"), _T("标题"));
	m_WndIni.strClz = ini.ReadString(_T("窗口"), _T("类名"));
	m_WndIni.strLDPath = ini.ReadString(_T("窗口"), _T("雷电路径"));
}

int CEngine::GetWndList()
{
	if (m_WndIni.strLDPath.GetLength() > 5)
	{
		return m_pWndMgr->GetLDList(m_arrWnd);
	}
	else 
	{
		return m_pWndMgr->GetWndList(m_arrWnd);
	}
	
}


void CEngine::Start()  // 启动引擎
{
	for (int i = 0; i < m_arrWnd.GetCount(); i++)
	{
		tagWndInfo* pInfo = m_arrWnd[i]; // 获取窗口信息
		if (pInfo->strTitle.Find(_T("雷电模拟器-1")) != -1)
		{
			tagTaskInfo* pTask = new tagTaskInfo;  // 创建任务信息
			pTask->id = pInfo->id;
			pTask->pWnd = pInfo; // 关联窗口信息
			pTask->pTask = new CTaskThread(pInfo->strTitle);
			pTask->pTask->StartThread();
			m_arrTask.Add(pTask);

		}
		
	

	}



	
}