
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
	DDX_Control(pDX,IDC_SLIDER1,mSlider);
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
	mSlider.SetRange(0,6);		//������Χ
	mSlider.SetPos(5);
	SliderPos = 5;
	mSlider.SetTicFreq(1);	//ÿ(n)����λһ���̶�

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

	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(-1);
		return;
	}

	CString str;
	DWORD readmem=0;

	switch(iPos)
	{
	case 0:		//Ǯ
		if(!trainer.rMymoney(&readmem))
			::MessageBox(NULL,"��ȡʧ�ܣ���ȷ����Ϸ�Ѿ���ʼ","��ʾ:",NULL);
		break;

	case 1:		//�����������
		if(!trainer.rBuildMaxm(&readmem))
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
	CString str;
	DWORD writemem=0;

	if (!trainer.IsGameRunning(RUNNING_ALERT)) { return; }

	GetDlgItemText(IDC_EDIT1,str);
	writemem = _ttoi(str);

	int iPos=((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();		//��ǰѡ�е���
	switch(iPos)
	{
	case 0:
		trainer.wMymoney(writemem);
		break;

	case 1:
		trainer.wBuildMaxm(writemem);
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
	if (!trainer.IsGameRunning(RUNNING_ALERT)) { return; }
	trainer.QuickBuild();
}

//��Ϊ�������
void CRATrainerDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!trainer.IsGameRunning(RUNNING_ALERT)) { return; }
	trainer.TobeGhost();
}

//����ʤ��
void CRATrainerDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!trainer.IsGameRunning(RUNNING_ALERT)) { return; }
	trainer.WinImme();
}

//���޳���
void CRATrainerDlg::OnBnClickedCheck12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck();		//��ǰ�Ƿ�ѡ��

	if(!trainer.IsGameRunning(RUNNING_ALERT))
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
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK13))->SetCheck(0);
		::MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);
		return;
	}

	int iSwi=((CButton*)GetDlgItem(IDC_CHECK13))->GetCheck();		//��ǰ�Ƿ�ѡ��
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
	switch(nIDEvent)
	{
	case 1:
		trainer.SuperOn();
		break;
	case 2:
		trainer.RadarOn();
		break;
	case 3:
		Timer_IsGameRunning(trainer.IsGameRunning(RUNNING_SILENT));
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

/**************************************************************************************/
//�����߳�����
char* allMap = "\
[ENABLE]\n\
alloc(newm,128)\n\
createthread(newm)\n\
LdrInitializeThunk:\n\
DB 8B FF 55 8B EC\n\
newm:\n\
pushad\n\
mov edx,[00A83D4C]\n\
mov ecx,0087F7E8\n\
push edx\n\
call 00577D90\n\
popad\n\
ret\n\
[DISABLE]\n\
dealloc(newm)\n\
";
inline const char * const BoolToString(bool b)
{
	return b ? "true" : "false";
}
//��ͼȫ��
void CRATrainerDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// 
	if (!trainer.IsGameRunning(RUNNING_ALERT)) { return; }
	DWORD Player;
	trainer.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	// trainer.AllMap();
	bool ret = trainer.AutoAssemble(trainer.pid, allMap, 1);
	Sleep(300);
	trainer.AutoAssemble(trainer.pid, allMap, 0);
	// char* ver = trainer.Ver();
	//::MessageBox(NULL, BoolToString(ret), "��ʾ:", NULL);
}

//���һ��һ���Ժ˵�
void CRATrainerDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!trainer.IsGameRunning(RUNNING_ALERT)) { return; }
	DWORD Player;
	trainer.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	trainer.GetaNuclearBomb();
}

//ѡ�е�λ������
void CRATrainerDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!trainer.IsGameRunning(RUNNING_ALERT)) { return; }
	DWORD Player;
	trainer.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	trainer.UpChose();
}

//ѡ�е�λ����--�����ͷɻ�����
void CRATrainerDlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!trainer.IsGameRunning(RUNNING_ALERT)) { return; }
	DWORD Player;
	trainer.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	trainer.SpeedUpChose();
}

void CRATrainerDlg::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!trainer.IsGameRunning(RUNNING_ALERT)) { return; }
	DWORD Player;
	trainer.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	trainer.MineChose();
}

