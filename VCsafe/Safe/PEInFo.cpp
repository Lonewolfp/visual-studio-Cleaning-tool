// PEInFo.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "PEInFo.h"
#include "afxdialogex.h"
#include"PESection.h"
#include "PERVAtoFOA.h"
#include "PETime.h"
#include "PEDirectory.h"


// PEInFo 对话框

//IMPLEMENT_DYNAMIC(PEInFo, CDialogEx)

PEInFo::PEInFo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PEINFO, pParent)
	, m_AddressOfEntryPoint(_T(""))
	, m_ImageBase(_T(""))
	, m_SizeOfImage(_T(""))
	, m_BaseOfCode(_T(""))
	, m_BaseOfData(_T(""))
	, m_SectionAlignment(_T(""))
	, m_FileAlignment(_T(""))
	, m_Magic(_T(""))
	, m_Subsystem(_T(""))
	, m_NumberOfSections(_T(""))
	, m_TimeDateStamp(_T(""))
	, m_SizeOfHeaders(_T(""))
	, m_Characteristics(_T(""))
	, m_CheckSum(_T(""))
	, m_SizeOfOptionalHeader(_T(""))
	, m_NumberOfRvaAndSizes(_T(""))
{

}

PEInFo::~PEInFo()
{
}

void PEInFo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_AddressOfEntryPoint);
	DDX_Text(pDX, IDC_EDIT10, m_ImageBase);
	DDX_Text(pDX, IDC_EDIT12, m_SizeOfImage);
	DDX_Text(pDX, IDC_EDIT14, m_BaseOfCode);
	DDX_Text(pDX, IDC_EDIT16, m_BaseOfData);
	DDX_Text(pDX, IDC_EDIT2, m_SectionAlignment);
	DDX_Text(pDX, IDC_EDIT4, m_FileAlignment);
	DDX_Text(pDX, IDC_EDIT6, m_Magic);
	DDX_Text(pDX, IDC_EDIT9, m_Subsystem);
	DDX_Text(pDX, IDC_EDIT11, m_NumberOfSections);
	DDX_Text(pDX, IDC_EDIT13, m_TimeDateStamp);
	DDX_Text(pDX, IDC_EDIT15, m_SizeOfHeaders);
	DDX_Text(pDX, IDC_EDIT17, m_Characteristics);
	DDX_Text(pDX, IDC_EDIT18, m_CheckSum);
	DDX_Text(pDX, IDC_EDIT19, m_SizeOfOptionalHeader);
	DDX_Text(pDX, IDC_EDIT20, m_NumberOfRvaAndSizes);
}

BEGIN_MESSAGE_MAP(PEInFo, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &PEInFo::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &PEInFo::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &PEInFo::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &PEInFo::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &PEInFo::OnBnClickedButton4)
END_MESSAGE_MAP()


// PEInFo 消息处理程序

  //消息映射函数
BOOL PEInFo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//提高权限拖拽文件
	ChangeWindowMessageFilterEx(this->m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(this->m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);

	
	m_hFile = NULL;        //释放资源

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}




 //拖曳文件
void PEInFo::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//获取文件路径
	TCHAR szPatch[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPatch, MAX_PATH);
	m_FileRoute = szPatch;

	//获取文件信息
	GetFileInfo();

	//刷新到控件
	UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);
}


