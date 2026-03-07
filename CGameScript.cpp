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
	LogD(_T("坐标:%s"), 识别坐标());
	//LogD(_T("测试运行中..."));
	Sleep(1000);
	return true;
}

CString CGameScript::识别坐标()
{
	CString strIP = Ocr(_T("坐标区域"));
	Sleep(1000);
	return RepStr(strIP, _T(":."), _T(":"));  // 替换掉可能的干扰字符
	
}