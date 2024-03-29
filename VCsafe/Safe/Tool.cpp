// Tool.cpp: 实现文件
//

#include "stdafx.h"
#include "Safe.h"
#include "Tool.h"
#include "afxdialogex.h"
#include"PEInFo.h"
#include "CPUDDRInfo.h"
#include "Clean.h"


// Tool 对话框

IMPLEMENT_DYNAMIC(Tool, CDialogEx)

Tool::Tool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TAB2, pParent)
{

}

Tool::~Tool()
{
}

void Tool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Tool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Tool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_CPU, &Tool::OnBnClickedButtonCpu)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, &Tool::OnBnClickedButtonClean)
END_MESSAGE_MAP()


// Tool 消息处理程序


//进入PE查看器
void Tool::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	PEInFo*pPE = new PEInFo;
	//pPE->DoModal();   //模态会话框

	//非模态对话框
	pPE->Create(IDD_DIALOG_PEINFO);
	pPE->ShowWindow(SW_NORMAL);
}

//查看CPU、内存使用状态CPUDDRInfo
void Tool::OnBnClickedButtonCpu()
{
	// TODO: 在此添加控件通知处理程序代码.

	//非模态对话框
	CPUDDRInfo*pCPU = new CPUDDRInfo;
	pCPU->Create(IDD_DIALOG_CPUDDR);
	pCPU->ShowWindow(SW_NORMAL);

}

//清理垃圾按钮
void Tool::OnBnClickedButtonClean()
{
	// TODO: 在此添加控件通知处理程序代码
	Clean *pClean = new Clean;
	pClean->DoModal();
}
