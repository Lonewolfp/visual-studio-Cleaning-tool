// AntiScan.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "AntiScan.h"
#include "afxdialogex.h"

#include "Md5.h"

// AntiScan 对话框

IMPLEMENT_DYNAMIC(AntiScan, CDialogEx)

AntiScan::AntiScan(CString FilePath, DWORD dwType, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ANTISCAN, pParent)
{
	m_dwScanType = dwType;
	m_FilePath = FilePath;
	m_FolderPath = FilePath;
}

AntiScan::~AntiScan()
{
}

void AntiScan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ANTIVIRUS, m_ListAntiScan);
	DDX_Control(pDX, IDC_TEXT_SCANINFO, m_TextScanInfo);
}


BEGIN_MESSAGE_MAP(AntiScan, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ANTI_PAUSE, &AntiScan::OnBnClickedButtonAntiPause)
	ON_BN_CLICKED(IDC_BUTTON_ANTI_CANCLE, &AntiScan::OnBnClickedButtonAntiCancle)
	ON_BN_CLICKED(IDC_BUTTON_ANTI_OK, &AntiScan::OnBnClickedButtonAntiOk)
END_MESSAGE_MAP()


// AntiScan 消息处理程序
BOOL AntiScan::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListAntiScan.AddColumn(3, L"位置", 300, L"MD5", 250, L"类型", 100);

	//读取数据库
	ReadDataBase();

	//创建扫描线程
	hScanThread = AfxBeginThread(MyThreadScan, this);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void AntiScan::Scandispense()
{
	//根据扫描类型分发
	switch (m_dwScanType)
	{
	case 0:
		//快速扫描
		FastScan();
		break;
	case 1:
		//全盘扫描
		ScanAll();
		break;
	case 2:
		//文件扫描
		ScanFile();
		break;
	case 3:
		//备用
		break;
	case 4:
		break;
	default:
		break;
	}
}

//快速扫描
void AntiScan::FastScan()
{
	//扫描进程
	ScanProcess();
	//扫描系统文件夹
	ScanFloder(m_FolderPath);

	m_TextScanInfo.SetWindowTextW(L"扫描完成");
	MessageBox(L"扫描完成!\r\n请处理！", L"提示");
}

//全盘扫描
void AntiScan::ScanAll()
{
	//获取盘符;
	//TCHAR szBuf[1024] = {};
	//GetLogicalDriveStrings(1024, szBuf);
	TCHAR *pstr = new TCHAR[1024]{0};

	_stprintf_s(pstr,1024, L"%s", m_FolderPath.GetBuffer());

	while (*pstr)
	{
		m_vecDisk.push_back(pstr);
		//获取下一个盘符;
		pstr += _tcslen(pstr) + 1;
	}
	//扫描进程
	ScanProcess();

	//循环遍历盘符
	for (DWORD i = 0; i < m_vecDisk.size(); i++)
	{
		ScanFloder(m_vecDisk[i]);
	}
	MessageBox(L"扫描完成!\r\n请处理！", L"提示");
}


//扫描单个文件
void AntiScan::ScanFile()
{
	//转换文件名;
	CStringA FinalFileName = CW2A(m_FilePath.GetBuffer(), CP_THREAD_ACP);

	//获取MD5
	char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
	m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

	m_TextScanInfo.SetWindowTextW(L"扫描完成");
	//对比MD5
	if (DataBaseCMP(m_ScanMd5))
	{
		m_vecFilePath.push_back(m_FilePath);
		m_ListAntiScan.AddItem(3, m_FilePath, m_ScanMd5, L"病毒文件");
		//MessageBox(L"病毒文件!\r\n请处理！");
	}
	else
	{
		m_ListAntiScan.AddItem(3, m_FilePath, m_ScanMd5, L"正常文件");
		MessageBox(L"正常文件!", L"提示");
	}
}

//扫描文件夹
void AntiScan::ScanFloder(CString FloderPath)
{
	//取消任务
	if (m_IsCancle)
	{
		//结束进程
		return;
	}

	//暂停任务
	while (m_IsPause)
	{
	}

	//空路径退出;
	if (FloderPath.IsEmpty())
	{
		m_IsCancle = true;
		return;
	}

	//打开夹;
	WIN32_FIND_DATA wsd = {};
	HANDLE hFind = FindFirstFile(FloderPath, &wsd);
	FloderPath += L"\\*";
	hFind = FindFirstFile(FloderPath, &wsd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
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
			CString NextDir = FloderPath.Left(FloderPath.GetLength() - 1);
			NextDir += wsd.cFileName;
			ScanFloder(NextDir);
		}
		//转换文件名格式;
		TCHAR* szBuff = wsd.cFileName;
		//获取后缀名;
		TCHAR* szExName = PathFindExtension(szBuff);
		//对比后缀名;

		//对比后缀名；此处也可选用文件浏览模块中的文件类型对比
		if (!wcscmp(szExName, L".exe") ||
			!wcscmp(szExName, L".EXE"))
		{
			//拼接文件名;
			CString m_ScanFileFath = FloderPath.Left(FloderPath.GetLength() - 1);
			m_ScanFileFath += wsd.cFileName;
			CStringA FinalFileName = CW2A(m_ScanFileFath.GetBuffer(), CP_THREAD_ACP);

			//获取MD5
			char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
			m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

			//更新扫描状态
			CString strScanInfo = L"正在扫描：";
			strScanInfo += m_ScanFileFath;
			m_TextScanInfo.SetWindowTextW(strScanInfo);

			//对比MD5
			if (DataBaseCMP(m_ScanMd5))
			{
				m_vecFilePath.push_back(m_ScanFileFath);
				m_ListAntiScan.AddItem(3, m_ScanFileFath, m_ScanMd5, L"病毒文件");
			}
		}
	}
	while (FindNextFile(hFind, &wsd));
	FindClose(hFind);
	return;
}

