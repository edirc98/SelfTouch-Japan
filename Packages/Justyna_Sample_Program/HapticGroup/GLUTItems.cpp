///////////////////////////////////////////////////////////////////////////////////////////////////
//  GLUTTools.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
//  This source file includes some items for rendering 3D graphics with GLUT
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Programmer:		Masayuki HARA (Assitant Professor)
//  Affiliation:	Higuchi & Yamamoto Lab. (Advanced Mechatoronics Lab.)
//					School of Engineering, The University of Tokyo
//  Created date:	18.04.2012
//	Updated date:	18.04.2012
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  include

#include <math.h>
#include <windows.h>

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

#include "DeviceControl.h"
#include "GLUTItems.h"
#include "GLUTTools.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//  define



///////////////////////////////////////////////////////////////////////////////////////////////////
//  variable



///////////////////////////////////////////////////////////////////////////////////////////////////
//  function



///////////////////////////////////////////////////////////////////////////////////////////////////
//	GLUT_ITEMS::GLUT_ITEMS()
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Constructor (Initialize parameters for using drawing 3D graphics)
///////////////////////////////////////////////////////////////////////////////////////////////////
GLUT_ITEMS::GLUT_ITEMS()
{
	static float specular[] = {1.0f, 1.0f, 1.0f, 1.0f};		//  Specular reflection
	static float diffuse[] = {0.1f, 0.1f, 0.1f, 0.2f};		//  Diffusion light of shadow
//	static float diffuse[] = {0.2f, 0.2f, 0.2f, 0.3f};		//  Diffusion light of shadow

	//  Configure specular light of object
	for (unsigned int i = 0; i < 4; i++) {
		SpecularLight[i] = specular[i];
	}

	//  Configuration of diffusion light of shadown
	for(unsigned int i = 0; i < 4; i++){
		ShadowDiffuse[i] = diffuse[i];
	}
}

//	GLUT_ITEMS::GLUT_ITEMS()
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_ITEMS::ConfigObjectParam(double *, double *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Configure object parameters
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_ITEMS::ConfigObjectParam(double *pos, double *rot, double *size, double *color)
{
	//  Translation of object
	glTranslatef((float)pos[0], (float)pos[1], (float)pos[2]);

	//  Rotation of object
	glRotatef((float)rot[0], 1.0f, 0.0f, 0.0f);					//  X direction
	glRotatef((float)rot[1], 0.0f, 1.0f, 0.0f);					//  Y direction
	glRotatef((float)rot[2], 0.0f, 0.0f, 1.0f);					//  Z direction

	//  Scaling of Object
	glScalef((float)size[0], (float)size[1], (float)size[2]);

	//  Color configulation
	glColor4f((float)color[0], (float)color[1], (float)color[2], (float)color[3]);
}

//	void GLUT_ITEMS::ConfigObjectParam(double *, double *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_ITEMS::CalcShadowMatrix(float *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Calculation of shadow matrix
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_ITEMS::CalcShadowMatrix(double *pos, double *pmat, double *cw_data)
{
	double dc[3];										//  Direction cosine of light sourcs
	double a, b, c, d;									//  Parameter of floor
	double dist;										//  Distance from light source
	double light_pos[3];								//  Position of light source
	double delta[3];
	int i;

	//  Calculation of directional vector of light source
	for(i = 0; i < 3; i++){
		light_pos[i] = cw_data[i + 11];
		delta[i] = light_pos[i] - pos[i];
	}

	dist = sqrt(delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2]);
	for(i = 0; i < 3; i++){
		dc[i] = delta[i] / dist;
	}

	//  Directional vector of floor
	a = 0.0;
	b = 0.0;
	c = 4.5;
	d = -1.0;

	//  Calculation of projection matrix
	pmat[0] = b * dc[1] + c * dc[2];
	pmat[1] = -a * dc[1];
	pmat[2] = -a * dc[2];
	pmat[3] = 0.0;

	pmat[4] = -b * dc[0];
	pmat[5] = a * dc[0] + c * dc[2];
	pmat[6] = -b * dc[2];
	pmat[7] = 0.0;

	pmat[8] = -c * dc[0];
	pmat[9] = -c * dc[1];
	pmat[10] = a * dc[0] + b * dc[1];
	pmat[11] = 0.0;

	pmat[12] = -d * dc[0];
	pmat[13] = -d * dc[1];
	pmat[14] = -d * dc[2];
	pmat[15] = a * dc[0] + b * dc[1] + c * dc[2];
}

//	void GLUT_ITEMS::CalcShadowMatrix(float *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_ITEMS::ConfigShadowParam(double *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Configuration of shadow parameter
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_ITEMS::ConfigShadowParam(double *pos, double *pmat, double *cw_data)
{
	//  Configuration of shadow
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ShadowDiffuse);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	CalcShadowMatrix(pos, pmat, cw_data);
	glMultMatrixd(pmat);
}

