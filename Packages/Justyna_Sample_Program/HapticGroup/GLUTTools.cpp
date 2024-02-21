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
//	Updated date:	06.05.2012
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  include

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "GL/glext.h"

#include "resource.h"

#include "DeviceControl.h"
#include "GLUTItems.h"
#include "GLUTTools.h"
#include "SamplerTools.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//	define



///////////////////////////////////////////////////////////////////////////////////////////////////
//	variable

HDC GLUT_hdcGLWnd;

int GLUT_flag_GLWnd = 0;			//  Flag for window of OpenGL
int	GLUT_flag_GLDlg = 0;			//  Flag for configuration dialog box of OpenGL

int GLUT_FloorType[2] = {0, 1};
int GLUT_Shadow[2] = {0, 1};
int GLUT_Stereovision[2] = {0, 1};

SCROLLINFO GLUT_ScrollBar[10];
int	GLUT_ScrollBarFlag;

double GLUT_distC;
double GLUT_thetaC;
double GLUT_phaiC;

double GLUT_fov = 25.0;				//  Field of view (FOV) based on the light source

int GLUT_texWidth = 512;			//  Width of texture for shadow mapping
int GLUT_texHeight = 512;			//  Height of texture for shadow mapping

int GLUT_dispFlag;

double GLUT_ViewData[19];
double GLUT_WindowViewData[19];

GLUT_ITEMS TestModel;

//  For GLMetaseq
MQO_MODEL glut_mqo_model[GLUT_MODEL_NUM];
char glut_mqo_model_name[GLUT_MODEL_NUM][64] = {
	"Model/left_hand.mqo",
	"Model/right_hand.mqo",
	"Model/right_hand2.mqo",
	"Model/g_body.mqo",
	"Model/g_antenna.mqo",
	"Model/g_legs_f.mqo",
	"Model/g_legs_m.mqo",
	"Model/g_legs_r.mqo",
	"Model/knife.mqo",
};
double glut_mqo_pos[GLUT_MODEL_NUM][3] = {	//  X-Y-Z
	{30.0, 0.0, 7.5},
//	{30.0, 0.0, 20.0},						//  left_hand_2.mqo
	{40.0, 0.0, 150.0},
	{40.0, 0.0, 150.0},
	{-1000.0, -125.0, 12.5},
	{-1000.0, -125.0, 12.5},
	{-1000.0, -125.0, 12.5},
	{-1000.0, -125.0, 12.5},
	{-1000.0, -125.0, 12.5},
	{30.0, -60.0, 800.0}
};
double glut_mqo_rot[GLUT_MODEL_NUM][3] = {	//  É¡-Éø-É¿
	{0.0, 0.0, 0.0},
//	{-25.0, 0.0, 0.0},						//  left_hand_2.mqo
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, 90.0, 90.0},
	{0.0, 90.0, 90.0},
	{0.0, 90.0, 90.0},
	{0.0, 90.0, 90.0},
	{0.0, 90.0, 90.0},
	{-45.0, 180.0, 0.0}
};
double glut_mqo_size[GLUT_MODEL_NUM][3] = {
	{30.0, 30.0, 30.0},
	{30.0, 30.0, 30.0},
	{30.0, 30.0, 30.0},
	{3.5, 5.5, 5.5},
	{3.5, 5.5, 5.5},
	{3.5, 5.5, 5.5},
	{3.5, 5.5, 5.5},
	{3.5, 5.5, 5.5},
	{6.5, 8.5, 8.5}
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//	function

//  Main functions of GLUT
void GLUT_Display(void);
void GLUT_Idle(void);
void GLUT_Resize(int, int);
void GLUT_Keyboard(unsigned char, int, int);
void GLUT_Special(int, int, int);
void GLUT_Mouse(int, int, int, int);
void GLUT_Motion(int, int);

//  Functions for rendering 3D graphics
void GLUT_Init(double *);
void GLUT_SetLightSource(double *);
void GLUT_SetShadowMap(double *);
void GLUT_SetCamera(int, double *);

//  Functions for dialogbox
LRESULT CALLBACK GLUT_ConfigDlgProc(HWND, UINT, WPARAM, LPARAM);

void GLUT_Draw3DSpaceDlg(HDC, HWND, double *);
BOOL GLUT_Adjust3DSpace(double *, double *);

int GLUT_InitScrollBar(HWND);
int GLUT_DisplayScrollBar(HWND);
int GLUT_DisplayScrollBarStatus(HWND);
int GLUT_ReconfigureCamera(HWND, double *);

double b_vel = 0.0;
double b_vel_d;
double b_vel_u;
int b_flag = 0;
double b_d[3] = {40.0, 40.0, 40.0};
double b_pos[3] = {25.0, -75.0, 800.0};
double b_rot[3] = {0.0, 0.0, 0.0};
double b_color[4] = {0.5, 0.5, 0.5, 1.0};
double b_height = 400.0;

HWND hTmpWnd;


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Main functions of GLUT 
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  void GLUT_Display(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Display 3D models
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_Display(void)
{
	GLdouble modelview[16];						//  Buffer for modelview
//	GLint viewport[4];							//  Buffer for viewport
//	GLdouble projection[16];					//  Buffer for perspective transformation matrix
	float shadowDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};	//  Diffusion light of shadow
//	float shadowDiffuse[] = {0.2f, 0.2f, 0.2f, 1.0f};	//  Diffusion light of shadow
	float shadowSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};	//	Specular light of shadow
	int num;

	double glut_radius = 350.0;
	double glut_freq = 0.2;
	double glut_path = 95.0;

	double r_mtx[3][3];							//  Rotational matrix
	double glut_size[GLUT_MODEL_NUM][3];
	double glut_rot[GLUT_MODEL_NUM][3];
	double S0, S12, S3, S4, S5, C0, C12, C3, C4, C5;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//  Set variables
	///////////////////////////////////////////////////////////////////////////////////////////////
	//  Calculate the movement of left hand
	glut_mqo_pos[0][1] = dSliderPosition;

	//  Calculate the movement of right hand
	glut_mqo_pos[1][0] = dDesiredPosition[0] - 75.0;
	glut_mqo_pos[1][1] = dDesiredPosition[1];
	glut_mqo_pos[1][2] = dDesiredPosition[2] - dContactPosition[2] + 40.0;

	glut_mqo_pos[2][0] = dDesiredPosition[0] - 75.0;
	glut_mqo_pos[2][1] = dDesiredPosition[1];
	glut_mqo_pos[2][2] = dDesiredPosition[2] - dContactPosition[2] + 40.0;

	//  Set mqo size
	for (unsigned int i = 0; i < GLUT_MODEL_NUM; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			glut_size[i][j] = glut_mqo_size[i][j];
		}
	}

	//  Caluculate sine and cosine functions
	S0 = sin(dPhantomAngle[0]);
	S12 = sin(dPhantomAngle[2]);
	S3 = sin(dGimbalAngle[0]);
	S4 = sin(dGimbalAngle[1]);
	S5 = sin(dGimbalAngle[2]);
	C0 = cos(dPhantomAngle[0]);
	C12 = cos(dPhantomAngle[2]);
	C3 = cos(dGimbalAngle[0]);
	C4 = cos(dGimbalAngle[1]);
	C5 = cos(dGimbalAngle[2]);

	//  Calculate the rotational matrix
	r_mtx[0][0] = (-(C0 * C12 * C3 + S0 * S3) * S4 
		- C0 * S12 * C4) * C5 - (C0 * C12 * S3 - S0 * C3) * S5;
	r_mtx[0][1] = -(-(C0 * C12 * C3 + S0 * S3) * S4 
		- C0 * S12 * C4) * S5 - (C0 * C12 * S3 - S0 * C3) * C5;
	r_mtx[0][2] = -(C0 * C12 * C3 + S0 * S3) * C4 + C0 * S12 * S4;

	r_mtx[1][0] = (-(-S0 * C12 * C3 + C0 * S3) * S4 
		+ S0 * S12 * C4) * C5 - (-S0 * C12 * S3 - C0 * C3) * S5;
	r_mtx[1][1] = -(-(-S0 * C12 * C3 + C0 * S3) * S4 
		+ S0 * S12 * C4) * S5 - (-S0 * C12 * S3 - C0 * C3) * C5;
	r_mtx[1][2] = -(-S0 * C12 * C3 + C0 * S3) * C4 - S0 * S12 * S4;

	r_mtx[2][0] = (-S12 * C3 * S4 + C12 * C4) * C5 - S12 * S3 * S5;
	r_mtx[2][1] = -(-S12 * C3 * S4 + C12 * C4) * S5 - S12 * S3 * C5;
	r_mtx[1][2] = -S12 * C3 * C4 - C12 * S4;

	//  Calculate the orientation
	dOrientation[0] = asin(r_mtx[2][1]);
	dOrientation[1] = asin(-r_mtx[2][0] / cos(dOrientation[0]));
	dOrientation[2] = asin(-r_mtx[0][1] / cos(dOrientation[0]));

	//  Calculate orientations
	glut_rot[0][0] = glut_mqo_rot[0][0];
	glut_rot[0][1] = glut_mqo_rot[0][1];
	glut_rot[0][2] = glut_mqo_rot[0][2];

	glut_rot[1][0] = glut_mqo_rot[1][0] + dOrientation[2] * 180.0 / GLUT_PI;
	glut_rot[1][1] = glut_mqo_rot[1][1] - dOrientation[1] * 180.0 / GLUT_PI;
	glut_rot[1][2] = glut_mqo_rot[1][2] + dOrientation[0] * 180.0 / GLUT_PI;

	glut_rot[2][0] = glut_mqo_rot[2][0] + dOrientation[2] * 180.0 / GLUT_PI;
	glut_rot[2][1] = glut_mqo_rot[2][1] - dOrientation[1] * 180.0 / GLUT_PI;
	glut_rot[2][2] = glut_mqo_rot[2][2] + dOrientation[0] * 180.0 / GLUT_PI;

