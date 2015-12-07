#include "stdafx.h"
#include "OpenGL_Model\OpenGLControl.h"


using namespace std;

void COpenGLControl::Draw_RArm()
{
	float matrix[16];
	glPushMatrix();		 
	glDisable(GL_COLOR_MATERIAL);
	for(int num = 0 ; num < 9 ; num++)
	{
		//cout<<"num: "<<num<<endl;
		glPushMatrix();
		for(int j=0;j<4;j++)
			for(int i=0;i<4;i++)
				matrix[i+j*4] = RArm.DHarm->R[num](i+1,j+1);
		glMultMatrixf(matrix);

		if(EE_Axis)
			Draw_Axis(0.1);

		glCallList(GL_RArmJ[num]);

		glPopMatrix();
	}
	glEnable(GL_COLOR_MATERIAL);
	glPopMatrix();
}

//void COpenGLControl::Draw_CurrentRArm()
//{
//	if(MotorAction && DrawCurRArm && CurRArmTheta != NULL)
//	{
//		float matrix[16];
//		RArm_for_Draw.ResetTheta(CurRArmTheta);
//		
//		//glEnable(GL_COLOR_MATERIAL);
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//
//		glPushMatrix();
//		glColor4f(0.8,0.2,0.2,0.5);
//		for(int num = 1 ; num < 10 ; num++)
//		{
//			glPushMatrix();
//			for(int j=0;j<4;j++)
//				for(int i=0;i<4;i++)
//				{
//					matrix[i+j*4] = RArm_for_Draw.DHarm->R[num](i+1,j+1);
//					printf("-------------------------------\n");
//					printf("%f\n",matrix[i+j*4]);
//					system("pause");
//				}
//			glMultMatrixf(matrix);
//			glCallList(GL_RArmJ[num]);
//			glPopMatrix();
//		}
//		glPopMatrix();
//
//		//glDisable(GL_BLEND);
//	}
//}

void COpenGLControl::Draw_Axis(float LINK_LENGTH)
{
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	float LINK_RADIUS  = 0.0025;

	glEnable(GL_COLOR_MATERIAL);

	glPushMatrix();
	glColor3f(0.8,0,0);
	gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,20,32);
	glTranslatef(0, 0,LINK_LENGTH);
	glutSolidCone(0.008,0.012,10,10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0,0.8,0);
	glRotatef(-90,1,0,0);
	gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,20,32);
	glTranslatef(0, 0 ,LINK_LENGTH);
	glutSolidCone(0.008,0.012,10,10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0,0,0.8);
	glRotatef(90,0,1,0);
	gluCylinder(quadratic,LINK_RADIUS,LINK_RADIUS,LINK_LENGTH,20,32);
	glTranslatef(0, 0,LINK_LENGTH);
	glutSolidCone(0.008,0.012,10,10);
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
}

void COpenGLControl::Draw_grid()
{
	int Width[2]  = {-2,2};
	int Length[2] = {-2,2};
	float scale=0.1;
	glColor3f(0,0.3,0);
	glLineStipple(1,0xaaaa);
	glEnable(GL_LINE_STIPPLE);
	for(float x=Width[0] ; x <= Width[1] ;x=x+scale)
	{
		glBegin(GL_LINES);
		glVertex3f(x,Length[0],1);
		glVertex3f(x,Length[1],1);
		glEnd();
	}
	for(float y=Length[0] ; y <= Length[1] ;y=y+scale)
	{
		glBegin(GL_LINES);
		glVertex3f(Width[0],y,1);
		glVertex3f(Width[1],y,1);
		glEnd();
	}
	glDisable(GL_LINE_STIPPLE);
	glColor3f(1,1,1);
}

void COpenGLControl::Draw_RE1()
{	
	glPushMatrix();

		glDisable(GL_COLOR_MATERIAL);
		//glPushMatrix();
		//glTranslatef(T1[0],T1[1],T1[2]);
		//glRotatef(180,0,0,1);

		//glPopMatrix();

		//glPushMatrix();
		//glTranslatef(T2[0],T2[1],T2[2]);
		//glCallList(RE2);
		//glPopMatrix();
		glCallList(RE1);

		glEnable(GL_COLOR_MATERIAL);
	
		//glTranslatef(0.1,0,0.7);
		//glColor3f(0.5,0.5,0.55);
		//glutSolidSphere(0.06,10,10);

	glPopMatrix();	
}

void COpenGLControl::Draw_Desk()
{	
	glPushMatrix();

		glDisable(GL_COLOR_MATERIAL);

		glCallList(Desk);

		glEnable(GL_COLOR_MATERIAL);
	
	glPopMatrix();	
}

AUX_RGBImageRec* COpenGLControl::LoadBMP(char* Filename)
{
	FILE *File=NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;
}

bool COpenGLControl::LoadSkyboxTextures(void)
{
	int Status = FALSE;// Status Indicator
	const int texture_num = 2;
	int i ;									

	AUX_RGBImageRec *TextureImage[texture_num];					// Create Storage Space For The Texture
	memset(TextureImage,0,sizeof(void *)*texture_num);           	// Set The Pointer To NULL	

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if ((TextureImage[0]=LoadBMP("OpenGL_Model/BackgroundData/BricksLgMortar.bmp")) && 
		(TextureImage[1]=LoadBMP("OpenGL_Model/BackgroundData/BricksLgMortar.bmp")))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(texture_num, texture);						// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		for (i= 0;i<texture_num;i++)
		{
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i]->sizeX, TextureImage[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
	}

	// release memory
	//===========================================================================
	for (i= 0;i<texture_num;i++)
	{
		if (TextureImage[i])									// If Texture Exists
		{
			if (TextureImage[i]->data)							// If Texture Image Exists
			{
				free(TextureImage[i]->data);					// Free The Texture Image Memory
			}

			free(TextureImage[i]);								// Free The Image Structure
		}
	}
	return Status;										// Return The Status	
}

void COpenGLControl::DrawGround(void)
{
	float Fig_num = 10;
	float Fwidth = 2.0f;
	float Fhigh  = 2.0f;
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();		
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_POLYGON);
			glColor3f(1.0f,1.0f,1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(Fwidth,-Fhigh, 0.0);
			glTexCoord2f(Fig_num, 0.0f); glVertex3f(Fwidth,Fhigh,0.0);
			glTexCoord2f(Fig_num, Fig_num); glVertex3f(-Fwidth,Fhigh,0.0);
			glTexCoord2f(0.0f, Fig_num); glVertex3f(-Fwidth,-Fhigh,0.0);
		glEnd();
		
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}
