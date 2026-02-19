#include "pch.h"
#include "CWndManager.h"
#include "CEngine.h"

extern CEngine* g_pEngine;  // 声明全局指针，指向CEngine实例

CWndManager::CWndManager() // 构造函数
{

}

CWndManager::~CWndManager()  // 析构函数
{

}

// 获取窗口列表
int CWndManager::GetWndList(CArray<tagWndInfo>& arrWnd)  // 获取窗口列表
{
	CArray<tagEnumExeWndParam> arrEnumWnd;  // 临时数组
	
	// 调用全局引擎的窗口配置信息
	GetProcessWnd
	(
		g_pEngine->m_WndIni.strProc,   // 进程名
		g_pEngine->m_WndIni.strTitle,  // 窗口标题
		g_pEngine->m_WndIni.strClz,    // 窗口类名
		arrEnumWnd                      // 输出结果
	);

	for (int i = 0; i < arrEnumWnd.GetSize(); i++) 
	{
		if (arrEnumWnd[i].hWnds[0] == 0)
			continue; // 没找到，跳过

		tagWndInfo info;
		info.id = (int)arrEnumWnd[i].hWnds[0];
		info.hWnd = arrEnumWnd[i].hWnds[0];

		TCHAR title[MAX_PATH] = { 0 }; // 窗口标题
		GetWindowText(info.hWnd, title, MAX_PATH);
		info.strTitle = title;
		::GetWindowRect(info.hWnd, info.rtWnd);

		bool bExist = false;  // 标记窗口是否已存在
		for(int j = 0; j < arrWnd.GetCount(); j++) 
		{  
			if (info.hWnd == arrWnd[j].hWnd)
			{  
				arrWnd[j].rtWnd = info.rtWnd;  // 更新窗口信息
				arrWnd[j].strTitle = info.strTitle;  
				bExist = true;    
			}
			
		}
		if(!bExist)
			arrWnd.Add(info);  // 添加到输出数组
	}

	
	// 打印找到的窗口
	for (int i = 0; i < arrEnumWnd.GetSize(); i++) {
		if (arrEnumWnd[i].hWnds[0] == 0)
			continue;
			
		TCHAR title[MAX_PATH] = { 0 };  // 在此循环中重新定义
		GetWindowText(arrEnumWnd[i].hWnds[0], title, MAX_PATH); // 获取窗口标题
		LogD(_T("%p - %s"), (void*)arrEnumWnd[i].hWnds[0], title); // 打印窗口句柄和标题
	}
	
	return arrWnd.GetCount();  // 建议返回找到的窗口数量
}


// 获取雷电模拟器列表
int CWndManager::GetLDList(CArray<tagWndInfo>& arrWnd)
{
	CString strRet = ListVM(); // 获取雷电模拟器列表
	if (strRet.GetLength() < 1) // 如果没有获取到列表，返回0
		return 0;
	CStringArray strArray;
	Split(strRet, strArray, _T("\r\n")); // 将列表按行分割成数组

	for (int i = 0; i < strArray.GetSize(); i++) // 遍历数组
	{
		//0,雷电模拟器,0,0,0,-1,-1
		tagWndInfo infoTmp;
		CString strVm = strArray.GetAt(i); // 获取当前行
		CString strTmp; 

		AfxExtractSubString(strTmp, (LPCTSTR)strVm, 0, _T(','));
		infoTmp.id = _ttoi(strTmp);

		AfxExtractSubString(strTmp, (LPCTSTR)strVm, 1, _T(','));
		infoTmp.strTitle = strTmp; // 提取模拟器名称作为窗口标题

		AfxExtractSubString(strTmp, (LPCTSTR)strVm, 3, _T(','));
		infoTmp.hWnd = (HWND)_ttoi(strTmp); // 提取窗口句柄

		if (infoTmp.hWnd != NULL)
			::GetWindowRect(infoTmp.hWnd, infoTmp.rtWnd); // 获取窗口位置和大小
		bool bExist = false;
		for (int j = 0; j < arrWnd.GetCount(); j++)
		{
			if (arrWnd[j].id == infoTmp.id) // 根据id判断是否已存在
			{
				arrWnd[j].strTitle = infoTmp.strTitle; // 更新窗口标题
				arrWnd[j].rtWnd = infoTmp.rtWnd;  // 更新窗口位置和大小
				bExist = true;
				break;
			}


		}
		if (!bExist)
			arrWnd.Add(infoTmp);
	}
	return arrWnd.GetCount();
}


CString CWndManager::ExeCmd(CString pszCmd)
{
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	HANDLE hRead, hWrite;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return _T("");
	}

	// 设置命令行进程启动信息(以隐藏方式启动命令并定位其输出到hWrite
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;

	// 启动命令行
	PROCESS_INFORMATION pi;
	if (!CreateProcess(NULL, pszCmd.GetBuffer(), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		pszCmd.ReleaseBuffer();
		return _T("");
	}

	pszCmd.ReleaseBuffer();

	CloseHandle(hWrite);

	// 读取命令行返回值
	CStringA strRetTmp;
	char buff[1024 * 2] = { 0 };
	DWORD dwRead = 0;
	strRetTmp = buff;
	while (ReadFile(hRead, buff, 1024 * 2, &dwRead, NULL))
	{
		strRetTmp += buff;
	}
	CloseHandle(hRead);

	LPCSTR pszSrc = strRetTmp.GetString();
	int nLen = MultiByteToWideChar(CP_ACP, 0, buff, -1, NULL, 0);
	if (nLen == 0)
		return _T("");

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return _T("");

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	CString strRet(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return strRet;
}


CString CWndManager::ListVM() // 获取雷电模拟器列表
{
	CString strParam;

	if (g_pEngine->m_WndIni.strLDPath != _T(""))
		strParam.Format(_T("%s\\ldconsole.exe list2"), g_pEngine->m_WndIni.strLDPath); // 构造命令行参数

	return ExeCmd(strParam);
}