//获取文件PE信息
void PEInFo::GetFileInfo()
{
	//打开文件，获取句柄
	m_hFile = CreateFile(m_FileRoute, GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//获取文件大小
	DWORD dwSize = GetFileSize(m_hFile, NULL);
	PBYTE pBuf = new BYTE[dwSize]{};

	//读取文件
	ReadFile(m_hFile, pBuf, dwSize, &dwSize, NULL);

	//判断是否PE文件，判断DOS头
	m_pDos = (PIMAGE_DOS_HEADER)pBuf;
	if (m_pDos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(L"不是PE文件", L"提示", MB_OK | MB_ICONERROR);
		CloseHandle(m_hFile);  //关闭句柄
		m_hFile = NULL;        //释放资源
		return;
	}

	//判断是否PE文件，判断NT头
	m_pNTHeader = (PIMAGE_NT_HEADERS)(m_pDos->e_lfanew + pBuf);
	if (m_pNTHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(L"不是PE文件", L"提示", MB_OK | MB_ICONERROR);
		CloseHandle(m_hFile);  //关闭句柄
		m_hFile = NULL;        //释放资源
		return;
	}

	//获取文件头信息
	m_pFile = &m_pNTHeader->FileHeader;
	m_NumberOfSections.Format(L"%04X", m_pFile->NumberOfSections);           //区段数目
	m_TimeDateStamp.Format(L"%p", m_pFile->TimeDateStamp);                   //日期时间标志
	m_SizeOfOptionalHeader.Format(L"%04X", m_pFile->SizeOfOptionalHeader);   //可选头部大小
	m_Characteristics.Format(L"%04X", m_pFile->Characteristics);             //特征值

	//扩展头信息
	m_pOpt = &m_pNTHeader->OptionalHeader;

	m_AddressOfEntryPoint.Format(L"%p", m_pOpt->AddressOfEntryPoint);    //入口点
	m_ImageBase.Format(L"%p", m_pOpt->ImageBase);                        //镜像基址
	m_SizeOfImage.Format(L"%08X", m_pOpt->SizeOfImage);                  //镜像大小
	m_BaseOfCode.Format(L"%p", m_pOpt->BaseOfCode);                      //代码基址
	m_BaseOfData.Format(L"%p", m_pOpt->BaseOfData);                      //数据基址
	m_SectionAlignment.Format(L"%08X", m_pOpt->SectionAlignment);        //块对齐
	m_FileAlignment.Format(L"%08X", m_pOpt->FileAlignment);              //文件块对齐
	m_Magic.Format(L"%04X", m_pOpt->Magic);                              //标志字
	m_Subsystem.Format(L"%04X", m_pOpt->Subsystem);                      //子系统
	m_SizeOfHeaders.Format(L"%08X", m_pOpt->SizeOfHeaders);              //首部大小
	m_NumberOfRvaAndSizes.Format(L"%08X", m_pOpt->NumberOfRvaAndSizes);  //RVA大小
	m_CheckSum.Format(L"%08X", m_pOpt->CheckSum);                        //校验和


}

//打开文件
void PEInFo::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CFileDialog dlg(TRUE, _T("exe"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		            _T("可执行程序|*.exe|所有文件|*||"));

	int nRet = dlg.DoModal();

	if (nRet == IDOK) 
	{

		m_FileRoute = dlg.GetPathName();

		//获取文件信息
		GetFileInfo();

		//刷新
		UpdateData(FALSE);
	}
}

//按钮控件，打开区段表
void PEInFo::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!m_hFile)
	{
		MessageBox(L"请打开文件!", L"提示!");
		return;
	}
	
	PESection * pSection = new PESection(m_pNTHeader);
	pSection->DoModal();
}

//按钮控件，位置计算ROAtoFOA
void PEInFo::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_hFile)
	{
		MessageBox(L"请打开文件!", L"提示!");
		return;
	}
	
	PERVAtoFOA *pRVA = new PERVAtoFOA(m_pNTHeader);
	pRVA->DoModal();
}

//按钮控件，时间转换器
void PEInFo::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!m_hFile)
	{
		MessageBox(L"请打开文件!", L"提示!");
		return;
	}

	PETime * pTime = new PETime(m_pFile->TimeDateStamp);
	pTime->DoModal();
}

//按钮控件，打开目录表
void PEInFo::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_hFile)
	{
		MessageBox(L"请打开文件!", L"提示!");
		return;
	}

	PEDirectory * pDirectory = new PEDirectory(m_pDos, m_pNTHeader);
	pDirectory->DoModal();
}
