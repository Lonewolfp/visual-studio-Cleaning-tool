// Services.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "Services.h"
#include "afxdialogex.h"
#include <Winsvc.h>

// Services 对话框

IMPLEMENT_DYNAMIC(Services, CDialogEx)

Services::Services(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SERVICES, pParent)
{

}

Services::~Services()
{
}

void Services::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SERVICE, m_ListServices);
}


BEGIN_MESSAGE_MAP(Services, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SERVICE, &Services::OnItemchangedListService)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32784, ID_32787, OnMenuService)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SERVICE, &Services::OnColumnclickListService)
END_MESSAGE_MAP()


// Services 消息处理程序


BOOL Services::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化列表
	m_ListServices.AddColumn(4, L"名称", 200, L"描述", 400, L"状态", 100, L"类型", 100);
	
	UpDataService();		   //刷新服务

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



 //刷新服务
void Services::UpDataService()
{
	m_ListServices.DeleteAllItems();   //刷新列表
	GetServices();					  //获取服务
}

//获取服务
void Services::GetServices()
{
	//打开服务控制管理器
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	//第一次调用,获取所需内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,SERVICE_STATE_ALL,
		                 NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);

	//申请所需内存
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];

	//第二次枚举
	bool bStatus = false;
	bStatus = EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL, (PBYTE)pEnumService, dwSize, &dwSize, &dwServiceNum, NULL, NULL);

	//遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//服务名
		m_ServiceName = pEnumService[i].lpServiceName;

		//状态
		DWORD dwStatus = pEnumService[i].ServiceStatusProcess.dwCurrentState;
		m_ServiceStatu = ServiceStatus[dwStatus];

		//详细信息
		SC_HANDLE hService = OpenService(hSCM, pEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);

		QueryServiceConfig(hService, NULL, 0, &dwSize);									  //获取缓冲区大小
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize]; //分配内存
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);					  //获取信息

		DWORD dwType = pServiceConfig->dwStartType;
		m_ServiceType = ServerStartType[dwType];

		if (dwType > 0)
		{
			//获取描述信息大小
			QueryServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &dwSize);
			LPSERVICE_DESCRIPTION pServiceDesc = (LPSERVICE_DESCRIPTION)new char[dwSize];

			//获取描述
			QueryServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)pServiceDesc, dwSize, &dwSize);
			m_ServiceDesc = pServiceDesc->lpDescription;
		}
		else
		{
			//获取不到杀软服务的信息
			m_ServiceDesc = L" ";
		}
		m_ListServices.AddItem(4, m_ServiceName, m_ServiceDesc, m_ServiceStatu, m_ServiceType);
	}
}

//启动/停止服务
void Services::StartAndStopService(bool IsStart)
{
	//获取选中项
	int nSel = m_ListServices.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据", L"提示");
	}
	//获取服务名
	CString strServiceName = m_ListServices.GetItemText(nSel, 0);

	//打开服务控制管理器
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(L"获取服务对象信息失败！", L"提示");
		return;
	}

	//打开服务
	SC_HANDLE hService = OpenService(hSCM, strServiceName,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);

	if (hService == NULL)
	{
		MessageBox(L"打开服务失败！", L"提示");
		CloseServiceHandle(hSCM);
		return;
	}

	//获取服务状态
	SERVICE_STATUS Status;
	if (QueryServiceStatus(hService, &Status) == FALSE)
	{
		MessageBox(L"获取服务状态失败！", L"提示");
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
		return;
	}

	//启动服务选项
	if (IsStart)
	{
		if (StartService(hService, NULL, NULL) == FALSE)
		{
			MessageBox(L"启动服务失败！", L"提示");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			return;
		}
		//等待服务启动完成
		while (QueryServiceStatus(hService, &Status) == TRUE)
		{
			Sleep(Status.dwWaitHint);
			if (Status.dwCurrentState == SERVICE_RUNNING)
			{
				MessageBox(L"启动服务成功！", L"提示");
				CloseServiceHandle(hSCM);
				CloseServiceHandle(hService);
				return;
			}
		}
	}
	else
	{
		if (ControlService(hService, SERVICE_CONTROL_STOP, &Status) == FALSE)
		{
			MessageBox(L"停止服务失败！", L"提示");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			return;
		}
		//等待服务停止
		while (QueryServiceStatus(hService, &Status) == TRUE)
		{
			Sleep(Status.dwWaitHint);
			if (Status.dwCurrentState == SERVICE_STOPPED)
			{
				MessageBox(L"停止服务成功！", L"提示");
				CloseServiceHandle(hSCM);
				CloseServiceHandle(hService);
				return;
			}
		}
	}
}


//右键菜单
void Services::OnItemchangedListService(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取选中项
	int nSel = m_ListServices.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据", L"提示");
	}
	//获取服务名
	CString strServiceStatu = m_ListServices.GetItemText(nSel, 2);

	//获取弹出菜单;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU2);
	CMenu* pSub = pMenu->GetSubMenu(3);

	//更改菜单选项，判断运行状态
	if (strServiceStatu == L"正在运行")
	{
		::EnableMenuItem(pSub->m_hMenu, ID_32784, MF_BYCOMMAND | MF_GRAYED);
	}

	//禁用指定选项
	else if (strServiceStatu == L"已停止")
	{
		::EnableMenuItem(pSub->m_hMenu, ID_32786, MF_BYCOMMAND | MF_GRAYED);
	}

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}


//点击列头排序
void Services::OnColumnclickListService(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取点击列;
	int Sort_Colum = pNMLV->iSubItem;
	m_ListServices.SortItemsPlus(Sort_Colum, false);

	*pResult = 0;
}


//菜单
void Services::OnMenuService(UINT uID)
{
	switch (uID)
	{
	case ID_32784:
		StartAndStopService(true);	  //启动或停止服务
		break;
	case ID_32786:
		StartAndStopService(false);	  //启动或停止服务
		break;
	default:
		break;
	}
	UpDataService();				  //刷新服务
}
