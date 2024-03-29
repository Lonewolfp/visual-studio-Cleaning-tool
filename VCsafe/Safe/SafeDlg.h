
// SafeDlg.h: 头文件
//

#pragma once
#include "Task.h"
#include"Tool.h"
#include "Antivirus.h"


// CSafeDlg 对话框
class CSafeDlg : public CDialogEx
{
// 构造
public:
	CSafeDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAFE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);		 //老板键

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_maintab;  //首页tab控件变量

	Task m_para1;        //任务管理对话框变量
	Tool m_para2;        //实用工具对话框变量
	Antivirus m_para3;   //杀毒对话框变量

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnMenuTop(UINT id);		//响应快捷键
	afx_msg void OnBnClickedButton1();
};
