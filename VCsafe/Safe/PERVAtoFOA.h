#pragma once


// PERVAtoFOA 对话框

class PERVAtoFOA : public CDialogEx
{
	DECLARE_DYNAMIC(PERVAtoFOA)

public:
	PERVAtoFOA(PIMAGE_NT_HEADERS32 pNTHeader,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PERVAtoFOA();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RVAtoFOA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedButton1();   //RVAtoFOA
	DECLARE_MESSAGE_MAP()
private:
	PIMAGE_NT_HEADERS32 m_pNTHeader;	        //NT头
	CString m_strVA;                    //VA
	CString m_strRVA;                   //RVA
	CString m_strFOA;                   //FO
public:
	
	static DWORD RVAtoFOA(DWORD dwRva, PIMAGE_NT_HEADERS32 pNt);	//静态函数给外部调用
};
