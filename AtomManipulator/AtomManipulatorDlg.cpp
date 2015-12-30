// Robot ArmDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "AtomManipulator.h"
#include "AtomManipulatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRobotArmDlg *pMainDlg;

// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CRobotArmDlg 對話方塊
CRobotArmDlg::CRobotArmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRobotArmDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRobotArmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK3, DrawEE_Axis);
	DDX_Control(pDX, IDC_CHECK_Motor  , Motor_Action_Btn);
	DDX_Control(pDX, IDC_ROBOT_MODE  , Robot_Mode_Btn);

	DDX_Control(pDX, IDC_EDIT_NumPts, m_edCtrl_NumPts);
	DDX_Control(pDX, IDC_EDIT_X, m_edCtrl_X);
	DDX_Control(pDX, IDC_EDIT_Y, m_edCtrl_Y);
	DDX_Control(pDX, IDC_EDIT_Z, m_edCtrl_Z);
	DDX_Control(pDX, IDC_EDIT_RX, m_edCtrl_RX);
	DDX_Control(pDX, IDC_EDIT_RY, m_edCtrl_RY);
	DDX_Control(pDX, IDC_EDIT_RZ, m_edCtrl_RZ);

	DDX_Check(pDX, IDC_CHECK_DrawGround, openGLControl.CDrawGround);
	DDX_Check(pDX, IDC_CHECK_DrawRealArm, openGLControl.DrawCurRArm);
	DDX_Check(pDX, IDC_ROBOT_MODE, Robot_Mode);

	DDX_Text(pDX, IDC_EDIT_X, openGLControl.StaticX);
	DDX_Text(pDX, IDC_EDIT_Y, openGLControl.StaticY);
	DDX_Text(pDX, IDC_EDIT_Z, openGLControl.StaticZ);
	DDX_Text(pDX, IDC_EDIT_RX, openGLControl.StaticRX);
	DDX_Text(pDX, IDC_EDIT_RY, openGLControl.StaticRY);
	DDX_Text(pDX, IDC_EDIT_RZ, openGLControl.StaticRZ);
	DDX_Text(pDX, IDC_EDIT_NumPts, openGLControl.NumPts);
}

BEGIN_MESSAGE_MAP(CRobotArmDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON5, &CRobotArmDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK3, &CRobotArmDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK_DrawGround, &CRobotArmDlg::OnBnClickedCheckDrawground)
	ON_BN_CLICKED(IDC_CHECK_Motor, &CRobotArmDlg::OnBnClickedCheckMotor)
	ON_BN_CLICKED(IDC_BUTTON11, &CRobotArmDlg::OnBnClickedButton11)
	ON_EN_CHANGE(IDC_EDIT_NumPts, &CRobotArmDlg::OnEnChangeEditNumpts)
	ON_EN_CHANGE(IDC_EDIT_X, &CRobotArmDlg::OnEnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, &CRobotArmDlg::OnEnChangeEditY)
	ON_EN_CHANGE(IDC_EDIT_Z, &CRobotArmDlg::OnEnChangeEditZ)
	ON_EN_CHANGE(IDC_EDIT_RX, &CRobotArmDlg::OnEnChangeEditRx)
	ON_EN_CHANGE(IDC_EDIT_RY, &CRobotArmDlg::OnEnChangeEditRy)
	ON_EN_CHANGE(IDC_EDIT_RZ, &CRobotArmDlg::OnEnChangeEditRz)
	ON_STN_CLICKED(IDC_STATUS, &CRobotArmDlg::OnStnClickedStatus)
	ON_BN_CLICKED(IDC_CHECK_DrawRealArm, &CRobotArmDlg::OnBnClickedCheckDrawrealarm)
	ON_BN_CLICKED(IDC_ROBOT_MODE, &CRobotArmDlg::OnBnClickedRobotMode)
END_MESSAGE_MAP()


// CRobotArmDlg 訊息處理常式

