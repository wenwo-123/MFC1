#include "pch.h"
#include "CEngine.h"
#include "CWndManager.h"
#include "CTaskManager.h"
#include "CTaskThread.h"
#include "locale.h"  

CEngine::CEngine() 
{
	m_pWndMgr =		new CWndManager();
	m_pTaskMgr =	new CTaskManager();
}

CEngine::~CEngine() 
{
	for (int i = 0; i < m_arrWnd.GetCount(); i++)
	{
		tagWndInfo* pInfo = m_arrWnd[i];
		delete pInfo;
	}

	m_arrWnd.RemoveAll();

	if (m_pWndMgr)
		delete m_pWndMgr;

	if (m_pTaskMgr)
		delete m_pTaskMgr;
}




void CEngine::Init()													// 获取窗口信息配置
{
	TCHAR szPath[256] = { 0 }; 
	CWHService::GetWorkDirectory(szPath, 256);							// 获取工作路径
	m_strWorkPath = szPath;

	CWHIniData ini;
	ini.SetIniFilePath(m_strWorkPath+_T("/全局配置.ini"));

	m_WndIni.strProc = ini.ReadString(_T("窗口"), _T("进程"));
	m_WndIni.strTitle = ini.ReadString(_T("窗口"), _T("标题"));
	m_WndIni.strClz = ini.ReadString(_T("窗口"), _T("类名"));
	m_WndIni.strLDPath = ini.ReadString(_T("窗口"), _T("雷电路径"));

	m_WndIni.strDisplay = ini.ReadString(_T("窗口"), _T("display"));
	m_WndIni.strMouse = ini.ReadString(_T("窗口"), _T("mouse"));
	m_WndIni.strKeypad = ini.ReadString(_T("窗口"), _T("keypad"));
	m_WndIni.strPublic = ini.ReadString(_T("窗口"), _T("public"));
	m_WndIni.iMode = ini.ReadInt(_T("窗口"), _T("mode"), 0);

	CString strTmp;
	strTmp = ini.ReadString(_T("窗口"), _T("窗口尺寸"));
	CStringArray arrTmp;
	Split(strTmp, arrTmp, _T(","));
	if (arrTmp.GetCount() == 2)
	{
		m_iWidth = _ttoi(arrTmp[0]);
		m_iHeight = _ttoi(arrTmp[1]);
	}
	LogN(_T("窗口配置文件读取完成"));
	
	LoadRes();
	LogN(_T("资源文件文件读取完成"));
}


int CEngine::GetWndList()
{
	if (m_WndIni.strLDPath.GetLength() > 5)
	{
		return m_pWndMgr->GetLDList(m_arrWnd);
	}
	else 
	{
		return m_pWndMgr->GetWndList(m_arrWnd);
	}
	
}


void CEngine::Start()											// 启动引擎
{
	for (int i = 0; i < m_arrWnd.GetCount(); i++)
	{
		tagWndInfo* pInfo = m_arrWnd[i];						// 获取窗口信息
		if (pInfo->strTitle.Find(_T("雷电模拟器-1")) != -1)
		{
			tagTaskInfo* pTask = new tagTaskInfo;				// 创建任务信息
			pTask->id = pInfo->id;
			pTask->pWnd = pInfo;								// 关联窗口信息
			pTask->pTask = new CTaskThread(pInfo);
			pTask->pTask->StartThread();
			m_arrTask.Add(pTask);

		}

	}
	
}

bool CEngine :: LoadRes() 
{
	CString strResFile = m_strWorkPath + _T("/res/res.txt");
	if (!PathFileExists(strResFile))
	{
		LogN(_T("资源文件不存在: %s"), strResFile);
		return false;
	}
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));			// 保存当前的locale设置
	setlocale(LC_CTYPE, "chs");										// 设置locale为中文，以正确处理中文字符

	CStdioFile file;												// 创建CStdioFile对象
	if (!file.Open(strResFile, CFile::modeRead))					// 打开资源文件
	{
		LogN(_T("无法打开资源文件: %s"), strResFile);
		return false;
	}

	CString strRead = _T("");										
	while (file.ReadString(strRead))								// 逐行读取文件内容
	{
		if (strRead.GetLength() < 10)								// 简单的过滤掉无效行
			continue;
		if (strRead.GetAt(0) == _T('#'))							// 过滤掉注释行
			continue;

		CStringArray arrTmp;
		Split(strRead, arrTmp, _T("="));
		if (arrTmp.GetCount() != 2)
		{
			LogE(_T("内容无法解析:%s"), strRead);
			ASSERT(FALSE);											// 断言失败，内容无法解析
			return false;
		}

		CString strName = arrTmp[0];

		bool bExist = false;
		for (int i = 0; i < m_arrRes.GetCount(); i++)
		{
			if (m_arrRes[i]->strName == strName)
			{
				bExist = true;
				break;
			}
		}

		if (bExist)
		{
			ASSERT(FALSE);											// 断言失败，资源名称重复
			continue;
		}

		CString strValue = arrTmp[1];
		arrTmp.RemoveAll();											// 清空临时数组
		Split(strValue, arrTmp, _T(">"));							//  以">"分割资源值
		if (arrTmp.GetCount() != 6)
		{
			LogE(_T("资源值无法解析:%s"), strValue);
			ASSERT(FALSE);											// 断言失败，资源值无法解析
			return false;
		}
		CStringArray arrRect;
		Split(arrTmp[0], arrRect, _T(","));
		if (arrRect.GetCount() != 4)
		{
			LogE(_T("内容无法解析:%s"), strRead);
			ASSERT(FALSE);											// 断言失败，内容无法解析
			return false;
		}

		CString strRes = arrTmp[1];
		CString strDelclr = arrTmp[2];
		CString strSim = arrTmp[3];
		CString strDir = arrTmp[4];
		CString strType = arrTmp[5];

		tagResItem* pItem = new tagResItem;
		pItem->strName = strName;
		pItem->rtArea = CRect(_ttoi(arrRect[0]), _ttoi(arrRect[1]), _ttoi(arrRect[2]), _ttoi(arrRect[3]));
		ASSERT(pItem->rtArea.left >= 0 && pItem->rtArea.left < pItem->rtArea.right);		// 断言区域坐标有效
		ASSERT(pItem->rtArea.right <= m_iWidth);
		ASSERT(pItem->rtArea.top >= 0 && pItem->rtArea.top < pItem->rtArea.bottom);
		ASSERT(pItem->rtArea.bottom <= m_iHeight);

		pItem->strRes = strRes; 
		pItem->strDeltaClr = strDelclr;
		pItem->dSim = _ttof(strSim);
		pItem->iDir = _ttoi(strDir);

		if (strType == _T("找图"))
		{
			pItem->iType = 0;
		}
		else if (strType == _T("找色"))
		{
			pItem->iType = 1;
		}
		else if (strType == _T("找字"))
		{
			pItem->iType = 2;
		}
		m_arrRes.Add(pItem);
	}


	setlocale(LC_CTYPE, old_locale);						// 恢复之前的locale设置
	free(old_locale);										// 释放之前保存的locale字符串
	file.Close();											// 关闭文件

	return true;
}


tagResItem* CEngine::GetResItem(CString& strName)
{
	for (int i = 0; i < m_arrRes.GetCount(); i++)
	{
		if (m_arrRes[i]->strName == strName)
		{
			return m_arrRes[i];
		}
	}

	return NULL;
}