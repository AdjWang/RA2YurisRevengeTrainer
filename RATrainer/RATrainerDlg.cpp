
// RATrainerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RATrainer.h"
#include "RATrainerDlg.h"
#include "afxdialogex.h"
#include "trainerfunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRATrainerDlg 对话框



CRATrainerDlg::CRATrainerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRATrainerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRATrainerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//滑块关联
	DDX_Control(pDX,IDC_SLIDER1,m_Slider);
}

BEGIN_MESSAGE_MAP(CRATrainerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRATrainerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRATrainerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRATrainerDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CRATrainerDlg::OnBnClickedButton4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CRATrainerDlg::OnCbnSelchangeCombo1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON5, &CRATrainerDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CRATrainerDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CRATrainerDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CRATrainerDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CRATrainerDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CRATrainerDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CRATrainerDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_CHECK1, &CRATrainerDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CRATrainerDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CRATrainerDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CRATrainerDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CRATrainerDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CRATrainerDlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &CRATrainerDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, &CRATrainerDlg::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK9, &CRATrainerDlg::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK10, &CRATrainerDlg::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK11, &CRATrainerDlg::OnBnClickedCheck11)
	ON_BN_CLICKED(IDC_CHECK12, &CRATrainerDlg::OnBnClickedCheck12)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK13, &CRATrainerDlg::OnBnClickedCheck13)
	ON_BN_CLICKED(IDC_CHECK14, &CRATrainerDlg::OnBnClickedCheck14)
	ON_BN_CLICKED(IDC_CHECK15, &CRATrainerDlg::OnBnClickedCheck15)
	ON_BN_CLICKED(IDC_CHECK16, &CRATrainerDlg::OnBnClickedCheck16)
	ON_BN_CLICKED(IDC_CHECK17, &CRATrainerDlg::OnBnClickedCheck17)
	ON_BN_CLICKED(IDC_CHECK18, &CRATrainerDlg::OnBnClickedCheck18)
	ON_BN_CLICKED(IDC_CHECK19, &CRATrainerDlg::OnBnClickedCheck19)
	ON_BN_CLICKED(IDC_CHECK20, &CRATrainerDlg::OnBnClickedCheck20)
	ON_BN_CLICKED(IDC_CHECK21, &CRATrainerDlg::OnBnClickedCheck21)
	ON_BN_CLICKED(IDC_CHECK22, &CRATrainerDlg::OnBnClickedCheck22)
	ON_BN_CLICKED(IDC_CHECK23, &CRATrainerDlg::OnBnClickedCheck23)
	ON_BN_CLICKED(IDC_CHECK24, &CRATrainerDlg::OnBnClickedCheck24)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CRATrainerDlg::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_CHECK25, &CRATrainerDlg::OnBnClickedCheck25)
	ON_BN_CLICKED(IDC_CHECK26, &CRATrainerDlg::OnBnClickedCheck26)
	ON_BN_CLICKED(IDC_CHECK27, &CRATrainerDlg::OnBnClickedCheck27)
	ON_BN_CLICKED(IDC_CHECK28, &CRATrainerDlg::OnBnClickedCheck28)
	ON_BN_CLICKED(IDC_CHECK29, &CRATrainerDlg::OnBnClickedCheck29)
	ON_BN_CLICKED(IDC_CHECK30, &CRATrainerDlg::OnBnClickedCheck30)
END_MESSAGE_MAP()


// CRATrainerDlg 消息处理程序

BOOL CRATrainerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//下拉框初始化
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString("钱");
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString("建造队列上限");

	//调速Slider初始化
	m_Slider.SetRange(0,6);		//滑动范围
	m_Slider.SetPos(5);
	Slider_Pos = 5;
	m_Slider.SetTicFreq(1);	//每(n)个单位一个刻度

	//游戏运行检测
	SetTimer(3,1000,NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRATrainerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRATrainerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRATrainerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRATrainerDlg::OnOK()
{}
void CRATrainerDlg::OnCancel()
{}
void CRATrainerDlg::OnClose()	//重载WM_CLOSE消息响应函数
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	EndDialog(IDCANCEL);    //关闭窗口
	CDialogEx::OnClose();
}

