// ProcessInfo.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "ProcessInfo.h"
#include "afxdialogex.h"

#include <TlHelp32.h>
#include <tchar.h>

// ProcessInfo 对话框

IMPLEMENT_DYNAMIC(ProcessInfo, CDialogEx)

ProcessInfo::ProcessInfo(CWnd* pParent, int nMID, int nPID)
	: CDialogEx(IDD_DIALOG_PRO_INFO, pParent)
{
	this->m_nMenuID = nMID;
	this->m_nPID = nPID;
	m_bFlag = false;
}

ProcessInfo::~ProcessInfo()
{
}

void ProcessInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PopLIST1, m_PopList);
}


BEGIN_MESSAGE_MAP(ProcessInfo, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_PopLIST1, &ProcessInfo::OnRclickPoplist1)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32791, ID_32793, &ProcessInfo::OnMenuThread)	//处理菜单组;
END_MESSAGE_MAP()


// ProcessInfo 消息处理程序


//遍历线程并显示
void ProcessInfo::ShowThread(int nPid)
{
	//创建快照;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);

	//保存进程信息结构体;
	THREADENTRY32 ThreadInfo = { sizeof(THREADENTRY32) };

	//遍历进程;
	if (Thread32First(hSnapShot, &ThreadInfo))
	{
		m_PopList.AddColumn(3, L"线程ID", 160, L"优先级", 160, L"状态", 170);
		do
		{
			//显示符合条件的线程;
			if (ThreadInfo.th32OwnerProcessID == m_nPID)
			{
				CString PID, Levele, State;
				PID.Format(L"%d", ThreadInfo.th32ThreadID);
				Levele.Format(L"%d", ThreadInfo.tpBasePri);

				//获取线程句柄;
				DWORD dwState = 0;
				HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION |
					THREAD_QUERY_LIMITED_INFORMATION, FALSE, ThreadInfo.th32ThreadID);

				//判断线程状态;
				GetExitCodeThread(hThread, &dwState);
				if (dwState == STILL_ACTIVE)
				{
					State = L"运行中";
				}
				else
				{
					State = L"挂起";
				}
				CloseHandle(hThread);
				m_PopList.AddItem(3, PID, Levele, State);
			}

		} while (Thread32Next(hSnapShot, &ThreadInfo));
	}
	else
	{
		MessageBox(L"获取线程信息失败！", L"提示");
	}
}

//显示遍历模块并显示
void ProcessInfo::ShowModule(int nPid)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPid);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"打开进程信息失败！", L"提示");
		return;
	}
	MODULEENTRY32 me = { sizeof(MODULEENTRY32) };
	if (!Module32First(hSnapShot, &me))
	{
		MessageBox(L"获取模块信息失败！", L"提示");
		return;
	}
	m_PopList.AddColumn(3, L"模块名称", 100, L"模块大小", 100, L"模块路径", 290);
	do
	{
		TCHAR* SizeBuf = new TCHAR[11];
		_stprintf_s(SizeBuf, 10, L"%d", me.modBaseSize);
		m_PopList.AddItem(3, me.szModule, SizeBuf, me.szExePath);
	} while (Module32Next(hSnapShot, &me));

	return;
}



//线程列表鼠标右键菜单;
void ProcessInfo::OnRclickPoplist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//非线程列表结束;
	if (!m_bFlag)
	{
		return;
	}

	//线程列表下显示菜单;

	//获取弹出菜单;
	CMenu* pMenuThread = new CMenu;
	pMenuThread->LoadMenuW(IDR_MENU2);

	CMenu* pSubThread = pMenuThread->GetSubMenu(1);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSubThread->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}

//响应右键菜单;
void ProcessInfo::OnMenuThread(UINT id)
{
	int nSel = m_PopList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据", L"提示");
	}
	int PID = _wtoi(m_PopList.GetItemText(nSel, 0));

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, PID);
	switch (id)
	{
	case  ID_32791:
		if (SuspendThread(hThread) == -1)
		{
			MessageBox(L"挂起线程失败！", L"提示");
		}
		else
		{
			MessageBox(L"挂起线程成功！", L"提示");
		}
		break;
	case ID_32792:
		if (ResumeThread(hThread) == -1)
		{
			MessageBox(L"恢复线程失败！", L"提示");
		}
		else
		{
			MessageBox(L"恢复线程成功！", L"提示");
		}
		break;
	case ID_32793:
		if (!TerminateThread(hThread, 0))
		{
			MessageBox(L"结束线程失败！", L"提示");
		}
		else
		{
			MessageBox(L"结束线程成功！", L"提示");
		}
		break;
	default:
		break;
	}


}




//消息处理
BOOL ProcessInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	ChangeWindowMessageFilterEx(this->m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(this->m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	switch (m_nMenuID)
	{
	case ID_32772:
		m_bFlag = true;
		ShowThread(m_nPID);
		break;
	case ID_32773:
		ShowModule(m_nPID);
		break;
	/*case ID_32774:
		ShowHeap(m_nPID);
		break;*/
	default:
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}