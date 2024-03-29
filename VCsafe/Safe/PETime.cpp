// PETime.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "PETime.h"
#include "afxdialogex.h"


// PETime 对话框

IMPLEMENT_DYNAMIC(PETime, CDialogEx)

PETime::PETime(DWORD TimeDateStamp, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TIME, pParent)
	, m_TimeStamp(_T(""))
	, m_TimeData(_T(""))
{
	m_dwTimeDateStamp = TimeDateStamp;
}

PETime::~PETime()
{
}

void PETime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TIMEDSTAMP, m_TimeStamp);
	DDX_Text(pDX, IDC_EDIT_TIMEDATA, m_TimeData);
}


BEGIN_MESSAGE_MAP(PETime, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TIME, &PETime::OnBnClickedButtonTime)
END_MESSAGE_MAP()


// PETime 消息处理程序
BOOL PETime::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	TimeStamp2TimeData(m_dwTimeDateStamp);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//将时间标志转换北京时间
void PETime::TimeStamp2TimeData(DWORD TimeDateStamp)
{
	//获取时间标志
	time_t t = TimeDateStamp;
	char buf[65] = {};
	tm* FileTime = new tm{};

	//解码
	localtime_s(FileTime, &t);
	strftime(buf, 64, "%Y-%m-%d %H:%M:%S", FileTime);

	//显示
	m_TimeData = buf;
	m_TimeStamp.Format(L"%p", TimeDateStamp);
}

//控件按钮，计算
void PETime::OnBnClickedButtonTime()
{
	// TODO: 在此添加控件通知处理程序代码
	//清空当前时间
	m_dwTimeDateStamp = 0;
	m_TimeStamp = "";

	//重新获取时间
	UpdateData(TRUE);

	//计算
	m_dwTimeDateStamp = wcstoll(m_TimeStamp, NULL, 16);
	TimeStamp2TimeData(m_dwTimeDateStamp);

	//刷新
	UpdateData(FALSE);
}

