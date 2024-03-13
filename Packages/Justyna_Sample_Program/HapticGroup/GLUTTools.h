///////////////////////////////////////////////////////////////////////////////////////////////////
//  GLUTTools.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Header of GLUTTools.cpp
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

//  Headers for Metaseq
#include "GLMetaseq.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//  define

#define GLUT_MODEL_NUM	9

#define GLUT_DOLLY_MAX	1000
#define GLUT_DOLLY_MIN	200

#define GLUT_TUMBLE_MAX	360
#define GLUT_TUMBLE_MIN	0

#define GLUT_PAN_MAX	360
#define GLUT_PAN_MIN	0

#define GLUT_ZOOM_MAX	100
#define GLUT_ZOOM_MIN	20

#define GLUT_UPDOWN_MAX	180
#define GLUT_UPDOWN_MIN	0

#define GLUT_UPDOWNTILT_MAX	180
#define GLUT_UPDOWNTILT_MIN	0

#define GLUT_XTRACK_MAX	2000
#define GLUT_XTRACK_MIN	0
#define GLUT_YTRACK_MAX	2000
#define GLUT_YTRACK_MIN	0
#define GLUT_ZTRACK_MAX	2000
#define GLUT_ZTRACK_MIN	20

#define GLUT_PARALLAX_MIN	0
#define GLUT_PARALLAX_MAX	100

#define GLUT_DOLLY_DEF	600
#define GLUT_TUMBLE_DEF	0
#define GLUT_ZOOM_DEF	25
#define GLUT_UPDOWN_DEF	50

#define GLUT_PARALLAX_DEF	25

#define GLUT_VIEWWND_X	600//1280
#define GLUT_VIEWWND_Y	420//0

#define GLUT_VIEWWND_W	640//1280//600//1024//400	
#define GLUT_VIEWWND_H	360//720//480//768//300

#define GLUT_SHADOW_OFF	0
#define GLUT_SHADOW_ON	1

#define GLUT_STEREOVISION_OFF	0
#define GLUT_STEREOVISION_SBS	1
#define GLUT_STEREOVISION_TSS	2

//  Type of camera work (TOCW)
enum GLUT_TOCW {GLUT_DOLLY, GLUT_PAN, GLUT_TUMBLE, GLUT_ZOOM,GLUT_UPDOWN, GLUT_UPDOWNTILT, 
GLUT_XTRACK, GLUT_YTRACK, GLUT_ZTRACK, GLUT_PARALLAX}; 

//  Type of switch (TOSW)
enum GLUT_TOSW {GLUT_DIF, GLUT_ABS};


///////////////////////////////////////////////////////////////////////////////////////////////////
//  variable

extern double GLUT_distC;
extern double GLUT_thetaC;
extern double GLUT_phaiC;

extern double GLUT_ViewData[19];
extern double GLUT_WindowViewData[19];

extern MQO_MODEL glut_mqo_model[GLUT_MODEL_NUM];
extern char glut_mqo_model_name[GLUT_MODEL_NUM][64];
extern double glut_mqo_pos[GLUT_MODEL_NUM][3];
extern double glut_mqo_rot[GLUT_MODEL_NUM][3];
extern double glut_mqo_size[GLUT_MODEL_NUM][3];


///////////////////////////////////////////////////////////////////////////////////////////////////
//  function

BOOL GLUT_MakeOpenGLConfigDlg(HWND);

void GLUT_InitViewData(void);
void GLUT_InitLineOfSight(void);
BOOL GLUT_ConfigureCamera(int, int, float, double *);

void GLUT_Draw3DSpace(double *);