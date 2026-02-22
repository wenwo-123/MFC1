#pragma once
#include "WHThread.h"

class dmsoft;

class CTaskThread : public CWHThread
{
public:
	CTaskThread(CString strTitle); // 构造函数
	~CTaskThread();

public:
	virtual bool OnEventThreadRun()override;  // 线程运行事件
	virtual bool OnEventThreadStart()override; // 线程开始事件
	virtual bool OnEventThreadConclude()override; // 线程终止事件


private:
	dmsoft* m_pDm;
	CString m_strTitle; // 任务标题
	bool m_bind;

};