BOOL CRobotArmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	CRect rect;
	rect.left   = 5;
	rect.right  = 725;
	rect.top    = 5;
	rect.bottom = 645;
	//GetDlgItem(IDC_Draw_Scene)->GetWindowRect(rect);
	openGLControl.Create(rect,this);
	openGLControl.Dwidth = rect.right  - rect.left;
	openGLControl.Dhigh  = rect.bottom - rect.top;

	CheckDrawGround = &openGLControl.CDrawGround;

	AtomManipulatorInitialization();

	m_edCtrl_NumPts.SetWindowTextA("100");
	m_edCtrl_X.SetWindowTextA("0.5");
	m_edCtrl_Y.SetWindowTextA("0.");
	m_edCtrl_Z.SetWindowTextA("1.55");
	m_edCtrl_RX.SetWindowTextA("0.0");
	m_edCtrl_RY.SetWindowTextA("0.0");
	m_edCtrl_RZ.SetWindowTextA("0.0");

	pMainDlg = this;

	CString str;
	str.Format("%.2f",openGLControl.rpX);
	m_edCtrl_RX.SetWindowTextA(str);
	str.Format("%.2f",openGLControl.rpY);
	m_edCtrl_RY.SetWindowTextA(str);
	str.Format("%.2f",openGLControl.rpZ);
	m_edCtrl_RZ.SetWindowTextA(str);

	SetDlgItemText(IDC_STATUS, "[Simulation Mode] Atom is ready to roll.");

	// TODO: 在此加入額外的初始設定
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CRobotArmDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CRobotArmDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CRobotArmDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRobotArmDlg::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	openGLControl.EE_Axis = DrawEE_Axis.GetCheck();
}

void CRobotArmDlg::OnBnClickedCheckDrawground()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CRobotArmDlg::OnBnClickedCheckDrawrealarm()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	
	if (openGLControl.DrawCurRArm)
		DrawEE_Axis.EnableWindow(TRUE);
	else
		DrawEE_Axis.EnableWindow(FALSE);
}

void CRobotArmDlg::OnBnClickedCheckMotor()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);	
	openGLControl.MotorAction = Motor_Action_Btn.GetCheck();
	
	std::shared_ptr<motor::MotorController> atom_motors 
		= motor::MotorControlFactory::GetInstance().GetCurrentMotorController();
	atom_motors->SetMotorAction(Motor_Action_Btn.GetCheck());

	if (openGLControl.MotorAction)
	{
		Robot_Mode_Btn.EnableWindow(FALSE);
		SetDlgItemText(IDC_STATUS, "[Real-robot Mode] Atom starts rolling.");
	}
	else
	{
		Robot_Mode_Btn.EnableWindow(TRUE);
		SetDlgItemText(IDC_STATUS, "[Real-robot Mode] Atom stops rolling.");
	}
}

void CRobotArmDlg::OnEnChangeEditNumpts()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);	
}

void CRobotArmDlg::OnEnChangeEditX()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);	

	CString str;
	str.Format("%.2f",openGLControl.StaticRX);
	m_edCtrl_RX.SetWindowTextA(str);
	str.Format("%.2f",openGLControl.StaticRY);
	m_edCtrl_RY.SetWindowTextA(str);
	str.Format("%.2f",openGLControl.StaticRZ);
	m_edCtrl_RZ.SetWindowTextA(str);
}

void CRobotArmDlg::OnEnChangeEditY()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);	

	CString str;
	str.Format("%.2f",openGLControl.StaticRX);
	m_edCtrl_RX.SetWindowTextA(str);
	str.Format("%.2f",openGLControl.StaticRY);
	m_edCtrl_RY.SetWindowTextA(str);
	str.Format("%.2f",openGLControl.StaticRZ);
	m_edCtrl_RZ.SetWindowTextA(str);
}

void CRobotArmDlg::OnEnChangeEditZ()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);	

	CString str;
	str.Format("%.2f",openGLControl.StaticRX);
	m_edCtrl_RX.SetWindowTextA(str);
	str.Format("%.2f",openGLControl.StaticRY);
	m_edCtrl_RY.SetWindowTextA(str);
	str.Format("%.2f",openGLControl.StaticRZ);
	m_edCtrl_RZ.SetWindowTextA(str);
}

void CRobotArmDlg::OnEnChangeEditRx()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
		UpdateData(true);	
}

void CRobotArmDlg::OnEnChangeEditRy()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
		UpdateData(true);	
}

