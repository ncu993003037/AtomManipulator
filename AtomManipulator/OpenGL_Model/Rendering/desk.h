/*
This file was produced by Deep Exploration Plugin: CPP Export filter. 

Copyright (C) 1999-2006 Right Hemisphere
Mail support@righthemisphere.com for support.
Visit http://www.righthemisphere.com/dexp.htm for updates.
*/
#include <windows.h>
#include <OpenGL_Model\GL\gl.h>
#include <OpenGL_Model\GL\glu.h>

namespace DeskModel
{
struct sample_MATERIAL
{
	GLfloat ambient[3];
	GLfloat diffuse[3];
	GLfloat specular[3];
	GLfloat emission[3];
	GLfloat alpha;
	GLfloat phExp;
	int texture;
};

static sample_MATERIAL materials [5] = {
 {{0.117647f,0.117647f,0.117647f},	{0.752941f,0.752941f,0.752941f},	{0.301177f,0.301177f,0.301177f},	{0.0f,0.0f,0.0f},	1.0f,8.0f,-1}, //Explorer Default
 {{0.117647f,0.117647f,0.117647f},	{0.839216f,0.611765f,0.388235f},	{0.0329412f,0.0329412f,0.0329412f},	{0.0f,0.0f,0.0f},	1.0f,8.0f,-1}, //Material #1
 {{0.117647f,0.117647f,0.117647f},	{0.239216f,0.478431f,0.309804f},	{0.0160784f,0.0160784f,0.0160784f},	{0.0f,0.0f,0.0f},	1.0f,8.0f,-1}, //Material #3
 {{0.117647f,0.117647f,0.117647f},	{0.298039f,0.298039f,0.298039f},	{0.0129412f,0.0129412f,0.0129412f},	{0.0f,0.0f,0.0f},	1.0f,8.0f,-1}, //Material #4
 {{0.117647f,0.117647f,0.117647f},	{0.690196f,0.85098f,1.0f},	{0.02f,0.02f,0.02f},	{0.0f,0.0f,0.0f},	1.0f,8.0f,-1} //Material #2
};

// 28 Verticies
// 0 Texture Coordinates
// 5 Normals
// 52 Triangles

static BYTE face_indicies[52][9] = {
//  DE       15
	{2,1,0 ,0,0,0 ,0,0,0}, {0,3,2 ,0,0,0 ,0,0,0}
//  DE       31
	, {3,5,4 ,1,1,1 ,0,0,0}, {4,0,3 ,1,1,1 ,0,0,0}
//  DE       47
	, {7,2,1 ,2,2,2 ,0,0,0}, {1,6,7 ,2,2,2 ,0,0,0}
//  DE       63
	, {0,1,6 ,3,3,3 ,0,0,0}, {6,4,0 ,3,3,3 ,0,0,0}
//  DE       79
	, {6,7,5 ,4,4,4 ,0,0,0}, {5,4,6 ,4,4,4 ,0,0,0}
//  DE       95
	, {10,9,8 ,0,0,0 ,0,0,0}, {8,11,10 ,0,0,0 ,0,0,0}
//  DE      109
	, {11,13,12 ,1,1,1 ,0,0,0}, {12,8,11 ,1,1,1 ,0,0,0}
//  DE      123
	, {15,10,9 ,2,2,2 ,0,0,0}, {9,14,15 ,2,2,2 ,0,0,0}
//  DE      137
	, {8,9,14 ,3,3,3 ,0,0,0}, {14,12,8 ,3,3,3 ,0,0,0}
//  DE      153
	, {14,15,13 ,4,4,4 ,0,0,0}, {13,12,14 ,4,4,4 ,0,0,0}
//  DE      193
	, {17,7,2 ,3,3,3 ,0,0,0}, {7,17,16 ,3,3,3 ,0,0,0}, {17,2,3 ,3,3,3 ,0,0,0},
	{18,3,5 ,3,3,3 ,0,0,0}, {3,18,17 ,3,3,3 ,0,0,0}, {18,5,19 ,3,3,3 ,0,0,0},
	{7,10,5 ,3,3,3 ,0,0,0}, {10,16,15 ,3,3,3 ,0,0,0}, {16,10,7 ,3,3,3 ,0,0,0},
	{15,16,13 ,3,3,3 ,0,0,0}, {11,13,19 ,3,3,3 ,0,0,0}, {19,13,16 ,3,3,3 ,0,0,0},
	{11,19,5 ,3,3,3 ,0,0,0}, {10,11,5 ,3,3,3 ,0,0,0}
//  DE      209
	, {20,21,22 ,3,3,3 ,0,0,0}, {22,23,20 ,3,3,3 ,0,0,0}
//  DE      223
	, {25,24,27 ,3,3,3 ,0,0,0}, {27,26,25 ,3,3,3 ,0,0,0}
//  DE      239
	, {22,21,24 ,2,2,2 ,0,0,0}, {24,27,22 ,2,2,2 ,0,0,0}
//  DE      259
	, {21,24,17 ,0,0,0 ,0,0,0}, {17,24,25 ,0,0,0 ,0,0,0}, {17,18,20 ,0,0,0 ,0,0,0},
	{17,20,21 ,0,0,0 ,0,0,0}
//  DE      279
	, {22,16,27 ,4,4,4 ,0,0,0}, {16,22,23 ,4,4,4 ,0,0,0}, {27,16,26 ,4,4,4 ,0,0,0},
	{19,16,23 ,4,4,4 ,0,0,0}
//  DE      295
	, {20,23,19 ,1,1,1 ,0,0,0}, {19,18,20 ,1,1,1 ,0,0,0}
//  DE      311
	, {16,17,25 ,2,2,2 ,0,0,0}, {25,26,16 ,2,2,2 ,0,0,0}
};
static GLfloat vertices [28][3] = {
{0.05001f,0.2f,0.50005f},{-0.14999f,0.2f,0.50005f},{-0.14999f,0.2f,0.49995f},
{0.05001f,0.2f,0.49995f},{0.05001f,-9.90603e-010f,0.50005f},{0.05001f,-9.90603e-010f,0.49995f},
{-0.14999f,-1.93408e-009f,0.50005f},{-0.14999f,-1.93408e-009f,0.49995f},{0.05001f,-1.00108e-005f,0.50005f},
{-0.14999f,-1.00117e-005f,0.50005f},{-0.14999f,-1.00117e-005f,0.49995f},{0.05001f,-1.00108e-005f,0.49995f},
{0.05001f,-0.20001f,0.50005f},{0.05001f,-0.20001f,0.49995f},{-0.14999f,-0.20001f,0.50005f},
{-0.14999f,-0.20001f,0.49995f},{-0.15f,-0.5f,0.49995f},{-0.15f,0.5f,0.49995f},
{0.35f,0.5f,0.49995f},{0.35f,-0.5f,0.49995f},{0.35f,0.5f,-0.50005f},
{-0.35f,0.5f,-0.50005f},{-0.35f,-0.5f,-0.50005f},{0.35f,-0.5f,-0.50005f},
{-0.35f,0.5f,0.50005f},{-0.15f,0.5f,0.50005f},{-0.15f,-0.5f,0.50005f},
{-0.35f,-0.5f,0.50005f}
};
static GLfloat normals [5][3] = {
{-4.71738e-009f,1.0f,0.0f},{-1.0f,-4.71738e-009f,0.0f},{1.0f,4.71738e-009f,0.0f},
{0.0f,0.0f,1.0f},{4.71738e-009f,-1.0f,0.0f}
};
GLfloat textures[1][2]={{0.0f,0.0f}};
/*Material indicies*/
/*{material index,face count}*/
static int material_ref [7][2] = {
{1,6},
{2,2},
{1,8},
{3,2},
{1,18},
{4,2},
{1,14}
};
void MyMaterial(GLenum mode, GLfloat * f, GLfloat alpha)
{
	GLfloat d[4];
	d[0] = f[0];
	d[1] = f[1];
	d[2] = f[2];
	d[3] = alpha;
	glMaterialfv(GL_FRONT_AND_BACK, mode, d);
}

/*
 *  SelectMaterial uses OpenGL commands to define facet colors.
 *
 *  Returns:
 *    Nothing
 */

void SelectMaterial(int i)
{
//
// Define the reflective properties of the 3D Object faces.
//
	glEnd();
	GLfloat alpha = materials[i].alpha;
	MyMaterial(GL_AMBIENT, materials[i].ambient, alpha);
	MyMaterial(GL_DIFFUSE, materials[i].diffuse, alpha);
	MyMaterial(GL_SPECULAR, materials[i].specular, alpha);
	MyMaterial(GL_EMISSION, materials[i].emission, alpha);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materials[i].phExp);
	glBegin(GL_TRIANGLES);

};

GLint Gen3DObjectList()
{
	int i;
	int j;

 GLint lid=glGenLists(1);
	int mcount=0;
	int mindex=0;
   glNewList(lid, GL_COMPILE);
   
   glPushMatrix();
   glTranslatef(0.75,0,0.499);

    glBegin (GL_TRIANGLES);
      for(i=0;i<sizeof(face_indicies)/sizeof(face_indicies[0]);i++)
       {
	if (!mcount)
	{
		SelectMaterial(material_ref[mindex][0]);
		mcount = material_ref[mindex][1];
		mindex ++;
	}
	mcount --;
       for(j=0;j<3;j++)
        {
          int vi=face_indicies[i][j];
          int ni=face_indicies[i][j+3];//Normal index
          int ti=face_indicies[i][j+6];//Texture index
           glNormal3f (normals[ni][0],normals[ni][1],normals[ni][2]);
           glTexCoord2f(textures[ti][0],textures[ti][1]);
           glVertex3f (vertices[vi][0],vertices[vi][1],vertices[vi][2]);
        }
       }
    glEnd ();

	glPopMatrix();

   glEndList();
   return lid;
};

}