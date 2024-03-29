// DesktopWindow.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "DesktopWindow.h"
#include "afxdialogex.h"
#include <vector>

using std::vector;
vector<MYWINDOWINFO> vecWindowInfo;       //窗口信息容器


// DesktopWindow 对话框

IMPLEMENT_DYNAMIC(DesktopWindow, CDialogEx)

DesktopWindow::DesktopWindow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DESWINDOW, pParent)
{

}

DesktopWindow::~DesktopWindow()
{
}

void DesktopWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WINDOW, m_WindowList);
}


BEGIN_MESSAGE_MAP(DesktopWindow, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_WINDOW, &DesktopWindow::OnRclickListWindow)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32811, ID_32813, &DesktopWindow::OnPopMenu)
END_MESSAGE_MAP()


// DesktopWindow 消息处理程序
//初始化函数
BOOL DesktopWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//插入列;
	m_WindowList.AddColumn(3, L"窗口句柄", 80, L"窗口标题", 250, L"窗口类名", 200);
	//插入窗口;
	DestktopWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//枚举窗口
BOOL CALLBACK DesktopWindow::DestktopWindowProc(HWND hWnd, LPARAM lParam)
{
	MYWINDOWINFO* WindowInfo = new MYWINDOWINFO;				   //创建结构体;
	_stprintf_s(WindowInfo->hWnd, L"%p", hWnd);					   //格式化句柄;

	//获取窗口标题，类名
	::GetWindowTextW(hWnd, WindowInfo->WindowName, MAX_PATH);
	::GetClassNameW(hWnd, WindowInfo->ClassName, MAX_PATH);

	//窗体可见存入容器;	且窗口有标题;
	if (::GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE && WindowInfo->WindowName[0])
	{
		vecWindowInfo.push_back(*WindowInfo);
	}
	return TRUE;
}

//添加窗口
void DesktopWindow::DestktopWindow()
{
	//枚举窗口
	::EnumWindows(DestktopWindowProc, 0);
	//循环插入窗口
	for (size_t i = 0; i < vecWindowInfo.size(); i++)
	{
		m_WindowList.AddItem(3,vecWindowInfo[i].hWnd,vecWindowInfo[i].
			                 WindowName,vecWindowInfo[i].ClassName);
	}
}


//右键菜单函数
void DesktopWindow::OnRclickListWindow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取弹出菜单;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU2);
	CMenu* pSub = pMenu->GetSubMenu(2);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);
	*pResult = 0;
}

//响应鼠标右键菜单
void DesktopWindow::OnPopMenu(UINT id)
{
	int nSel = m_WindowList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据",L"提示");
	}

	DWORD dwTemp;
	swscanf_s(m_WindowList.GetItemText(nSel, 0), L"%x", &dwTemp);

	//判断菜单ID;
	switch (id)
	{
	case ID_32811:   //切换
		::SetWindowPos((HWND)dwTemp, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		::PostMessage((HWND)dwTemp, WM_SYSCOMMAND, SC_RESTORE, 0);
		break;
	case ID_32812:   //刷新
		m_WindowList.DeleteAllItems();
		vecWindowInfo.clear();
		DestktopWindow();
		break;
	case ID_32813:   //关闭
		::PostMessage((HWND)dwTemp, WM_SYSCOMMAND, SC_CLOSE, 0);
		break;
	default:
		break;
	}
}

