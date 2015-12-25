#include "stdafx.h"
#include "OpenGLControl.h"
#include "math.h"

//===========include OpenGl Model=============
#include "Rendering/RE1_5601.h"
#include "Rendering/BASE.h"

#include "Rendering/J0.h"
#include "Rendering/J1.h"
#include "Rendering/J2.h"
#include "Rendering/J3.h"
#include "Rendering/J4.h"
#include "Rendering/J5.h"
#include "Rendering/J6.h"
#include "Rendering/J7.h"
#include "Rendering/J8.h"

#include "Rendering/desk.h"

#include "MotionGeneration/KineChain/KineChain.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLControl

COpenGLControl::COpenGLControl()
: r_mouse_button(false)
, m_mouse_button(false)
, l_mouse_button(false)
, r_mouse_ini_pos(0)
, m_mouse_ini_pos(0)
, L_Button_Pos(0)
, EE_Axis(false)
, CDrawGround(false)
, PerspectiveView(true)
, MotorAction(0)
, DrawCurRArm(false)
, RArm("OpenGL_Model\\DHParameter.txt")
{
	dc = NULL;
	rotation = 0.0f;

	for(int i = 0 ; i < 3 ; i++)
		l_mouse_Pos[i] = 0;
}

COpenGLControl::~COpenGLControl()
{
	if (dc)
	{
		delete dc;
	}
}


BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SYSKEYDOWN()
	END_MESSAGE_MAP()


void COpenGLControl::InitGL()
{	
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);							
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

//--------------Initial Camera----------------------------
	eye[0]    = -2;	eye[1]    = 0;	eye[2]    = 1.5;
	center[0] = 0;	center[1] = 0;	center[2] = 0;
	up[0]     = 0;  up[1]     = 0;	up[2]     = 1;

//---------------Load Arm and RE1 OpenGL Model------------------
	RE1 = BASEModel::Gen3DObjectList();
	Desk = DeskModel::Gen3DObjectList();


	GL_RArmJ[0] = J0::Gen3DObjectList();
	GL_RArmJ[1] = J1::Gen3DObjectList();
	GL_RArmJ[2] = J2::Gen3DObjectList();
	GL_RArmJ[3] = J3::Gen3DObjectList();
	GL_RArmJ[4] = J4::Gen3DObjectList();
	GL_RArmJ[5] = J5::Gen3DObjectList();
	GL_RArmJ[6] = J6::Gen3DObjectList();
	GL_RArmJ[7] = J7::Gen3DObjectList();
	GL_RArmJ[8] = J8::Gen3DObjectList();

	LoadSkyboxTextures();

	//--------initial 場景轉換的參數--------------------------------------------------
	rot_m [0] = 0 ;
	rot_m [1] = 0 ;
	trans_m[0] = 0;
	trans_m[1] = 1;
	scale_m = 3.0;
	move_sensitivity = 1.0;

	// setup lighting
	//===================================================
	GLfloat glfLightAmbient[]  = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat glfLightDiffuse[]  = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat glfLightSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat glfLightPosition[] = { 0,0,15,1.0f};

    // Add a light to the scene.   //
    glLightfv (GL_LIGHT0, GL_AMBIENT, glfLightAmbient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, glfLightDiffuse);
    glLightfv (GL_LIGHT0, GL_SPECULAR, glfLightSpecular);
	glLightfv (GL_LIGHT0, GL_POSITION,glfLightPosition);
	
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);

	// Spot light setting
	GLfloat glfLightDiffuse1[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat glfLightSpecular1[] = { 0.3f, 0.3f, 0.3f, 1.0f };

	GLfloat glfLightPosition1[] = { 0.0f, 0.0f, 10.0f, 1.0f };
	//GLfloat glSpotDirection1[]  = {-0.95f,-1.8f,-4.2f};

	glLightfv(GL_LIGHT1, GL_DIFFUSE ,glfLightDiffuse1 );
	glLightfv(GL_LIGHT1, GL_POSITION,glfLightPosition1);
    glLightfv(GL_LIGHT1, GL_SPECULAR,glfLightSpecular1);

	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,glSpotDirection1);
	glLightf (GL_LIGHT1, GL_SPOT_CUTOFF   , 80.0);
	glLightf (GL_LIGHT1, GL_SPOT_EXPONENT ,128.0);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT1);


    //
    // Enable depth testing and backface culling.
    //

    glEnable (GL_DEPTH_TEST);    
    glDisable(GL_CULL_FACE);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT ,glfLightAmbient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE ,1.0f);

	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	SetTimer(0,1,NULL);//MFC Timer (GL thread with delay time 1ms)
}

