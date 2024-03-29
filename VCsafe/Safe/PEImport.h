#pragma once
#include "MyList.h"

// PEImport 对话框

class PEImport : public CDialogEx
{
	DECLARE_DYNAMIC(PEImport)

public:
	PEImport(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNTHeader, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PEImport();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IMPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	MyList m_ListDll;       //DLL模块列表
	MyList m_ListFun;       //函数名列表

	PIMAGE_DOS_HEADER m_pDos;				//DOS头
	PIMAGE_NT_HEADERS32 m_pNTHeader;		//NT头
	PIMAGE_DATA_DIRECTORY m_pDataDir;		//数据目录地址
	PIMAGE_THUNK_DATA32 m_pThunk;			//输入名称表与输入地址表数据类型

	CString m_DllName;						//DLL名
	CString m_NameRVA;						//导入文件的名字RVA
	CString m_INT;							//输入名称表
	CString m_IAT;							//输入地址表

	CString m_FunOrdinal;					//函数序号
	CString m_FunName;						//函数名称

	void GetDllInfo();						//获取DLL信息
	void GetFunInfo();						//获取函数信息
public:
	virtual BOOL OnInitDialog();            //初始化
	afx_msg void OnLvnItemchangedListDll(NMHDR *pNMHDR, LRESULT *pResult);  //DLL列表事件函数
};