/*	for (unsigned int i = 2; i < 7; i++) {
		glut_mqo_pos[i][0] = glut_radius * cos(-2.0 * GLUT_PI * glut_freq * AnimationCounter * SAMP + GLUT_PI);
		glut_mqo_pos[i][1] = glut_radius * sin(-2.0 * GLUT_PI * glut_freq * AnimationCounter * SAMP + GLUT_PI) 
			- glut_radius - glut_path;
	}

	glut_rot[2][0] = glut_mqo_rot[2][0];
	glut_rot[2][1] = glut_mqo_rot[2][1]
				+ atan2(glut_mqo_pos[2][1] + glut_radius + glut_path, glut_mqo_pos[2][0]) * 180.0 / GLUT_PI - 90.0;
	glut_rot[2][2] = glut_mqo_rot[2][2];

	glut_rot[3][0] = glut_mqo_rot[2][0];
	glut_rot[3][1] = glut_mqo_rot[2][1] 
				+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * AnimationCounter * SAMP + 2.0 * GLUT_PI / 3.0)
				+ atan2(glut_mqo_pos[2][1] + glut_radius + glut_path, glut_mqo_pos[2][0]) * 180.0 / GLUT_PI - 90.0;
	glut_rot[3][2] = glut_mqo_rot[2][2];

	glut_rot[4][0] = glut_mqo_rot[2][0];
	glut_rot[4][1] = glut_mqo_rot[2][1] 
				+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * AnimationCounter * SAMP)
				+ atan2(glut_mqo_pos[2][1] + glut_radius + glut_path, glut_mqo_pos[2][0]) * 180.0 / GLUT_PI - 90.0;
	glut_rot[4][2] = glut_mqo_rot[2][2];

	glut_rot[5][0] = glut_mqo_rot[2][0];
	glut_rot[5][1] = glut_mqo_rot[2][1] 
				+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * AnimationCounter * SAMP + 2.0 * GLUT_PI / 3.0)
				+ atan2(glut_mqo_pos[2][1] + glut_radius + glut_path, glut_mqo_pos[2][0]) * 180.0 / GLUT_PI - 90.0;
	glut_rot[5][2] = glut_mqo_rot[2][2];

	glut_rot[6][0] = glut_mqo_rot[2][0];
	glut_rot[6][1] = glut_mqo_rot[2][1] 
				+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * AnimationCounter * SAMP + 4.0 * GLUT_PI / 3.0)
				+ atan2(glut_mqo_pos[2][1] + glut_radius + glut_path, glut_mqo_pos[2][0]) * 180.0 / GLUT_PI - 90.0;
	glut_rot[6][2] = glut_mqo_rot[2][2];
*/
	ULONG glut_start = 1000;
	ULONG glut_end = 1250;

	for (unsigned int i = 3; i < 8; i++) {
		if (AnimationCounter <= glut_start) {
			glut_mqo_pos[i][0] = glut_radius * cos(-2.0 * GLUT_PI * glut_freq 
								* AnimationCounter * SAMP + GLUT_PI);
			glut_mqo_pos[i][1] = glut_radius * sin(-2.0 * GLUT_PI * glut_freq 
								* AnimationCounter * SAMP + GLUT_PI) - glut_radius - glut_path;
		} else if (glut_start < AnimationCounter && AnimationCounter <= glut_end) {
			glut_mqo_pos[i][0] = glut_radius * cos(-2.0 * GLUT_PI * glut_freq 
								* glut_start * SAMP + GLUT_PI);
			glut_mqo_pos[i][1] = glut_radius * sin(-2.0 * GLUT_PI * glut_freq 
								* glut_start * SAMP + GLUT_PI) - glut_radius - glut_path;
		} else {
			glut_mqo_pos[i][0] = glut_radius * cos(-2.0 * GLUT_PI * glut_freq 
								* (AnimationCounter - (glut_end - glut_start)) * SAMP + GLUT_PI);
			glut_mqo_pos[i][1] = glut_radius * sin(-2.0 * GLUT_PI * glut_freq 
								* (AnimationCounter - (glut_end - glut_start)) * SAMP + GLUT_PI) - glut_radius - glut_path;
		}
	}

	glut_rot[3][0] = glut_mqo_rot[3][0];
	glut_rot[3][2] = glut_mqo_rot[3][2];

	glut_rot[4][0] = glut_mqo_rot[4][0];
	glut_rot[4][2] = glut_mqo_rot[4][2];

	glut_rot[5][0] = glut_mqo_rot[5][0];
	glut_rot[5][2] = glut_mqo_rot[5][2];

	glut_rot[6][0] = glut_mqo_rot[6][0];
	glut_rot[6][2] = glut_mqo_rot[6][2];

	glut_rot[7][0] = glut_mqo_rot[7][0];
	glut_rot[7][2] = glut_mqo_rot[7][2];

	glut_rot[8][0] = glut_mqo_rot[8][0];
	glut_rot[8][1] = glut_mqo_rot[8][1];
	glut_rot[8][2] = glut_mqo_rot[8][2];

	if (AnimationCounter <= glut_start) {
		glut_rot[3][1] = glut_mqo_rot[3][1]
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[4][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * AnimationCounter * SAMP + 2.0 * GLUT_PI / 3.0)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[5][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * AnimationCounter * SAMP)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[6][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * AnimationCounter * SAMP + 2.0 * GLUT_PI / 3.0)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[7][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * AnimationCounter * SAMP + 4.0 * GLUT_PI / 3.0)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
	} else if (glut_start < AnimationCounter && AnimationCounter <= glut_end) {
		glut_rot[3][1] = glut_mqo_rot[3][1]
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[4][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * glut_start * SAMP + 2.0 * GLUT_PI / 3.0)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[5][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * glut_start * SAMP)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[6][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * glut_start * SAMP + 2.0 * GLUT_PI / 3.0)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[7][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * glut_start * SAMP + 4.0 * GLUT_PI / 3.0)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
	} else {
		glut_rot[3][1] = glut_mqo_rot[3][1]
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[4][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * (AnimationCounter - (glut_end - glut_start)) * SAMP + 2.0 * GLUT_PI / 3.0)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[5][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * (AnimationCounter - (glut_end - glut_start)) * SAMP)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[6][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 *(AnimationCounter - (glut_end - glut_start)) * SAMP + 2.0 * GLUT_PI / 3.0)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
		glut_rot[7][1] = glut_mqo_rot[3][1] 
					+ 5.0 * sin(2.0 * GLUT_PI * 8.0 * (AnimationCounter - (glut_end - glut_start)) * SAMP + 4.0 * GLUT_PI / 3.0)
					+ atan2(glut_mqo_pos[3][1] + glut_radius + glut_path, glut_mqo_pos[3][0]) * 180.0 / GLUT_PI - 90.0;
	}

	if (AnimationCounter > (int)(1.0 / glut_freq / SAMP) + (glut_end - glut_start)) {
		AnimationFlag = ANIMATION_OFF;
		AnimationCounter = 0;
	}

	if (AnimationFlag2 == ANIMATION_ON) {
		if (b_pos[2] <= b_d[2] / 2.0) {
			b_pos[2] = b_d[2] / 2.0;
//			b_pos[2] = b_vel - 0.5 * 9.8 * 100.0 * (AnimationCounter2 * SAMP) * (AnimationCounter2 * SAMP) + 350.0;
		} else {
			b_vel = 9.8 * AnimationCounter2 * SAMP; 
			b_pos[2] = -0.5 * 9.8 * 150.0 * (AnimationCounter2 * SAMP) * (AnimationCounter2 * SAMP) + 800.0;
		}
	} else if (AnimationFlag2 == ANIMATION_OFF) {
		b_pos[2] = 800.0;
		b_vel = 0.0;
		AnimationCounter2 = 0;
	}

	if (AnimationFlag3 == ANIMATION_ON) {
		if (glut_mqo_pos[8][2] <= 150.0) {
			glut_mqo_pos[8][2] = 150.0;
		} else {
			glut_mqo_pos[8][2] = -0.5 * 9.8 * 150.0 * (AnimationCounter3 * SAMP) * (AnimationCounter3 * SAMP) + 800.0;
		}
	} else if (AnimationFlag3 == ANIMATION_OFF) {
		glut_mqo_pos[8][2] = 800.0;
		AnimationCounter3 = 0;
	}	

	//  Select normal vision or stereovision
	if (GLUT_WindowViewData[18] == GLUT_STEREOVISION_OFF) {
		num = 1;
	} else if (GLUT_WindowViewData[18] == GLUT_STEREOVISION_SBS) {
		num = 2;
	}

	if (ConditionFlag == 1 || ConditionFlag == 2) {
		for (unsigned int i = 0; i < 3; i++) {
			glut_size[2][i] = 0.0;
		}
	} else if (ConditionFlag == 1 || ConditionFlag == 2) {
		for (unsigned int i = 0; i < 3; i++) {
			glut_size[1][i] = 0.0;
		}
	}

	//  Draw 3D graphics
	for (unsigned int i = 0; i < num; i++) {
		GLUT_dispFlag = i;

		///////////////////////////////////////////////////////////////////////////////////////////
		//  Step 1: Create depth map
		///////////////////////////////////////////////////////////////////////////////////////////////
		//  Clear depth buffer
		glClear(GL_DEPTH_BUFFER_BIT);

		//  Buffer current viewport
//		glGetIntegerv(GL_VIEWPORT, viewport);
  
		//  Configure the viewport in texture size
		glViewport(0, 0, GLUT_texWidth, GLUT_texHeight);

		//  Buffer current perspective transformation matrix
//		glGetDoublev(GL_PROJECTION_MATRIX, projection);
  
		//  Configure the perspective transformation matrix as unit matrix 
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
 
		//  Configure modelview transformation matrix based on the position of light source
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluPerspective(GLUT_fov, 
			GLUT_texWidth / GLUT_texHeight, GLUT_WindowViewData[3], GLUT_WindowViewData[4]);
		gluLookAt(GLUT_WindowViewData[11], 
			GLUT_WindowViewData[12], GLUT_WindowViewData[13], 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

		//  Buffer modelview matrix for Step 3 
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

		//  Keep frame buffer because only the information of depth buffer is needed
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	
		//  Turn off the light 
		glDisable(GL_LIGHTING);
	
		//  Buffer the depth of back face in the depth buffer
		glCullFace(GL_FRONT);

		//  Draw the scene for creating the depth map
		TestModel.DrawFloor(GLUT_WindowViewData);
		if (VisualFlag == 1) {
			TestModel.DrawMqoModel(glut_mqo_pos, glut_rot, glut_size, GLUT_WindowViewData);
			TestModel.DrawSphere(16, b_pos, b_rot, b_d, b_color, GLUT_WindowViewData);
		}

		//  Transfer the information of depth buffer into texture memory 
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, GLUT_texWidth, GLUT_texHeight, 0);

 		///////////////////////////////////////////////////////////////////////////////////////////
		//  Step 2: Draw entire space
		///////////////////////////////////////////////////////////////////////////////////////////
		//  Return to the normal configuration 
		GLUT_Resize(GLUT_WindowViewData[0], GLUT_WindowViewData[1]);

//		GLUT_SetCamera(i, GLUT_WindowViewData);
		glCullFace(GL_BACK);
		glColorMask(1, 1, 1, 1); 
//		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
//		glMatrixMode(GL_PROJECTION);
//		glLoadMatrixd(projection);
//		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//		glEnable(GL_LIGHTING);
//		glCullFace(GL_BACK);

		//  Turn on the light source
		glEnable(GL_LIGHTING);

		//  Clear color and depth bufferes
		if (i == 0) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		GLUT_SetLightSource(GLUT_WindowViewData); 

		glShadeModel(GL_SMOOTH);

//		if(flagFlat) glShadeModel(GL_FLAT);
//		else glShadeModel(GL_SMOOTH);

		//  Draw the scene for creating the depth map
		TestModel.DrawFloor(GLUT_WindowViewData);
		if (VisualFlag == 1) {
			TestModel.DrawMqoModel(glut_mqo_pos, glut_rot, glut_size, GLUT_WindowViewData);
			TestModel.DrawSphere(16, b_pos, b_rot, b_d, b_color, GLUT_WindowViewData);
		}

 		///////////////////////////////////////////////////////////////////////////////////////////
		//  Step 3: Draw shadows
		///////////////////////////////////////////////////////////////////////////////////////////
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
	 
		//  Put the range [-1,1] in texture coordinate system into the range [0,1]
		glTranslated(0.5, 0.5, 0.5);
		glScaled(0.5, 0.5, 0.5);
  
		//  Multiply the modelview transfomation matrix by the perspective transformation matrix
		glMultMatrixd(modelview);
  
		//  Return to the modelview transformation matrix
		glMatrixMode(GL_MODELVIEW);
  
		//  Activate automatic generation of texture mapping and texture coordinate system
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glEnable(GL_TEXTURE_GEN_Q);
	  
		//  Activate alpha test and draw only shadow area
		glEnable(GL_ALPHA_TEST);
  
		//  Change comparison function in order to draw the lighted parts on the object
		glDepthFunc(GL_LEQUAL);

		//  Set the diffusion and specular lights of shadow
		glLightfv(GL_LIGHT0, GL_DIFFUSE, shadowDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, shadowSpecular);

		//  Draw the scene for creating the depth map
		if (GLUT_WindowViewData[17] == GLUT_SHADOW_ON) {
			TestModel.DrawFloor(GLUT_WindowViewData);
			if (VisualFlag == 1) {
				TestModel.DrawMqoModel(glut_mqo_pos, glut_rot, glut_size, GLUT_WindowViewData);
				TestModel.DrawSphere(16, b_pos, b_rot, b_d, b_color, GLUT_WindowViewData);
			}
		}

		//  Return to the comparison function of depth
		glDepthFunc(GL_LESS);
	  
		//  Disable the alpha test
		glDisable(GL_ALPHA_TEST);

		//  Disable the automatic generation of texture mapping and texture coordinate system
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_GEN_Q);
		glDisable(GL_TEXTURE_2D);


		TestModel.DrawSphere(16, b_pos, b_rot, b_d, b_color, GLUT_WindowViewData);
	}

	//  Doulbe buffering
	glutSwapBuffers();
}

