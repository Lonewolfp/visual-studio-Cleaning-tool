// Clean.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "Clean.h"
#include "afxdialogex.h"


// Clean 对话框

IMPLEMENT_DYNAMIC(Clean, CDialogEx)

Clean::Clean(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CLEAN, pParent)
	, m_DirName(_T(""))
	, m_DeleVSName(_T(""))
	, m_CbSystem(FALSE)
	, m_CbIE(FALSE)
	, m_CbRecycle(FALSE)
	, m_CbDisk(FALSE)
	, m_CbAll(FALSE)
{

}

Clean::~Clean()
{
}

void Clean::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VSC_EDIT2, m_DirName);
	DDX_Text(pDX, IDC_VSC_EDIT1, m_DeleVSName);
	DDX_Control(pDX, IDC_LIST_CLEAN, m_ListClean);
	DDX_Check(pDX, IDC_CHECK_SYSTEM, m_CbSystem);
	DDX_Check(pDX, IDC_CHECK_IE, m_CbIE);
	DDX_Check(pDX, IDC_CHECK_RECYCLE, m_CbRecycle);
	DDX_Check(pDX, IDC_CHECK_DISK, m_CbDisk);
	DDX_Check(pDX, IDC_CHECK_ALL, m_CbAll);
}


BEGIN_MESSAGE_MAP(Clean, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, &Clean::OnBnClickedButtonClean)
	ON_BN_CLICKED(IDC_VSC_BUTTON, &Clean::OnBnClickedVscButton)
	ON_BN_CLICKED(IDC_CHECK_ALL, &Clean::OnBnClickedCheckAll)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// Clean 消息处理程序

//初始化
BOOL Clean::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//允许管理员权限拖曳文件;
	ChangeWindowMessageFilterEx(this->m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(this->m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);
	// 0x0049 == WM_COPYGLOBALDATA


	//定义清理文件类型
	m_DeleVSName = L"\\\.obj \\\.tlog \\\.lastbuildstate \\\.idb \\\.pdb \\\.pch \\\.res \\\.ilk \\\.exe \\\.sdf \\\.ipch \\\.log \\\.db";
	m_DeleFileName = L"\\\.tmp \\\._mp \\\.log \\\.gid \\\.chk \\\.old \\\.bak";

	//定义路径
	m_PathA = L"%%windir%%";
	m_PathB = L"%%systemdrive%%";
	m_PathIE = L"%%userprofile%%\\Local Settings\\Temporary Internet Files\\";

	m_ListClean.AddColumn(3, L"序号", 50, L"文件路径", 200, L"状态", 100);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//自定义清理
void Clean::OnBnClickedVscButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取路径;
	UpdateData(TRUE);

	m_ListClean.DeleteAllItems();
	m_Result = L"自定义类型";
	m_dwIndex = 1;

	//清理;
	if (DeleFile(m_DirName, m_DeleVSName))
	{
		MessageBox(L"清理完成", L"提示");
	}
	else
	{
		MessageBox(L"清理失败", L"提示");
	}
}

//清理垃圾
bool Clean::DeleFile(CString DirName, CString FileName)
{
	//空路径退出;
	if (DirName.IsEmpty())
	{
		MessageBox(L"路径有误", L"提示");
		return	false;
	}

	//判断是否文件夹;
	WIN32_FIND_DATA wsd = {};
	HANDLE hFind = FindFirstFile(DirName, &wsd);
	if (!(wsd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
	{
		//MessageBox(L"不是文件夹");
		return false;
	}

	DirName += L"\\*";
	hFind = FindFirstFile(DirName, &wsd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"打开路径失败", L"提示");
	}

	do
	{
		//过滤本级和上级;
		if (!wcscmp(wsd.cFileName, L".")
			|| !wcscmp(wsd.cFileName, L".."))
		{
			continue;
		}
		//递归遍历文件夹;
		else if (wsd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			CString NextDir = DirName.Left(DirName.GetLength() - 1);
			NextDir += wsd.cFileName;
			DeleFile(NextDir, FileName);
		}
		//转换文件名格式;
		TCHAR* szBuff = wsd.cFileName;
		//获取后缀名;
		TCHAR* szExName = PathFindExtension(szBuff);
		//对比后缀名;
		if (FileName.Find(szExName) >= 0)
		{
			//删除指定文件;
			CString DeleFileName = DirName.Left(DirName.GetLength() - 1);
			DeleFileName += wsd.cFileName;
			DeleteFile(DeleFileName);
			m_Index.Format(L"%d", m_dwIndex);
			m_DeleFileName = DeleFileName;
			m_ListClean.AddItem(3, m_Index, m_DeleFileName, m_Result);
			m_dwIndex++;
		}

	} while (FindNextFile(hFind, &wsd));

	FindClose(hFind);

	return true;
}

//清理信息
void Clean::CleanRecycle()
{
	//初始化
	SHQUERYRBINFO RecycleBinInfo = {};
	RecycleBinInfo.cbSize = sizeof(SHQUERYRBINFO);

	//查询信息
	SHQueryRecycleBin(NULL, &RecycleBinInfo);

	//清空
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

//拖动文件
void Clean::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	TCHAR szPatch[MAX_PATH] = { 0 };

	UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (UINT idx = 0; idx < nCount; idx++)
	{
		DragQueryFile(hDropInfo, idx, szPatch, MAX_PATH);
		SetDlgItemText(IDC_VSC_EDIT2, szPatch);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}


// 全选按钮
void Clean::OnBnClickedCheckAll()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_CbAll)
	{
		m_CbSystem = TRUE;
		m_CbRecycle = TRUE;
		m_CbDisk = TRUE;
		m_CbIE = TRUE;
	}
	else
	{
		m_CbSystem = FALSE;
		m_CbRecycle = FALSE;
		m_CbDisk = FALSE;
		m_CbIE = FALSE;
	}
	UpdateData(FALSE);
}


//清理
void Clean::OnBnClickedButtonClean()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	m_ListClean.DeleteAllItems();
	m_dwIndex = 1;
	if (m_CbSystem)
	{
		m_Result = L"系统垃圾";
		DeleFile(m_PathA, m_DeleFileName);
		DeleFile(m_PathB, m_DeleFileName);
	}

	if (m_CbIE)
	{
		m_Result = L"浏览器垃圾";
		DeleFile(m_PathIE, m_DeleFileName);
	}

	if (m_CbRecycle)
	{
		m_Result = L"回收站";
		CleanRecycle();
	}

	MessageBox(L"清理完成！", L"提示");

}