//重载消息函数使编辑框接收回车修改
BOOL CRATrainerDlg::PreTranslateMessage(MSG* pMsg)		
{   
	if (WM_KEYFIRST <= pMsg-> message && pMsg-> message <= WM_KEYLAST)   
    {  
		if(pMsg-> wParam==VK_RETURN)   
        {  
			HWND hwnd = ::GetFocus();
			int iID = ::GetDlgCtrlID(hwnd);
//			CString str;
//			str.Format("%d",iID);
//			::MessageBox(NULL,str,"提示:",NULL);
			if(iID == IDC_EDIT1)	//编辑框接受回车修改
				OnBnClickedButton1();
        }   
    }   
    return CDialog::PreTranslateMessage(pMsg);
}  

/**************************************************************************************/
//复位所有CheckBox控件
void CRATrainerDlg::ResetAllCheckBoxes()
{
	CWnd *pWnd = CWnd::GetWindow(GW_CHILD);		//得到控件指针，子窗口就是控件

	char cClassName[255]={0};
	while(pWnd != NULL)
	{
		::GetClassName(pWnd->GetSafeHwnd(),cClassName,255);		//得到控件的类名
		//CString str;
		//str.Format(_T("%s"),cClassName);
		//::MessageBox(NULL,str,"test",NULL);
		if(strcmp(cClassName,"CheckBox"))
		{
			CButton *pBtn = (CButton*) pWnd;	//CheckBox的基类是CButton
			pBtn->SetCheck(0);		//直接关闭，内存问题由TrainerBase::virtualAE处理
//				::MessageBox(NULL,"get","test",NULL);
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	((CButton*)GetDlgItem(IDC_CHECK15))->EnableWindow(FALSE);	//禁用全科技
	((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(FALSE);	//禁用自动攻击
}

//定时器游戏运行状态检测
void CRATrainerDlg::Timer_IsGameRunning(BOOL IsGameRunning)
{
	if(IsGameRunning)		//游戏运行中
	{
		::SetWindowText(AfxGetMainWnd()->m_hWnd,"游戏运行中");
	}
	else//未检测到运行
	{
		ResetAllCheckBoxes();	//复位所有CheckBox

		::SetWindowText(AfxGetMainWnd()->m_hWnd,"游戏未运行");
	}
}

/**************************************************************************************/
//寄存器操作类型
//下拉列表
void CRATrainerDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
//	char i[10];
	int iPos=((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();		//当前选中的行

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(-1);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	CString str;
	DWORD readmem=0;

	switch(iPos)
	{
	case 0:		//钱
		if(!tfs.rMymoney(&readmem))
			::MessageBox(NULL,"读取失败，请确认游戏已经开始","提示:",NULL);
		break;

	case 1:		//建造队列上限
		if(!tfs.rBuildMaxm(&readmem))
			::MessageBox(NULL,"读取失败，请确认游戏已经开始","提示:",NULL);
		break;

	default:
		break;
	}

	str.Format("%d",readmem);		//int转CString
	SetDlgItemText(IDC_EDIT1,str);

//	sprintf_s(i,"%d",iPos);
//	OutputDebugString(i);
}

//修改
void CRATrainerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerFunctions tfs;
	CString str;
	DWORD writemem=0;

	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"请开启游戏!!","提示:",NULL);return;}

	GetDlgItemText(IDC_EDIT1,str);
	writemem = _ttoi(str);

	int iPos=((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();		//当前选中的行
	switch(iPos)
	{
	case 0:
		tfs.wMymoney(writemem);
		break;

	case 1:
		tfs.wBuildMaxm(writemem);
		break;
		
	default:
		break;
	}
//	OutputDebugString(sMoney);
}

//快速建造
void CRATrainerDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"请开启游戏!!","提示:",NULL);return;}
	tfs.QuickBuild();
}

//成为幽灵玩家
void CRATrainerDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"请开启游戏!!","提示:",NULL);return;}
	tfs.TobeGhost();
}

//立即胜利
void CRATrainerDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"请开启游戏!!","提示:",NULL);return;}
	tfs.WinImme();
}

