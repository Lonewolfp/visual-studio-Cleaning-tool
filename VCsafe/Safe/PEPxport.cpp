// PEPxport.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "PEPxport.h"
#include "afxdialogex.h"
#include "PERVAtoFOA.h"


// PEPxport 对话框

IMPLEMENT_DYNAMIC(PEPxport, CDialogEx)

PEPxport::PEPxport(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNTHeader, 
	                CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_EXPORT, pParent)
	, m_Name(_T(""))
	, m_FunNumber(_T(""))
	, m_FunNumberName(_T(""))
	, m_FunAdd(_T(""))
	, m_FunNameAdd(_T(""))
	, m_FunOrdAdd(_T(""))
{
	m_pDos = pDos;                        				   //DOS头
	m_pNTHeader = pNTHeader;							   //NT头
	m_pDataDir = pNTHeader->OptionalHeader.DataDirectory;  //数据目录地址
}

PEPxport::~PEPxport()
{
}

void PEPxport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_NFUN, m_FunNumber);
	DDX_Text(pDX, IDC_EDIT_NNAME, m_FunNumberName);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_FunAdd);
	DDX_Text(pDX, IDC_EDIT_ADD_NAME, m_FunNameAdd);
	DDX_Text(pDX, IDC_EDIT_ADD_ORDINALS, m_FunOrdAdd);
	DDX_Control(pDX, IDC_LIST_EXPORT, m_ExportList);
}


BEGIN_MESSAGE_MAP(PEPxport, CDialogEx)
END_MESSAGE_MAP()


// PEPxport 消息处理程序

//初始化
BOOL PEPxport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化列表
	m_ExportList.AddColumn(4,L"Ordinal",60, L"RVA", 100, L"Offset", 100, L"Function Name", 100);
	GetExportInfo();     //获取导出表信息
	UpdateData(FALSE);   //刷新数据到列表中

	return TRUE;         // return TRUE unless you set the focus to a control
				         // 异常: OCX 属性页应返回 FALSE
}

//获取导出表信息
void PEPxport::GetExportInfo()
{
	//找到目录
	PBYTE pBuf = (PBYTE)m_pDos;                                                   //DOS头大小
	DWORD dwExportRVA = m_pDataDir[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;  //数据目录表基址
	DWORD dwExportSize = m_pDataDir[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;           //数据目录表大小
	
	//找到导出表
	DWORD dwExportFO = PERVAtoFOA::RVAtoFOA(dwExportRVA, m_pNTHeader);               //导出表文件偏移地址FO
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(pBuf + dwExportFO);	 //找到导出表

	//输出导出表信息
	m_Name += (CHAR*)(pBuf + PERVAtoFOA::RVAtoFOA(pExport->Name, m_pNTHeader));      //导出表名称
	m_FunNumber.Format(L"%08X", pExport->NumberOfFunctions);						 //导出表函数数量
	m_FunNumberName.Format(L"%08X", pExport->NumberOfNames);						 //导出表函数名数量
	m_FunAdd.Format(L"%p", pExport->AddressOfFunctions);							 //导出表函数地址
	m_FunNameAdd.Format(L"%p", pExport->AddressOfNames);							 //导出表函数名地址
	m_FunOrdAdd.Format(L"%p", pExport->AddressOfNameOrdinals);						 //导出表函数名序号地址
	
	//将信息插入列表
	PDWORD pArrAdd = (PDWORD)(pBuf + PERVAtoFOA::RVAtoFOA(pExport->AddressOfFunctions, m_pNTHeader));	   //函数地址
	PDWORD pArrName = (PDWORD)(pBuf + PERVAtoFOA::RVAtoFOA(pExport->AddressOfNames, m_pNTHeader));		   //函数名地址
	PWORD pArrOrdinal = (PWORD)(pBuf + PERVAtoFOA::RVAtoFOA(pExport->AddressOfNameOrdinals, m_pNTHeader)); //函数名序号地址
	//地址表循环
	for (int i = 0; i < pExport->NumberOfFunctions; i++)
	{
		DWORD dwFO = PERVAtoFOA::RVAtoFOA(pArrAdd[i], m_pNTHeader);     //获取FO
		
		m_ListOrdinal.Format(L"%04X", i + pExport->Base);		        //序号
		m_ListRVA.Format(L"%p", pArrAdd[i]);				            //RVA
		m_ListFO.Format(L"%p", dwFO);						            //文件偏移FO

		bool bFind = false;
		//序号表循环
		for (int j = 0; j < pExport->NumberOfNames; j++)
		{
			if (pArrOrdinal[j] == i)
			{
				m_ListName = "";
				m_ListName += (CHAR*)(pBuf + PERVAtoFOA::RVAtoFOA(pArrName[j], m_pNTHeader));
				bFind = true;
				break;
			}

		}
		if (!bFind)
		{
			m_ListName = "-";
		}

		m_ExportList.AddItem(4, m_ListOrdinal, m_ListRVA, m_ListFO, m_ListName);
	}
}
