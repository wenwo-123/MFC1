#pragma once
#include "WHThread.h"

class dmsoft;

class CTaskThread : public CWHThread
{
public:
	CTaskThread(tagWndInfo* pWnd); // 构造函数
	~CTaskThread();


private:
	dmsoft* m_pDm;
	tagWndInfo* m_pWnd;
	bool m_bind;

private:
	bool Bind();


public:
	virtual bool OnEventThreadRun()override;  // 线程运行事件
	virtual bool OnEventThreadStart()override; // 线程开始事件
	virtual bool OnEventThreadConclude()override; // 线程终止事件


};







