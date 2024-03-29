#pragma once


// Tool 对话框

class Tool : public CDialogEx
{
	DECLARE_DYNAMIC(Tool)

public:
	Tool(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Tool();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();      //进入PE查看器
	afx_msg void OnBnClickedButtonCpu();    //查看CPU、内存使用状态
	afx_msg void OnBnClickedButtonClean();  //清理垃圾按钮
};
