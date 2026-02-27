#include "pch.h"
#include "CTaskThread.h"
#include "obj.h"
#include "CEngine.h"

extern CEngine* g_pEngine;  // 声明全局指针，指向CEngine实例

CTaskThread::CTaskThread(tagWndInfo* pWnd)
{
	m_pWnd = pWnd;
	m_pDm = new dmsoft();
	m_bind = false;
}

CTaskThread::~CTaskThread()
{

}

bool CTaskThread::OnEventThreadRun()

{
	if (!::IsWindow(m_pWnd->hWnd))                      // 先检查窗口是否还有效
	{
		LogE(_T("%s: 窗口句柄无效或已关闭"), m_pWnd->strTitle);
		return false; // 终止线程
	}
	
	if (!m_bind)
	{
		LogN(_T("%s: 开始绑定窗口"), m_pWnd->strTitle);
		if (Bind())
		{
			
			m_bind = true;
			// 根据需求判断是否需要调整窗口大小
			if (m_pWnd->rtWnd.Width() != g_pEngine->m_iWidth ||
				m_pWnd->rtWnd.Height() != g_pEngine->m_iHeight)
			{
				::MoveWindow(m_pWnd->hWnd, 0, 0, g_pEngine->m_iWidth, g_pEngine->m_iHeight, TRUE);
			}
			LogN(_T("%s: 绑定窗口成功"), m_pWnd->strTitle);
			m_pDm->SetPath(g_pEngine->m_strWorkPath + _T("/res"));
		} 
		else {
			Sleep(1000); // 失败后延迟重试
		}

	}


	// 第二阶段：绑定成功后，执行具体任务
	CPoint ptFind;
	if (Find(_T("包裹按钮"), ptFind))
	{
		LogD(_T("包裹按钮: %d, %d"), ptFind.x, ptFind.y);
	}
	
	//DoTask();

	Sleep(1000);
	return true;
}


bool CTaskThread::OnEventThreadStart() // 线程开始事件
{
	return true;
}

bool CTaskThread::OnEventThreadConclude()
{
	LogN(_T("%s: 任务线程结束"), m_pWnd->strTitle);
	return true;
}

bool CTaskThread::Bind()
{
	// 先检查窗口是否有效
	if (!::IsWindow(m_pWnd->hWnd))
	{
		LogE(_T("%s: 窗口句柄无效，无法绑定"), m_pWnd->strTitle);
		return false;
	}
	// 先尝试解绑，确保没有旧的绑定状态
	m_pDm->UnBindWindow();

	//绑定窗口
	long ret = m_pDm->BindWindowEx((long)m_pWnd->hWnd,
		g_pEngine->m_WndIni.strDisplay,
		g_pEngine->m_WndIni.strMouse,
		g_pEngine->m_WndIni.strKeypad,
		g_pEngine->m_WndIni.strPublic,
		g_pEngine->m_WndIni.iMode
	);


	if (ret == 1)
	{
		LogN(_T("%s: 绑定成功"), m_pWnd->strTitle);
		return true;
	}
	else {
		LogE(_T("%s: 绑定失败，返回值: %d"), m_pWnd->strTitle, ret);
		m_pDm->UnBindWindow();
		return false;
	}
}



void CTaskThread::Click(CPoint ptClk)
{
	Click(ptClk.x, ptClk.y);
}

void CTaskThread::Click(int x, int y)
{
	m_pDm->MoveTo(x, y);
	Sleep(50 + rand() % 100);
	m_pDm->LeftClick();
}


bool CTaskThread::Find(CString strUI, CPoint& ptFind)
{
	tagResItem* pItem = g_pEngine->GetResItem(strUI);
	if (!pItem)
	{
		LogE(_T("%s: 没有发现该资源项"), strUI);
		return false;
	}
	long x, y = -1;
	long ret = -1;
	if (pItem->iType == 0)									// 找图
	{
		ret = m_pDm->FindPic(pItem->rtArea.left - 1,
						pItem->rtArea.top -1 ,
						pItem->rtArea.right + 1,
						pItem->rtArea.bottom + 1,
						pItem->strRes,pItem->strDeltaClr, pItem->dSim, pItem->iDir,&x,&y);
	}
	else if (pItem->iType == 1)								// 找色
	{
		ret = m_pDm->FindColor(pItem->rtArea.left - 1,
				pItem->rtArea.top - 1,
				pItem->rtArea.right + 1, 
				pItem->rtArea.bottom + 1,
				pItem->strRes,  pItem->dSim, pItem->iDir, &x, &y);
	}
	//else if (pItem->iType == 2) // 找字
	//{
	//	ret = m_pDm->FindStr(pItem->rtArea.left, pItem->rtArea.top, pItem->rtArea.right, pItem->rtArea.bottom,
	//		pItem->strRes, pItem->strDeltaClr, pItem->dSim, pItem->iDir);
	//}
	ptFind.x = x;
	ptFind.y = y;
	return x != -1;
}


bool CTaskThread::Find(CString strUI)
{
	CPoint ptFind;
	return Find(strUI, ptFind);
}


void CTaskThread::DoTask()
{
	for (int i = 0; i < 5; i++)
	{





		Sleep(2000);
	}

}