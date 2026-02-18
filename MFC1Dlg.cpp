// MFC1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFC1.h"
#include "MFC1Dlg.h"
#include "afxdialogex.h"
#include "CEngine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 全局指针，指向CEngine实例
CEngine* g_pEngine = NULL; 


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC1Dlg 对话框

#define TIME_UPDATE_WND 1

CMFC1Dlg::CMFC1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WND, m_lstWnd);
	DDX_Control(pDX, IDC_LIST_TASK, m_lstTask);
	DDX_Control(pDX, IDC_LIST_TASK_RUN, m_lstTaskRun);

	DDX_Control(pDX, IDC_TRACE_MESSAGE , m_TraceServiceControl);
}

BEGIN_MESSAGE_MAP(CMFC1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	
	ON_BN_CLICKED(IDC_BUTTON_START, &CMFC1Dlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_SUSPEND, &CMFC1Dlg::OnBnClickedButtonSuspend)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TASK_RUN, &CMFC1Dlg::OnLvnItemchangedListTaskRun)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMFC1Dlg 消息处理程序

BOOL CMFC1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//#——————————————lstwnd设置
	DWORD dwStyle = m_lstWnd.GetExtendedStyle();  // 获取当前的扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT;  // 添加全行选择样式
	dwStyle |= LVS_EX_GRIDLINES;	  // 添加网格线样式
	dwStyle |= LVS_EX_CHECKBOXES;	  // 添加复选框样式
	m_lstWnd.SetExtendedStyle(dwStyle);  // 设置新的扩展样式
	m_lstWnd.InsertColumn(0, _T("窗口句柄"), LVCFMT_CENTER, 90); // 插入第一列
	m_lstWnd.InsertColumn(1, _T("标题"), LVCFMT_CENTER, 90); 


	//int iRow = m_lstWnd.GetItemCount();  // 获取当前行数
	//m_lstWnd.InsertItem(iRow, _T(""));  // 插入新行
	//CString strId;
	//strId.Format(_T("%d"), 1);  // 将整数转换为字符串
	//m_lstWnd.SetItemText(iRow, 0, strId);  // 设置第一列文本
	//m_lstWnd.SetItemText(iRow, 1, _T("跑图"));  // 设置第一列文本

	//iRow = m_lstWnd.GetItemCount();  
	//m_lstWnd.InsertItem(iRow, _T(""));  
	//strId.Format(_T("%d"), 2);  
	//m_lstWnd.SetItemText(iRow, 0, strId);  
	//m_lstWnd.SetItemText(iRow, 1, _T("跑图")); 

	//#——————————————m_lstTask 设置———————————————————————#
	dwStyle = m_lstTask.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;  
	dwStyle |= LVS_EX_GRIDLINES;	 
	m_lstTask.SetExtendedStyle(dwStyle);  
	m_lstTask.InsertColumn(0, _T("当前任务"), LVCFMT_CENTER, 200);

	//#——————————————m_lstTaskRun 设置
	dwStyle = m_lstTaskRun.GetExtendedStyle();  
	dwStyle |= LVS_EX_FULLROWSELECT;  
	dwStyle |= LVS_EX_GRIDLINES;	  
	m_lstTaskRun.SetExtendedStyle(dwStyle);  
	m_lstTaskRun.InsertColumn(0, _T("执行任务"), LVCFMT_CENTER, 200);
	 
	

	// 创建CEngine实例并赋值给全局指针
	g_pEngine = new CEngine();  
	g_pEngine->Init();

	SetTimer(TIME_UPDATE_WND, 5000,NULL);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFC1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// "开始"按钮点击事件处理程序
void CMFC1Dlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("Start"));
	CTraceService::TraceString(_T("启动了"), TraceLevel_Debug);
}

void CMFC1Dlg::OnBnClickedButtonSuspend()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("Suspend"));
	CTraceService::TraceString(_T("暂停了"), TraceLevel_Warning);
}

void CMFC1Dlg::OnLvnItemchangedListTaskRun(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

// 定时器事件处理程序
void CMFC1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == TIME_UPDATE_WND)
	{
		int nWndCount = g_pEngine->GetWndList(); 
		if (nWndCount > 0)
		{
			for (int i = 0; i < nWndCount; i++) 
			{
				int hWnd = (int)g_pEngine->m_arrWnd[i].hWnd;

				if (IsWndExist(hWnd))
				{
					m_lstWnd.SetItemText(i, 1, g_pEngine->m_arrWnd[i].strTitle); 
				}
				else 
				{
					int iRow = m_lstWnd.GetItemCount();  // 获取当前行数
					m_lstWnd.InsertItem(iRow, _T(""));  // 插入新行
					m_lstWnd.SetItemText(i, 0, I2S((int)g_pEngine->m_arrWnd[i].hWnd));
					m_lstWnd.SetItemText(i, 1, g_pEngine->m_arrWnd[i].strTitle);  // 设置第一列文本

				}
			}
		}
		
	}


	CDialogEx::OnTimer(nIDEvent);

}


bool  CMFC1Dlg::IsWndExist(int hwnd)
{
	for (int i = 0; i < m_lstWnd.GetItemCount(); i++) // 遍历列表控件的每一行
	{
		int iHwndTmp = _ttoi(m_lstWnd.GetItemText(i, 0));
		if (hwnd == iHwndTmp)
			return true;
	}

	return false;


}
