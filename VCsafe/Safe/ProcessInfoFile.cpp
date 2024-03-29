// ProcessInfoFile.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "ProcessInfoFile.h"
#include "afxdialogex.h"
#include "MD5.h"

// ProcessInfoFile 对话框

IMPLEMENT_DYNAMIC(ProcessInfoFile, CDialogEx)

ProcessInfoFile::ProcessInfoFile(CString  Path, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FILEINFO, pParent)
	, m_StrFileName(_T(""))
	, m_StrType(_T(""))
	, m_StrPath(Path)
	, m_StrSize(_T(""))
	, m_StrCreatTime(_T(""))
	, m_StrViewTime(_T(""))
	, m_StrChangeTime(_T(""))
	, m_bOnlyRead(FALSE)
	, m_bHide(FALSE)
	, m_StrMD5(_T(""))
{

}

ProcessInfoFile::~ProcessInfoFile()
{
}

void ProcessInfoFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_StrFileName);
	DDX_Text(pDX, IDC_EDIT2, m_StrType);
	DDX_Text(pDX, IDC_EDIT3, m_StrPath);
	DDX_Text(pDX, IDC_EDIT4, m_StrSize);
	DDX_Text(pDX, IDC_EDIT5, m_StrCreatTime);
	DDX_Text(pDX, IDC_EDIT6, m_StrViewTime);
	DDX_Text(pDX, IDC_EDIT7, m_StrChangeTime);
	DDX_Check(pDX, IDC_CHECK1, m_bOnlyRead);
	DDX_Check(pDX, IDC_CHECK2, m_bHide);
	DDX_Text(pDX, IDC_EDIT8, m_StrMD5);
}


BEGIN_MESSAGE_MAP(ProcessInfoFile, CDialogEx)
	ON_BN_CLICKED(IDOK, &ProcessInfoFile::OnBnClickedOk)
END_MESSAGE_MAP()


// ProcessInfoFile 消息处理程序

//初始化
BOOL ProcessInfoFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	
	GetFileInfo();  //获取文件信息

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//确定按钮
void ProcessInfoFile::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);         //更新数据到控件

	//获取文件路径
	DWORD Attr = GetFileAttributes(m_StrPath);
	Attr = Attr & ~FILE_ATTRIBUTE_READONLY &~FILE_ATTRIBUTE_HIDDEN;

	//多选框的状态
	if (m_bOnlyRead)   //只读
	{
		Attr |= FILE_ATTRIBUTE_READONLY;
	}
	if (m_bHide)     //隐藏
	{
		Attr |= FILE_ATTRIBUTE_HIDDEN;
	}
	//设置文件的属性
	SetFileAttributes(m_StrPath, Attr);

	CDialogEx::OnOK();
}


//获取文件信息
void ProcessInfoFile::GetFileInfo()
{

	//获取文件信息;	
	CFileStatus FileStatus;
	if (CFile::GetStatus(m_StrPath, FileStatus))
	{
		m_StrFileName = FileStatus.m_szFullName;                                     //文件名
		m_StrCreatTime.Format(FileStatus.m_ctime.Format("%Y年%m月%d日  %H:%M:%S"));  //创建时间
		m_StrChangeTime.Format(FileStatus.m_mtime.Format("%Y年%m月%d日  %H:%M:%S")); //修改时间
		m_StrViewTime.Format(FileStatus.m_atime.Format("%Y年%m月%d日  %H:%M:%S"));   //访问时间

		//文件大小
		if (FileStatus.m_size / 1024 < 1)
			m_StrSize.Format(L"%d Byte", FileStatus.m_size);
		else if (FileStatus.m_size / 1024 / 1024 < 1)
			m_StrSize.Format(L"%d KB", FileStatus.m_size / 1024);
		else if (FileStatus.m_size / 1024 / 1024 / 1024 < 1)
			m_StrSize.Format(L"%d MB", FileStatus.m_size / 1024 / 1024);
		else if (FileStatus.m_size / 1024 / 1024 / 1024 / 1024 < 1)
			m_StrSize.Format(L"%d GB", FileStatus.m_size / 1024 / 1024 / 1024);

		//只读属性
		if ((FileStatus.m_attribute & CFile::readOnly) == CFile::readOnly)
		{
			m_bOnlyRead = TRUE;
		}
		//隐藏属性
		if ((FileStatus.m_attribute & CFile::hidden) == CFile::hidden)
		{
			m_bHide = TRUE;
		}

		//MD5		
		CStringA strPath = CW2A(m_StrPath.GetBuffer(), CP_THREAD_ACP);
		m_StrMD5 = md5FileValue(strPath.GetBuffer());

		//获取文件类型;
		SHFILEINFO FileInfo = {};
		SHGetFileInfo(m_StrPath, 0, &FileInfo, sizeof(SHFILEINFO), SHGFI_TYPENAME);
		m_StrType = FileInfo.szTypeName;

		UpdateData(FALSE);   //更新数据到控件
	}

	else
	{
		MessageBox(L"无权限访问", L"提示");
	}
}




