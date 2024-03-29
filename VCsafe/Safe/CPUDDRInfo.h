#pragma once


// CPUDDRInfo 对话框

class CPUDDRInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CPUDDRInfo)

public:
	CPUDDRInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPUDDRInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CPUDDR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CProgressCtrl m_ProCPU;		//CPU进度
	CProgressCtrl m_ProDDR;		//内存进度
	CString m_szCPU;			//CPU使用率
	CString m_szDDR;			//内存使用率
	CString m_szStatus;         //状态提醒

	virtual BOOL OnInitDialog();   //初始化函数

	BOOL CreateChildThreadCPU();  //创建监控CPU使用率的子线程
	void GetMemoryStatus();        //获取内存使用率
	
	MEMORYSTATUS memStatus;        //保存内存信息变量
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	void ClearMemory();						   //内存清理
	afx_msg void OnBnClickedButtonCleanddr();  //内存优化按键
	
};
