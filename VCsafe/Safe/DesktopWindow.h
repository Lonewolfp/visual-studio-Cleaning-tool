#pragma once
#include "MyList.h"

// DesktopWindow 对话框

class DesktopWindow : public CDialogEx
{
	DECLARE_DYNAMIC(DesktopWindow)

public:
	DesktopWindow(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DesktopWindow();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DESWINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();					                   //初始化函数

	afx_msg void OnRclickListWindow(NMHDR *pNMHDR, LRESULT *pResult);  //右键菜单函数
	afx_msg void OnPopMenu(UINT id);								   //响应鼠标右键菜单
	DECLARE_MESSAGE_MAP()

private:
	MyList m_WindowList;								//窗口列表变量
	void DestktopWindow();			                    //添加窗口
public:													
	
	static BOOL CALLBACK DestktopWindowProc(HWND hWnd, LPARAM lParam); //枚举窗口回调函数
};


//窗口结果体
typedef struct _MYWINDOWINFO
{
	TCHAR hWnd[MAX_PATH + 1];
	TCHAR WindowName[MAX_PATH + 1];
	TCHAR ClassName[MAX_PATH + 1];
}MYWINDOWINFO, *PMYWINDOWINFO;
