
// RATrainerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RATrainer.h"
#include "RATrainerDlg.h"
#include "afxdialogex.h"
#include "trainerfunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CRATrainerDlg �Ի���



CRATrainerDlg::CRATrainerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRATrainerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRATrainerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//�������
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


// CRATrainerDlg ��Ϣ�������

BOOL CRATrainerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//�������ʼ��
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString("Ǯ");
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString("�����������");

	//����Slider��ʼ��
	m_Slider.SetRange(0,6);		//������Χ
	m_Slider.SetPos(5);
	Slider_Pos = 5;
	m_Slider.SetTicFreq(1);	//ÿ(n)����λһ���̶�

	//��Ϸ���м��
	SetTimer(3,1000,NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRATrainerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRATrainerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRATrainerDlg::OnOK()
{}
void CRATrainerDlg::OnCancel()
{}
void CRATrainerDlg::OnClose()	//����WM_CLOSE��Ϣ��Ӧ����
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	EndDialog(IDCANCEL);    //�رմ���
	CDialogEx::OnClose();
}

//������Ϣ����ʹ�༭����ջس��޸�
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
//			::MessageBox(NULL,str,"��ʾ:",NULL);
			if(iID == IDC_EDIT1)	//�༭����ܻس��޸�
				OnBnClickedButton1();
        }   
    }   
    return CDialog::PreTranslateMessage(pMsg);
}  

/**************************************************************************************/
//��λ����CheckBox�ؼ�
void CRATrainerDlg::ResetAllCheckBoxes()
{
	CWnd *pWnd = CWnd::GetWindow(GW_CHILD);		//�õ��ؼ�ָ�룬�Ӵ��ھ��ǿؼ�

	char cClassName[255]={0};
	while(pWnd != NULL)
	{
		::GetClassName(pWnd->GetSafeHwnd(),cClassName,255);		//�õ��ؼ�������
		//CString str;
		//str.Format(_T("%s"),cClassName);
		//::MessageBox(NULL,str,"test",NULL);
		if(strcmp(cClassName,"CheckBox"))
		{
			CButton *pBtn = (CButton*) pWnd;	//CheckBox�Ļ�����CButton
			pBtn->SetCheck(0);		//ֱ�ӹرգ��ڴ�������TrainerBase::virtualAE����
//				::MessageBox(NULL,"get","test",NULL);
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	((CButton*)GetDlgItem(IDC_CHECK15))->EnableWindow(FALSE);	//����ȫ�Ƽ�
	((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(FALSE);	//�����Զ�����
}

//��ʱ����Ϸ����״̬���
void CRATrainerDlg::Timer_IsGameRunning(BOOL IsGameRunning)
{
	if(IsGameRunning)		//��Ϸ������
	{
		::SetWindowText(AfxGetMainWnd()->m_hWnd,"��Ϸ������");
	}
	else//δ��⵽����
	{
		ResetAllCheckBoxes();	//��λ����CheckBox

		::SetWindowText(AfxGetMainWnd()->m_hWnd,"��Ϸδ����");
	}
}

/**************************************************************************************/
//�Ĵ�����������
//�����б�
void CRATrainerDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	char i[10];
	int iPos=((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();		//��ǰѡ�е���

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(-1);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	CString str;
	DWORD readmem=0;

	switch(iPos)
	{
	case 0:		//Ǯ
		if(!tfs.rMymoney(&readmem))
			::MessageBox(NULL,"��ȡʧ�ܣ���ȷ����Ϸ�Ѿ���ʼ","��ʾ:",NULL);
		break;

	case 1:		//�����������
		if(!tfs.rBuildMaxm(&readmem))
			::MessageBox(NULL,"��ȡʧ�ܣ���ȷ����Ϸ�Ѿ���ʼ","��ʾ:",NULL);
		break;

	default:
		break;
	}

	str.Format("%d",readmem);		//intתCString
	SetDlgItemText(IDC_EDIT1,str);

//	sprintf_s(i,"%d",iPos);
//	OutputDebugString(i);
}

//�޸�
void CRATrainerDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerFunctions tfs;
	CString str;
	DWORD writemem=0;

	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);return;}

	GetDlgItemText(IDC_EDIT1,str);
	writemem = _ttoi(str);

	int iPos=((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();		//��ǰѡ�е���
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

//���ٽ���
void CRATrainerDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);return;}
	tfs.QuickBuild();
}

//��Ϊ�������
void CRATrainerDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);return;}
	tfs.TobeGhost();
}

//����ʤ��
void CRATrainerDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);return;}
	tfs.WinImme();
}

//���޳���
void CRATrainerDlg::OnBnClickedCheck12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	if(iSwi == 1)	//ѡ�в���
	{
		SetTimer(1,250,NULL);	//ID,ʱ��(ms),�ص�����
	}
	else
	{
		KillTimer(1);
	}
}

