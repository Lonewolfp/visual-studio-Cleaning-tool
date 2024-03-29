#pragma once
#include "MyList.h"
#include "ProcessInfo.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
// Process 对话框

class Process : public CDialogEx
{
	DECLARE_DYNAMIC(Process)

public:
	Process(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Process();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void On32776();					//刷新;
	afx_msg void OnClose();					//结束进程;
	afx_msg void OnAttribute();				//文件属性;
	afx_msg void OnColumnclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult);	//点击列头排序;
	afx_msg void OnMenuClick(UINT id);											//响应右键菜单;
	afx_msg void OnRclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult);			//弹出菜单;

	DECLARE_MESSAGE_MAP()
private:
	MyList m_list;
	void ShowProcess();  //显示进程

	afx_msg void OnHook();
	afx_msg void OnOffHook();


	HANDLE TaskhProcess;
	HMODULE TestModule;
	
};
