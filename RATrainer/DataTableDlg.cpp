// DataTableDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RATrainer.h"
#include "DataTableDlg.h"
#include "afxdialogex.h"


// CDataTableDlg �Ի���

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


// CDataTableDlg ��Ϣ�������


void CDataTableDlg::OnOK()
{}
void CDataTableDlg::OnCancel()
{}
void CDataTableDlg::OnClose()	//����WM_CLOSE��Ϣ��Ӧ����
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	EndDialog(IDCANCEL);    //�رմ���
	CDialogEx::OnClose();
}

//������Ϣ����ʹ�༭����ջس��޸�
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
						::MessageBox(NULL,str,"��ʾ:",NULL);	//test

//			if (iID == IDC_EDIT1)	//�༭����ܻس��޸�
//				OnBnClickedButton1();
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