//  void GLUT_Display(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void GLUT_Idle(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Redraw 3D graphics
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_Idle(void)
{
	//  Redrawing 3D graphics
	glutPostRedisplay();
}

//  void GLUT_Idle(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void GLUT_Resize(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Resize the display window
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_Resize(int width, int height)
{
	double norm;
	double up[3];

	//  Put a limitation on the reduction of window size 
	if (width < GLUT_texWidth || height < GLUT_texHeight) {
		if (width < GLUT_texWidth){
			width = GLUT_texWidth;
		}
		if (height < GLUT_texHeight) {
			height = GLUT_texHeight;
		}
		glutReshapeWindow(width, height);
	}

	//  Update window size
	GLUT_WindowViewData[0] = width;
	GLUT_WindowViewData[1] = height;

	//  Set camera
	GLUT_SetCamera(GLUT_dispFlag, GLUT_WindowViewData);
}

//  void GLUT_Resize(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void GLUT_Keyboard(unsigned char, int, int)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Functions for keystroke
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_Keyboard(unsigned char key, int x, int y)
{
	switch((unsigned char)key) {
		case 'g':
			if (AnimationFlag == ANIMATION_OFF) {
				AnimationFlag = ANIMATION_ON;
			} else if (AnimationFlag == ANIMATION_ON) {
				AnimationFlag = ANIMATION_OFF;
			}
			break;

		case 'b':
			if (AnimationFlag2 == ANIMATION_OFF) {
				AnimationFlag2 = ANIMATION_ON;
			} else if (AnimationFlag2 == ANIMATION_ON) {
				AnimationFlag2 = ANIMATION_OFF;
			}
			break;

		case 'k':
			if (AnimationFlag3 == ANIMATION_OFF) {
				AnimationFlag3 = ANIMATION_ON;
			} else if (AnimationFlag3 == ANIMATION_ON) {
				AnimationFlag3 = ANIMATION_OFF;
			}
			break;

		case '1':
			dForceAmplitude = 0.0;
			break;

		case '2':
			dForceAmplitude = 0.5;
			break;

		case '3':
			dForceAmplitude = 1.0;
			break;

		case '4':
			dForceAmplitude = 2.0;
			break;

		case 'd':		
			if (GLUT_WindowViewData[18] == GLUT_STEREOVISION_OFF) {
				GLUT_WindowViewData[18] = GLUT_STEREOVISION_SBS;	
			} else if  (GLUT_WindowViewData[18] == GLUT_STEREOVISION_SBS) {
				GLUT_WindowViewData[18] = GLUT_STEREOVISION_OFF;
			}
		default:
			break;
	}
}

