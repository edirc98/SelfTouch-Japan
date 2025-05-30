///////////////////////////////////////////////////////////////////////////////////////////////////
//  WinStyleMain.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Header of WinStyleMain.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Programmer:		Masayuki HARA (Assitant Professor)
//  Affiliation:	Higuchi & Yamamoto Lab. (Advanced Mechatoronics Lab.)
//					School of Engineering, The University of Tokyo
//  Created date:	29.12.2008
//	Updated date:	29.12.2008
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  include


///////////////////////////////////////////////////////////////////////////////////////////////////
//  define

#define WM_ENDTHREAD 1000
#define MSG_OK 101
#define MSG_CANCEL 102

#define GRAPH_NUM 2
#define GRAPH_MAX 2000


///////////////////////////////////////////////////////////////////////////////////////////////////
//  variable

extern HWND hAppWnd;

extern BOOL GraphReset;

struct PrintThreadParmTag
{
	BOOL ThreadEnd;
	HWND hWnd;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//  function

HFONT SetFont(HDC, LPCTSTR, int, int, int, BOOL, BOOL);