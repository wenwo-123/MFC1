#include "pch.h"
#include "CTaskThread.h"
#include "obj.h"


CTaskThread::CTaskThread(CString strTitle)
{
	m_strTitle = strTitle;
}

CTaskThread::~CTaskThread()
{

}

bool CTaskThread::OnEventThreadRun()

{
	LogN(_T("%s: 任务线程执行中"), m_strTitle);
	if (!m_bind)
	{
		//执行绑定过程
	}
	return true;
}

bool CTaskThread::OnEventThreadStart() // 线程开始事件
{
	return true;
}

bool CTaskThread::OnEventThreadConclude()
{
	LogN(_T("%s: 任务线程结束"), m_strTitle);
	return true;
}