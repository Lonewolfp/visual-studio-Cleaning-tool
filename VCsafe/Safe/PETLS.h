#pragma once


// PETLS 对话框

class PETLS : public CDialogEx
{
	DECLARE_DYNAMIC(PETLS)

public:
	PETLS(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNTHeader, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PETLS();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:								
	CString m_TLSStart;					//源数据起始地址
	CString m_TLSEnd;					//源数据终止地址
	CString m_TLSIndex;					//索引的位置地址
	CString m_TLSCallBack;				//回调函数地址
	CString m_TLSZero;					//填充0数量
	CString m_TLSChara;					//属性，恒为0

	PIMAGE_DOS_HEADER m_pDos;		    //DOS头
	PIMAGE_NT_HEADERS32 m_pNTHeader;    //NT头
	PIMAGE_DATA_DIRECTORY m_pDataDir;   //目录头
	
public:								    
	virtual BOOL OnInitDialog();        //初始化函数
	void GetTLSInfo();				    //获取TLS信息
};