//ɾ����λ
void CRATrainerDlg::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!trainer.IsGameRunning(RUNNING_ALERT)) { return; }
	DWORD Player;
	trainer.readMemory(0x00A83D4C,&Player);
	if(Player==0){::MessageBox(NULL,"�����ȿ�ʼս��!!","��ʾ:",NULL);return;}
	trainer.DeleteThis();
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
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
		return;
	}
	trainer.NoLoad(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck());
}
//�з�ͣ��(�����У���ͣʹ��)
void CRATrainerDlg::OnBnClickedCheck16()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK16))->GetCheck();		//��ǰ�Ƿ�ѡ��
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK16))->SetCheck(0);
		return;
	}
	if(iSwi == 1)	//ѡ�в���
	{
		trainer.NoPower();
	}
}

//���⽨��
void CRATrainerDlg::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(0);
		return;
	}
	trainer.PutAsWill(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck());
}

//�Զ�����
void CRATrainerDlg::OnBnClickedCheck3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(0);
		return;
	}
	trainer.AutoRepair(((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck());	//��ҽ���
}

//�����������
void CRATrainerDlg::OnBnClickedCheck4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(0);
		return;
	}
	trainer.RevengeYuri(((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck());
}

//ȫ���ҵ�-����
void CRATrainerDlg::OnBnClickedCheck5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(0);
		return;
	}
	trainer.MineAttack(((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck());
}

//ȫ���ҵ�-פ��EVA_StructureGarrisoned
void CRATrainerDlg::OnBnClickedCheck6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(0);
		return;
	}
	trainer.MineBuildIn(((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck());
}

//ȫ���ҵ�-ռ��EVA_BuildingCaptured
void CRATrainerDlg::OnBnClickedCheck7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(0);
		return;
	}
	trainer.MineUnEngineer(((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck());
}

//˲�佨��
void CRATrainerDlg::OnBnClickedCheck8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(0);
		return;
	}
	trainer.BuildImme(((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck());
	OnBnClickedButton2();	//����ս���Զ����ٽ���
}

//�޵�
void CRATrainerDlg::OnBnClickedCheck9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(0);
		return;
	}
	trainer.Unbeatable(((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck());
	trainer.AntiChrono(((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck());	//�޵�ʱ����ʱ��
	trainer.AntiChronoDisable(((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck());

}

//��ɱ
void CRATrainerDlg::OnBnClickedCheck10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(0);
		return;
	}
	//��ͣʹ��
}

//�з�ѪԽ��Խ��
void CRATrainerDlg::OnBnClickedCheck11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(0);
		return;
	}
	trainer.UnRepair(((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck());
}

//����ģʽ
void CRATrainerDlg::OnBnClickedCheck14()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK14))->SetCheck(0);
		return;
	}

	int iSwi=((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck();		//��ǰ�Ƿ�ѡ��
	trainer.InvadeMode(iSwi);
}

//ȫ�Ƽ�
void CRATrainerDlg::OnBnClickedCheck15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK15))->SetCheck(0);
		return;
	}
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK15))->GetCheck();		//��ǰ�Ƿ�ѡ��
	trainer.UnlockTech(iSwi);
}

//���ٹ���
void CRATrainerDlg::OnBnClickedCheck17()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK17))->SetCheck(0);
		return;
	}
	trainer.FastAttack(((CButton*)GetDlgItem(IDC_CHECK17))->GetCheck());
//	::MessageBox(NULL,"�˹��ܹ���ǿ�󣬿��ܵ�����Ϸ�������������������ʹ�ã�","����:",NULL);	//2.3������Bug
	trainer.FastTurnBattery(((CButton*)GetDlgItem(IDC_CHECK17))->GetCheck());
}

//������װ
void CRATrainerDlg::OnBnClickedCheck18()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK18))->SetCheck(0);
		return;
	}
	trainer.FastReload(((CButton*)GetDlgItem(IDC_CHECK18))->GetCheck());
}

//��ҩ����
void CRATrainerDlg::OnBnClickedCheck19()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK19))->SetCheck(0);
		return;
	}
	trainer.FullAmmunition(((CButton*)GetDlgItem(IDC_CHECK19))->GetCheck());
}

