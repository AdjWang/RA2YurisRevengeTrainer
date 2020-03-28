#pragma once


// CDataTableDlg 对话框

class CDataTableDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataTableDlg)

public:
	CDataTableDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataTableDlg();

// 对话框数据
	enum { IDD = IDD_DATATABLE_DIALOG };

protected:
	BOOL CDataTableDlg::PreTranslateMessage(MSG* pMsg);	//重载消息函数使编辑框接收回车修改

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};
