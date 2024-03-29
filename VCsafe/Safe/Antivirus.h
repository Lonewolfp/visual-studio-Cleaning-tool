#pragma once


// Antivirus 对话框

class Antivirus : public CDialogEx
{
	DECLARE_DYNAMIC(Antivirus)

public:
	Antivirus(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Antivirus();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();		  //闪电查毒
	afx_msg void OnBnClickedButton2();		  //全盘杀毒
	afx_msg void OnBnClickedButton3();		  //指定位置杀毒
};
