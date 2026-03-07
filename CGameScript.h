#pragma once
#include "CTaskThread.h"
class CGameScript : public CTaskThread
{
public:
	CGameScript(tagWndInfo* pWnd);
	~CGameScript();

protected:
	virtual bool OnLoop();

private:
	CString Ê¶±ð×ø±ê();
};

