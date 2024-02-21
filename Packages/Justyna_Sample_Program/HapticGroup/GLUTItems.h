///////////////////////////////////////////////////////////////////////////////////////////////////
//  GLUTItems.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Header of GLUTItems.cpp
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



///////////////////////////////////////////////////////////////////////////////////////////////////
//  define

#define GLUT_PI 3.1415926535

#define GLUT_GRID_FLOOR		0
#define GLUT_CHECK_FLOOR	1


///////////////////////////////////////////////////////////////////////////////////////////////////
//  class

class GLUT_ITEMS {
public:
	GLUT_ITEMS();													//  Constructor

	void DrawFloor(double *);										//  Draw grid/check floor
	void DrawCube(double *, double *, double *, double *, double *);//  Draw cube
	void DrawSphere(short, double *, double *, double *, double *, double *);
																	//  Draw sphere
	void DrawMqoModel(double pos[][3], double rot[][3], double size[][3], double *setData);
																	//  Draw mqo model

private:
	float SpecularLight[4];											//  Specular light of object
	float ShadowDiffuse[4];											//  Diffusion light of shadow
	double pmat[16];												//  Projection matrix

	void ConfigObjectParam(double *, double *, double *, double *);	//  Set object parameters
	void CalcShadowMatrix(double *, double *, double *);			//  Calculate shadow matrix
	void ConfigShadowParam(double *, double *, double *);			//  Set shadow parameter

	void UnitCube(void);											//  Unit cube
	void UnitSphere(short);											//  Unit sphere
};