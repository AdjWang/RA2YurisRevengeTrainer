#pragma once


// CDataTableDlg �Ի���

class CDataTableDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataTableDlg)

public:
	CDataTableDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataTableDlg();

// �Ի�������
	enum { IDD = IDD_DATATABLE_DIALOG };

protected:
	BOOL CDataTableDlg::PreTranslateMessage(MSG* pMsg);	//������Ϣ����ʹ�༭����ջس��޸�

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};