//  void GLUT_Keyboard(unsigned char, int, int)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void GLUT_Special(int, int, int)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Functions for special keys
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_Special(int key, int x, int y)
{
	switch(key) {
		default:
			break;
	}
}

//  void GLUT_Special(int, int, int)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void GLUT_Mouse(int, int, int, int)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Read mouse actions
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_Mouse(int button, int state, int x, int y)
{
	switch (button) {
		default:
			break;
	}

}

//  void GLUT_Mouse(int, int, int, int)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void GLUT_Motion(int, int)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Functions for mouse actions
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_Motion(int x, int y)
{

}

//  void GLUT_Motion(int, int)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Functions for rendering 3D graphics
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_Draw3DSpace(double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Start rendering of 3D graphics
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_Draw3DSpace(double *setData)
{
	int argc = 1;
	char *argv[] = {"Window for 3D Graphics"};

	//  Initialize GLUT library
	glutInit(&argc, argv);

	//  Configure display mode
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//  Configure window size
	glutInitWindowSize(GLUT_WindowViewData[0], GLUT_WindowViewData[1]);
	
	//  Configure window position
	glutInitWindowPosition(GLUT_VIEWWND_X, GLUT_VIEWWND_Y);
	
	//  Create a display window based on the configured size and position
	glutCreateWindow(argv[0]);
  
	//  Resize the display window
	glutReshapeFunc(GLUT_Resize);
  
	//  Display 3D graphics
	glutDisplayFunc(GLUT_Display);
  
	//  Use keyboard function
	glutKeyboardFunc(GLUT_Keyboard);
	
	//  Use special keys
	glutSpecialFunc(GLUT_Special);
	
	//  Use mouse operation
	glutMouseFunc(GLUT_Mouse);
	glutMotionFunc(GLUT_Motion);

	//  Redraw 3D graphics
	glutIdleFunc(GLUT_Idle);
	
	//  Initialize rendering of 3D graphics
	GLUT_Init(setData);

    //  Get device context handle
    HDC glDC = wglGetCurrentDC();
 
    //  Get window handle
    HWND glWnd = WindowFromDC(glDC);
 
    //  Change property and position of window
    SetWindowLong(glWnd, GWL_STYLE, WS_POPUP);
    SetWindowPos(glWnd, HWND_TOP, GLUT_VIEWWND_X, GLUT_VIEWWND_Y,
		GLUT_WindowViewData[0], GLUT_WindowViewData[1], SWP_SHOWWINDOW);
	
	//  Loop function
	glutMainLoop();
}

//	void GLUT_Draw3DSpace(double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void GLUT_Init(double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Initialize rendering of 3D graphics
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_Init(double *setData)
{
	//  Configure color of background (white)
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
//	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

	//  Configure the light source
	GLUT_SetLightSource(setData);
	
	//  Initialize depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);

	//  Configure shadow map
	GLUT_SetShadowMap(setData);
}

//  void GLUT_Init(double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_SetLightSource(double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Configure light source
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_SetLightSource(double *setData)
{
	float lightDiffuse0[] = {1.0f, 1.0f, 1.0f, 1.0f};		//  Diffusion light
	float lightAmbient0[] = {0.2f, 0.2f, 0.2f, 1.0f};		//  Ambient light
	float lightSpecular0[] = {0.4f, 0.4f, 0.4f, 1.0f};		//  Specular light
//	float lightSpecular0[] = {1.0f, 1.0f, 1.0f, 1.0f};		//  Specular light

//	float lightDiffuse0[] = {1.0f, 1.0f, 1.0f, 1.0f};		//  Diffusion light
//	float lightAmbient0[] = {0.4f, 0.4f, 0.4f, 1.0f};		//  Ambient light
//	float lightSpecular0[] = {1.0f, 1.0f, 1.0f, 1.0f};		//  Specular light
	float lightPos[4];

	//  Set the light position and type
	lightPos[0] = (float)setData[11];
	lightPos[1] = (float)setData[12];
	lightPos[2] = (float)setData[13];
	lightPos[3] = (float)setData[14];

	//  Set the light source
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	//  Turn on the light source 0
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

//	void GLUT_SetLightSource(double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_SetShadowMap(double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Configure shadow map
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_SetShadowMap(double *setData)
{
	//  Allocate texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, setData[0], 
		setData[1], 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	//  Configure scaling method of texture 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
  
	//  Configure repeating method of texture 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//  Configure texture coordinate sysstem to view coordinate system
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	
	//  Coefficient for creating texture
	static const GLdouble param[][4] = {
		{1.0, 0.0, 0.0, 0.0},
		{0.0, 1.0, 0.0, 0.0},
		{0.0, 0.0, 1.0, 0.0},
		{0.0, 0.0, 0.0, 1.0}
	};
	glTexGendv(GL_S, GL_EYE_PLANE, param[0]);
	glTexGendv(GL_T, GL_EYE_PLANE, param[1]);
	glTexGendv(GL_R, GL_EYE_PLANE, param[2]);
	glTexGendv(GL_Q, GL_EYE_PLANE, param[3]);

	//  Compare R value with texel value 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);  
	
	//  R value > texel value => Shadow 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
	
	//  Get alpha value 
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);
	
	//  Configure alpha test
	glAlphaFunc(GL_GREATER, 0.5);

	//  Activate automatic generation of texture mapping and texture coordinate system
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);
//	glEnable(GL_TEXTURE_GEN_R);
//	glEnable(GL_TEXTURE_GEN_Q);
}

