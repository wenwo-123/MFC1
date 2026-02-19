#include "pch.h"
#include "CEngine.h"
#include "CWndManager.h"


CEngine::CEngine() 
{
	m_wndMgr = new CWndManager(); // 
}

CEngine::~CEngine() 
{

}


void CEngine::Init()  // 获取窗口信息配置
{
	TCHAR szPath[256] = { 0 }; 
	CWHService::GetWorkDirectory(szPath, 256);  // 获取工作路径
	m_strWorkPath = szPath;

	CWHIniData ini;
	ini.SetIniFilePath(m_strWorkPath+_T("/全局配置.ini"));

	m_WndIni.strProc = ini.ReadString(_T("窗口"), _T("进程"));
	m_WndIni.strTitle = ini.ReadString(_T("窗口"), _T("标题"));
	m_WndIni.strClz = ini.ReadString(_T("窗口"), _T("类名"));
	m_WndIni.strLDPath = ini.ReadString(_T("窗口"), _T("雷电路径"));
}

int CEngine::GetWndList()
{
	if (m_WndIni.strLDPath.GetLength() > 5)
	{
		return m_wndMgr->GetLDList(m_arrWnd);
	}
	else 
	{
		return m_wndMgr->GetWndList(m_arrWnd);
	}
	
}
