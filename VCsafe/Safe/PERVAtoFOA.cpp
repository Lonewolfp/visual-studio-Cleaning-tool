// PERVAtoFOA.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "PERVAtoFOA.h"
#include "afxdialogex.h"


// PERVAtoFOA 对话框

IMPLEMENT_DYNAMIC(PERVAtoFOA, CDialogEx)

PERVAtoFOA::PERVAtoFOA(PIMAGE_NT_HEADERS32 pNTHeader,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RVAtoFOA, pParent)
	, m_strVA(_T(""))
	, m_strRVA(_T(""))
	, m_strFOA(_T(""))
{
	m_pNTHeader = pNTHeader;
}

PERVAtoFOA::~PERVAtoFOA()
{
	
}

void PERVAtoFOA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VA, m_strVA);
	DDX_Text(pDX, IDC_EDIT_RVA, m_strRVA);
	DDX_Text(pDX, IDC_EDIT_FOA, m_strFOA);
}


BEGIN_MESSAGE_MAP(PERVAtoFOA, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &PERVAtoFOA::OnBnClickedButton1)
END_MESSAGE_MAP()


// PERVAtoFOA 消息处理程序


////RVAtoFOA
void PERVAtoFOA::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//更新数据到变量
	UpdateData(TRUE);

	//RVA和FOA转换
	DWORD dwRVA = wcstol(m_strRVA, NULL, 16);
	DWORD dwFOA = RVAtoFOA(dwRVA, m_pNTHeader);

	//判断输入地址是否有问题
	if (dwFOA == -1)
	{
		MessageBox(L"地址范围有误", L"提示");
		return;
	}

	//获取VA
	DWORD dwVA = m_pNTHeader->OptionalHeader.ImageBase + dwRVA;

	//输出
	m_strVA.Format(L"%p", dwVA);
	m_strRVA.Format(L"%p", dwRVA);
	m_strFOA.Format(L"%p", dwFOA);

	//更新数据到控件
	UpdateData(FALSE);
}


//静态函数给外部调用
DWORD PERVAtoFOA::RVAtoFOA(DWORD dwRva, PIMAGE_NT_HEADERS32 pNt)
{
	//文件对齐与内存对齐相等
	if (pNt->OptionalHeader.FileAlignment == pNt->OptionalHeader.SectionAlignment)
	{
		return dwRva;
	}

	//获取区段头
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNt);

	//循环查找
	for (int i = 0; i < pNt->FileHeader.NumberOfSections; i++)
	{
		//比对地址
		if (dwRva >= pSec[i].VirtualAddress&&
			dwRva <= pSec[i].VirtualAddress + pSec[i].SizeOfRawData)
		{
			return	dwRva - pSec[i].VirtualAddress + pSec[i].PointerToRawData;
		}
	}
	return -1;
}