//	void GLUT_SetShadowMap(double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_SetCamera(int, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Set camera
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_SetCamera(int num, double *setData)
{
	double norm;
	double up[3];

	//  Caluclate view up vector
	up[0] = -(setData[8] 
			- setData[5]) * (setData[10] - setData[7]);
	up[1] = -(setData[9] 
			- setData[6]) * (setData[10] - setData[7]);
	up[2] = (setData[8] 
			- setData[5]) * (setData[8] - setData[5]) 
			+ (setData[9] - setData[6]) * (setData[9] 
			- setData[6]);
	norm = sqrt(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
	for (unsigned int i = 0; i < 3; i++) {
		up[i] /= norm;
	}

	//  Reconfigure the viewpoint
	if (setData[18] == GLUT_STEREOVISION_OFF) {
		glViewport(0, 0, setData[0], setData[1]);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(setData[2], 
			(double)(setData[0] / setData[1]), setData[3], setData[4]);
		gluLookAt(setData[5], setData[6], setData[7], setData[8], 
			setData[9], setData[10], up[0], up[1], up[2]);
	} else if (setData[18] == GLUT_STEREOVISION_SBS) {
		glViewport((int)(setData[0] / 2.0) * num, 0, (int)(setData[0] / 2.0), setData[1]);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(setData[2], (double)(setData[0] / setData[1]), setData[3], setData[4]);
		gluLookAt(setData[5], setData[6] - setData[15] * pow(-1.0, (double)num), 
			setData[7], setData[8], setData[9], setData[10], up[0], up[1], up[2]);
	}
}

//	void GLUT_SetCamera(int, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_InitViewData(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Initialize view data for rendering
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_InitViewData(void)
{
	GLUT_ViewData[0] = GLUT_VIEWWND_W;					//  Width of display
	GLUT_ViewData[1] = GLUT_VIEWWND_H;					//  Height of display
	GLUT_ViewData[2] = GLUT_ZOOM_DEF;					//  Field of view
	GLUT_ViewData[3] = 1.0;								//  Near screen
	GLUT_ViewData[4] = 100.0;							//  Far screen
	GLUT_ViewData[5] = 563.0;							//  view[0]
	GLUT_ViewData[6] = 0.0;								//  view[1]
	GLUT_ViewData[7] = 205.0;							//  view[2]
	GLUT_ViewData[8] = 0.0;								//  cut[0]
	GLUT_ViewData[9] = 0.0;								//  cut[1]
	GLUT_ViewData[10] = 0.0;							//  cut[2]
	GLUT_ViewData[11] = 250.0;							//  X position of light source
	GLUT_ViewData[12] = 250.0;							//  Y position of light source
	GLUT_ViewData[13] = 1000.0;							//  Z position of light source
	GLUT_ViewData[14] = 1.0;							//  Type of light source
	GLUT_ViewData[15] = GLUT_PARALLAX_DEF;				//  Parallax
	GLUT_ViewData[16] = GLUT_CHECK_FLOOR;				//  Type of floor
	GLUT_ViewData[17] = GLUT_SHADOW_ON;					//  Shadow configuration (on/off)
	GLUT_ViewData[18] = GLUT_STEREOVISION_SBS;			//  Stereovision (off/side-by-side/time-sharing system)

	GLUT_WindowViewData[0] = GLUT_VIEWWND_W;			//  Width of display
	GLUT_WindowViewData[1] = GLUT_VIEWWND_H;			//  Height of display
	GLUT_WindowViewData[2] = GLUT_ZOOM_DEF;				// Field of view
	GLUT_WindowViewData[3] = 1.0;						//  Near screen
	GLUT_WindowViewData[4] = 100.0;						//  Far screen
	GLUT_WindowViewData[5] = 563.0;						//  view[0]
	GLUT_WindowViewData[6] = 0.0;						//  view[1]
	GLUT_WindowViewData[7] = 205.0;						//  view[2]
	GLUT_WindowViewData[8] = 0.0;						//  cut[0]
	GLUT_WindowViewData[9] = 0.0;						//  cut[1]
	GLUT_WindowViewData[10] = 0.0;						//  cut[2]
	GLUT_WindowViewData[11] = 250.0;					//  X position of light source
	GLUT_WindowViewData[12] = 250.0;					//  Y position of light source
	GLUT_WindowViewData[13] = 1000.0;					//  Z position of light source
	GLUT_WindowViewData[14] = 1.0;						//  Type of light source
	GLUT_WindowViewData[15] = GLUT_PARALLAX_DEF;		//  Parallax
	GLUT_WindowViewData[16] = GLUT_CHECK_FLOOR;			//  Type of floor
	GLUT_WindowViewData[17] = GLUT_SHADOW_ON;			//  Shadow configuration (on/off)
	GLUT_WindowViewData[18] = GLUT_STEREOVISION_SBS;	//  Stereovision (off/side-by-side/time-sharing system)
}

//	void GLUT_InitViewData(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void GLUT_InitLineOfSight(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Configure default line of sight
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_InitLineOfSight(void)
{
	GLUT_distC = GLUT_DOLLY_DEF;
	GLUT_thetaC = GLUT_TUMBLE_DEF;
	GLUT_phaiC = GLUT_UPDOWN_DEF;
}

//  void GLUT_InitLineOfSight(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL GLUT_ConfigureCamera(int, int, float, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Adjust line of sight
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL GLUT_ConfigureCamera(int TOCW, int TOSW, float num, double *setData)
{
	double view_dumy[3], cut_dumy;

	switch (TOCW) {
		case GLUT_DOLLY:
			if (TOSW == GLUT_DIF) {
				if ((GLUT_distC + num) >= GLUT_DOLLY_MIN && (GLUT_distC + num) <= GLUT_DOLLY_MAX){
					GLUT_distC += num; 
				}
			} else if (TOSW == GLUT_ABS) {
				if ((num >= GLUT_DOLLY_MIN) && (num <= GLUT_DOLLY_MAX)) {
					GLUT_distC = num; 
				}
			} else {
				return FALSE;
			}

			//  Caluclate field of view
			setData[3] = 0.2 * GLUT_distC;					//  Near screen
			setData[4] = GLUT_distC + 500;					//  Far screen

			for (unsigned int i = 0; i < 3; i++) {
				view_dumy[i] = setData[i + 5];
			}
			setData[5] = setData[8]
				+ GLUT_distC * cos(GLUT_PI * GLUT_thetaC / 180) * cos(GLUT_PI * GLUT_phaiC / 180);
			setData[6] = setData[9] 
				+ GLUT_distC * sin(GLUT_PI * GLUT_thetaC / 180) * cos(GLUT_PI * GLUT_phaiC / 180);
			setData[7] = setData[10] 
				+ GLUT_distC * sin(GLUT_PI * GLUT_phaiC / 180);

			if (setData[7] < 0) {
				for (unsigned int i = 0; i < 3; i++){
					setData[i + 5] = view_dumy[i];
				}
			}
			break;
			
		case GLUT_PAN:
			if (TOSW == GLUT_DIF) {
				if (((GLUT_thetaC + num) >= GLUT_PAN_MIN) && ((GLUT_thetaC + num) <= GLUT_PAN_MAX)) {
					GLUT_thetaC += num;
				}
			} else if (TOSW == GLUT_ABS) {
				if ((num >= GLUT_PAN_MIN) && (num <= GLUT_PAN_MAX)) {
					GLUT_thetaC = num;
				}
			} else {
				return FALSE;
			}
	
			setData[8] = setData[5] 
				- GLUT_distC * cos(GLUT_PI * GLUT_thetaC / 180) * cos(GLUT_PI * GLUT_phaiC / 180);
			setData[9] = setData[6] 
				- GLUT_distC * sin(GLUT_PI * GLUT_thetaC / 180) * cos(GLUT_PI * GLUT_phaiC / 180);
			setData[10] = setData[7] 
				- GLUT_distC * sin(GLUT_PI * GLUT_phaiC / 180);
			break;
			
		case GLUT_TUMBLE:
			if (TOSW == GLUT_DIF) {
				if (((GLUT_thetaC + num) >= GLUT_TUMBLE_MIN) && ((GLUT_thetaC + num) <= GLUT_TUMBLE_MAX)) {
					GLUT_thetaC += num;
				}
			} else if (TOSW == GLUT_ABS) {
				if ((num >= GLUT_TUMBLE_MIN) && (num <= GLUT_TUMBLE_MAX)) {
					GLUT_thetaC = num;
				}
			} else {
				return FALSE;
			}
	
			for (unsigned int i = 0; i < 3; i++){
				view_dumy[i] = setData[i + 5];
			}
			setData[5] = setData[8] 
				+ GLUT_distC * cos(GLUT_PI * GLUT_thetaC / 180) * cos(GLUT_PI * GLUT_phaiC / 180);
			setData[6] = setData[9] 
				+ GLUT_distC * sin(GLUT_PI * GLUT_thetaC / 180) * cos(GLUT_PI * GLUT_phaiC / 180);
			setData[7] = setData[10] 
				+ GLUT_distC * sin(GLUT_PI * GLUT_phaiC / 180);

			if (setData[7] < 0) {
				for (unsigned int i = 0; i < 3; i++) {
					setData[i + 5] = view_dumy[i];
				}
			}
			break;
			
		case GLUT_ZOOM:
			if (TOSW == GLUT_DIF) {
				if (((setData[2] + num) >= GLUT_ZOOM_MIN) && ((setData[2] + num) <= GLUT_ZOOM_MAX)) {
					setData[2] += num;
				}
			} else if (TOSW == GLUT_ABS) {
				if ((num >= GLUT_ZOOM_MIN) && (num <= GLUT_ZOOM_MAX)) {
					setData[2] = num;
				}
			} else {
				return FALSE;
			}
			break;

		case GLUT_UPDOWN:
			if (TOSW == GLUT_DIF) {
				if( ((GLUT_phaiC + num) >= GLUT_UPDOWN_MIN) && ((GLUT_phaiC + num) <= GLUT_UPDOWN_MAX) ){
					GLUT_phaiC += num;
				}
			} else if (TOSW == GLUT_ABS) {
				if (num >= GLUT_UPDOWN_MIN && num <= GLUT_UPDOWN_MAX) {
					GLUT_phaiC = num;
				}
			} else {
				return FALSE;
			}

			for (unsigned int i = 0; i < 3; i++) {
				view_dumy[i] = setData[i + 5];
			}
			setData[5] = setData[8] 
				+ GLUT_distC * cos(GLUT_PI * GLUT_thetaC / 180) * cos(GLUT_PI * GLUT_phaiC / 180);
			setData[6] = setData[9] 
				+ GLUT_distC * sin(GLUT_PI * GLUT_thetaC / 180) * cos(GLUT_PI * GLUT_phaiC / 180);
			setData[7] = setData[10] 
				+ GLUT_distC*  sin(GLUT_PI * GLUT_phaiC / 180);

			if (setData[7] < 0) {
				for (unsigned int i = 0; i < 3; i++) {
					setData[i + 5] = view_dumy[i];
				}
			}
			break;

		case GLUT_UPDOWNTILT:
			if (TOSW == GLUT_DIF) {
				if (((GLUT_phaiC + num) >= GLUT_UPDOWNTILT_MIN) && ((GLUT_phaiC + num) <= GLUT_UPDOWNTILT_MAX) ){
					GLUT_phaiC += num;
				}
			} else if (TOSW == GLUT_ABS) {
				if (num >= GLUT_UPDOWNTILT_MIN && num <= GLUT_UPDOWNTILT_MAX) {
					GLUT_phaiC = num;
				}
			} else {
				return FALSE;
			}

			setData[8] = setData[5] 
				- GLUT_distC * cos(GLUT_PI * GLUT_thetaC / 180) * cos(GLUT_PI * GLUT_phaiC / 180);
			setData[9] = setData[6] 
				- GLUT_distC * sin(GLUT_PI * GLUT_thetaC / 180) * cos(GLUT_PI * GLUT_phaiC / 180);
			setData[10] = setData[7] 
				- GLUT_distC * sin(GLUT_PI * GLUT_phaiC / 180);
			break;

		case GLUT_XTRACK:
			if( TOSW == GLUT_DIF) {
				if (((setData[5] + num) > GLUT_XTRACK_MIN) && ((setData[5] + num) < GLUT_XTRACK_MAX)) {
					setData[5] += num;
					setData[8] += num;
				}
			} else if (TOSW == GLUT_ABS) {
				if (num > GLUT_XTRACK_MIN && num < GLUT_XTRACK_MAX) {
					view_dumy[0] = setData[5];
					cut_dumy = setData[8];
					setData[5] = num;
					setData[8] = setData[8] + (num - view_dumy[0]);
				}
			} else {
				return FALSE;
			}
			break;
		
		case GLUT_YTRACK:
			if (TOSW == GLUT_DIF) {
				if (((setData[6] + num) > GLUT_YTRACK_MIN) && ((setData[6] + num) < GLUT_YTRACK_MAX)) {
					setData[6] += num;
					setData[9] += num;
				}
			} else if (TOSW == GLUT_ABS) {
				if (num > GLUT_YTRACK_MIN && num < GLUT_YTRACK_MAX) {
					view_dumy[1] = setData[6];
					cut_dumy = setData[9];
					setData[6] = num;
					setData[9] = setData[9] + (num - view_dumy[1]);				}
			} else {
				return FALSE;
			}
			break;

		case GLUT_ZTRACK:
			if (TOSW == GLUT_DIF) {
				if (((setData[7] + num) >= GLUT_ZTRACK_MIN) && ((setData[10] + num ) >= GLUT_ZTRACK_MIN)
					&& ((setData[7] + num) < GLUT_ZTRACK_MAX)) {
					setData[7] += num;
					setData[10] += num;
				}
			} else if (TOSW == GLUT_ABS) {
				if (num >= GLUT_ZTRACK_MIN && num < GLUT_ZTRACK_MAX) {
					view_dumy[2] = setData[7];
					cut_dumy = setData[10];
					setData[7] = num;
					setData[10] = setData[10] + (num - view_dumy[2]);
				}
			} else {
				return FALSE;
			}
			break;

		case GLUT_PARALLAX:
			if (TOSW == GLUT_DIF) {
				if (((setData[15] + num) >= GLUT_PARALLAX_MIN) 
					&& ((setData[15] + num) <= GLUT_PARALLAX_MAX)) {
					setData[15] += num;
				}
			} else if (TOSW == GLUT_ABS) {
				if ((num >= GLUT_PARALLAX_MIN) && (num <= GLUT_PARALLAX_MAX)) {
					setData[15] = num;
				}
			} else {
				return FALSE;
			}
			break;


		default:
			break;
	}

	return TRUE;
}

//	BOOL GLUT_ConfigureCamera(int, int, float, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Functions for dialogbox
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL GLUT_MakeOpenGLConfigDlg(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Make window for configuring camerawork
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL GLUT_MakeOpenGLConfigDlg(HWND hWnd)
{
	HWND hDlgWnd;
	HINSTANCE hIns;

	if (GLUT_flag_GLDlg == 0) {
		hIns = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		hDlgWnd = CreateDialog(hIns, 
			MAKEINTRESOURCE(IDD_CAMERA_WORK), hWnd, (DLGPROC)GLUT_ConfigDlgProc);

		if (hDlgWnd == NULL) {
			MessageBox(hWnd, TEXT("Configuration Dialog Box cannot be Opened"),
				TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		GLUT_flag_GLDlg = 1;

		ShowWindow(hDlgWnd, SW_SHOW);
		UpdateWindow(hDlgWnd);
		InvalidateRect(hWnd, NULL, TRUE);
	} else if (GLUT_flag_GLDlg == 1) {
		MessageBox(hWnd, TEXT("Configuration Dialog Box is already Opened"),
				TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	} else {
		return FALSE;
	}

	return TRUE;
}

//	BOOL GLUT_MakeOpenGLConfigDlg(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	LRESULT CALLBACK GLUT_ConfigDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Procedure of configuration dialog box
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK GLUT_ConfigDlgProc(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HWND hWndFlag;
	char *dummy = new char[1024];
	char str[64];

	hTmpWnd = hDlgWnd;

    switch (msg) {
		case WM_INITDIALOG:
			SendMessage(GetDlgItem(hDlgWnd,	IDC_GRID), 
				BM_SETCHECK, (WPARAM)GLUT_FloorType[0], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_CHECK), 
				BM_SETCHECK, (WPARAM)GLUT_FloorType[1], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_SHADOW_OFF), 
				BM_SETCHECK, (WPARAM)GLUT_Shadow[0], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_SHADOW_ON), 
				BM_SETCHECK, (WPARAM)GLUT_Shadow[1], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_STEREOVISION_OFF), 
				BM_SETCHECK, (WPARAM)GLUT_Stereovision[0], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_STEREOVISION_ON), 
				BM_SETCHECK, (WPARAM)GLUT_Stereovision[1], 0L);
			GLUT_InitViewData();
			GLUT_InitLineOfSight();
			GLUT_InitScrollBar(hDlgWnd);
			GLUT_ConfigureCamera(GLUT_DOLLY, GLUT_DIF, 0.0, GLUT_ViewData);
			GLUT_DisplayScrollBar(hDlgWnd);
			break;

		case WM_COMMAND:
			switch (LOWORD(wp)) {
				case IDC_INIT_VIEWPOINT:
					SendMessage(hDlgWnd, WM_INITDIALOG, 0, 0L);
					GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)),
						GetDlgItem(hDlgWnd, IDC_PICT_BOX), GLUT_ViewData);
					break;
				
				case IDC_GRID:
					GLUT_ViewData[16] = GLUT_GRID_FLOOR;
					GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)),
						GetDlgItem(hDlgWnd, IDC_PICT_BOX), GLUT_ViewData);
					break;

				case IDC_CHECK:
					GLUT_ViewData[16] = GLUT_CHECK_FLOOR;
					GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)),
						GetDlgItem(hDlgWnd, IDC_PICT_BOX), GLUT_ViewData);
					break;

				case IDC_SHADOW_OFF:
					GLUT_ViewData[17] = GLUT_SHADOW_OFF;
					GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)),
						GetDlgItem(hDlgWnd, IDC_PICT_BOX), GLUT_ViewData);
					break;

				case IDC_SHADOW_ON:
					GLUT_ViewData[17] = GLUT_SHADOW_ON;
					GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)),
						GetDlgItem(hDlgWnd, IDC_PICT_BOX), GLUT_ViewData);
					break;

				case IDC_STEREOVISION_OFF:
					GLUT_ViewData[18] = GLUT_STEREOVISION_OFF;
					GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)),
						GetDlgItem(hDlgWnd, IDC_PICT_BOX), GLUT_ViewData);
					break;

				case IDC_STEREOVISION_ON:
					GLUT_ViewData[18] = GLUT_STEREOVISION_SBS;
					GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)),
						GetDlgItem(hDlgWnd, IDC_PICT_BOX), GLUT_ViewData);
					break;

                default:
                    return FALSE;
            }
            break;

		case WM_HSCROLL:
			hWndFlag = (HWND)(lp);
			if (hWndFlag == GetDlgItem(hDlgWnd, IDC_SCROLLBAR1)) {
				GLUT_ScrollBarFlag = 0;
			} else if (hWndFlag == GetDlgItem(hDlgWnd, IDC_SCROLLBAR2)) {
				GLUT_ScrollBarFlag = 1;
			} else if (hWndFlag == GetDlgItem(hDlgWnd, IDC_SCROLLBAR3)) {
				GLUT_ScrollBarFlag = 2;
			} else if (hWndFlag == GetDlgItem(hDlgWnd, IDC_SCROLLBAR4)) {
				GLUT_ScrollBarFlag = 3;
			} else if (hWndFlag == GetDlgItem(hDlgWnd, IDC_SCROLLBAR5)) {
				GLUT_ScrollBarFlag = 4;
			} else if (hWndFlag == GetDlgItem(hDlgWnd, IDC_SCROLLBAR6)) {
				GLUT_ScrollBarFlag = 5;
			} else if (hWndFlag == GetDlgItem(hDlgWnd, IDC_SCROLLBAR7)) {
				GLUT_ScrollBarFlag = 6;
			} else if (hWndFlag == GetDlgItem(hDlgWnd, IDC_SCROLLBAR8)) {
				GLUT_ScrollBarFlag = 7;
			} else if (hWndFlag == GetDlgItem(hDlgWnd, IDC_SCROLLBAR9)) {
				GLUT_ScrollBarFlag = 8;
			} else if (hWndFlag == GetDlgItem(hDlgWnd, IDC_SCROLLBAR10)) {
				GLUT_ScrollBarFlag = 9;
			}

			switch (LOWORD(wp)) {
				case SB_LINELEFT:
					GLUT_ScrollBar[GLUT_ScrollBarFlag].nPos--;
					break;
				case SB_LINERIGHT:
					GLUT_ScrollBar[GLUT_ScrollBarFlag].nPos++;
					break;
				case SB_PAGELEFT:
					GLUT_ScrollBar[GLUT_ScrollBarFlag].nPos -= 5;
					break;
				case SB_PAGERIGHT:
					GLUT_ScrollBar[GLUT_ScrollBarFlag].nPos += 5;
					break;
				case SB_THUMBTRACK:
					GLUT_ScrollBar[GLUT_ScrollBarFlag].nPos = (int)(HIWORD(wp));
					break;
				default:
					break;
			}
			GLUT_DisplayScrollBar(hDlgWnd);
			break;
		
		case WM_CLOSE:
			GLUT_flag_GLDlg = 0;
            EndDialog(hDlgWnd, IDCANCEL);
            break;

		default:
			return FALSE;
	}

	return TRUE;
}