//�����״�
void CRATrainerDlg::OnBnClickedCheck13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK13))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK13))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	if(iSwi == 1)	//ѡ�в���
	{
		SetTimer(2,300,NULL);	//ID,ʱ��(ms),�ص�����
	}
	else
	{
		KillTimer(2);
	}
}

//��ʱ��
void CRATrainerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
//�����߳�����
//��ͼȫ��
void CRATrainerDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	tfs.AllMap();
}

//���һ��һ���Ժ˵�
void CRATrainerDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	tfs.GetaNuclearBomb();
}

//ѡ�е�λ������
void CRATrainerDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	tfs.UpChose();
}

//ѡ�е�λ����--�����ͷɻ�����
void CRATrainerDlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	tfs.SpeedUpChose();
}

void CRATrainerDlg::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	tfs.MineChose();
}

//ɾ����λ
void CRATrainerDlg::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerFunctions tfs;
	TrainerBase tbe;
	if(!tbe.IsGameRunning()){::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);return;}
	DWORD Player;
	tbe.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	tfs.DeleteThis();
}

//���ݲ鿴����
void CRATrainerDlg::OnBnClickedButton11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDataTableDlg dlg;		//�´��ڵ���
	//���ɴ��ڣ��������ɵĴ����˳�����һ������ֵ
	INT_PTR nRet = dlg.DoModal();


}

/**************************************************************************************/
//����ע������
//���޵���
void CRATrainerDlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.NoLoad();
	}
	else
	{
		tfs.FreeNoLoad();
	}
}
//�з�ͣ��
void CRATrainerDlg::OnBnClickedCheck16()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK16))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK16))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.NoPower();
	}
	else
	{
		tfs.FreeNoPower();
	}
}

//���⽨��
void CRATrainerDlg::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
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

//�Զ�����
void CRATrainerDlg::OnBnClickedCheck3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.AutoRepair();	//��ҽ���
		tfs.AutoRepairStruc();	//��������
	}
	else
	{
		tfs.FreeAutoRepair();
		tfs.FreeAutoRepairStruc();
	}
}

//�����������
void CRATrainerDlg::OnBnClickedCheck4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
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

//ȫ���ҵ�-����
void CRATrainerDlg::OnBnClickedCheck5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.MineAttack();
	}
	else
	{
		tfs.FreeMineAttack();
	}
}

//ȫ���ҵ�-פ��EVA_StructureGarrisoned
void CRATrainerDlg::OnBnClickedCheck6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.MineBuildIn();
	}
	else
	{
		tfs.FreeMineBuildIn();
	}
}

//ȫ���ҵ�-ռ��EVA_BuildingCaptured
void CRATrainerDlg::OnBnClickedCheck7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.MineUnEngineer();
	}
	else
	{
		tfs.FreeMineUnEngineer();
	}
}

//˲�佨��
void CRATrainerDlg::OnBnClickedCheck8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.BuildImme();
		OnBnClickedButton2();	//����ս���Զ����ٽ���
	}
	else
	{
		tfs.FreeBuildImme();
	}
}

//�޵�
void CRATrainerDlg::OnBnClickedCheck9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;

	int iSwiU=((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck();		//�޵��Ƿ�ѡ��
	int iSwiS=((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();		//��ɱ�Ƿ�ѡ��
	
	tfs.UnbeatableSecKill_Pre();	//���ָ�벻���Զ��ͷ� ����й¶
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
	else if((iSwiU == 0) && (iSwiS == 0))		//ȫȡ����ԭ���� �ͷ��ڴ�
	{
		tfs.FreeUnbeatableSecKill();
	}

	if(iSwiU == 1)	//ѡ�в���
	{
		tfs.AntiChrono();	//�޵�ʱ����ʱ��
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

//��ɱ
void CRATrainerDlg::OnBnClickedCheck10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;

	int iSwiU=((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck();		//�޵��Ƿ�ѡ��
	int iSwiS=((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();		//��ɱ�Ƿ�ѡ��

	tfs.UnbeatableSecKill_Pre();	//���ָ�벻���Զ��ͷ� ����й¶
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
	else if((iSwiU == 0) && (iSwiS == 0))		//ȫȡ����ԭ���� �ͷ��ڴ�
	{
		tfs.FreeUnbeatableSecKill();
	}
}

//�з�ѪԽ��Խ��
void CRATrainerDlg::OnBnClickedCheck11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.UnRepair();
	}
	else
	{
		tfs.FreeUnRepair();
	}
}

//��ɾ������ѡ��
void CRATrainerDlg::OnBnClickedCheck14()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK14))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.UnRefreshCon();
		((CButton*)GetDlgItem(IDC_CHECK15))->EnableWindow(TRUE);	//����ȫ�Ƽ�
	}
	else
	{
		tfs.FreeUnRefreshCon();
		if(((CButton*)GetDlgItem(IDC_CHECK15))->GetCheck() == 1)
		{
			((CButton*)GetDlgItem(IDC_CHECK15))->SetCheck(0);
			OnBnClickedCheck15();		//���ȫ�Ƽ�������һ��ر�
		}
		((CButton*)GetDlgItem(IDC_CHECK15))->EnableWindow(FALSE);	//����ȫ�Ƽ�

	}
}

//ȫ�Ƽ�
void CRATrainerDlg::OnBnClickedCheck15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK15))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK15))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
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

