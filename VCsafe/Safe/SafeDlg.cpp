
// SafeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "SafeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

CAboutDlg::CAboutDlg()/* : CDialogEx(IDD_ABOUTBOX)*/
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSafeDlg 对话框



CSafeDlg::CSafeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SAFE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSafeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_maintab);
}

BEGIN_MESSAGE_MAP(CSafeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSafeDlg::OnTcnSelchangeTab1)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32804, ID_32809, OnMenuTop)			//主菜单消息处理;
	ON_BN_CLICKED(IDC_BUTTON1, &CSafeDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSafeDlg 消息处理程序

BOOL CSafeDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码

	//注册全局快捷键;
	::RegisterHotKey(GetSafeHwnd(), 0x1234, MOD_CONTROL, 'H');	//必须大写字母;

	m_maintab.InsertItem(0, L"任务管理");
	m_maintab.InsertItem(1, L"实用工具");
	m_maintab.InsertItem(2, L"查毒功能");

	CRect rect;
	m_para1.Create(IDD_DIALOG_TAB1, &m_maintab);
	m_para2.Create(IDD_DIALOG_TAB2, &m_maintab);
	m_para3.Create(IDD_DIALOG_TAB3, &m_maintab);

	m_maintab.GetClientRect(&rect);
	rect.top += 30;
	rect.bottom -= 10;
	rect.left += 10;
	rect.right -= 10;

	//移动tab栏
	m_para1.MoveWindow(&rect);
	m_para2.MoveWindow(&rect);
	m_para3.MoveWindow(&rect);

	//显示内容到Tab
	m_para1.ShowWindow(true);
	m_para2.ShowWindow(false);
	m_para3.ShowWindow(false);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}





void CSafeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSafeDlg::OnPaint()
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
HCURSOR CSafeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSafeDlg::OnTcnSelchangeTab1(NMHDR * pNMHDR, LRESULT * pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	int CurSel = m_maintab.GetCurSel();

	switch (CurSel)
	{
	case 0:
		m_para1.ShowWindow(true);
		m_para2.ShowWindow(false);
		m_para3.ShowWindow(false);
		break;
	case 1:
		m_para1.ShowWindow(false);
		m_para2.ShowWindow(true);
		m_para3.ShowWindow(false);
		break;
	case 2:
		m_para1.ShowWindow(false);
		m_para2.ShowWindow(false);
		m_para3.ShowWindow(true);
		break;
	default:;
	}
	*pResult = 0;
}


void CSafeDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取弹出菜单;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU1);

	CMenu* pSub = pMenu->GetSubMenu(0);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);
}


void CSafeDlg::OnMenuTop(UINT id)
{
	//标志位
	int Flag = 0;

	switch (id)
	{
	case ID_32804:
		LockWorkStation();
		break;
	/*case ID_32805:
		SetSuspendState(TRUE, FALSE, FALSE);
		break;*/
	/*case ID_32806:
		SetSuspendState(FALSE, FALSE, FALSE);
		break;*/
	case ID_32807:
		ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		break;
	case ID_32808:
		//关机提示;
		Flag = MessageBox(L"确定要关机吗？\r\n点击【确定】关机\r\n点击【取消】继续", L"是否关机?", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}
		break;
	case ID_32809:
		//重启提示;
		Flag = MessageBox(L"确定要重启吗？\r\n点击【确定】重启\r\n点击【取消】继续", L"是否重启?", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}
		break;
	}
}

//老板键
BOOL CSafeDlg::PreTranslateMessage(MSG * pMsg)
{
	//响应快捷键;
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 0x1234))
	{
		if (IsWindowVisible() == TRUE)
		{
			ShowWindow(SW_HIDE);
		}
		else
		{
			ShowWindow(SW_SHOW);
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
