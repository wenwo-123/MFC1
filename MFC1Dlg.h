
// MFC1Dlg.h: 头文件
//

#pragma once


// CMFC1Dlg 对话框
class CMFC1Dlg : public CDialogEx
{
// 构造
public:
	CMFC1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private: // 存放变量
	CListCtrl m_lstWnd;
	CListCtrl m_lstTask;
	CListCtrl m_lstTaskRun;
	CTraceServiceControl	m_TraceServiceControl;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public: // 存放函数

	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonSuspend();

	afx_msg void OnLvnItemchangedListTaskRun(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
