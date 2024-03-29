#pragma once


// PEInFo 对话框

class PEInFo : public CDialogEx
{
	
public:
	PEInFo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PEInFo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PEINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();                //消息映射函数

	DECLARE_MESSAGE_MAP()
private:
	//PE文件信息变量
	CString m_AddressOfEntryPoint;    //入口点
	CString m_ImageBase;              //镜像基址
	CString m_SizeOfImage;            //镜像大小
	CString m_BaseOfCode;             //代码基址
	CString m_BaseOfData;             //数据基址
	CString m_SectionAlignment;       //块对齐
	CString m_FileAlignment;          //文件块对齐
	CString m_Magic;                  //标志字
	CString m_Subsystem;              //子系统
	CString m_NumberOfSections;       //区段数量
	CString m_TimeDateStamp;          //日期时间标志
	CString m_SizeOfHeaders;          //首部大小
	CString m_Characteristics;        //特征值
	CString m_CheckSum;               //校验和
	CString m_SizeOfOptionalHeader;   //可选头部大小
	CString m_NumberOfRvaAndSizes;    //RVA数大小

	HANDLE m_hFile;                   //文件句柄
	CString m_FileRoute;              //文件路径
	
	PIMAGE_DOS_HEADER m_pDos;         //DOS头
	PIMAGE_NT_HEADERS m_pNTHeader;    //NT头
	PIMAGE_FILE_HEADER m_pFile;       //文件头
	PIMAGE_OPTIONAL_HEADER m_pOpt;    //扩展头

	void GetFileInfo();               //获取文件PE信息

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);  //拖曳文件
	afx_msg void OnBnClickedButton1();          //打开文件
	afx_msg void OnBnClickedButton2();          //打开区段表
	afx_msg void OnBnClickedButton3();          //位置计算ROAtoFOA
	afx_msg void OnBnClickedButton5();          //时间转换器
	afx_msg void OnBnClickedButton4();          //打开目录表  
};
