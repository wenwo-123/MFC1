#include "pch.h"
#include "CGameScript.h"

CGameScript::CGameScript(tagWndInfo* pWnd) : CTaskThread(pWnd)
{

}

CGameScript::~CGameScript()
{
}


bool CGameScript::OnLoop()
{
	//LogD(_T("坐标:%s"), 识别坐标());
	// Sleep(1000);
	乱点();

	return true;
}

CString CGameScript::识别坐标()
{
	CString strIP = Ocr(_T("坐标区域"));
	Sleep(1000);
	return RepStr(strIP, _T(":."), _T(":"));  // 替换掉可能的干扰字符
	
}

void CGameScript::乱点()
{
	clk(_T("系统应用"));
	Sleep(2000);
	clk(_T("中间位置"));
	Sleep(2000);
}