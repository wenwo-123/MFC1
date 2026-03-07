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
			// m_pDm->DownCpu(1,50);											// 降低CPU占用
			m_pDm->EnableRealMouse(2, 30 + rand() % 50, 20 + rand() % 50); // 启用真实鼠标，增加随机延迟模拟人类操作
			m_pDm->EnableRealKeypad(1);									  // 启用真实键盘，增加随机延迟模拟人类操作
			for (int i = 0; i < g_pEngine->m_arrRes.GetCount(); i++)
			{
				tagResItem* pItem = g_pEngine->m_arrRes[i];
				if (pItem->iType == 3)
				{
					m_pDm->SetDict(pItem->iDir, pItem->strRes);
				}
			}
		} 
		else {
			Sleep(1000); // 失败后延迟重试
		}

	}


	//// 第二阶段：绑定成功后，执行具体任务
	//CPoint ptFind;
	//if (Find(_T("包裹按钮"), ptFind))
	//{
	//	LogD(_T("包裹按钮: %d, %d"), ptFind.x, ptFind.y);
	//}
	
	//DoTask();

	Sleep(1);
	return OnLoop();
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

void CTaskThread::DClick(CPoint ptClk)
{
	DClick(ptClk.x, ptClk.y);
}

void CTaskThread::DClick(int x, int y)
{
	m_pDm->MoveTo(x, y);
	Sleep(50 + rand() % 100);
	m_pDm->LeftClick();
	Sleep(20 + rand() % 50);
	m_pDm->LeftClick();
}

void CTaskThread::RClick(CPoint ptClk) // 右键点击
{
	RClick(ptClk.x, ptClk.y);
}

void CTaskThread::RClick(int x, int y)
{
	m_pDm->MoveTo(x, y);
	Sleep(50 + rand() % 100);
	m_pDm->RightClick();		
}

void CTaskThread::DRClick(CPoint ptClk)
{
	DRClick(ptClk.x, ptClk.y);
}

void CTaskThread::DRClick(int x, int y)
{
	m_pDm->MoveTo(x, y);
	Sleep(50 + rand() % 100);
	m_pDm->RightClick();
	Sleep(20 + rand() % 50);
	m_pDm->RightClick();
}

void CTaskThread::KeyDown(long vk) 
{
	m_pDm->KeyDown(vk);
}

void CTaskThread::KeyPress(long vk)
{
	m_pDm->KeyPress(vk);
}

void CTaskThread::KeyUp(long vk)
{
	m_pDm->KeyUp(vk);
}

CString CTaskThread::Ocr(CString strUI, CRect rtArea)
{
	tagResItem* pItem = g_pEngine->GetResItem(strUI);
	if (!pItem)
	{
		LogE(_T("%s: 没有发现该资源项"), strUI);
		return _T("");
	}

	if (rtArea.left != 0)
		pItem->rtArea = rtArea;

	m_pDm->UseDict(pItem->iDir);
	return m_pDm->Ocr(pItem->rtArea.left, pItem->rtArea.top, pItem->rtArea.right, pItem->rtArea.bottom, pItem->strDeltaClr, pItem->dSim);
}

bool CTaskThread::Find(CString strUI, CPoint& ptFind, CRect rtArea)
{
	tagResItem* pItem = g_pEngine->GetResItem(strUI);
	if (!pItem)
	{
		LogE(_T("%s: 没有发现该资源项"), strUI);
		return false;
	}

	if (rtArea.left != 0)
	{
		pItem->rtArea = rtArea;
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
	
	ptFind.x = x;
	ptFind.y = y;
	return x != -1;
}

bool CTaskThread::Find(CString strUI, CRect rtArea)
{
	CPoint ptFind;
	return Find(strUI, ptFind);
}

void CTaskThread::MoveTo(int x, int y)
{
	m_pDm->MoveTo(x, y);
}

void CTaskThread::MoveR(int rx, int ry)
{
	m_pDm->MoveR(rx, ry);
}

void CTaskThread::LeftClick()
{
	m_pDm->LeftClick();
}

void CTaskThread::LeftDown()
{
	m_pDm->LeftDown();
}

void CTaskThread::LeftUp()
{
	m_pDm->LeftUp();
}

void CTaskThread::RightClick()
{
	m_pDm->RightClick();
}

void CTaskThread::RightDown()
{
	m_pDm->RightDown();
}

void CTaskThread::RightUp()
{
	m_pDm->RightUp();
}

void CTaskThread::WheelDown()
{
	m_pDm->WheelDown();
}

void CTaskThread::WheelUp()
{
	m_pDm->WheelUp();
}


















void CTaskThread::DoTask()
{
	for (int i = 0; i < 5; i++)
	{





		Sleep(2000);
	}

}