//	void GLUT_ITEMS::ConfigShadowParam(float *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Create unit objects
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_ITEMS::UnitCube(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Create unit cube
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_ITEMS::UnitCube(void)
{
	float p[8][3] = {
		{ 0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
		{-0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f, 0.5f},
		{ 0.5f, 0.5f,-0.5f}, {-0.5f, 0.5f,-0.5f},
		{-0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f,-0.5f}
	};

	//  Render polygon
	glBegin(GL_QUADS);	
		//  X direction
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3fv(p[0]); glVertex3fv(p[3]); 
		glVertex3fv(p[7]); glVertex3fv(p[4]); 

		//  Y direction
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3fv(p[0]); glVertex3fv(p[4]); 
		glVertex3fv(p[5]); glVertex3fv(p[1]); 

		//  Z direction
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3fv(p[0]); glVertex3fv(p[1]); 
		glVertex3fv(p[2]); glVertex3fv(p[3]); 

		//  -X direction
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3fv(p[2]); glVertex3fv(p[1]); 
		glVertex3fv(p[5]); glVertex3fv(p[6]); 

		//  -Y direction
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3fv(p[3]); glVertex3fv(p[2]); 
		glVertex3fv(p[6]); glVertex3fv(p[7]); 

		//  -Z direction
		glNormal3f(0.0f, 0.0f,-1.0f);
		glVertex3fv(p[4]); glVertex3fv(p[7]); 
		glVertex3fv(p[6]); glVertex3fv(p[5]); 
	glEnd();
}

//	void GLUT_ITEMS::UnitCube(void)
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_ITEMS::UnitSphere(short)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Render unit sphere
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_ITEMS::UnitSphere(short Nxy)
{
	float p[1089][3];
	double phai, theta;
	short p_num[33][33];
	short i, j, Nz;

	//  Limit division number
	if (Nxy > 32 || Nxy < 3){ 
		Nxy = 32;
	}	
	Nz = Nxy;

	//  Calculate number of vertex
	for (i = 0; i <= Nxy; i++) {
		for (j = 0; j <= Nz; j++)  {
			p_num[i][j] = i + (Nxy + 1) * j;
		}
	}

	//  Calculate vertex
	for (i = 0; i <= Nxy; i++) {
		//  Calculate LAT
		theta = 2.0 * GLUT_PI * (double)i / (double)Nxy;

		for (j = 0; j <= Nz; j++) {
			//  Calculate LNG
			phai = GLUT_PI / 2.0 - GLUT_PI * (double)j / (double)Nz;

			p[p_num[i][j]][0] = (float)(0.5 * cos(theta) * cos(phai));	//  X direction
			p[p_num[i][j]][1] = (float)(0.5 * sin(theta) * cos(phai));	//  Y direction
			p[p_num[i][j]][2] = (float)(0.5 * sin(phai));				//  Z direction
		}
	}

	//  Render polygon
	for (i = 0; i < Nxy; i++) {
		//  Vertex of each aspect
		for (j = 0; j < Nz - 1; j++) {
			glBegin(GL_QUADS);
				glNormal3fv(p[p_num[i][j]]); glVertex3fv(p[p_num[i][j]]);
				glNormal3fv(p[p_num[i][j + 1]]); glVertex3fv(p[p_num[i][j + 1]]);
				glNormal3fv(p[p_num[i + 1][j + 1]]); glVertex3fv(p[p_num[i + 1][j + 1]]);
				glNormal3fv(p[p_num[i + 1][j]]); glVertex3fv(p[p_num[i + 1][j]]);
			glEnd();
		}
	}

	//  Render polygon (Base)
	j = Nz - 1;

	//  Vertex of each aspect
	for (i = 0; i < Nxy; i++) {
		glBegin(GL_TRIANGLES);
			glNormal3fv(p[p_num[i][j]]); glVertex3fv(p[p_num[i][j]]);  
			glNormal3fv(p[p_num[i][j + 1]]); glVertex3fv(p[p_num[i][j + 1]]); 
			glNormal3fv(p[p_num[i + 1][j]]); glVertex3fv(p[p_num[i + 1][j]]); 
		glEnd();
	}
}

//	void GLUT_ITEMS::UnitSphere(short)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Render 3D objects
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_ITEMS::DrawFloor(double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Draw grid or check floor
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_ITEMS::DrawFloor(double* setData)
{
	double a;
	short i, j, nx, ny;
	float widG, widFloorX, widFloorY, x0, x1, y0, y1;
	float FloorSpecularLight[] = {0.1f, 0.1f, 0.1f, 1.0f};		//  Specular light of floor
	float CFloorDiffusionLight1[] = {0.8f, 0.5f, 0.3f, 1.0f};	//EDifussion light1 of check floor
	float CFloorDiffusionLight2[] = {0.3f, 0.5f, 0.8f, 1.0f};	//EDiffusion light2 of check floor
	float GFloorDiffusionLight[] = {0.2f, 0.2f, 0.2f, 1.0f};	//  Diffusion light of grid floor

	widG = 100.0f;										//  Width of grid
	widFloorX = 1000.0f;								//  Width of floor (X direction)
	widFloorY = 2000.0f;								//  Width of floor (Y direction)
	nx = (short)(widFloorX / widG);
	ny = (short)(widFloorY / widG);
	glMaterialfv(GL_FRONT, GL_SPECULAR, FloorSpecularLight);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

	//  Draw floor
	switch ((short)setData[16]) {
		case GLUT_GRID_FLOOR:
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GFloorDiffusionLight);
			
			//  X axis
			x0 = widFloorX / 2.0f;
			x1 = -widFloorX / 2.0f;
			for (j = -ny / 2; j <= ny / 2; j++) {
				y1 = (float)j * widG;
				glBegin(GL_LINES);
					glVertex3f(x0, y1, -1.0f);
					glVertex3f(x1, y1, -1.0f);
				glEnd();
			}

			//	Y axis
			y0 = widFloorY/2.0f;
			y1 = -widFloorY/2.0f;
			for (i = -nx / 2; i <= nx / 2; i++){
				x1 = (float)i * widG;
				glBegin(GL_LINES);
					glVertex3f(x1, y0, -1.0f);
					glVertex3f(x1, y1, -1.0f);
				glEnd();
			}
			break;

		case GLUT_CHECK_FLOOR:
			for (j = 0; j < ny; j++){
				for(i = 0; i < nx; i++) {
					a = (double)(i + j);
					if (fmod(a, 2.0) == 0.0) {
						glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, CFloorDiffusionLight1);
					} else {
						glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, CFloorDiffusionLight2);
					}
					glPushMatrix();
						x0 = -widFloorX/2.0f + (float)i*widG + widG/2.0f;
						y0 = -widFloorY/2.0f + (float)j*widG + widG/2.0f;
						glTranslatef(x0, y0, 0.0f);
						glBegin(GL_QUADS);
							glNormal3f(0.0f, 0.0f, 1.0f);
							glVertex3f( widG/2.0f, -widG/2.0f, 0.0f);
							glVertex3f( widG/2.0f,  widG/2.0f, 0.0f);
							glVertex3f(-widG/2.0f,  widG/2.0f, 0.0f);
							glVertex3f(-widG/2.0f, -widG/2.0f, 0.0f);
						glEnd();
					glPopMatrix();
				}
			}
			break;

		default:
			break;
	}
}

