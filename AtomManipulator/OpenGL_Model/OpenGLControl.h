#if !defined(AFX_OPENGLCONTROL_H__52A6B63B_01A2_449D_8691_1FF59EECAB71__INCLUDED_)
#define AFX_OPENGLCONTROL_H__52A6B63B_01A2_449D_8691_1FF59EECAB71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGLControl.h : Header

// Rendering
#include "DH_Model.h"
#include "OpenGLDevice.h"

#include "GL/GL.h"
#include "GL/GLU.h"
#include "GL/GLUT.H"
#include "GL/GLAUX.H"

#include "atltypes.h"
#include <vector>
#include <fstream>

struct Dimension { float *Pos; };

class COpenGLControl : public CWnd
{
public:
	COpenGLControl();

public:
	void Create(CRect rect,CWnd* parent);
	virtual ~COpenGLControl();	

public:
	void InitGL();
	void DrawGLScene();
	OpenGLDevice openGLDevice;
	CClientDC* dc;
	float rotation;

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnTimer(UINT nIDEvent);
	bool Set3DViewPort(void);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);	
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);	
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
public:
	//--------------------使用按鍵的參數--------------------------------
	bool r_mouse_button;// 看滑鼠右鍵是否被按住
	bool m_mouse_button;
	bool l_mouse_button;
	CPoint r_mouse_ini_pos; //按右鍵滑鼠座標起始位置
	CPoint m_mouse_ini_pos; //按中建滑鼠座標起始位置
	CPoint l_mouse_ini_pos;
	CPoint L_Button_Pos;
	double l_mouse_Pos[3];
	//--------------------宣告場景轉換的參數-------------------------
	float rot_m[2];
	float trans_m[2];
	float scale_m;
	float move_sensitivity;

	bool PerspectiveView;	

	bool MovingOBS;
	int  CDrawGround;

    void Draw_RArm();
	void Draw_LArm();
	void Draw_grid();
	void Draw_RE1();
	void Draw_Desk();

	void Draw_Axis(float LINK_LENGTH);
	bool EE_Axis;

	//===============貼圖用=====================
	void DrawGround(void);
	bool LoadSkyboxTextures(void);
	GLuint texture[2];
private:
	AUX_RGBImageRec* LoadBMP(char* Filename);

public:
	//======右手Motion的Parameter======

	int  Rcount;

	//====Dialog間的互傳=====
	CDialog   *subDialog[3];

	//--------Load OpenGL----------
	GLint		RE1;
	GLint       Desk;
	GLint		GL_RArmJ[10];
	GLint       GL_LArmJ[10];

	//-----------------------------

	//--------Camera--------------
	int Dwidth; //視窗的寬度
	int Dhigh;  //視窗的高度

	GLdouble eye[3];
	GLdouble center[3];
	GLdouble up[3];
	double ModelView [16];
	double Projection[16];
	int    ViewPort[4];
	
	//====Motor Control====
	int MotorAction;

	int DrawCurRArm;
	//------------------------------------------------------------------


	double TrajInput[3];

	float pX, pY, pZ, rpX, rpY, rpZ;
	bool IKbreak;
	float dx, dy, dz;

	float StaticX, StaticY, StaticZ, StaticRX, StaticRY, StaticRZ;
	float degx, degy, degz, rdegx, rdegy, rdegz;
	int NumPts;

	ArmKinematic RArm;
};

#endif // AFX_OPENGLCONTROL_H__52A6B63B_01A2_449D_8691_1FF59EECAB71__INCLUDED_