//���ٹ���
void CRATrainerDlg::OnBnClickedCheck17()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK17))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK17))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.FastAttack();
//		::MessageBox(NULL,"�˹��ܹ���ǿ�󣬿��ܵ�����Ϸ�������������������ʹ�ã�","����:",NULL);	//2.3������Bug
		tfs.FastTurnBattery();
	}
	else
	{
		tfs.FreeFastAttack();
		tfs.FreeFastTurnBattery();
	}
}

//������װ
void CRATrainerDlg::OnBnClickedCheck18()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK18))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK18))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.FastReload();
	}
	else
	{
		tfs.FreeFastReload();
	}
}

//��ҩ����
void CRATrainerDlg::OnBnClickedCheck19()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK19))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK19))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.FullAmmunition();
	}
	else
	{
		tfs.FreeFullAmmunition();
	}
}

//Զ�̴��
void CRATrainerDlg::OnBnClickedCheck20()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK20))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK20))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.AllRangeAttack();
		((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(TRUE);	//�����Զ�����
	}
	else
	{
		tfs.FreeAllRangeAttack();
		if(((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck() == 1)
		{
			((CButton*)GetDlgItem(IDC_CHECK21))->SetCheck(0);
			OnBnClickedCheck21();		//����Զ�����������һ��ر�
		}
		((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(FALSE);	//�����Զ�����

	}
}

//Զ�̾���
void CRATrainerDlg::OnBnClickedCheck21()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK21))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.AllRangeAlert();
	}
	else
	{
		tfs.FreeAllRangeAlert();
	}
}

//����ת��
void CRATrainerDlg::OnBnClickedCheck22()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK22))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK22))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.FastTurnRound();
	}
	else
	{
		tfs.FreeFastTurnRound();
	}
}

//˲�䳬ʱ���ƶ��͹���
void CRATrainerDlg::OnBnClickedCheck23()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK23))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK23))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
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

//����
void CRATrainerDlg::OnBnClickedCheck24()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK24))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK24))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.AntiSpy();
	}
	else
	{
		tfs.FreeAntiSpy();
	}
}

//�ٶȿ���Slider
void CRATrainerDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	const CString str_Pos[7] = {"����","��","��","�е�","��","��","���"};
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD nPos = m_Slider.GetPos();	//��ȡ����λ��
	if(nPos != Slider_Pos)
	{
		Slider_Pos = nPos;		//��¼��һ��λ��
		//test
		//CString str;
		//str.Format(_T("%d"),nPos);
		//GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);
		GetDlgItem(IDC_STATIC)->SetWindowTextA(str_Pos[nPos]);
		TrainerFunctions tfs;
		tfs.wSpeed(6-nPos);		//��Ϸ���� ��6-0��
	}

	*pResult = 0;
}

//�������
void CRATrainerDlg::OnBnClickedCheck25()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK25))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK25))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.SpeedSet();
		m_Slider.EnableWindow(TRUE);
		tfs.rSpeed(&Slider_Pos);
		Slider_Pos = 6-Slider_Pos;		//��Ϸ���� ��6-0��
	}
	else
	{
		tfs.FreeSpeedSet();
		m_Slider.EnableWindow(FALSE);
	}
}

//̱���ѷ������
void CRATrainerDlg::OnBnClickedCheck26()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK26))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK26))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.DisableGAGAP();
	}
	else
	{
		tfs.FreeDisableGAGAP();
	}
}

//̱���з����е�λ
void CRATrainerDlg::OnBnClickedCheck27()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK27))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK27))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.DisableAll();
	}
	else
	{
		tfs.FreeDisableAll();
	}
}

//������
void CRATrainerDlg::OnBnClickedCheck28()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK28))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK28))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
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

//����Ь
void CRATrainerDlg::OnBnClickedCheck29()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK29))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK29))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
	{
		tfs.SoldierFlashMove();
	}
	else
	{
		tfs.FreeSoldierFlashMove();
	}
}

//�������������
void CRATrainerDlg::OnBnClickedCheck30()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK30))->GetCheck();		//��ǰ�Ƿ�ѡ��

	TrainerBase tbe;
	if(!tbe.IsGameRunning())
	{
		((CButton*)GetDlgItem(IDC_CHECK30))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	TrainerFunctions tfs;
	if(iSwi == 1)	//ѡ�в���
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