//无限超武
void CRATrainerDlg::OnBnClickedCheck12()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	if(iSwi == 1)	//选中操作
	{
		SetTimer(1,250,NULL);	//ID,时间(ms),回调函数
	}
	else
	{
		KillTimer(1);
	}
}

//永久雷达
void CRATrainerDlg::OnBnClickedCheck13()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK13))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK13))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	if(iSwi == 1)	//选中操作
	{
		SetTimer(2,300,NULL);	//ID,时间(ms),回调函数
	}
	else
	{
		KillTimer(2);
	}
}

//定时器
void CRATrainerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TrainerFunctions tfs;
	TrainerBase tb;
	switch(nIDEvent)
	{
	case 1:
		tfs.SuperOn();
		break;
	case 2:
		tfs.RadarOn();
		break;
	case 3:
		Timer_IsGameRunning(tb.IsGameRunning());
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

/**************************************************************************************/
//创建线程类型
//地图全开
void CRATrainerDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"请开启游戏!!","提示:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"必须先开始战斗!!","提示:",NULL);return;}
	tfs.AllMap();
}

//获得一个一次性核弹
void CRATrainerDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"请开启游戏!!","提示:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"必须先开始战斗!!","提示:",NULL);return;}
	tfs.GetaNuclearBomb();
}

//选中单位升三级
void CRATrainerDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"请开启游戏!!","提示:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"必须先开始战斗!!","提示:",NULL);return;}
	tfs.UpChose();
}

//选中单位加速--建筑和飞机不行
void CRATrainerDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"请开启游戏!!","提示:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"必须先开始战斗!!","提示:",NULL);return;}
	tfs.SpeedUpChose();
}

void CRATrainerDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"请开启游戏!!","提示:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"必须先开始战斗!!","提示:",NULL);return;}
	tfs.MineChose();
}

//删除单位
void CRATrainerDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"请开启游戏!!","提示:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"必须先开始战斗!!","提示:",NULL);return;}
	tfs.DeleteThis();
}

//数据查看窗口
void CRATrainerDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataTableDlg dlg;		//新窗口的类
	//生成窗口，若新生成的窗口退出会有一个返回值
	INT_PTR nRet = dlg.DoModal();


}

/**************************************************************************************/
//代码注入类型
//无限电力
void CRATrainerDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.NoLoad();
	}
	else
	{
		tfs.FreeNoLoad();
	}
}
//敌方停电
void CRATrainerDlg::OnBnClickedCheck16()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK16))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK16))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.NoPower();
	}
	else
	{
		tfs.FreeNoPower();
	}
}

//随意建筑
void CRATrainerDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.PutAsWill();
		tfs.PutWaterAble();
	}
	else
	{
		tfs.FreePutAsWill();
		tfs.FreePutWaterAble();
	}
}

//自动修理
void CRATrainerDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.AutoRepair();	//玩家建筑
		tfs.AutoRepairStruc();	//中立建筑
	}
	else
	{
		tfs.FreeAutoRepair();
		tfs.FreeAutoRepairStruc();
	}
}

//社会主义万岁
void CRATrainerDlg::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.RevengeYuri();
		tfs.PermanentYuri();
	}
	else
	{
		tfs.FreeRevengeYuri();
		tfs.FreePermanentYuri();
	}
}

//全是我的-攻击
void CRATrainerDlg::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.MineAttack();
	}
	else
	{
		tfs.FreeMineAttack();
	}
}

//全是我的-驻军EVA_StructureGarrisoned
void CRATrainerDlg::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.MineBuildIn();
	}
	else
	{
		tfs.FreeMineBuildIn();
	}
}

//全是我的-占领EVA_BuildingCaptured
void CRATrainerDlg::OnBnClickedCheck7()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.MineUnEngineer();
	}
	else
	{
		tfs.FreeMineUnEngineer();
	}
}

//瞬间建造
void CRATrainerDlg::OnBnClickedCheck8()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.BuildImme();
		OnBnClickedButton2();	//步兵战车自动快速建造
	}
	else
	{
		tfs.FreeBuildImme();
	}
}

