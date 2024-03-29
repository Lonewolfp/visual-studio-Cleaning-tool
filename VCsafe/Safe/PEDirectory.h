#pragma once


// PEDirectory 对话框

class PEDirectory : public CDialogEx
{
	DECLARE_DYNAMIC(PEDirectory)

public:
	PEDirectory(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNTHeader, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PEDirectory();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DIRECTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();                        //初始化函数
	DECLARE_MESSAGE_MAP()

private:
	//目录的RVA变量
	CString m_Rva_Out;			     //输出表
	CString m_Rva_In;			     //输入表
	CString m_Rva_Resouce;		     //资源表
	CString m_Rva_Except;		     //列外
	CString m_Rva_Security;		     //安全
	CString m_Rva_BaseReloc;	     //重定位
	CString m_Rva_Debug;		     //调试
	CString m_Rva_Architecture;	     //版本
	CString m_Rva_GlobalPTR;	     //全局指针
	CString m_Rva_TLS;			     //TLS表
	CString m_Rva_load;			     //载入配置
	CString m_Rva_Bound;		     //输入范围
	CString m_Rva_IAT;			     //IAT
	CString m_Rva_Delay;		     //延迟输入
	CString m_Rva_COM;			     //COM
	CString m_Rva_Retain;		     //保留

	////目录的大小变量
	CString m_Size_Out;				 //输出表
	CString m_Size_In;				 //输入表
	CString m_Size_Resouce;			 //资源表
	CString m_Size_Except;			 //列外
	CString m_Size_Security;		 //安全
	CString m_Size_BaseReloc;		 //重定位
	CString m_Size_Debug;			 //调试
	CString m_Size_Architecture;	 //版本
	CString m_Size_GlobalPTR;		 //全局指针
	CString m_Size_TLS;				 //TLS表
	CString m_Size_Load;			 //载入配置
	CString m_Size_Bound;			 //输入范围
	CString m_Size_IAT;				 //IAT
	CString m_Size_Delay;			 //延迟输入
	CString m_Size_COM;				 //COM
	CString m_Size_Retain;			 //保留

	PIMAGE_DOS_HEADER m_pDos;				//DOS头
	PIMAGE_NT_HEADERS32 m_pNTHeader;		//NT头
	PIMAGE_DATA_DIRECTORY m_pDataDir;		//目录头

	//绑定数组变量
	CString m_ArrRva[16];					//RVA数组
	CString m_ArrSize[16];					//大小数组

	void GetDirectoryInfo();	        	//输出目录信息
	
public:
	afx_msg void OnBnClickedButtonOut();        //控件按钮，获取导出表信息
	afx_msg void OnBnClickedButtonIn();         //控件按钮，获取导入表信息
	afx_msg void OnBnClickedButtonResouce();    //控件按钮，获取资源表信息
	afx_msg void OnBnClickedButtonBasereloc();  //控件按钮，获取资源表信息
	afx_msg void OnBnClickedButtonTls();        //控件按钮，获取TLS表信息
	afx_msg void OnBnClickedButtonDelay();		 //延迟载入表
};
