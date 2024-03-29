#pragma once


// PETime 对话框

class PETime : public CDialogEx
{
	DECLARE_DYNAMIC(PETime)

public:
	PETime(DWORD TimeDateStamp, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PETime();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TIME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_TimeStamp;         //时间标志
	CString m_TimeData;          //北京时间

	DWORD   m_dwTimeDateStamp;	//时间戳

	void TimeStamp2TimeData(DWORD TimeDateStamp);		//将时间标志转换北京时间
public:
	afx_msg void OnBnClickedButtonTime();      //控件按钮，开始转换
	virtual BOOL OnInitDialog();
};
