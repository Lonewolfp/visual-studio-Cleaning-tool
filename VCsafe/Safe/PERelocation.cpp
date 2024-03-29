// PERelocation.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "PERelocation.h"
#include "afxdialogex.h"
#include "PERVAtoFOA.h"


// PERelocation 对话框

IMPLEMENT_DYNAMIC(PERelocation, CDialogEx)

PERelocation::PERelocation(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNTHeader, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RELOCATION, pParent)
{
	m_pDos = pDos;										    //DOS头
	m_pNTHeader = pNTHeader;							    //NT头
	m_pDataDir = pNTHeader->OptionalHeader.DataDirectory;   //目录头
}

PERelocation::~PERelocation()
{
}

void PERelocation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SECTION, m_ListSection);
	DDX_Control(pDX, IDC_LIST_DATA, m_ListData);
}


BEGIN_MESSAGE_MAP(PERelocation, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SECTION, &PERelocation::OnLvnItemchangedListSection)
END_MESSAGE_MAP()


// PERelocation 消息处理程序
//初始化函数
BOOL PERelocation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化区块名称
	m_ListSection.AddColumn(4, L"索引", 100, L"区段", 100, L"RVA", 100, L"数量", 100);

	//重定位信息
	GetRelocationInfo();

	//初始化块项目数据名称
	m_ListData.AddColumn(6, L"索引", 50, L"RVA", 80, L"FO", 80, L"类型", 80, L"FAR", 100, L"Data", 150);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//响应重定位区块列表事件函数
void PERelocation::OnLvnItemchangedListSection(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取选中数据，判断是否选中
	int nSel = m_ListSection.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据", L"提示");
		return;
	}

	//获取列表序号
	int nOrdinal = wcstol(m_ListSection.GetItemText(nSel, 0), NULL, 10);

	//获取重定位区块详细数据信息
	GetDataInfo(m_vecPReloc[nOrdinal]);

	*pResult = 0;
}

//获取重定位信息
void PERelocation::GetRelocationInfo()
{
	//定义临时指针
	PBYTE pBuf = (PBYTE)m_pDos;

	//找到重定位表
	m_pReloc = (PIMAGE_BASE_RELOCATION)(pBuf +PERVAtoFOA::RVAtoFOA
	           (m_pDataDir[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress, m_pNTHeader));

	//定义索引
	DWORD dwRelocOrinal = 0;

	//循环获取
	while (m_pReloc->VirtualAddress)
	{
		m_vecPReloc.push_back(m_pReloc);								  //存入容器	

		m_RelocOrdinal.Format(L"%d", dwRelocOrinal);					  //插入序号
		m_RelocRVA.Format(L"%p", m_pReloc->VirtualAddress);				  //插入RVA
		GetSectionName(m_pReloc->VirtualAddress);						  //获得段名
		DWORD dwCount = (m_pReloc->SizeOfBlock - 8) / sizeof(WORD);		  //获得数量
		m_NumberBlock.Format(L"%Xh/%dh", dwCount, dwCount);				  //插入数据
		m_ListSection.AddItem(4, m_RelocOrdinal, m_SectionName, m_RelocRVA, m_NumberBlock);  //插入列表名称

		//递增循环
		dwRelocOrinal++;
		m_pReloc = (PIMAGE_BASE_RELOCATION)((PBYTE)m_pReloc + m_pReloc->SizeOfBlock);
	}

}

//获取区块名
void PERelocation::GetSectionName(DWORD dwRVA)
{
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(m_pNTHeader);
	//循环查找
	for (int i = 0; i < m_pNTHeader->FileHeader.NumberOfSections; i++)
	{
		//比对地址
		if (dwRVA >= pSec[i].VirtualAddress&&
			dwRVA <= pSec[i].VirtualAddress + pSec[i].SizeOfRawData)
		{
			//获得区段名	
			CHAR pName[9] = {};
			memcpy_s(pName, 9, pSec[i].Name, 8);
			m_SectionName = pName;
			return;
		}
	}
	m_SectionName = L"-";
}

//获取区块数据
void PERelocation::GetDataInfo(PIMAGE_BASE_RELOCATION pReloc)
{
	m_ListData.DeleteAllItems();
	PBYTE pBuf = (PBYTE)m_pDos;                                     //定义临时指针

	DWORD dwCount = (pReloc->SizeOfBlock - 8) / sizeof(WORD);
	TYPEOFFSET *pOffset = (TYPEOFFSET*)(pReloc + 1);

	//循环输出
	for (DWORD i = 0; i < dwCount; i++)
	{
		m_DataOrinal.Format(L"%d", i);								  //序号

		DWORD RvaFinal = pReloc->VirtualAddress + pOffset[i].offset;
		m_DataRVA.Format(L"%08X", RvaFinal);						  //RVA

		DWORD dwFO = PERVAtoFOA::RVAtoFOA(RvaFinal, m_pNTHeader);
		m_DataFO.Format(L"%08X", dwFO);								  //FO（偏移）

		m_Type.Format(L"%d", pOffset[i].type);						  //类型

		//判断类型
		if (pOffset[i].type == 3)
		{
			PDWORD pDataFind = (PDWORD)(pBuf + dwFO);
			m_DataAdd.Format(L"%08X", *pDataFind);

			//转换实际数据地址
			DWORD dwDataVA = *pDataFind;
			DWORD dwDataRVA = dwDataVA - m_pNTHeader->OptionalHeader.ImageBase;
			DWORD dwDataFO = PERVAtoFOA::RVAtoFOA(dwDataRVA, m_pNTHeader);

			//使用无符号PBYTE类型
			PBYTE pDataByte = (PBYTE)(pBuf + dwDataFO);
			//判断数据类型
			if ((*pDataByte >= 'A'&&*pDataByte <= 'z') &&
				(*(pDataByte + 1) >= 'A'&&*(pDataByte + 1) <= 'z'))
			{
				m_DataInfo = pDataByte;
			}
			else
			{
				//输出十六进制字节数据信息
				m_DataInfo.Format(L"%02X  %02X  %02X  %02X  %02X  %02X  ",
					*(pDataByte),
					*(pDataByte + 1),
					*(pDataByte + 2),
					*(pDataByte + 3),
					*(pDataByte + 4),
					*(pDataByte + 5));
			}
		}
		else
		{
			m_DataAdd = L"-";
			m_DataInfo = L"-";
		}
		m_ListData.AddItem(6, m_DataOrinal, m_DataRVA, m_DataFO, m_Type, m_DataAdd, m_DataInfo);
	}

}
