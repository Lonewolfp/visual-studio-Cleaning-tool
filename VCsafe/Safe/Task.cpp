// Task.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "Task.h"
#include "afxdialogex.h"
#include"Process.h"
#include"DesktopWindow.h"
#include "Services.h"
#include "Run.h"
#include"Software.h"


// Task 对话框

IMPLEMENT_DYNAMIC(Task, CDialogEx)

Task::Task(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TAB1, pParent)
{

}

Task::~Task()
{
}

void Task::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Task, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Task::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_DESKWIN, &Task::OnBnClickedButtonWindow)
	ON_BN_CLICKED(IDC_BUTTON_SERVICES, &Task::OnBnClickedButtonServices)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &Task::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_SOFTWARE, &Task::OnBnClickedButtonSoftware)
END_MESSAGE_MAP()


// Task 消息处理程序


//进入进程管理
void Task::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	Process*ppr = new Process;
	ppr->DoModal();
}

//进入桌面窗口
void Task::OnBnClickedButtonWindow()
{
	// TODO: 在此添加控件通知处理程序代码
	DesktopWindow pWin = new DesktopWindow;
	pWin.DoModal();
}

//服务信息
void Task::OnBnClickedButtonServices()
{
	// TODO: 在此添加控件通知处理程序代码
	Services pSe = new Services;
	pSe.DoModal();
}

//启动项
void Task::OnBnClickedButtonRun()
{
	// TODO: 在此添加控件通知处理程序代码
	Run pRun = new Run;
	pRun.DoModal();
}

//软件管理
void Task::OnBnClickedButtonSoftware()
{
	// TODO: 在此添加控件通知处理程序代码
	Software pSof = new Software;
	pSof.DoModal();
}
