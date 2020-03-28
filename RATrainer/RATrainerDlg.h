
// RATrainerDlg.h : 头文件
//

#pragma once
#include "DataTableDlg.h"
#include "trainerfunctions.h"

// CRATrainerDlg 对话框
class CRATrainerDlg : public CDialogEx
{
// 构造
public:
	CRATrainerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RATRAINER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();

//软件功能
	void ResetAllCheckBoxes();
	void Timer_IsGameRunning(BOOL IsGameRunning);	//游戏运行状态检测

// 修改功能
protected:
	HICON m_hIcon;
	TrainerFunctions trainer;	// 修改器功能全局对象
	CSliderCtrl mSlider;	//调速Slider对象
	DWORD SliderPos;		//Slider位置记录防止多余触发

	BOOL CRATrainerDlg::PreTranslateMessage(MSG* pMsg);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedCheck10();
	afx_msg void OnBnClickedCheck11();
	afx_msg void OnBnClickedCheck12();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheck13();
	afx_msg void OnBnClickedCheck14();
	afx_msg void OnBnClickedCheck15();
	afx_msg void OnBnClickedCheck16();
	afx_msg void OnBnClickedCheck17();
	afx_msg void OnBnClickedCheck18();
	afx_msg void OnBnClickedCheck19();
	afx_msg void OnBnClickedCheck20();
	afx_msg void OnBnClickedCheck21();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedCheck22();
	afx_msg void OnBnClickedCheck23();
	afx_msg void OnBnClickedCheck24();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck25();
	afx_msg void OnBnClickedCheck26();
	afx_msg void OnBnClickedCheck27();
	afx_msg void OnBnClickedCheck28();
	afx_msg void OnBnClickedCheck29();
	afx_msg void OnBnClickedCheck30();
};