//无敌
void CRATrainerDlg::OnBnClickedCheck9()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;

	int iSwiU=((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck();		//无敌是否选中
	int iSwiS=((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();		//秒杀是否选中
	
	tfs.UnbeatableSecKill_Pre();	//如果指针不空自动释放 不会泄露
	if((iSwiU == 1) && (iSwiS == 0))
	{
		tfs.Unbeatable();
	}
	else if((iSwiU == 0) && (iSwiS == 1))
	{
		tfs.SecKill();
	}
	else if((iSwiU == 1) && (iSwiS == 1))
	{
		tfs.UnbeatableSecKill();
	}
	else if((iSwiU == 0) && (iSwiS == 0))		//全取消则还原代码 释放内存
	{
		tfs.FreeUnbeatableSecKill();
	}

	if(iSwiU == 1)	//选中操作
	{
		tfs.AntiChrono();	//无敌时反超时空
		tfs.AntiChronoDisbuild();
		tfs.AntiChronoDisable();
	}
	else
	{
		tfs.FreeAntiChrono();
		tfs.FreeAntiChronoDisbuild();
		tfs.FreeAntiChronoDisable();
	}

}

//秒杀
void CRATrainerDlg::OnBnClickedCheck10()
{
	// TODO: 在此添加控件通知处理程序代码
	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;

	int iSwiU=((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck();		//无敌是否选中
	int iSwiS=((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();		//秒杀是否选中

	tfs.UnbeatableSecKill_Pre();	//如果指针不空自动释放 不会泄露
	if((iSwiU == 1) && (iSwiS == 0))
	{
		tfs.Unbeatable();
	}
	else if((iSwiU == 0) && (iSwiS == 1))
	{
		tfs.SecKill();
	}
	else if((iSwiU == 1) && (iSwiS == 1))
	{
		tfs.UnbeatableSecKill();
	}
	else if((iSwiU == 0) && (iSwiS == 0))		//全取消则还原代码 释放内存
	{
		tfs.FreeUnbeatableSecKill();
	}
}

//敌方血越修越少
void CRATrainerDlg::OnBnClickedCheck11()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.UnRepair();
	}
	else
	{
		tfs.FreeUnRepair();
	}
}

//不删除建造选项
void CRATrainerDlg::OnBnClickedCheck14()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK14))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.UnRefreshCon();
		((CButton*)GetDlgItem(IDC_CHECK15))->EnableWindow(TRUE);	//启用全科技
	}
	else
	{
		tfs.FreeUnRefreshCon();
		if(((CButton*)GetDlgItem(IDC_CHECK15))->GetCheck() == 1)
		{
			((CButton*)GetDlgItem(IDC_CHECK15))->SetCheck(0);
			OnBnClickedCheck15();		//如果全科技开启就一起关闭
		}
		((CButton*)GetDlgItem(IDC_CHECK15))->EnableWindow(FALSE);	//禁用全科技

	}
}

//全科技
void CRATrainerDlg::OnBnClickedCheck15()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK15))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK15))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.AllBuild();
		tfs.AllCarBoat();
		tfs.AllSoldier();
		tfs.AllPlane();
	}
	else
	{
		tfs.FreeAllBuild();
		tfs.FreeAllCarBoat();
		tfs.FreeAllSoldier();
		tfs.FreeAllPlane();
	}
}

//极速攻击
void CRATrainerDlg::OnBnClickedCheck17()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK17))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK17))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.FastAttack();
//		::MessageBox(NULL,"此功能过于强大，可能导致游戏卡顿甚至卡死，请谨慎使用！","警告:",NULL);	//2.3版解决了Bug
		tfs.FastTurnBattery();
	}
	else
	{
		tfs.FreeFastAttack();
		tfs.FreeFastTurnBattery();
	}
}

//闪电重装
void CRATrainerDlg::OnBnClickedCheck18()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK18))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK18))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.FastReload();
	}
	else
	{
		tfs.FreeFastReload();
	}
}

//弹药充足
void CRATrainerDlg::OnBnClickedCheck19()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK19))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK19))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.FullAmmunition();
	}
	else
	{
		tfs.FreeFullAmmunition();
	}
}

