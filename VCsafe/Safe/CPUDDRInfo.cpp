// CPUDDRInfo.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "CPUDDRInfo.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <psapi.h>

// CPUDDRInfo 对话框

IMPLEMENT_DYNAMIC(CPUDDRInfo, CDialogEx)

CPUDDRInfo::CPUDDRInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CPUDDR, pParent)
	, m_szCPU(_T(""))
	, m_szDDR(_T(""))
	, m_szStatus(_T(""))
{

}

CPUDDRInfo::~CPUDDRInfo()
{
}

void CPUDDRInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_CPU, m_ProCPU);
	DDX_Control(pDX, IDC_PROGRESS_DDR, m_ProDDR);
	DDX_Text(pDX, IDC_CPU, m_szCPU);
	DDX_Text(pDX, IDC_STATIC_DDR, m_szDDR);
	DDX_Text(pDX, IDC_STATIC_REMIND, m_szStatus);
}


BEGIN_MESSAGE_MAP(CPUDDRInfo, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLEANDDR, &CPUDDRInfo::OnBnClickedButtonCleanddr)
END_MESSAGE_MAP()


// CPUDDRInfo 消息处理程序

//初始化函数
BOOL CPUDDRInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CreateChildThreadCPU();  //监控CPU使用率的子线程

	//GetMemoryStatus();

	SetTimer(1, 1000, NULL); //每隔一秒获取一下内存信息  响应OnTimer消息

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//创建监控CPU使用率的子线程
DWORD WINAPI ThreadProc_CPU(LPVOID lpParam)
{
	HANDLE hEvent;
	FILETIME preidleTime, prekernelTime, preuserTime;
	FILETIME idleTime, kernelTime, userTime;
	DOUBLE FreeTime1;
	DOUBLE FreeTime2;
	DOUBLE KernelTime1;
	DOUBLE KernelTime2;
	DOUBLE UserTime1;
	DOUBLE UserTime2;
	DWORD CPU;

	//1.第一次获取处理器时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
	//2.创建内核对象，并且等待1000毫秒
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	while (1)
	{
		WaitForSingleObject(hEvent, 1000);
		//3.第二次获取处理器时间
		GetSystemTimes(&idleTime, &kernelTime, &userTime);
		//4.转换时间
		FreeTime1 = (DOUBLE)(preidleTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)preidleTime.dwLowDateTime;
		FreeTime2 = (DOUBLE)(idleTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)idleTime.dwLowDateTime;
		KernelTime1 = (DOUBLE)(prekernelTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)prekernelTime.dwLowDateTime;
		KernelTime2 = (DOUBLE)(kernelTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)kernelTime.dwLowDateTime;
		UserTime1 = (DOUBLE)(preuserTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)preuserTime.dwLowDateTime;
		UserTime2 = (DOUBLE)(userTime.dwHighDateTime*4.294967296E9) +
			(DOUBLE)userTime.dwLowDateTime;

		//5.计算CPU使用率
		CPU = (DWORD)(100.0 - (FreeTime2 - FreeTime1) /
			(KernelTime2 - KernelTime1 + UserTime2 - UserTime1)*100.0);

		//6.向主窗口发送消息，更新CPU使用率
		((CPUDDRInfo*)lpParam)->m_ProCPU.SetPos(CPU);
		((CPUDDRInfo*)lpParam)->m_szCPU.Format(_T("%d %%"), CPU);

		/*CString Buff;
		Buff.Format(_T("%d %%"), CPU);
		((CPUDDRInfo*)lpParam)->m_szCPU.SetWindowText(Buff);*/

		//7.重新复制，循环刷新CPU使用率
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
	}
	return 0;
}

//创建监控CPU使用率的子线程
BOOL CPUDDRInfo::CreateChildThreadCPU()
{
	HANDLE hThread = CreateThread(NULL, 0, ThreadProc_CPU, this, 0, NULL);
	if (hThread)
	{
		return TRUE;
	}
	return FALSE;
}

//获取内存使用率
void CPUDDRInfo::GetMemoryStatus()
{
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	//当前内存使用率：
	//绘制内存进度条的进度
	m_ProDDR.SetPos(memStatus.dwMemoryLoad);
	//已用物理内存大小：

	m_szDDR.Format(_T("%.2f GB"),(DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024 / 1024);
	//更新到显示文本
	UpdateData(FALSE);
}

//OnTimer
void CPUDDRInfo::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		//获取内存信息
		GetMemoryStatus();
	}
	CDialogEx::OnTimer(nIDEvent);
}

//内存清理
void CPUDDRInfo::ClearMemory()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize = sizeof(ProcessInfo);
	int count = 0;

	//获取系统中第一个进程的信息  
	BOOL Status = Process32First(hProcessSnap, &ProcessInfo);
	while (Status)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE,
			ProcessInfo.th32ProcessID);
		if (hProcess)
		{
			//设置进程工作区大小
			SetProcessWorkingSetSize(hProcess, -1, -1);
			//尽可能多的将指定进程的页面从工作区移除
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		//获取下一个进程的信息  
		Status = Process32Next(hProcessSnap, &ProcessInfo);
	}
}

//内存优化按键
void CPUDDRInfo::OnBnClickedButtonCleanddr()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_szStatus = _T("正在清理内存中... ...");      //初始状态
	UpdateData(FALSE);							   //更新

	//获取清理前的内存信息
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;

	//开始清理内存
	ClearMemory();
	//获取清理后的内存信息
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//输出清理信息
	m_szStatus.Format(_T("内存清理完毕！本次清理 : %.2f MB"), preUsedMem - afterUsedMem);

}