//扫描进程
void AntiScan::ScanProcess()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"HELP 函数调用失败！", L"提示");
		return;
	}

	//初始化快照信息结构体;
	PROCESSENTRY32 pi = { sizeof(PROCESSENTRY32) };
	if (!Process32First(hSnapShot, &pi))
	{
		MessageBox(L"获取进程信息失败", L"提示");
		return;
	}
	do
	{
		TCHAR *szBuf[5] = { new TCHAR[MAX_PATH], new TCHAR[6],  new TCHAR[6],  new TCHAR[6], new TCHAR[MAX_PATH], };
		szBuf[0] = pi.szExeFile;

		//打开要获取路径的进程
		HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION
			| PROCESS_QUERY_LIMITED_INFORMATION,
			FALSE, pi.th32ProcessID);
		//获取进程完整路径;
		if (hPro)
		{
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hPro, NULL, szBuf[4], &dwSize);
		}
		else
		{
			szBuf[4] = L"无权访问";
		}

		CString  FileFathW = szBuf[4];
		CStringA FinalFileName = CW2A(szBuf[4], CP_THREAD_ACP);
		//获取MD5
		char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
		m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

		CString strScanInfo = L"正在扫描进程：";
		strScanInfo += FileFathW;
		m_TextScanInfo.SetWindowTextW(strScanInfo);

		//对比MD5
		if (DataBaseCMP(m_ScanMd5))
		{

			if (TerminateProcess(hPro, 0))
			{
				MessageBox(L"发现可疑进程!\r\n已结束进程\r\n", L"提示");
			}
			else
			{
				MessageBox(L"发现可疑进程!\r\n处理失败!\r\n请手动处理！", L"提示");
			}
			m_vecFilePath.push_back(FileFathW);
			m_ListAntiScan.AddItem(3, FileFathW, m_ScanMd5, L"病毒进程");
		}
		CloseHandle(hPro);

	} while (Process32Next(hSnapShot, &pi));
}

//读取本地数据库
void AntiScan::ReadDataBase()
{
	//读取病毒库
	CString strDatabase = szFileName[0];
	CStdioFile sfDatabase;
	if (sfDatabase.Open(strDatabase, CFile::modeRead))
	{
		CString temp;
		//循环读取
		while (sfDatabase.ReadString(temp))
		{
			m_vecVirusDatabase.push_back(temp);
		}
		sfDatabase.Close();
	}
	else
	{
		MessageBox(L"读取本地病毒库失败！", L"提示");
		m_dwScanType = 4;
		return;
	}

	//读取白名单
	CString strWhite = szFileName[1];
	CStdioFile sfWhite;
	if (sfWhite.Open(strWhite, CFile::modeRead))
	{
		CString temp;
		//循环读取
		while (sfWhite.ReadString(temp))
		{
			m_vecWhite.push_back(temp);
		}
		sfWhite.Close();
	}

	//读取黑名单
	CString strBlack = szFileName[2];
	CStdioFile sfBlack;
	if (sfBlack.Open(strBlack, CFile::modeRead))
	{
		CString temp;
		//循环读取
		while (sfBlack.ReadString(temp))
		{
			m_vecBlack.push_back(temp);
		}
		sfBlack.Close();
	}
}

//对比数据库
bool AntiScan::DataBaseCMP(CString strMD5)
{
	//对比白名单
	for (DWORD i = 0; i < m_vecWhite.size(); i++)
	{
		if (strMD5 == m_vecWhite[i])
		{
			return false;
		}
	}

	//对比黑名单
	for (DWORD i = 0; i < m_vecBlack.size(); i++)
	{
		if (strMD5 == m_vecBlack[i])
		{
			return true;
		}
	}

	//对比病毒库
	for (DWORD i = 0; i < m_vecVirusDatabase.size(); i++)
	{
		if (strMD5 == m_vecVirusDatabase[i])
		{
			return true;
		}
	}
	return false;
}


//全盘扫描线程
UINT AntiScan::MyThreadScan(LPVOID lpParam)
{
	AntiScan* pAntiScan = (AntiScan*)lpParam;

	pAntiScan->Scandispense();

	pAntiScan->hScanThread = NULL;

	return 0;
}


//暂停任务
void AntiScan::OnBnClickedButtonAntiPause()
{
	// TODO: 在此添加控件通知处理程序代码

	//任务未开始不能暂停
	if (!hScanThread)
	{
		MessageBox(L"扫描任务已结束！", L"提示");
	}
	m_IsPause = !m_IsPause;

	//更改按钮状态
	if (m_IsPause)
	{
		GetDlgItem(IDC_BUTTON_ANTI_PAUSE)->SetWindowTextW(L"继续扫描");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ANTI_PAUSE)->SetWindowTextW(L"暂停扫描");
	}
}

//取消扫描
void AntiScan::OnBnClickedButtonAntiCancle()
{
	// TODO: 在此添加控件通知处理程序代码
	m_IsCancle = true;
}

//处理病毒
void AntiScan::OnBnClickedButtonAntiOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//未发现需要处理项目
	if (m_vecFilePath.size() == 0)
	{
		MessageBox(L"暂无可处理项目！", L"提示");
		return;
	}

	//删除病毒
	for (DWORD i = 0; i < m_vecFilePath.size(); i++)
	{
		DeleteFile(m_vecFilePath[0]);
	}
	MessageBox(L"处理完成！", L"提示");
}
