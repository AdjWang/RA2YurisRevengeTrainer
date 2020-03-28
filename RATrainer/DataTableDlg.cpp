// DataTableDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RATrainer.h"
#include "DataTableDlg.h"
#include "afxdialogex.h"


// CDataTableDlg 对话框

IMPLEMENT_DYNAMIC(CDataTableDlg, CDialogEx)

CDataTableDlg::CDataTableDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataTableDlg::IDD, pParent)
{

}

CDataTableDlg::~CDataTableDlg()
{
}

void CDataTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDataTableDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDataTableDlg 消息处理程序


void CDataTableDlg::OnOK()
{}
void CDataTableDlg::OnCancel()
{}
void CDataTableDlg::OnClose()	//重载WM_CLOSE消息响应函数
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	EndDialog(IDCANCEL);    //关闭窗口
	CDialogEx::OnClose();
}

//重载消息函数使编辑框接收回车修改
BOOL CDataTableDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			HWND hwnd = ::GetFocus();
			int iID = ::GetDlgCtrlID(hwnd);
//test
						CString str;	//test
						str.Format("%d",iID);	//test
						::MessageBox(NULL,str,"提示:",NULL);	//test

//			if (iID == IDC_EDIT1)	//编辑框接受回车修改
//				OnBnClickedButton1();
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
