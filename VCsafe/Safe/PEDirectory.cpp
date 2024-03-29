// PEDirectory.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "PEDirectory.h"
#include "afxdialogex.h"
#include "PEPxport.h"
#include "PEImport.h"
#include "PEResource.h"
#include "PERelocation.h"
#include "PETLS.h"
#include "CPEDelay.h"


// PEDirectory 对话框

IMPLEMENT_DYNAMIC(PEDirectory, CDialogEx)

PEDirectory::PEDirectory(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNTHeader, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DIRECTORY, pParent)
	, m_Rva_Out(_T(""))
	, m_Rva_In(_T(""))
	, m_Rva_Resouce(_T(""))
	, m_Rva_Except(_T(""))
	, m_Rva_Security(_T(""))
	, m_Rva_BaseReloc(_T(""))
	, m_Rva_Debug(_T(""))
	, m_Rva_Architecture(_T(""))
	, m_Rva_GlobalPTR(_T(""))
	, m_Rva_TLS(_T(""))
	, m_Rva_load(_T(""))
	, m_Rva_Bound(_T(""))
	, m_Rva_IAT(_T(""))
	, m_Rva_Delay(_T(""))
	, m_Rva_COM(_T(""))
	, m_Rva_Retain(_T(""))
	, m_Size_Out(_T(""))
	, m_Size_In(_T(""))
	, m_Size_Resouce(_T(""))
	, m_Size_Except(_T(""))
	, m_Size_Security(_T(""))
	, m_Size_BaseReloc(_T(""))
	, m_Size_Debug(_T(""))
	, m_Size_Architecture(_T(""))
	, m_Size_GlobalPTR(_T(""))
	, m_Size_TLS(_T(""))
	, m_Size_Load(_T(""))
	, m_Size_Bound(_T(""))
	, m_Size_IAT(_T(""))
	, m_Size_Delay(_T(""))
	, m_Size_COM(_T(""))
	, m_Size_Retain(_T(""))
{
	m_pDos = pDos;                                       	  //DOS头
	m_pNTHeader = pNTHeader;								  //NT头
	m_pDataDir = pNTHeader->OptionalHeader.DataDirectory;	  //目录头

}

PEDirectory::~PEDirectory()
{
}

void PEDirectory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//RVA
	DDX_Text(pDX, IDC_EDIT_RVA_OUT, m_ArrRva[0]);
	DDX_Text(pDX, IDC_EDIT_RVA_IN, m_ArrRva[1]);
	DDX_Text(pDX, IDC_EDIT_RVA_RESOUCE, m_ArrRva[2]);
	DDX_Text(pDX, IDC_EDIT_RVA_EXCEPTION, m_ArrRva[3]);
	DDX_Text(pDX, IDC_EDIT_RVA_SECURITY, m_ArrRva[4]);
	DDX_Text(pDX, IDC_EDIT_RVA_BASERELOC, m_ArrRva[5]);
	DDX_Text(pDX, IDC_EDIT_RVA_DEBUG, m_ArrRva[6]);
	DDX_Text(pDX, IDC_EDIT_RVA_ARCHITECTURE, m_ArrRva[7]);
	DDX_Text(pDX, IDC_EDIT_RVA_GLOBALPTR, m_ArrRva[8]);
	DDX_Text(pDX, IDC_EDIT_RVA_TLS, m_ArrRva[9]);
	DDX_Text(pDX, IDC_EDIT_RVA_LOAD, m_ArrRva[10]);
	DDX_Text(pDX, IDC_EDIT_RVA_BOUND, m_ArrRva[11]);
	DDX_Text(pDX, IDC_EDIT_RVA_IAT, m_ArrRva[12]);
	DDX_Text(pDX, IDC_EDIT_RVA_DELAY, m_ArrRva[13]);
	DDX_Text(pDX, IDC_EDIT_RVA_COM, m_ArrRva[14]);
	DDX_Text(pDX, IDC_EDIT_RVA_RETAIN, m_ArrRva[15]);

	//大小
	DDX_Text(pDX, IDC_EDIT_SIZE_OUT, m_ArrSize[0]);
	DDX_Text(pDX, IDC_EDIT_SIZE_IN, m_ArrSize[1]);
	DDX_Text(pDX, IDC_EDIT_SIZE_RESOUCE, m_ArrSize[2]);
	DDX_Text(pDX, IDC_EDIT_SIZE_EXCEPTION, m_ArrSize[3]);
	DDX_Text(pDX, IDC_EDIT_SIZE_SECURITY, m_ArrSize[4]);
	DDX_Text(pDX, IDC_EDIT_SIZE_BASERELOC, m_ArrSize[5]);
	DDX_Text(pDX, IDC_EDIT_SIZE_DEBUG, m_ArrSize[6]);
	DDX_Text(pDX, IDC_EDIT_SIZE_ARCHITECTURE, m_ArrSize[7]);
	DDX_Text(pDX, IDC_EDIT_SIZE_GLOBALPTR, m_ArrSize[8]);
	DDX_Text(pDX, IDC_EDIT_SIZE_TLS, m_ArrSize[9]);
	DDX_Text(pDX, IDC_EDIT_SIZE_LOAD, m_ArrSize[10]);
	DDX_Text(pDX, IDC_EDIT_SIZE_BOUND, m_ArrSize[11]);
	DDX_Text(pDX, IDC_EDIT_SIZE_IAT, m_ArrSize[12]);
	DDX_Text(pDX, IDC_EDIT_SIZE_DELAY, m_ArrSize[13]);
	DDX_Text(pDX, IDC_EDIT_SIZE_COM, m_ArrSize[14]);
	DDX_Text(pDX, IDC_EDIT_SIZE_RETAIN, m_ArrSize[15]);
}