//	LRESULT CALLBACK GLUT_ConfigDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void GLUT_Draw3DSpaceDlg(HDC, HWND, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Draw 3D space in pictura box of configuration dialog box
///////////////////////////////////////////////////////////////////////////////////////////////////
void GLUT_Draw3DSpaceDlg(HDC hDC, HWND PICTURE, double *setData)
{
	RECT rt;

	GetClientRect(PICTURE, &rt);
	setData[0] = (double)rt.right;
	setData[1] = (double)rt.bottom;

	GLUT_Adjust3DSpace(GLUT_WindowViewData, setData);
}

//	void GLUT_Draw3DSpaceDlg(HDC, HWND, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL GLUT_Adjust3DSpace(double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Adjust 3D space to picture box
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL GLUT_Adjust3DSpace(double *setWndData, double *setDlgData)
{
	double width, height;

	width = setDlgData[0];
	height = setDlgData[1];

	if (GLUT_VIEWWND_W <= GLUT_VIEWWND_H) {
		width = GLUT_VIEWWND_W * height / GLUT_VIEWWND_H;
		setDlgData[0] = width;
	} else if (GLUT_VIEWWND_H < GLUT_VIEWWND_W) {
		height = (double)(GLUT_VIEWWND_H * width / GLUT_VIEWWND_W);
		setDlgData[1] = height;
	} else {
		return FALSE;
	}

	for (unsigned int i = 2; i < 19; i++) {
		setWndData[i] = setDlgData[i];
	}

	return TRUE;
}

