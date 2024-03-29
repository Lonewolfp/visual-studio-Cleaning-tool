#pragma once
#include "MyList.h"
#include <vector>
using namespace std;

// PERelocation 对话框

class PERelocation : public CDialogEx
{
	DECLARE_DYNAMIC(PERelocation)

public:
	PERelocation(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNTHeader, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PERelocation();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RELOCATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:					                
	MyList m_ListSection;                         //区块列表
	MyList m_ListData;                            //块项目数据

	PIMAGE_DOS_HEADER m_pDos;			          //DOS头
	PIMAGE_NT_HEADERS32 m_pNTHeader;	          //NT头
	PIMAGE_DATA_DIRECTORY m_pDataDir;	          //目录表头
	PIMAGE_BASE_RELOCATION m_pReloc;	          //重定位表
	vector<PIMAGE_BASE_RELOCATION> m_vecPReloc;   //重定位区块

	//区块列表
	CString m_RelocOrdinal;			//序号
	CString m_SectionName;			//数据块位置
	CString m_RelocRVA;				//RVA
	CString m_NumberBlock;			//数据

	//区块项数据
	CString m_DataOrinal;			//序号
	CString m_Type;					//类型
	CString m_DataFO;				//FO
	CString m_DataRVA;				//RVA
	CString m_DataAdd;				//地址
	CString m_DataInfo;				//数据信息

	void GetRelocationInfo();						  //获取重定位信息
	void GetSectionName(DWORD dwRVA);				  //获取区块名
	void GetDataInfo(PIMAGE_BASE_RELOCATION pReloc);  //获取区块数据

public:
	virtual BOOL OnInitDialog();                                               //初始化函数
	afx_msg void OnLvnItemchangedListSection(NMHDR *pNMHDR, LRESULT *pResult); //响应重定位区块列表事件函数
};

//结构体
typedef struct MyStruct
{
	WORD offset : 12;	//重定位小偏移
	WORD type : 4;		//重定位类型
}TYPEOFFSET;