void GLCubeWLH( float Width, float Length, float Height)
{
	// width, length, height are arranged in x, y, z directions

	if( Width == 0 || Length == 0 || Height == 0 ) {
		return; // draw nothing
	}

	//glEnable( GL_BLEND ); // required for showing alpha
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); // required for showing alpha
	//glAlphaFunc( GL_GREATER, 0.01 );
	//glEnable( GL_ALPHA_TEST );

	// When rendering polygon, please render counter-clockwise for correct display!!!
	// button face
	glColor4f( 0.1, 0.3, 1.0, 1);

	glBegin( GL_POLYGON );
	glNormal3f( 0.0, 0.0, -1.0 );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 0.0, Length, 0.0 );
	glVertex3f( Width, Length, 0.0 );
	glVertex3f( Width, 0.0, 0.0 );
	glEnd();
	// side face 1
	glBegin( GL_POLYGON );
	glNormal3f( 0.0, -1.0, 0.0 );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( Width, 0.0, 0.0 );
	glVertex3f( Width, 0.0, Height );
	glVertex3f( 0.0, 0.0, Height );
	glEnd();
	// side face 2
	glBegin( GL_POLYGON );
	glNormal3f( 1.0, 0.0, 0.0 );
	glVertex3f( Width, 0.0, 0.0 );
	glVertex3f( Width, Length, 0.0 );
	glVertex3f( Width, Length, Height );
	glVertex3f( Width, 0.0, Height );
	glEnd();
	// side face 3
	glBegin( GL_POLYGON );
	glNormal3f( 0.0, 1.0, 0.0 );
	glVertex3f( 0.0, Length, 0.0 );
	glVertex3f( 0.0, Length, Height );
	glVertex3f( Width, Length, Height );
	glVertex3f( Width, Length, 0.0 );
	glEnd();
	// side face 4
	glBegin( GL_POLYGON );
	glNormal3f( -1.0, 0.0, 0.0 );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 0.0, 0.0, Height );
	glVertex3f( 0.0, Length, Height );
	glVertex3f( 0.0, Length, 0.0 );
	glEnd();
	// top face 
	glBegin( GL_POLYGON );
	glNormal3f( 0.0, 0.0, 1.0 );
	glVertex3f( 0.0, 0.0, Height );
	glVertex3f( Width, 0.0, Height );
	glVertex3f( Width, Length, Height );
	glVertex3f( 0.0, Length, Height );
	glEnd();

}

