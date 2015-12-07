// Robot ArmDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "AtomManipulator.h"
#include "AtomManipulatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COpenGLControl openGLControl;
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

	DDX_Control(pDX, IDC_EDIT_NumPts, m_edCtrl_NumPts);
	DDX_Control(pDX, IDC_EDIT_X, m_edCtrl_X);
	DDX_Control(pDX, IDC_EDIT_Y, m_edCtrl_Y);
	DDX_Control(pDX, IDC_EDIT_Z, m_edCtrl_Z);
	DDX_Control(pDX, IDC_EDIT_RX, m_edCtrl_RX);
	DDX_Control(pDX, IDC_EDIT_RY, m_edCtrl_RY);
	DDX_Control(pDX, IDC_EDIT_RZ, m_edCtrl_RZ);

	DDX_Check(pDX, IDC_CHECK_DrawGround, openGLControl.CDrawGround);
	DDX_Check(pDX, IDC_CHECK_DrawRealArm, openGLControl.DrawCurRArm);
	DDX_Check(pDX, IDC_CHECK2, openGLControl.checkObs);

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
	ON_BN_CLICKED(IDC_MRVIC, &CRobotArmDlg::OnBnClickedMrvic)
	ON_BN_CLICKED(IDC_CHECK2, &CRobotArmDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK_DrawRealArm, &CRobotArmDlg::OnBnClickedCheckDrawrealarm)
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

	Robot_Mode = AtomManipulatorInitialization();

	if (Robot_Mode)
		Motor_Action_Btn.EnableWindow(TRUE);

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

	if (Robot_Mode)
		SetDlgItemText(IDC_STATUS, "[Real-robot Mode] Atom is ready to GO.");
	else
		SetDlgItemText(IDC_STATUS, "[Simulation Mode] Atom is ready to GO.");

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
#if MOTOR
	openGLControl.MotorAction = Motor_Action_Btn.GetCheck();
	Motors.SetMotorAction(Motor_Action_Btn.GetCheck());
#endif
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

	//openGLControl.GOmanubb();

	//CString str;
	//str.Format("%.2f",openGLControl.StaticRX);
	//m_edCtrl_RX.SetWindowTextA(str);
	//str.Format("%.2f",openGLControl.StaticRY);
	//m_edCtrl_RY.SetWindowTextA(str);
	//str.Format("%.2f",openGLControl.StaticRZ);
	//m_edCtrl_RZ.SetWindowTextA(str);

	//reactiveControllerTJ.modified = true;
}

void CRobotArmDlg::OnBnClickedButton5() //RESET BUTTON
{
	//breakflag = true;
	//openGLControl.ResetRArm();

	//CString str;
	//str.Format("%.2f",openGLControl.rpX);
	//m_edCtrl_RX.SetWindowTextA(str);
	//str.Format("%.2f",openGLControl.rpY);
	//m_edCtrl_RY.SetWindowTextA(str);
	//str.Format("%.2f",openGLControl.rpZ);
	//m_edCtrl_RZ.SetWindowTextA(str);

	//vic.toStaticObs = false;
	//vic.toJointLimit = false;
	//vic.nullMode = NULL_SPACE_SELECTION_J;
	//vic.withReferencePoints = false;
	//vic.toKinect = false;
	//memcpy( vic.qref, vic.qr[4].data, 6 * sizeof(double));

	#if SAVE_DATA
	//if (vic.counter >= 20000)
	{
		mfile_rP.close();
		mfile_BL.close();
		mfile_Xerr.close();
		//mfile_rPdot.close();
		//mfile_Qdiag.close();
		cout<<"mfile closed!"<<endl;
		system("pause");
	}
#endif
}

void CRobotArmDlg::OnStnClickedStatus()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);	
}

void CRobotArmDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	//if (openGLControl.checkObs)
	//{
	//	vic.toStaticObs = true;
	//} 
	//else
	//{
	//	vic.toStaticObs = false;
	//}
}

//DWORD_PTR GetNumCPUs() 
//{
//	SYSTEM_INFO m_si = {0, };
//	GetSystemInfo(&m_si);
//	return (DWORD_PTR)m_si.dwNumberOfProcessors;
//}


void CRobotArmDlg::OnBnClickedMrvic()
{
	// TODO: Add your control notification handler code here
#if KINECT
	//if ( vic.toKinect)
	{	
		kinectSensor.Nui_Zero();
		kinectSensor.GetKinectSkeleton();
	}
#endif

	//cout<<"Num. of CPUs: "<<GetNumCPUs()<<endl;

//#if MOTOR
	//gSendingThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)commandThreadLoop,(void*)0,0,&gTIDSending);
	//gSendingThreadOpened = true;
	//SetPriorityClass( gSendingThread, REALTIME_PRIORITY_CLASS);
	//SetThreadPriority(gSendingThread,THREAD_PRIORITY_TIME_CRITICAL);
	//SetThreadAffinityMask(gSendingThread, 1 << 1);
//#endif

	//Sleep(1000);
	//mrvicThread = new boost::thread(&mrvicThreadControl);

//#if MRVIC
	//gArmControllerThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)mrvicThreadLoop,(void*)0,0,&gTIDArmController);
	//gArmControllerThreadOpened = true;
	//SetPriorityClass( gArmControllerThread, REALTIME_PRIORITY_CLASS);
	//SetThreadPriority(gArmControllerThread,THREAD_PRIORITY_TIME_CRITICAL);
	//SetThreadAffinityMask(gArmControllerThread, 1 << 1);
//#endif
}

