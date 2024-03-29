// Process.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "Process.h"
#include "afxdialogex.h"
#include"ProcessInfoFile.h"

#include <TlHelp32.h>  //快照
#include <tchar.h>


// Process 对话框

IMPLEMENT_DYNAMIC(Process, CDialogEx)

Process::Process(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

Process::~Process()
{
}

void Process::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(Process, CDialogEx)
	ON_COMMAND(ID_32776, &Process::On32776)
	ON_COMMAND(ID_32775, &Process::OnClose)
	ON_COMMAND(ID_32777, &Process::OnAttribute)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32772, ID_32774, &Process::OnMenuClick)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &Process::OnRclickProcessList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, &Process::OnColumnclickProcessList1)
	ON_COMMAND(ID_32810, &Process::OnHook)
	ON_COMMAND(ID_32814, &Process::OnOffHook)
END_MESSAGE_MAP()


// Process 消息处理程序


//初始化进程信息
BOOL Process::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ChangeWindowMessageFilterEx(this->m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(this->m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	//初始化列表;
	m_list.AddColumn(5, L"名称", 100, L"PID", 50, L"父进程PID", 80, L"线程数量", 80, L"路径", 180);
	//显示进程;
	ShowProcess();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



//显示进程
void Process::ShowProcess()
{
	//创建快照函数，需要#include <TlHelp32.h> 
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;    //函数调用失败
	}

	//获取进程快照结构体长度
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };

	//找到第一个进程
	if (!Process32First(hProcessSnap,&pe))
	{
		MessageBox(L"获取进程信息失败", L"提示", MB_OK | MB_ICONERROR);
		return;
	}

	//循环遍历其它进程
	do 
	{
		TCHAR *szBuf[5] = { new TCHAR[MAX_PATH], new TCHAR[6],  new TCHAR[6],
						 new TCHAR[6], new TCHAR[MAX_PATH], };
		szBuf[0] = pe.szExeFile;      //进程名，输出内容
		if (pe.th32ProcessID>0&&pe.th32ParentProcessID>0&&pe.cntThreads>=0)
		{
			_stprintf_s(szBuf[1], 6, L"%d", pe.th32ProcessID);
			_stprintf_s(szBuf[2], 6, L"%d", pe.th32ParentProcessID);
			_stprintf_s(szBuf[3], 6, L"%d", pe.cntThreads);
		}
		else
		{
			szBuf[1] = L"  ";
			szBuf[2] = L"  ";
			szBuf[3] = L"  ";
		}

		//获取进程路径
		HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_QUERY_LIMITED_INFORMATION,
			                      FALSE, pe.th32ProcessID);

		//获取输出进程路径
		if (hPro)
		{
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hPro, NULL, szBuf[4], &dwSize);
		}
		else
		{
			szBuf[4] = L"无权访问";
		}
		m_list.AddItem(5, szBuf[0], szBuf[1], szBuf[2], szBuf[3], szBuf[4]);
		CloseHandle(hPro);
	}
	while (Process32Next(hProcessSnap, &pe));
}



//刷新进程列表
void Process::On32776()
{
	// TODO: 在此添加命令处理程序代码

	m_list.DeleteAllItems();
	ShowProcess();
}


//点击目录名倒序
void Process::OnColumnclickProcessList1(NMHDR * pNMHDR, LRESULT * pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取点击目录的列
	int Sort_Colum = pNMLV->iSubItem;

	//0列4列字符串排序;其他整型排序;
	if (Sort_Colum == 0 || Sort_Colum == 4)
		m_list.SortItemsPlus(Sort_Colum, false);
	else
		m_list.SortItemsPlus(Sort_Colum);

	*pResult = 0;
}


//弹出进程菜单
void Process::OnRclickProcessList1(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取弹出菜单
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU2);

	CMenu* pSub = pMenu->GetSubMenu(0);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出信息
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}


//右键响应菜单
void Process::OnMenuClick(UINT id)
{
	// TODO: 在此添加命令处理程序代码
	//获取选中项;
	int nSel = m_list.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据",L"提示");
	}
	int PID = _wtoi(m_list.GetItemText(nSel, 1));

	ProcessInfo DlgA(this, id, PID);
	DlgA.DoModal();

}


//结束进程
void Process::OnClose()
{
	// TODO: 在此添加命令处理程序代码
	//获取Pid;
	int nSel = m_list.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据", L"提示");
	}
	int PID = _wtoi(m_list.GetItemText(nSel, 1));
	//获取句柄;
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, PID);
	if (TerminateProcess(hProcess, 0))
	{
		MessageBox(L"结束成功!" ,L"提示");
	}
	else
	{
		MessageBox(L"结束失败!", L"提示");
	}
	CloseHandle(hProcess);
	m_list.DeleteAllItems();
	ShowProcess();
}

//进程属性
void Process::OnAttribute()
{
	// TODO: 在此添加命令处理程序代码
	//获取选中项;
	int nSel = m_list.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据", L"提示");
	}
	CString Path = m_list.GetItemText(nSel, 4);
	if ((Path[0] >= 'a'&&Path[0] <= 'z') ||(Path[0] >= 'A'&&Path[0] <= 'Z'))
	{
		ProcessInfoFile DlgF(Path);
 		DlgF.DoModal();
	}
	else
	{
		MessageBox(L"请选择有权限访问的路径", L"提示");
	}

}


void Process::OnHook()
{
	int i = m_list.GetSelectionMark();
	DWORD pPid = _tstoi(m_list.GetItemText(i, 1));
	DWORD Pid = 0, RealWrite = 0;
	//DWORD pPid = GetCurrentProcessId();
	CreateSemaphoreA(NULL, pPid, pPid + 1, "MyPid");
	HWND hwnd = ::FindWindow(NULL, L"任务管理器");
	GetWindowThreadProcessId(hwnd, &Pid);

	TaskhProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);

	LPVOID ProcessAddr = VirtualAllocEx(TaskhProcess, NULL, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	WriteProcessMemory(TaskhProcess, ProcessAddr, "E:\\InlineHook.dll", strlen("E:\\InlineHook.dll") + 1, &RealWrite);

	HANDLE Thread = CreateRemoteThread(TaskhProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, ProcessAddr, NULL, NULL);

	WaitForSingleObject(Thread, INFINITE);
}

void Process::OnOffHook()
{
	DWORD dwPid = GetProcessId(TaskhProcess);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);

	MODULEENTRY32 me32;
	me32.dwSize = sizeof(me32);

	//查找匹配的进程名称
	BOOL bRet = Module32First(hSnap, &me32);
	WCHAR dllPath[] = L"E:\\InlineHook.dll";
	while (bRet)
	{
		if (lstrcmp(me32.szExePath, dllPath) == 0)
		{
			break;
		}
		bRet = Module32Next(hSnap, &me32);
	}

	CloseHandle(hSnap);
	TestModule = me32.hModule;
	HANDLE Thread = CreateRemoteThread(TaskhProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)FreeLibrary, TestModule, NULL, NULL);
	//FreeLibrary()
	WaitForSingleObject(Thread, INFINITE);
	//FreeLibrary(TestModule);
}