void COpenGLControl::DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();
	GLdouble aspect = Dwidth/Dhigh;

	if(MotorAction) //2D
	{
		glMatrixMode(GL_PROJECTION);						
		glLoadIdentity();
		glOrtho(-2*aspect,2*aspect,-2,2,-4,4);	
		glMatrixMode(GL_MODELVIEW);
	}
	//-----------------------場景旋轉縮放---------------------------------------------	
	//-----------------------場景縮放位移----------------------
	if(PerspectiveView)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(55.0f,aspect,0.1f,20.0f);
		glMatrixMode(GL_MODELVIEW);

		move_sensitivity = 5*scale_m;
		center[0] = -cos(rot_m[0])*sin(rot_m[1])*trans_m[1] + sin(rot_m[0])*trans_m[0];
		center[1] =	-sin(rot_m[0])*sin(rot_m[1])*trans_m[1] - cos(rot_m[0])*trans_m[0];
		center[2] =  cos(rot_m[1])*trans_m[1];
	}
	else
	{
		move_sensitivity = 0.5;
		glMatrixMode(GL_PROJECTION);						
		glLoadIdentity();
		glOrtho((-scale_m-trans_m[0])*aspect,(scale_m-trans_m[0])*aspect,
			    (-scale_m+trans_m[1]),(scale_m+trans_m[1]),-2*(scale_m+1),2*(scale_m+1));	
		glMatrixMode(GL_MODELVIEW);
	}

	//-----------------------攝影機位置----------------------------------------------
	eye[0] = scale_m*cos(rot_m[0])*cos(rot_m[1])+center[0];
	eye[1] = scale_m*sin(rot_m[0])*cos(rot_m[1])+center[1];
	eye[2] = scale_m*sin(rot_m[1])+center[2];
	up[0] = -cos(rot_m[0])*sin(rot_m[1]);
	up[1] = -sin(rot_m[0])*sin(rot_m[1]);
	up[2] =  cos(rot_m[1]);	
	gluLookAt(eye[0],eye[1],eye[2],center[0],center[1],center[2],up[0],up[1],up[2]);
	//---------------------------------------------------------------------------------
	glGetDoublev(GL_MODELVIEW_MATRIX,ModelView);
	glGetDoublev(GL_PROJECTION_MATRIX,Projection);
	glGetIntegerv(GL_VIEWPORT,ViewPort);


	glPushMatrix();

	//if(VisionAction)
	//	Update_EnvirInfo();

	glPushMatrix();

	glPushMatrix();
		glTranslatef(StaticX,StaticY,StaticZ);
		//glTranslatef(pX/1000,pY/1000,pZ/1000);
		glPushMatrix();
			glRotatef( StaticRX, 1, 0, 0 );
			glRotatef( StaticRY, 0, 1, 0 );
			glRotatef( StaticRZ, 0, 0, 1 );
			glColor3f( 1, 1, 0 );
			glutSolidCube(0.07);
		glPopMatrix();
	glPopMatrix();

	//glPushMatrix();
	//	glTranslatef(pX/1000,pY/1000,pZ/1000);
	//	glPushMatrix();
	//		glRotatef( rpX, 1, 0, 0 );
	//		glRotatef( rpY, 0, 1, 0 );
	//		glRotatef( rpZ, 0, 0, 1 );
	//		glColor3f( 1, 0, 0 );
	//		glutSolidCube(0.07);
	//	glPopMatrix();
	//glPopMatrix();


	// Robot sticks
	motion::KineChain::GetRobotKineChain().draw_stick();

	glPushMatrix();
		glTranslatef( 0., 0., 0.);
		glPushMatrix();
			glRotatef(0.0,1,0,0);
			glRotatef(0.0,0,1,0);
			glRotatef(0.,0,0,1);
			Draw_Axis(0.1);				
		glPopMatrix();
	glPopMatrix();


	Draw_RE1();

	if (DrawCurRArm)
	{
		double th[6] = {0};
		motion::KineChain::GetRobotKineChain().get_q(th);
		float drawTheta[6] = {0};
		for (int i = 0 ; i < 6 ; i++)
			drawTheta[i] = th[i] * 180./3.1415926;
		RArm.ResetTheta(drawTheta);
		Draw_RArm();
	}

	if(CDrawGround)
		DrawGround();

	//vic.glDrawSticks();

	glPopMatrix();
	SwapBuffers(dc->m_hDC);
}



void COpenGLControl::Create(CRect rect, CWnd *parent)
{
	CString className = AfxRegisterWndClass(
		CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		NULL,
		(HBRUSH)GetStockObject(BLACK_BRUSH),
		NULL);

	CString a;
	a ="OpenGL";
	CreateEx(
		0,
		className,
		a,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		rect,
		parent,
		0);

}



