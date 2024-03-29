#pragma once


// Task 对话框

class Task : public CDialogEx
{
	DECLARE_DYNAMIC(Task)

public:
	Task(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Task();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_DIALOG_TAB1};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();               //进入进程管理
	afx_msg void OnBnClickedButtonWindow();          //进入桌面窗口
	afx_msg void OnBnClickedButtonServices();		 //服务信息
	afx_msg void OnBnClickedButtonRun();			  //启动项
	afx_msg void OnBnClickedButtonSoftware();		  //软件管理
};