//	void GLUT_ITEMS::DrawFloor(double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_ITEMS::DrawSphere(short, double *, double *, double *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Draw 3D sphere-shaped object
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_ITEMS::DrawSphere(short Nxy, double *pos,
						   double *rot, double *size, double *color, double *setData)
{
	//  Configure diffusion light
	float DiffusionLight[] = {(float)color[0], (float)color[1], (float)color[2], 1.0f};

	//  Configure specular reflection
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularLight);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

	//  Configure ambient/diffusion light
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, DiffusionLight);

	//  Draw sphere-shaped object
	glPushMatrix();
		//  Configure object parameters
		ConfigObjectParam(pos, rot, size, color);

		//  Draw unit sphere
		UnitSphere(Nxy);	
	glPopMatrix();
}

//	void GLUT_ITEMS::DrawSphere(short, double *, double *, double *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_ITEMS::DrawCube(double *, double *, double *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Draw 3D cube-shaped object
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_ITEMS::DrawCube(double *pos, double *rot, double *size, double *color, double *setData)
{
	//  Configure diffusion light
	float DiffusionLight[] = {(float)color[0], (float)color[1], (float)color[2], 1.0f};

	//  Configure specular reflection
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularLight);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

	//  Configure ambient/diffusion light
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, DiffusionLight);

	//  Draw cube-shaped object
	glPushMatrix();
		//  Configure object parameters
		ConfigObjectParam(pos, rot, size, color);
		
		//  Draw unit cube
		UnitCube();	
	glPopMatrix();
}

//	void GLUT_ITEMS::DrawCube(double *, double *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_ITEMS::DrawMqoModel(double *, double *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Draw MQO model
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_ITEMS::DrawMqoModel(double pos[][3], double rot[][3], double size[][3], double *setData)
{
	//  Draw MQO model
	for (unsigned int i = 0; i < GLUT_MODEL_NUM; i++) {
		glPushMatrix();
			glTranslatef((float)pos[i][0], (float)pos[i][1], (float)pos[i][2]);
			glRotatef((float)rot[i][2], 1.0f, 0.0f, 0.0f);
			glRotatef((float)rot[i][0], 0.0f, 0.0f, 1.0f);
			glRotatef((float)rot[i][1], 0.0f, 1.0f, 0.0f);
			glScalef((float)size[i][0], (float)size[i][1], (float)size[i][2]);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			mqoCallModel(glut_mqo_model[i]);
		glPopMatrix();
	}
}

//	void GLUT_ITEMS::DrawMqoModel(double *, double *, double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////