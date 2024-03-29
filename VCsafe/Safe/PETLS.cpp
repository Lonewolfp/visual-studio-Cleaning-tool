// PETLS.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "PETLS.h"
#include "afxdialogex.h"
#include"PERVAtoFOA.h"

// PETLS 对话框

IMPLEMENT_DYNAMIC(PETLS, CDialogEx)

PETLS::PETLS(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNTHeader, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TLS, pParent)
	, m_TLSStart(_T(""))
	, m_TLSEnd(_T(""))
	, m_TLSIndex(_T(""))
	, m_TLSCallBack(_T(""))
	, m_TLSZero(_T(""))
	, m_TLSChara(_T(""))
{
	m_pDos = pDos;										   //DOS头
	m_pNTHeader = pNTHeader;							   //NT头
	m_pDataDir = pNTHeader->OptionalHeader.DataDirectory;  //目录头
}

PETLS::~PETLS()
{
}

void PETLS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_TLSStart);
	DDX_Text(pDX, IDC_EDIT9, m_TLSEnd);
	DDX_Text(pDX, IDC_EDIT10, m_TLSIndex);
	DDX_Text(pDX, IDC_EDIT11, m_TLSCallBack);
	DDX_Text(pDX, IDC_EDIT12, m_TLSZero);
	DDX_Text(pDX, IDC_EDIT13, m_TLSChara);
}


BEGIN_MESSAGE_MAP(PETLS, CDialogEx)
END_MESSAGE_MAP()


// PETLS 消息处理程序

//初始化函数
BOOL PETLS::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	GetTLSInfo();       //获取TLS信息
	UpdateData(FALSE);	//刷新


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//获取TLS信息
void PETLS::GetTLSInfo()
{
	PBYTE pBuf = (PBYTE)m_pDos;
	PIMAGE_TLS_DIRECTORY32 pTLS = (PIMAGE_TLS_DIRECTORY32)(pBuf +
		PERVAtoFOA::RVAtoFOA(m_pDataDir[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress, m_pNTHeader));

	//输出信息
	m_TLSStart.Format(L"%p", pTLS->StartAddressOfRawData);
	m_TLSEnd.Format(L"%p", pTLS->EndAddressOfRawData);
	m_TLSIndex.Format(L"%08X", pTLS->AddressOfIndex);
	m_TLSCallBack.Format(L"%p", pTLS->AddressOfCallBacks);
	m_TLSZero.Format(L"%08X", pTLS->SizeOfZeroFill);
	m_TLSChara.Format(L"%08X", pTLS->Characteristics);
}
