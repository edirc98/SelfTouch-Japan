///////////////////////////////////////////////////////////////////////////////////////////////////
//  Graph.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Header of Graph.cpp
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



///////////////////////////////////////////////////////////////////////////////////////////////////
//  define

#define GRAPHINC


///////////////////////////////////////////////////////////////////////////////////////////////////
// variable

class tagGraph {
private:
	HWND hGraphWnd;						// Graph window handle
	float ScaleX, ScaleY;				// Scalling factor
	float dataXmin, dataXmax;			// X data limits
	float dataYmin, dataYmax;			// Y data limits
	UINT SizeX, SizeY;
	HBITMAP hGraphBMP;

public:
	BOOL Init(HWND, HINSTANCE, int, UINT, UINT, UINT, UINT, float, float, float, float);
	void Clear(void);
	void SetScale(float, float, float, float);
	BOOL Plot(float, float, COLORREF);
	BOOL Print(float, COLORREF);
	BOOL Draw(void);
	BOOL Paint(void);
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// function