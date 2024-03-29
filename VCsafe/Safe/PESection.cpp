// PESection.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "PESection.h"
#include "afxdialogex.h"
#include "PEInFo.h"


// PESection 对话框

IMPLEMENT_DYNAMIC(PESection, CDialogEx)

PESection::PESection(PIMAGE_NT_HEADERS32 pNTHeader,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SECTION, pParent)
{
	m_pNTHeader = pNTHeader;
}

PESection::~PESection()
{
}

void PESection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);;
	DDX_Control(pDX, IDC_LIST_SECTION, m_SectionList);
}


BEGIN_MESSAGE_MAP(PESection, CDialogEx)
END_MESSAGE_MAP()


// PESection 消息处理程序
BOOL PESection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化列表
	m_SectionList.AddColumn(6, L"名称", 80, L"RVA", 80, L"VSize", 80, 
		                    L"ROffset", 80, L"RSize", 80, L"标志", 80);
	GetSectionInfo();   //获取区段的信息

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//获取区段的信息
void PESection::GetSectionInfo()
{
	//获取区段头表
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(m_pNTHeader);

	//循环判断在哪个区段
	for (int i = 0; i < m_pNTHeader->FileHeader.NumberOfSections; i++)
	{
		CHAR pName[9] = {};
		memcpy_s(pName, 9, pSec[i].Name, 8);
		m_strName = pName;
		m_strRVA.Format(L"%p", pSec[i].VirtualAddress);
		m_strVSize.Format(L"%p", pSec[i].Misc.VirtualSize);
		m_strROffset.Format(L"%p", pSec[i].PointerToRawData);
		m_strRSize.Format(L"%p", pSec[i].SizeOfRawData);
		m_strSign.Format(L"%p", pSec[i].Characteristics);

		m_SectionList.AddItem(6, m_strName, m_strRVA, m_strVSize,
			                  m_strROffset, m_strRSize, m_strSign);

	}
}
