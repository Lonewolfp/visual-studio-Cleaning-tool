#pragma once
#include"MyList.h"

// PEPxport 对话框

class PEPxport : public CDialogEx
{
	DECLARE_DYNAMIC(PEPxport)

public:
	PEPxport(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNTHeader, 
		       CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PEPxport();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EXPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_Name;                      //名称字符串 
	CString m_FunNumber;	             //函数数量
	CString m_FunNumberName;             //函数名数量
	CString m_FunAdd;		             //函数地址
	CString m_FunNameAdd;	             //函数名称地址
	CString m_FunOrdAdd;	             //函数名序号地址

	MyList m_ExportList;	             //输出表列表

	CString m_ListOrdinal;				 //序号
	CString m_ListRVA;					 //RVA
	CString m_ListFO;					 //文件偏移
	CString m_ListName;					 //函数名

	PIMAGE_DOS_HEADER m_pDos;			 //DOS头
	PIMAGE_NT_HEADERS32 m_pNTHeader;	 //NT头
	PIMAGE_DATA_DIRECTORY m_pDataDir;	 //数据目录地址

	void GetExportInfo();				 //获取导出表信息

public:
	virtual BOOL OnInitDialog();         //初始化
};
