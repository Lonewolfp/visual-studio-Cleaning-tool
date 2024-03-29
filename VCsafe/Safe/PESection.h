#pragma once
#include"MyList.h"

// PESection 对话框

class PESection : public CDialogEx
{
	DECLARE_DYNAMIC(PESection)

public:
	PESection(PIMAGE_NT_HEADERS32 pNTHeader, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PESection();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	MyList m_SectionList;		       //区块表
	PIMAGE_NT_HEADERS32 m_pNTHeader;   //NT头

	CString	m_strName;				   //名称
	CString	m_strRVA;				   //RVA
	CString	m_strVSize;				   //VSize
	CString	m_strROffset;			   //ROffset区段的文件偏移
	CString	m_strRSize;				   //RSize区段在文件的大小
	CString	m_strSign;				   //标志

	void GetSectionInfo();			   //获取区块信息
};
