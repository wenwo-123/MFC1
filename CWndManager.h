#pragma once
// 窗口管理器类
class CWndManager
{
public:
	CWndManager();  // 构造函数
	~CWndManager();  // 析构函数


	int GetWndList(CArray<tagWndInfo>& arrWnd);  // 获取窗口列表
	int GetLDList(CArray<tagWndInfo>& arrWnd);

private:
	CString ExeCmd(CString pszCmd);
	CString ListVM();



};

