#pragma once
#include "MyList.h"
#include <vector>

// Software 对话框

//自定义软件信息结构体
typedef struct _SOFTINFO
{
	WCHAR szSoftName[MAX_PATH];
	WCHAR szSoftPub[100];
	WCHAR szSoftDate[20];
	WCHAR szSoftSize[20];
	WCHAR szSoftVer[30];
	WCHAR szSoftInsPath[MAX_PATH];
	WCHAR szSoftUniPath[MAX_PATH];

}SOFTINFO, *PSOFTINFO;


class Software : public CDialogEx
{
	DECLARE_DYNAMIC(Software)

public:
	Software(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Software();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SOFTWARE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnMenuSoftMG(UINT uID);										//响应菜单
	afx_msg void OnNMRClickListSoftware(NMHDR *pNMHDR, LRESULT *pResult);		//弹出菜单
	afx_msg void OnLvnColumnclickListSoftware(NMHDR *pNMHDR, LRESULT *pResult);	//列表列头排序
	DECLARE_MESSAGE_MAP()
private:
	MyList m_ListSoftware;				  //列表

	HKEY m_hRootKey;					  //主键
	CString m_SubKey1;					  //子键1
	CString m_SubKey2;					  //子键2

	CString m_KeyName;					  //键名
	CString m_KeyNameVer;				  //版本
	CString m_KeyNameDate;				  //日期
	CString m_KeyNameSize;				  //大小
	CString m_KeyNamePub;				  //厂家
	CString m_KeyNameInsPath;			  //安装路径
	CString m_KeyNameUniPath;			  //卸载路径

	CString m_SoftName;					  //显示名称
	CString m_SoftPub;					  //作者
	CString m_SoftDate;					  //日期
	CString m_SoftSize;					  //尺寸
	CString m_SoftVer;					  //版本
	CString m_SoftInsPath;				  //安装路径
	CString m_SoftUniPath;				  //卸载路径

	std::vector<SOFTINFO> m_vecSoftInfo;	//存储信息
	void GetSoftwareInfo(CString SubKey);	//获取软件信息
	void UpdateSoftInfo();					//刷新
	void OpenExploer();						//打开文件夹
	void UnistallSoft();					//卸载软件

};