//Զ�̴��
void CRATrainerDlg::OnBnClickedCheck20()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK20))->SetCheck(0);
		return;
	}

	int iSwi=((CButton*)GetDlgItem(IDC_CHECK20))->GetCheck();		//��ǰ�Ƿ�ѡ��
	trainer.AllRangeAttack(iSwi);
	if(iSwi == 1)	//ѡ�в���
	{
		((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(TRUE);	//�����Զ�����
	}
	else
	{
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
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK21))->SetCheck(0);
		return;
	}
	trainer.AllRangeAlert(((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck());
}

//����ת��
void CRATrainerDlg::OnBnClickedCheck22()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK22))->SetCheck(0);
		return;
	}
	trainer.FastTurnRound(((CButton*)GetDlgItem(IDC_CHECK22))->GetCheck());
}

//˲�䳬ʱ���ƶ��͹���
void CRATrainerDlg::OnBnClickedCheck23()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK23))->SetCheck(0);
		return;
	}
	trainer.InstantChronoMove(((CButton*)GetDlgItem(IDC_CHECK23))->GetCheck());
	trainer.InstantChronoAttack(((CButton*)GetDlgItem(IDC_CHECK23))->GetCheck());
}

//����
void CRATrainerDlg::OnBnClickedCheck24()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK24))->SetCheck(0);
		return;
	}
	trainer.AntiSpy(((CButton*)GetDlgItem(IDC_CHECK24))->GetCheck());
}

//�ٶȿ���Slider
void CRATrainerDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	const CString strPos[7] = {"����","��","��","�е�","��","��","���"};
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD nPos = mSlider.GetPos();	//��ȡ����λ��
	if(nPos != SliderPos)
	{
		SliderPos = nPos;		//��¼��һ��λ��
		//test
		//CString str;
		//str.Format(_T("%d"),nPos);
		//GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);
		GetDlgItem(IDC_STATIC)->SetWindowTextA(strPos[nPos]);
		
		trainer.wSpeed(6-nPos);		//��Ϸ���� ��6-0��
	}

	*pResult = 0;
}

//�������
void CRATrainerDlg::OnBnClickedCheck25()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	const CString strPos[7] = {"����","��","��","�е�","��","��","���"};
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK25))->SetCheck(0);
		return;
	}
	
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK25))->GetCheck();		//��ǰ�Ƿ�ѡ��
	trainer.SpeedSet(iSwi);
	if(iSwi == 1)	//ѡ�в���
	{
		mSlider.EnableWindow(TRUE);
		trainer.rSpeed(&SliderPos);		//��ȡ��Ϸ�ٶ� SliderPos��������
		SliderPos = 6 - SliderPos;		//��6-0��
		GetDlgItem(IDC_STATIC)->SetWindowTextA(strPos[SliderPos]);
		mSlider.SetPos(SliderPos);		//���»�����ʾ
	}
	else
	{
		mSlider.EnableWindow(FALSE);
	}
}

//̱���ѷ������
void CRATrainerDlg::OnBnClickedCheck26()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK26))->SetCheck(0);
		return;
	}
	trainer.DisableGAGAP(((CButton*)GetDlgItem(IDC_CHECK26))->GetCheck());
}

//̱���з����е�λ
void CRATrainerDlg::OnBnClickedCheck27()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK27))->SetCheck(0);
		return;
	}
	trainer.DisableAll(((CButton*)GetDlgItem(IDC_CHECK27))->GetCheck());
}

//������
void CRATrainerDlg::OnBnClickedCheck28()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK28))->SetCheck(0);
		return;
	}
	int iSwi=((CButton*)GetDlgItem(IDC_CHECK28))->GetCheck();		//��ǰ�Ƿ�ѡ��
	trainer.EnableSoldAll_Cursor(iSwi);
	trainer.EnableSoldAll_Belong(iSwi);
	trainer.EnableSoldAll_Builder(iSwi);
}

//����Ь
void CRATrainerDlg::OnBnClickedCheck29()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK29))->SetCheck(0);
		return;
	}
	trainer.SoldierFlashMove(((CButton*)GetDlgItem(IDC_CHECK29))->GetCheck());
}

//�������������
void CRATrainerDlg::OnBnClickedCheck30()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!trainer.IsGameRunning(RUNNING_ALERT))
	{
		((CButton*)GetDlgItem(IDC_CHECK30))->SetCheck(0);
		return;
	}
	trainer.AllElite(((CButton*)GetDlgItem(IDC_CHECK30))->GetCheck());
}
