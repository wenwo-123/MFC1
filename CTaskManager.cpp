#include "pch.h"
#include "CTaskManager.h"
#include "CEngine.h"
#include "CGameScript.h"

extern CEngine* g_pEngine;

CTaskManager::CTaskManager()
{

}

CTaskManager::~CTaskManager()
{


}


tagTaskInfo* CTaskManager::GetInfo(int id) // 根据id获取任务信息
{
	for (int i = 0; i < g_pEngine->m_arrTask.GetCount(); i++)
	{
		if (g_pEngine->m_arrTask[i]->id == id)
		{
			return g_pEngine->m_arrTask[i];
		}
	}

	return NULL;
}


void CTaskManager::Start(int id)
{
	for (int i = 0; i < g_pEngine->m_arrWnd.GetCount(); i++)
	{
		tagWndInfo* pInfo = g_pEngine->m_arrWnd[i];	

		bool bDo = false;
		if (id == -1)
			bDo = true;
		else if (pInfo->id == id)
			bDo = true;

		if (pInfo->strTitle.Find(_T("-")) == -1) // 标题不包含"-"的窗口不启动任务
			bDo = false;

		if (pInfo->hWnd == NULL || !IsWindow(pInfo->hWnd))
			bDo = false;

		if (bDo)
		{
			tagTaskInfo* pTask = GetInfo(id);
			if (pTask)
			{
				LogE(_T("CTaskManager: 启动任务失败 任务已经启动 ID:%d 标题:%s"), pTask->id, pTask->pWnd->strTitle);
				continue;
			}
			else {

				pTask = new tagTaskInfo;
			}

			pTask->id = pInfo->id;
			pTask->pWnd = pInfo;
			pTask->pTask = new CGameScript(pInfo);
			pTask->pTask->StartThread();

			g_pEngine->m_arrTask.Add(pTask);

		}


		
		
		
		//// 获取窗口信息
		//if (pInfo->strTitle.Find(_T("雷电模拟器-1")) != -1)
		//{
		//	tagTaskInfo* pTask = new tagTaskInfo;				// 创建任务信息
		//	pTask->id = pInfo->id;
		//	pTask->pWnd = pInfo;								// 关联窗口信息
		//	pTask->pTask = new CGameScript(pInfo);
		//	pTask->pTask->StartThread();
		//	g_pEngine->m_arrTask.Add(pTask);
		//}

	}
}

void CTaskManager::Stop(int id)
{
	for (int i = 0; i < g_pEngine->m_arrTask.GetCount(); i++)
	{
		bool bDo = false;
		if (id == -1) 
		{
			bDo = true;
		}
		else if (g_pEngine->m_arrTask[i]->id == id)
		{
			bDo = true;
		}

		if (bDo)
		{
			if (g_pEngine->m_arrTask[i]->pTask->StopThread(30000))
			{
				LogN(_T("CTaskManager: 停止任务成功 ID:%d 标题:%s"), g_pEngine->m_arrTask[i]->id, g_pEngine->m_arrTask[i]->pWnd->strTitle);
			}
			else
			{
				LogE(_T("CTaskManager: 停止任务超时 ID:%d 标题:%s"), g_pEngine->m_arrTask[i]->id, g_pEngine->m_arrTask[i]->pWnd->strTitle);
			}
		}
	}
}

void CTaskManager::Suspend(int id)
{
	for (int i = 0; i < g_pEngine->m_arrTask.GetCount(); i++)
	{
		bool bDo = false;
		if (id == -1)
		{
			bDo = true;
		}
		else if (g_pEngine->m_arrTask[i]->id == id)
		{
			bDo = true;
		}

		if (bDo)
		{
			g_pEngine->m_arrTask[i]->pTask->SuspendThread();
			LogN(_T("CTaskManager: 暂停任务成功 ID:%d 标题:%s"), g_pEngine->m_arrTask[i]->id, g_pEngine->m_arrTask[i]->pWnd->strTitle);
		}

	}

}