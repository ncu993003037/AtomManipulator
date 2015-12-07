// Robot ArmDlg.h : 標頭檔
//

#pragma once
#include "OpenGL_Model\OpenGLControl.h"

#include "afxcmn.h"
#include "afxwin.h"

#include <time.h>


// CRobotArmDlg 對話方塊
class CRobotArmDlg : public CDialog
{
// 建構
public:
	CRobotArmDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_ROBOTARM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援

	//ExactTimer rTimer2;

	CEdit m_edCtrl_NumPts;
	CEdit m_edCtrl_X;
	CEdit m_edCtrl_Y;
	CEdit m_edCtrl_Z;
	CEdit m_edCtrl_RX;
	CEdit m_edCtrl_RY;
	CEdit m_edCtrl_RZ;

	CEdit m_edCtrl_Fx;
	CEdit m_edCtrl_Fy;
	CEdit m_edCtrl_Fz;
	CEdit m_edCtrl_Mx;
	CEdit m_edCtrl_My;
	CEdit m_edCtrl_Mz;


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//CButton ButtonCamStandby;

	//COpenGLControl openGLControl;
	
public:
	//afx_msg void OnBnClickedButton1();
public:
	//afx_msg void OnBnClickedButton2();
public:
	//afx_msg void OnBnClickedButton3();
public:
	//afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedButton5();
public:
	//afx_msg void OnBnClickedmobs();
public:
	CButton DrawEE_Axis;
	CButton Motor_Action_Btn;
public:
	afx_msg void OnBnClickedCheck3();
public:
	//MyTabCtrl mTabCtrl;	
public:
	BOOL *CheckDrawGround;
public:
	afx_msg void OnBnClickedCheckDrawground();
public:
	afx_msg void OnBnClickedCheckMotor();
public:

	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnEnChangeEditNumpts();
	afx_msg void OnEnChangeEditX();
	afx_msg void OnEnChangeEditY();
	afx_msg void OnEnChangeEditZ();
	afx_msg void OnEnChangeEditRx();
	afx_msg void OnEnChangeEditRy();
	afx_msg void OnEnChangeEditRz();

	//void ResetRobotArm();

	afx_msg void OnStnClickedStatus();

	afx_msg void OnBnClickedMrvic();

	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheckDrawrealarm();
};
