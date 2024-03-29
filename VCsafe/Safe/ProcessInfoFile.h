#pragma once


// ProcessInfoFile 对话框

class ProcessInfoFile : public CDialogEx
{
	DECLARE_DYNAMIC(ProcessInfoFile)

public:
	ProcessInfoFile(CString  Path, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ProcessInfoFile();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILEINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();                        //初始化

	afx_msg void OnBnClickedOk();                      //确定按钮
	DECLARE_MESSAGE_MAP()

private:

	CString m_StrFileName;    // 文件名
	CString m_StrType;        //文件类型
	CString m_StrPath;        //文件位置
	CString m_StrSize;        //文件大小
	CString m_StrCreatTime;   //文件创建时间
	CString m_StrViewTime;    //最后访问时间
	CString m_StrChangeTime;  //最后修改时间
	CString m_StrMD5;         //MD5
	BOOL m_bOnlyRead;         //只读
	BOOL m_bHide;             //隐藏

	void GetFileInfo();       //获取文件信息
public:
	
};