//  BOOL GLUT_Adjust3DSpace(double *, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	int GLUT_InitScrollBar(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Initialize scroll bar
///////////////////////////////////////////////////////////////////////////////////////////////////
int GLUT_InitScrollBar(HWND hDlgWnd)
{
	for (unsigned int i = 0; i < 10; i++) {
		GLUT_ScrollBar[i].cbSize = sizeof(SCROLLINFO);
	}
	
	GLUT_ScrollBar[0].nPos = GLUT_DOLLY_DEF;
	GLUT_ScrollBar[1].nPos = GLUT_TUMBLE_DEF;
	GLUT_ScrollBar[2].nPos = GLUT_TUMBLE_DEF;
	GLUT_ScrollBar[3].nPos = GLUT_ZOOM_DEF;
	GLUT_ScrollBar[4].nPos = GLUT_UPDOWN_DEF;
	GLUT_ScrollBar[5].nPos = GLUT_UPDOWN_DEF;
	GLUT_ScrollBar[6].nPos = (int)GLUT_ViewData[5];
	GLUT_ScrollBar[7].nPos = (int)GLUT_ViewData[6];
	GLUT_ScrollBar[8].nPos = (int)GLUT_ViewData[7];
	GLUT_ScrollBar[9].nPos = GLUT_PARALLAX_DEF;

	GLUT_ScrollBar[0].nMax = GLUT_DOLLY_MAX;
	GLUT_ScrollBar[0].nMin = GLUT_DOLLY_MIN;
	GLUT_ScrollBar[1].nMax = GLUT_TUMBLE_MAX;
	GLUT_ScrollBar[1].nMin = GLUT_TUMBLE_MIN;
	GLUT_ScrollBar[2].nMax = GLUT_PAN_MAX;
	GLUT_ScrollBar[2].nMin = GLUT_PAN_MIN;
	GLUT_ScrollBar[3].nMax = GLUT_ZOOM_MAX;
	GLUT_ScrollBar[3].nMin = GLUT_ZOOM_MIN;
	GLUT_ScrollBar[4].nMax = GLUT_UPDOWN_MAX;
	GLUT_ScrollBar[4].nMin = GLUT_UPDOWN_MIN;
	GLUT_ScrollBar[5].nMax = GLUT_UPDOWNTILT_MAX;
	GLUT_ScrollBar[5].nMin = GLUT_UPDOWNTILT_MIN;
	GLUT_ScrollBar[6].nMax = GLUT_XTRACK_MAX;
	GLUT_ScrollBar[6].nMin = GLUT_XTRACK_MIN;
	GLUT_ScrollBar[7].nMax = GLUT_YTRACK_MAX;
	GLUT_ScrollBar[7].nMin = GLUT_YTRACK_MIN;
	GLUT_ScrollBar[8].nMax = GLUT_ZTRACK_MAX;
	GLUT_ScrollBar[8].nMin = GLUT_ZTRACK_MIN;
	GLUT_ScrollBar[9].nMax = GLUT_PARALLAX_MAX;
	GLUT_ScrollBar[9].nMin = GLUT_PARALLAX_MIN;

	GLUT_ScrollBar[0].fMask = SIF_ALL;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR1), SB_CTL, (LPSCROLLINFO)&GLUT_ScrollBar[0], TRUE);
	GLUT_ScrollBar[1].fMask = SIF_ALL;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR2), SB_CTL, (LPSCROLLINFO)&GLUT_ScrollBar[1], TRUE);
	GLUT_ScrollBar[2].fMask = SIF_ALL;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR3), SB_CTL, (LPSCROLLINFO)&GLUT_ScrollBar[2], TRUE);
	GLUT_ScrollBar[3].fMask = SIF_ALL;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR4), SB_CTL, (LPSCROLLINFO)&GLUT_ScrollBar[3], TRUE);
	GLUT_ScrollBar[4].fMask = SIF_ALL;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR5), SB_CTL, (LPSCROLLINFO)&GLUT_ScrollBar[4], TRUE);
	GLUT_ScrollBar[5].fMask = SIF_ALL;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR6), SB_CTL, (LPSCROLLINFO)&GLUT_ScrollBar[5], TRUE);
	GLUT_ScrollBar[6].fMask = SIF_ALL;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR7), SB_CTL, (LPSCROLLINFO)&GLUT_ScrollBar[6], TRUE);
	GLUT_ScrollBar[7].fMask = SIF_ALL;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR8), SB_CTL, (LPSCROLLINFO)&GLUT_ScrollBar[7], TRUE);
	GLUT_ScrollBar[8].fMask = SIF_ALL;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR9), SB_CTL, (LPSCROLLINFO)&GLUT_ScrollBar[8], TRUE);
	GLUT_ScrollBar[9].fMask = SIF_ALL;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR10), SB_CTL, (LPSCROLLINFO)&GLUT_ScrollBar[9], TRUE);

	GLUT_DisplayScrollBarStatus(hDlgWnd);

	return 0;
}

