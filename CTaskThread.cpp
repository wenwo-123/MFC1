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

		}


	}
	//执行绑定过程

	Sleep(5000);
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
	//绑定窗口
	long ret = m_pDm->BindWindowEx((long)m_pWnd->hWnd,
				g_pEngine->m_WndIni.strDisplay,
				g_pEngine->m_WndIni.strMouse,
				g_pEngine->m_WndIni.strKeypad,
				g_pEngine->m_WndIni.strPublic,
				g_pEngine->m_WndIni.iMode
);
	if (ret == 1)
		return true;
	
	return false;
}