BEGIN_MESSAGE_MAP(PEDirectory, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OUT, &PEDirectory::OnBnClickedButtonOut)
	ON_BN_CLICKED(IDC_BUTTON_IN, &PEDirectory::OnBnClickedButtonIn)
	ON_BN_CLICKED(IDC_BUTTON_RESOUCE, &PEDirectory::OnBnClickedButtonResouce)
	ON_BN_CLICKED(IDC_BUTTON_BASERELOC, &PEDirectory::OnBnClickedButtonBasereloc)
	ON_BN_CLICKED(IDC_BUTTON_TLS, &PEDirectory::OnBnClickedButtonTls)
	ON_BN_CLICKED(IDC_BUTTON_DELAY, &PEDirectory::OnBnClickedButtonDelay)
END_MESSAGE_MAP()


// PEDirectory 消息处理程序


BOOL PEDirectory::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDirectoryInfo();  //输出目录信息
	UpdateData(FALSE);   //更新

	return TRUE;         // return TRUE unless you set the focus to a control
						 // 异常: OCX 属性页应返回 FALSE
}

//输出目录信息
void PEDirectory::GetDirectoryInfo()
{
	//循环输出信息    
	for (int i = 0; i < 16; i++)
	{
		m_ArrRva[i].Format(L"%p", m_pDataDir[i].VirtualAddress); //RVA
		m_ArrSize[i].Format(L"%08X", m_pDataDir[i].Size);        //大小
	}
}

//控件按钮，获取导出表信息
void PEDirectory::OnBnClickedButtonOut()
{
	// TODO: 在此添加控件通知处理程序代码
	PEPxport *pExport = new PEPxport(m_pDos, m_pNTHeader);
	pExport->DoModal();
}

//控件按钮，获取导入表信息
void PEDirectory::OnBnClickedButtonIn()
{
	// TODO: 在此添加控件通知处理程序代码
	PEImport * pImport = new PEImport(m_pDos, m_pNTHeader);
	pImport->DoModal();
}

//控件按钮，获取资源表信息
void PEDirectory::OnBnClickedButtonResouce()
{
	// TODO: 在此添加控件通知处理程序代码

	PEResource *pResInfo = new PEResource(m_pDos, m_pNTHeader);
	pResInfo->DoModal();
}

//控件按钮，获取资源表信息
void PEDirectory::OnBnClickedButtonBasereloc()
{
	// TODO: 在此添加控件通知处理程序代码
	PERelocation *pRlock = new PERelocation(m_pDos, m_pNTHeader);
	pRlock->DoModal();
}

//控件按钮，获取TLS表信息
void PEDirectory::OnBnClickedButtonTls()
{
	// TODO: 在此添加控件通知处理程序代码
	PETLS* pTLS = new PETLS(m_pDos, m_pNTHeader);
	pTLS->DoModal();
}

//延迟载入表
void PEDirectory::OnBnClickedButtonDelay()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CPEDelay *pDelayInfo = new CPEDelay(m_pDos, m_pNTHeader);
	pDelayInfo->DoModal();
}