void CRobotArmDlg::OnEnChangeEditRz()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
		UpdateData(true);	
}

void CRobotArmDlg::OnBnClickedButton11()
{
	// TODO: Add your control notification handler code here
	if (!Robot_Mode)
		Robot_Mode_Btn.EnableWindow(FALSE);

	std::shared_ptr<motion::MotionGenerator> atom_motion
		= motion::MotionGenerationFactory::GetInstance().GetCurrentMotionGenerator();

	double p_r [3] = {openGLControl.StaticX, openGLControl.StaticY, openGLControl.StaticZ};// m
	double o_r [9] = {0};

	double rdegx = openGLControl.StaticRX/180.0*PI;
	double rdegy = openGLControl.StaticRY/180.0*PI;
	double rdegz = openGLControl.StaticRZ/180.0*PI;
	o_r[0] = cos(rdegy)*cos(rdegz);
	o_r[1] = -cos(rdegx)*sin(rdegz)+sin(rdegx)*sin(rdegy)*cos(rdegz);
	o_r[2] = sin(rdegx)*sin(rdegz)+cos(rdegx)*sin(rdegy)*cos(rdegz);
	o_r[3] = cos(rdegy)*sin(rdegz);
	o_r[4] = cos(rdegx)*cos(rdegz)+sin(rdegx)*sin(rdegy)*sin(rdegz);
	o_r[5] = -sin(rdegx)*cos(rdegz)+cos(rdegx)*sin(rdegy)*sin(rdegz);
	o_r[6] = -sin(rdegy);
	o_r[7] = sin(rdegx)*cos(rdegy);
	o_r[8] = cos(rdegx)*cos(rdegy);

	atom_motion->set_o_r(o_r);
	atom_motion->set_p_r(p_r);

	atom_motion->set_BK_p(0.5, 1);
	atom_motion->set_BK_o(0.5, 1);
	atom_motion->set_BK_q(1, 5);

	atom_motion->set_ik_control_mode(true);

	if (!atom_motion->isThreadCreated())
		atom_motion->Launch();
}

void CRobotArmDlg::OnBnClickedButton5() //RESET BUTTON
{
	std::shared_ptr<motion::MotionGenerator> atom_motion
		= motion::MotionGenerationFactory::GetInstance().GetCurrentMotionGenerator();

	double q_r[ROBOT_DOF] = {0};
	atom_motion->set_q_r(q_r);

	atom_motion->set_ik_control_mode(false);

	if (!Robot_Mode)
		Robot_Mode_Btn.EnableWindow(TRUE);
}

void CRobotArmDlg::OnStnClickedStatus()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);	
}

void CRobotArmDlg::OnBnClickedRobotMode()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	 
	if (Robot_Mode) {
		motor::gConfig atom_motor_config;
		atom_motor_config._DOF_ = ROBOT_DOF;
		atom_motor_config._expected_baud_rate_ = 115200;
		atom_motor_config._sleep_time_micro_sec_ = 6000;
		std::shared_ptr<motor::MotorController> atom_motors = 
			motor::MotorControlFactory::GetInstance()
			.NewMotorController(motor::FAULHABER, atom_motor_config);

		SetDlgItemText(IDC_STATUS, "[Real-robot Mode] Connecting....");

		atom_motors->Launch();

		if (atom_motors->isThreadCreated()) 
		{
			Motor_Action_Btn.EnableWindow(TRUE);
			SetDlgItemText(IDC_STATUS, "[Real-robot Mode] Atom is ready to roll.");
		} else {
			motor::MotorControlFactory::GetInstance().DeleteCurrentMotorController();
			Robot_Mode = 0;
			Robot_Mode_Btn.SetCheck(0);
			SetDlgItemText(IDC_STATUS, "[Real-robot Mode] Fail to connect motors..");
			Sleep(1000);
			SetDlgItemText(IDC_STATUS, "[Simulation Mode] Atom is ready to roll.");
		}
	} else {
		motor::MotorControlFactory::GetInstance().DeleteCurrentMotorController();

		Motor_Action_Btn.EnableWindow(FALSE);
		SetDlgItemText(IDC_STATUS, "[Simulation Mode] Atom is ready to roll.");
	}
}
