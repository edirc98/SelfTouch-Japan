///////////////////////////////////////////////////////////////////////////////////////////////////
//  MainGraph.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Header of MainGraph.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Programmer:		Masayuki HARA (Assitant Professor)
//  Affiliation:	Higuchi & Yamamoto Lab. (Advanced Mechatoronics Lab.)
//					School of Engineering, The University of Tokyo
//  Created date:	13.01.2009
//	Updated date:	19.01.2009
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  include

#ifndef GRAPHINC
#include "Graph.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
// define

#define IDC_CLOSEGRAPH 10000


///////////////////////////////////////////////////////////////////////////////////////////////////
//  variable

class tagMainGraph {
private:
	tagGraph *hChildGraph;
	int NumData;
	UINT MainGraph_XSize;
	UINT MainGraph_YSize;
	
public:
	BOOL Active;
	HWND hParent;
	tagMainGraph();
	HWND hMainGraphWnd;				//  Graph window handle
	BOOL Init(HWND, HINSTANCE, int, int, float, float, float, float, char WndTitle[], UINT, UINT);
	void Clear(void);
	void SetScale(float, float, float, float);
	BOOL Plot(float, float *, COLORREF);
	BOOL Paint(void);
	BOOL Draw(void);
};

extern tagMainGraph WinGraph;

///////////////////////////////////////////////////////////////////////////////////////////////////
// function