// Antivirus.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "Antivirus.h"
#include "afxdialogex.h"
#include "AntiScan.h"


// Antivirus 对话框

IMPLEMENT_DYNAMIC(Antivirus, CDialogEx)

Antivirus::Antivirus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TAB3, pParent)
{

}

Antivirus::~Antivirus()
{
}

void Antivirus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Antivirus, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Antivirus::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Antivirus::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Antivirus::OnBnClickedButton3)
END_MESSAGE_MAP()


// Antivirus 消息处理程序

//闪电查毒
void Antivirus::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePath = L"C:\\Users\\LP\\AppData\\Local\\Microsoft\\Windows";
	AntiScan *pAS = new AntiScan(FilePath, 0);
	pAS->Create(IDD_DIALOG_ANTISCAN);
	pAS->ShowWindow(SW_NORMAL);
}

//全盘杀毒
void Antivirus::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//此处路径为任意值，扫描类型1，类内再获取驱动器目录全盘扫描；
	CString FilePath = L"O:\\";
	AntiScan *pAS = new AntiScan(FilePath, 1);
	pAS->Create(IDD_DIALOG_ANTISCAN);
	pAS->ShowWindow(SW_NORMAL);
}

//指定位置杀毒
void Antivirus::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	//文件夹路径缓存
	TCHAR szDir[MAX_PATH];

	//必须初始化
	BROWSEINFO bfi = {};		//浏览信息
	ITEMIDLIST *pidl;			//文件夹标识符列表
	bfi.hwndOwner = this->m_hWnd;
	bfi.pszDisplayName = szDir;
	bfi.lpszTitle = _T("请选择文件夹");
	bfi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	pidl = SHBrowseForFolder(&bfi);

	//点了取消，或者无效文件夹返回NULL  
	if (pidl == NULL)
		return;

	//获取文件夹路径
	if (SHGetPathFromIDList(pidl, szDir))
	{
		CString FilePath = szDir;
		AntiScan *pAS = new AntiScan(FilePath, 2);
		pAS->Create(IDD_DIALOG_ANTISCAN);
		pAS->ShowWindow(SW_NORMAL);
	}
}