void COpenGLControl::OnPaint() 
{
	/** OpenGL section **/

	openGLDevice.makeCurrent();

	DrawGLScene();
	
	ValidateRect(NULL);
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);	
	if (cy == 0)								
	{
		cy = 1;						
	}
	GLdouble aspect = cx/cy;

	glViewport(0,0,cx,cy);
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();

	if(PerspectiveView)	
		gluPerspective(55.0f,aspect,0.1f,20.0f);
	else
		glOrtho(-0.8f*aspect,0.8f*aspect,-0.8f,0.8f,-8.0f,8.0f);

	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();
}


int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	dc = new CClientDC(this);

	openGLDevice.create(dc->m_hDC);
	InitGL();
	//SetTimer(0,5,NULL);

	return 0;
}

BOOL COpenGLControl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void COpenGLControl::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent == 1)
	{
		//if(Rcount >= 0)
			//RMotion();						
	}			
	//else if (nIDEvent == 7)
		//RMotion2();
	//else if(nIDEvent == 6)
	//	manubb();

	InvalidateRect(NULL,FALSE);
	CWnd::OnTimer(nIDEvent);
}


void COpenGLControl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	r_mouse_button = true;
	r_mouse_ini_pos = point;

	CWnd::OnRButtonDown(nFlags, point);
}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	float scale_sensitivity = 0.01;
	float trans_sensitivity = 0.02;
	float rot_sensitivity   = 0.005;

	if(r_mouse_button == true && m_mouse_button == false)//只按右鍵 旋轉
	{
		float rotx=(point.x-r_mouse_ini_pos.x)*rot_sensitivity;
		float roty=(point.y-r_mouse_ini_pos.y)*rot_sensitivity;
		rot_m[0]-=rotx;
		rot_m[1]+=roty;		
		r_mouse_ini_pos=point;
	}

	if(r_mouse_button && m_mouse_button)//按右鍵和中鍵 縮放
	{
		float scale_temp =- (point.y-m_mouse_ini_pos.y)*scale_sensitivity;
		scale_m += scale_temp;
		m_mouse_ini_pos = point;
	}

	if(m_mouse_button == true && r_mouse_button == false)//指案中鍵 位移
	{
		float transx=(point.x-m_mouse_ini_pos.x)*trans_sensitivity;
		float transy=(point.y-m_mouse_ini_pos.y)*trans_sensitivity;
		trans_m[0] += transx;
		trans_m[1] += transy;
		m_mouse_ini_pos = point;
	}

	CWnd::OnMouseMove(nFlags, point);
}


void COpenGLControl::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	r_mouse_button = false;

	CWnd::OnRButtonUp(nFlags, point);
}
void COpenGLControl::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	m_mouse_button  = true;
	m_mouse_ini_pos = point;

	CWnd::OnMButtonDown(nFlags, point);
}

void COpenGLControl::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	m_mouse_button=false;

	CWnd::OnMButtonUp(nFlags, point);
}

void COpenGLControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	l_mouse_button  = true;
	l_mouse_ini_pos = point;
	
	//===============轉換視角(視窗轉OpenGL)==============
	float winPoint[] = {(float)point.x , (float)(ViewPort[3]-point.y) , 0};
	double objPoint[3] = {0};
	//glReadPixels( int(winPoint[0]), int(winPoint[1]), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winPoint[2] );
	if(gluUnProject(winPoint[0],winPoint[1],winPoint[2],ModelView,Projection,ViewPort,&objPoint[0],&objPoint[1],&objPoint[2]) != GL_TRUE)
		printf("UnProject is ERROR\n");
	for(int i = 0 ; i < 3 ; i++)
		l_mouse_Pos[i] = objPoint[i];

	L_Button_Pos = point;
	L_Button_Pos.x=( (long)(L_Button_Pos.x)-300)/5.3/scale_m;
	L_Button_Pos.y=(-(long)(L_Button_Pos.y)+300)/5.3/scale_m;

	CWnd::OnLButtonDown(nFlags, point);
}

void COpenGLControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	l_mouse_button = false;

	CWnd::OnLButtonUp(nFlags, point);
}

void COpenGLControl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}