//void mrvicThreadLoop()
//{
//	float tmp[6] = {0.};
//	double humanBody[60] = {0.};
//
//#if SAVE_DATA 
//	mfile_rP.open("C:/Users/LSY/Desktop/sim3/sim3_rP.txt", ios::out);
//	mfile_BL.open("C:/Users/LSY/Desktop/sim3/sim3_dist.txt", ios::out);
//	mfile_Xerr.open("C:/Users/LSY/Desktop/sim3/sim3_Xerr.txt", ios::out);
//	//mfile_rPdot.open("C:/Users/LSY/Desktop/sim3_qdd.txt", ios::out);
//	//mfile_Qdiag.open("C:/Users/LSY/Desktop/exp1_Qdiag.txt", ios::out);
//	vic.counter = 0;
//#endif
//
//	LARGE_INTEGER StartTime;
//	LARGE_INTEGER CurrentTime;
//	LARGE_INTEGER nFreq;
//	LARGE_INTEGER ElapsedMicroseconds;
//
//	QueryPerformanceFrequency(&nFreq); 
//	QueryPerformanceCounter(&StartTime);
//
//	while(1)
//	{
//		if( breakflag)
//			break;
//
//		QueryPerformanceCounter(&CurrentTime);
//		ElapsedMicroseconds.QuadPart = CurrentTime.QuadPart - StartTime.QuadPart;
//		ElapsedMicroseconds.QuadPart *= 1000000;
//		ElapsedMicroseconds.QuadPart /= nFreq.QuadPart;
//		//cout<<ElapsedMicroseconds.QuadPart<<endl;
//		if (ElapsedMicroseconds.QuadPart >= 50)
//		{
//			//cout<<ElapsedMicroseconds.QuadPart<<endl;
//			QueryPerformanceCounter(&StartTime);
//
//#if KINECT
//			if (kinect.bFoundSkeleton && vic.toKinect)
//			{
//				for ( int i = 0 ; i < 20 ; i++)
//				{
//					humanBody[3*i] = -1.9 + kinect.HumanBody[i].z;
//					humanBody[3*i+1] = 0.45 + kinect.HumanBody[i].x;
//					humanBody[3*i+2] = 2.05 + kinect.HumanBody[i].y;
//
//					//humanBody[3*i] = -0.42 + kinectSensor.HumanBody[i].z*sin(45.*PI/180.) + kinectSensor.HumanBody[i].x*sin(45.*PI/180.);
//					//humanBody[3*i+1] = 0.9 + kinectSensor.HumanBody[i].z*sin(-45.*PI/180.) + kinectSensor.HumanBody[i].x*sin(45.*PI/180.);
//					//humanBody[3*i+2] = 1.35 +kinectSensor.HumanBody[i].y;
//
//					//humanBody[3*i] = -1.33 + kinectSensor.HumanBody[i].z * cos(27.0*PI/180.0) + kinectSensor.HumanBody[i].y * sin(27.0*PI/180.0);
//					//humanBody[3*i+1] = kinectSensor.HumanBody[i].x;
//					//humanBody[3*i+2] = 1.84 - kinectSensor.HumanBody[i].z * sin(27.0*PI/180.0) + kinectSensor.HumanBody[i].y * cos(27.0*PI/180.0);
//				}
//
//				memcpy( vic.kinectData, humanBody, 60 * sizeof(double));
//
//				vic.humanDetect = true;
//			}
//			else
//				vic.humanDetect = false;
//#endif
//
//			vic.callMultiPriorityController( KineAll.CrdAll->data, 
//																					KineAll.ZAxisAll->data, 
//																					KineAll.MinLL_real,
//																					KineAll.MaxLL_real);
//
//			for (int i = 0 ; i < 6 ; i++)
//			{
//				KineAll.FK_LLeg->theta[i+1] = vic.outputJoints[i];
//				tmp[i] = vic.outputJoints[i]*180./PI;
//			}
//
//			memcpy( finalInput, tmp, 6*sizeof(float));
//
//			//if ( openGLControl.DrawCurRArm)
//			//{
//			//	//gRenderKineWorking = true;
//				//RArm.ResetTheta(finalInput);
//			//	//gRenderKineWorking = false;
//			//}
//
//			KineAll.FindFK();
//
//			openGLControl.pX = KineAll.CrdAll->data[18];
//			openGLControl.pY = KineAll.CrdAll->data[19];
//			openGLControl.pZ = KineAll.CrdAll->data[20];
//
//			KineAll.GetLegsCoords();
//			KineAll.ComputeEulerAng(KineAll.LLegRotM, KineAll.EuAngLL);
//
//			openGLControl.rpX = KineAll.EuAngLL[0]/PI*180.;
//			openGLControl.rpY = KineAll.EuAngLL[1]/PI*180.;
//			openGLControl.rpZ = KineAll.EuAngLL[2]/PI*180.;
//
//			//Sleep(1);
//		}
//	}
//
//}

