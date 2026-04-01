#pragma once
// 窗口管理器类
class CWndManager
{
public:
	CWndManager();  // 构造函数
	~CWndManager();  // 析构函数


	int GetWndList();  // 获取窗口列表
	int GetLDList();

private:
	CString ExeCmd(CString pszCmd);
	CString ListVM();



}; 

