// PEImport.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "PEImport.h"
#include "afxdialogex.h"
#include "PERVAtoFOA.h"


// PEImport 对话框

IMPLEMENT_DYNAMIC(PEImport, CDialogEx)

PEImport::PEImport(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNTHeader, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_IMPORT, pParent)
{
	m_pDos = pDos;											//DOS头
	m_pNTHeader = pNTHeader;								//NT头
	m_pDataDir = pNTHeader->OptionalHeader.DataDirectory;	//数据目录
}

PEImport::~PEImport()
{
}

void PEImport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DLL, m_ListDll);
	DDX_Control(pDX, IDC_LIST_FUN, m_ListFun);
}

BEGIN_MESSAGE_MAP(PEImport, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DLL, &PEImport::OnLvnItemchangedListDll)
END_MESSAGE_MAP()

// PEImport 消息处理程序

//初始化
BOOL PEImport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_ListDll.AddColumn(3, L"DllName", 150, L"TNT(FO)", 150, L"IAT(FO)", 100); 	//初始化DLL列表
	GetDllInfo();																//获取DLL信息
	m_ListFun.AddColumn(2, L"FunOrdinal", 100, L"FunName", 200);				//初始化函数列表

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//DLL列表事件函数
void PEImport::OnLvnItemchangedListDll(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取选中数据，判断是否选中
	int nSel = m_ListDll.GetSelectionMark(); 
	if (nSel < 0)
	{
		MessageBox(L"请选中数据",L"提示");
		return;
	}

	//获取列表序号
	int  dwINTF0 = wcstoll(m_ListDll.GetItemText(nSel, 2), NULL, 16);
	if (dwINTF0 == -1)
	{
		dwINTF0 = wcstoll(m_ListDll.GetItemText(nSel, 1), NULL, 16);
	}

	if (dwINTF0 == -1)
	{
		return;
	}
	m_pThunk = (PIMAGE_THUNK_DATA32)((PBYTE)m_pDos + dwINTF0);  //转换

	GetFunInfo();    //获取函数信息

	*pResult = 0;
}

//获取DLL信息
void PEImport::GetDllInfo()
{
	//找到导入表
	PBYTE pBuf = (PBYTE)m_pDos;                                           //定义临时指针
	DWORD dwFO = PERVAtoFOA::RVAtoFOA(m_pDataDir
		        [IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, m_pNTHeader);

	//导入表在文件中的位置
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(pBuf + dwFO);

	//循环查找
	while (pImport->Name)
	{

		m_DllName = (CHAR*)(pBuf + PERVAtoFOA::RVAtoFOA(pImport->Name, m_pNTHeader));		 //获取DLL名
		m_INT.Format(L"%p", PERVAtoFOA::RVAtoFOA(pImport->OriginalFirstThunk, m_pNTHeader)); //获取输入名称表并输出
		m_IAT.Format(L"%p", PERVAtoFOA::RVAtoFOA(pImport->FirstThunk, m_pNTHeader));		 //获取输入地址表并输出

		m_ListDll.AddItem(3, m_DllName, m_INT, m_IAT);   //DLL模块列表
		pImport++;
	}
}

//获取函数信息
void PEImport::GetFunInfo()
{
	m_ListFun.DeleteAllItems();

	//定义临时指针
	PBYTE pBuf = (PBYTE)m_pDos;    

	//循环查找输出
	while (m_pThunk->u1.AddressOfData)
	{
		//序号命名
		if (IMAGE_SNAP_BY_ORDINAL32(m_pThunk->u1.Ordinal))
		{
			m_FunOrdinal.Format(L"%X", IMAGE_ORDINAL32(m_pThunk->u1.Ordinal));   //函数序号
			m_FunName = L"-";													 //函数名称
		}
		//名称命名
		else
		{
			PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(pBuf +PERVAtoFOA::RVAtoFOA
			                              (m_pThunk->u1.AddressOfData, m_pNTHeader));

			m_FunOrdinal.Format(L"%08X", pName->Hint);		 //函数序号
			m_FunName = (CHAR*)pName->Name;					 //函数名称
		}
		m_ListFun.AddItem(2, m_FunOrdinal, m_FunName);       //DLL模块列表
		m_pThunk++;
	}

}
