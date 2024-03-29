#pragma once
#include "MyList.h"

// Clean 对话框

class Clean : public CDialogEx
{
	DECLARE_DYNAMIC(Clean)

public:
	Clean(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Clean();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CLEAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_DirName;			  //文件夹路径;
	CString m_DeleVSName;		  //删除文件类型;
	MyList m_ListClean;           //列表

	BOOL m_CbSystem;			   //系统垃圾
	BOOL m_CbIE;				   //浏览器垃圾
	BOOL m_CbRecycle;			   //回收站
	BOOL m_CbDisk;				   //全盘
	BOOL m_CbAll;				   //全选

	CString m_Index;
	CString m_DeleFileName;
	CString m_Result;

	CString m_PathA;
	CString m_PathB;
	CString m_PathIE;

	DWORD m_dwIndex;

public:
	virtual BOOL OnInitDialog();             //初始化
	afx_msg void OnBnClickedButtonClean();    //清理
	afx_msg void OnBnClickedVscButton();      //自定义清理
	afx_msg void OnBnClickedCheckAll();		  // 全选按钮
	afx_msg void OnDropFiles(HDROP hDropInfo); //拖动文件

	bool DeleFile(CString DirName, CString FileName);  //清理程序;
	void CleanRecycle();							  //清理信息
	
};