//	int GLUT_InitScrollBar(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	int GLUT_DisplayScrollBar(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Display scroll bar
///////////////////////////////////////////////////////////////////////////////////////////////////
int GLUT_DisplayScrollBar(HWND hDlgWnd)
{
	for (unsigned int i = 0; i < 10; i++) {
		if (GLUT_ScrollBar[i].nPos < GLUT_ScrollBar[i].nMin) {
			GLUT_ScrollBar[i].nPos = GLUT_ScrollBar[i].nMin;
		}
		if (GLUT_ScrollBar[i].nPos > GLUT_ScrollBar[i].nMax) {
			GLUT_ScrollBar[i].nPos = GLUT_ScrollBar[i].nMax;
		}
	}

	//  Tumble & Pan, Tilt & Updown
/*	if (GLUT_ScrollBarFlag == 1) {
		GLUT_ScrollBar[2].nPos = GLUT_ScrollBar[1].nPos;
	} else if (GLUT_ScrollBarFlag == 2) {
		GLUT_ScrollBar[1].nPos = GLUT_ScrollBar[2].nPos;
	} else if (GLUT_ScrollBarFlag == 4) {
		GLUT_ScrollBar[5].nPos = GLUT_ScrollBar[4].nPos;
	} else if (GLUT_ScrollBarFlag == 5) {
		GLUT_ScrollBar[4].nPos = GLUT_ScrollBar[5].nPos;
	}
*/
	//  Move scroll bar
	GLUT_ScrollBar[0].fMask = SIF_POS;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR1), SB_CTL,  &GLUT_ScrollBar[0], TRUE);
	GLUT_ScrollBar[1].fMask = SIF_POS;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR2), SB_CTL,  &GLUT_ScrollBar[1], TRUE);
	GLUT_ScrollBar[2].fMask = SIF_POS;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR3), SB_CTL,  &GLUT_ScrollBar[2], TRUE);
	GLUT_ScrollBar[3].fMask = SIF_POS;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR4), SB_CTL,  &GLUT_ScrollBar[3], TRUE);
	GLUT_ScrollBar[4].fMask = SIF_POS;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR5), SB_CTL,  &GLUT_ScrollBar[4], TRUE);
	GLUT_ScrollBar[5].fMask = SIF_POS;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR6), SB_CTL,  &GLUT_ScrollBar[5], TRUE);
	GLUT_ScrollBar[6].fMask = SIF_POS;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR7), SB_CTL,  &GLUT_ScrollBar[6], TRUE);
	GLUT_ScrollBar[7].fMask = SIF_POS;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR8), SB_CTL,  &GLUT_ScrollBar[7], TRUE);
	GLUT_ScrollBar[8].fMask = SIF_POS;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR9), SB_CTL,  &GLUT_ScrollBar[8], TRUE);
	GLUT_ScrollBar[9].fMask = SIF_POS;
	SetScrollInfo(GetDlgItem(hDlgWnd, IDC_SCROLLBAR10), SB_CTL,  &GLUT_ScrollBar[9], TRUE);

	GLUT_DisplayScrollBarStatus(hDlgWnd);
	GLUT_ReconfigureCamera(hDlgWnd, GLUT_ViewData);

	if (GLUT_ScrollBarFlag == 6 || GLUT_ScrollBarFlag == 7 || GLUT_ScrollBarFlag == 8) {
		return 0;
	} else {
		GLUT_ScrollBar[6].nPos = (int)GLUT_ViewData[6];
		GLUT_ScrollBar[7].nPos = (int)GLUT_ViewData[7];
		GLUT_ScrollBar[8].nPos = (int)GLUT_ViewData[8];
	}

	return 0;
}

//	int GLUT_DisplayScrollBar(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	int GLUT_DisplayScrollBarStatus(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Display scroll bar status
///////////////////////////////////////////////////////////////////////////////////////////////////
int GLUT_DisplayScrollBarStatus(HWND hDlgWnd)
{
	char str[10][8];
	
	wsprintf(str[0], "%3d", GLUT_ScrollBar[0].nPos);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_DOLLY_EDIT), (LPCTSTR)str[0]);

	wsprintf(str[1], "%3d", GLUT_ScrollBar[1].nPos);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_TUMBLE_EDIT), (LPCTSTR)str[1]);

	wsprintf(str[2], "%3d", GLUT_ScrollBar[2].nPos);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_PAN_EDIT), (LPCTSTR)str[1]);

	wsprintf(str[3], "%3d", GLUT_ScrollBar[3].nPos);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_ZOOM_EDIT), (LPCTSTR)str[3]);

	wsprintf(str[4], "%3d", GLUT_ScrollBar[4].nPos);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_UPDOWN_EDIT), (LPCTSTR)str[4]);

	wsprintf(str[5], "%3d", GLUT_ScrollBar[5].nPos);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_UPDOWNTILT_EDIT), (LPCTSTR)str[5]);

	wsprintf(str[6], "%3d", GLUT_ScrollBar[6].nPos);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_X_TRACK_EDIT), (LPCTSTR)str[6]);

	wsprintf(str[7], "%3d", GLUT_ScrollBar[7].nPos);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_Y_TRACK_EDIT), (LPCTSTR)str[7]);

	wsprintf(str[8], "%3d", GLUT_ScrollBar[8].nPos);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_Z_TRACK_EDIT), (LPCTSTR)str[8]);

	wsprintf(str[9], "%3d", GLUT_ScrollBar[9].nPos);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_PARALLAX_EDIT), (LPCTSTR)str[9]);

	return 0;
}

//	int GLUT_DisplayScrollBarStatus(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	int GLUT_ReconfigureCamera(HWND, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Display scroll bar status
///////////////////////////////////////////////////////////////////////////////////////////////////
int GLUT_ReconfigureCamera(HWND hDlgWnd, double* setData)
{
	if (GLUT_ScrollBarFlag == 0) {
		GLUT_ConfigureCamera(GLUT_DOLLY, GLUT_ABS, (float)GLUT_ScrollBar[0].nPos, setData);
		GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)), 
			GetDlgItem(hDlgWnd, IDC_PICT_BOX), setData);
	} else if (GLUT_ScrollBarFlag == 1){
		GLUT_ConfigureCamera(GLUT_TUMBLE, GLUT_ABS, (float)GLUT_ScrollBar[1].nPos, setData);
		GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)),
			GetDlgItem(hDlgWnd, IDC_PICT_BOX), setData);
	} else if (GLUT_ScrollBarFlag == 2){
		GLUT_ConfigureCamera(GLUT_PAN, GLUT_ABS, (float)GLUT_ScrollBar[2].nPos, setData);
		GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)),
			GetDlgItem(hDlgWnd, IDC_PICT_BOX), setData);
	} else if (GLUT_ScrollBarFlag == 3){
		GLUT_ConfigureCamera(GLUT_ZOOM, GLUT_ABS, (float)GLUT_ScrollBar[3].nPos, setData);
		GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)), 
			GetDlgItem(hDlgWnd, IDC_PICT_BOX), setData);
	} else if (GLUT_ScrollBarFlag == 4){
		GLUT_ConfigureCamera(GLUT_UPDOWN, GLUT_ABS, (float)GLUT_ScrollBar[4].nPos, setData);
		GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)), 
			GetDlgItem(hDlgWnd, IDC_PICT_BOX), setData);
	} else if (GLUT_ScrollBarFlag == 5){
		GLUT_ConfigureCamera(GLUT_UPDOWNTILT, GLUT_ABS, (float)GLUT_ScrollBar[5].nPos, setData);
		GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)), 
			GetDlgItem(hDlgWnd, IDC_PICT_BOX), setData);
	} else if (GLUT_ScrollBarFlag == 6){
		GLUT_ConfigureCamera(GLUT_XTRACK, GLUT_ABS, (float)GLUT_ScrollBar[6].nPos, setData);
		GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)), 
			GetDlgItem(hDlgWnd, IDC_PICT_BOX), setData);
	} else if (GLUT_ScrollBarFlag == 7){
		GLUT_ConfigureCamera(GLUT_YTRACK, GLUT_ABS, (float)GLUT_ScrollBar[7].nPos, setData);
		GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)), 
			GetDlgItem(hDlgWnd, IDC_PICT_BOX), setData);
	} else if (GLUT_ScrollBarFlag == 8){
		GLUT_ConfigureCamera(GLUT_ZTRACK, GLUT_ABS, (float)GLUT_ScrollBar[8].nPos, setData);
		GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)), 
			GetDlgItem(hDlgWnd, IDC_PICT_BOX), setData);
	} else if (GLUT_ScrollBarFlag == 9){
		GLUT_ConfigureCamera(GLUT_PARALLAX, GLUT_ABS, (float)GLUT_ScrollBar[9].nPos, setData);
		GLUT_Draw3DSpaceDlg(GetDC(GetDlgItem(hDlgWnd, IDC_PICT_BOX)), 
			GetDlgItem(hDlgWnd, IDC_PICT_BOX), setData);
	}

	return 0;
}

//	int GLUT_ReconfigureCamera(HWND, double *)
///////////////////////////////////////////////////////////////////////////////////////////////////