//远程打击
void CRATrainerDlg::OnBnClickedCheck20()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK20))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK20))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.AllRangeAttack();
		((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(TRUE);	//启用自动攻击
	}
	else
	{
		tfs.FreeAllRangeAttack();
		if(((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck() == 1)
		{
			((CButton*)GetDlgItem(IDC_CHECK21))->SetCheck(0);
			OnBnClickedCheck21();		//如果自动攻击开启就一起关闭
		}
		((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(FALSE);	//禁用自动攻击

	}
}

//远程警戒
void CRATrainerDlg::OnBnClickedCheck21()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK21))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.AllRangeAlert();
	}
	else
	{
		tfs.FreeAllRangeAlert();
	}
}

//极速转身
void CRATrainerDlg::OnBnClickedCheck22()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK22))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK22))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.FastTurnRound();
	}
	else
	{
		tfs.FreeFastTurnRound();
	}
}

//瞬间超时空移动和攻击
void CRATrainerDlg::OnBnClickedCheck23()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK23))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK23))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.InstantChronoMove();
		tfs.InstantChronoAttack();
	}
	else
	{
		tfs.FreeInstantChronoMove();
		tfs.FreeInstantChronoAttack();
	}
}

//间间谍
void CRATrainerDlg::OnBnClickedCheck24()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK24))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK24))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.AntiSpy();
	}
	else
	{
		tfs.FreeAntiSpy();
	}
}

//速度控制Slider
void CRATrainerDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	const CString str_Pos[7] = {"最慢","慢","慢","中等","快","快","最快"};
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	DWORD nPos = m_Slider.GetPos();	//获取滑块位置
	if(nPos != Slider_Pos)
	{
		Slider_Pos = nPos;		//记录上一次位置
		//test
		//CString str;
		//str.Format(_T("%d"),nPos);
		//GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);
		GetDlgItem(IDC_STATIC)->SetWindowTextA(str_Pos[nPos]);
		TrainerFunctions tfs;
		tfs.wSpeed(6-nPos);		//游戏数据 慢6-0快
	}

	*pResult = 0;
}

//任务调速
void CRATrainerDlg::OnBnClickedCheck25()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK25))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK25))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.SpeedSet();
		m_Slider.EnableWindow(TRUE);
		tfs.rSpeed(&Slider_Pos);
		Slider_Pos = 6-Slider_Pos;		//游戏数据 慢6-0快
	}
	else
	{
		tfs.FreeSpeedSet();
		m_Slider.EnableWindow(FALSE);
	}
}

//瘫痪裂缝产生器
void CRATrainerDlg::OnBnClickedCheck26()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK26))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK26))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.DisableGAGAP();
	}
	else
	{
		tfs.FreeDisableGAGAP();
	}
}

//瘫痪敌方所有单位
void CRATrainerDlg::OnBnClickedCheck27()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK27))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK27))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.DisableAll();
	}
	else
	{
		tfs.FreeDisableAll();
	}
}

//卖卖卖
void CRATrainerDlg::OnBnClickedCheck28()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK28))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK28))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.EnableSoldAll_Cursor();
		tfs.EnableSoldAll_Belong();
		tfs.EnableSoldAll_Builder();
	}
	else
	{
		tfs.FreeEnableSoldAll_Cursor();
		tfs.FreeEnableSoldAll_Belong();
		tfs.FreeEnableSoldAll_Builder();
	}
}

//滑板鞋
void CRATrainerDlg::OnBnClickedCheck29()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK29))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK29))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.SoldierFlashMove();
	}
	else
	{
		tfs.FreeSoldierFlashMove();
	}
}

//造出来就是三级
void CRATrainerDlg::OnBnClickedCheck30()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK30))->GetCheck();		//当前是否选中

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK30))->SetCheck(0);
		::MessageBox(NULL,"请开启游戏!!","提示:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//选中操作
	{
		tfs.EliteBuild_Aircraft();
		tfs.EliteBuild_Vehicle();
		tfs.EliteBuild_Soldier();
	}
	else
	{
		tfs.FreeEliteBuild_Aircraft();
		tfs.FreeEliteBuild_Vehicle();
		tfs.FreeEliteBuild_Soldier();
	}
}
