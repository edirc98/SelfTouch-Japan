//////////////////////////////////////////////////////////////////////////////////////////////////
//  Graph.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
//  This source file includes functions for plotting graph on a window
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

#include <windows.h>
#include <stdio.h>

#include "Graph.h"
#include "WinStyleMain.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//  define

#define	marginXL 30
#define marginXR 15
#define marginYT 10
#define marginYB 20


///////////////////////////////////////////////////////////////////////////////////////////////////
//  variable



///////////////////////////////////////////////////////////////////////////////////////////////////
//  function

LRESULT CALLBACK GraphWndProc(HWND, UINT, WPARAM, LPARAM);


///////////////////////////////////////////////////////////////////////////////////////////////////
//  BOOL tagGraph::Init(HWND, HINSTANCE, int, UINT, UINT, UINT, UINT, float, float, float, float)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Initialize the graph window
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL tagGraph::Init(HWND hParentWnd, HINSTANCE hInst, int nCmdShow, UINT WndXmin, UINT WndYmin, 
	UINT WndXmax, UINT WndYmax, float _dataXmin, float _dataXmax, float _dataYmin, float _dataYmax)
{

	char szName[] = "Graph";
	WNDCLASS Window;
	HDC hdc;
//	PAINTSTRUCT ps;

	dataXmin = _dataXmin;
	dataXmax = _dataXmax;
	dataYmin = _dataYmin;
	dataYmax = _dataYmax;
	SizeX = WndXmax-WndXmin;
	SizeY = WndYmax-WndYmin;

	ScaleX = (float)((SizeX - marginXL - marginXR) / (dataXmax - dataXmin));
	ScaleY = (float)((SizeY - marginYT - marginYB) / (dataYmax - dataYmin));


	Window.style = CS_HREDRAW | CS_VREDRAW;
	Window.lpfnWndProc = GraphWndProc;
	Window.cbClsExtra = 0;
	Window.cbWndExtra = 0;
	Window.hInstance = hInst;
	Window.hIcon = NULL;
	Window.hCursor = NULL;
	Window.hbrBackground= (HBRUSH)GetStockObject(BLACK_BRUSH);
	Window.lpszMenuName	= NULL; // MAKEINTRESOURCE(IDR_MAINMENU);
	Window.lpszClassName= szName;

	RegisterClass(&Window);

	hGraphWnd = CreateWindow(
		szName, 
		"Graph", 
		WS_CHILD | WS_CLIPSIBLINGS,
		//WS_CHILD | WS_CAPTION | WS_THICKFRAME| WS_CLIPSIBLINGS,
		WndXmin,
		WndYmin,
		(WndXmax - WndXmin),
		(WndYmax - WndYmin),
		hParentWnd,
		NULL,
		hInst,
		NULL);

	ShowWindow(hGraphWnd, nCmdShow);
	UpdateWindow(hGraphWnd);

	hdc = GetDC(hGraphWnd);
	hGraphBMP = CreateCompatibleBitmap(hdc, SizeX, SizeY);
	ReleaseDC(hGraphWnd, hdc);
	return TRUE;
}

//  BOOL tagGraph::Init(HWND, HINSTANCE, int, UINT, UINT, UINT, UINT, float, float, float, float)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void tagGraph::Clear(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Release graph resources
///////////////////////////////////////////////////////////////////////////////////////////////////
void tagGraph::Clear(void)
{
	DeleteObject(hGraphBMP);
	DestroyWindow(hGraphWnd);
}

//  void tagGraph::Clear(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void tagGraph::SetScale(float, float, float, float)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Set the grpah scale
///////////////////////////////////////////////////////////////////////////////////////////////////
void tagGraph::SetScale(float _dataXmin, float _dataXmax, float _dataYmin, float _dataYmax)
{
	dataXmin = _dataXmin;
	dataXmax = _dataXmax;
	dataYmin = _dataYmin;
	dataYmax = _dataYmax;

	ScaleX = (float)((SizeX - marginXL - marginXR) / (dataXmax - dataXmin));
	ScaleY = (float)((SizeY - marginYT - marginYB) / (dataYmax - dataYmin));
}

//  void tagGraph::SetScale(float, float, float, float)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  BOOL tagGraph::Plot(float, float, COLORREF)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Plot a point on the graph
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL tagGraph::Plot(float dataX, float dataY, COLORREF color)
{
	HDC	 hdc;
	HDC hGraphDC;

	hdc = GetDC(hGraphWnd);
	hGraphDC = CreateCompatibleDC(hdc);
	SelectObject(hGraphDC, hGraphBMP);	
	
	// Set in the actual DC(Device Context)
	SetPixel(hdc, (int)(ScaleX * (dataX - dataXmin) + marginXL), 
		(int)(SizeY - marginYB - ScaleY * (dataY - dataYmin)), color);

	//  Set in the virtual DC
	SetPixel(hGraphDC,(int)(ScaleX * (dataX - dataXmin) + marginXL),
		(int)(SizeY - marginYB - ScaleY * (dataY - dataYmin)), color);

	DeleteDC(hGraphDC);
	ReleaseDC(hGraphWnd, hdc);

	return TRUE;
}

//  BOOL tagGraph::Plot(float, float, COLORREF)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  BOOL tagGraph::Print(float, COLORREF)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Print a point on the 
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL tagGraph::Print(float dataY, COLORREF color)
{
	HDC hdc;
	HDC	hGraphDC;
	HFONT hFont, hOldFont;
	char str[256];

	hdc = GetDC(hGraphWnd);
	hGraphDC = CreateCompatibleDC(hdc);
	SelectObject(hGraphDC, hGraphBMP);	
	
	//  Set in the actual DC(Device Context)
	hFont = SetFont(hdc, (LPCSTR)"Times New Roman", 15, 0, FW_NORMAL, FALSE, FALSE);
	hOldFont = (HFONT)SelectObject(hdc, hFont);
	SetBkMode(hdc, OPAQUE);
	SetBkColor(hdc, RGB(0,0,0));
	SetTextColor(hdc, color);
	SetTextAlign(hdc, TA_TOP | TA_LEFT); 

	sprintf(str, "%6.4f", dataY);
	TextOut(hdc, marginXL + 2, marginYT + 2, (LPCTSTR)str, lstrlen(str));

	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);
	
	//  Set in the virtual DC
	hFont = SetFont(hGraphDC, (LPCSTR)"Times New Roman", 15, 0, FW_NORMAL, FALSE, FALSE);
	hOldFont = (HFONT)SelectObject(hGraphDC, hFont);
	SetBkMode(hGraphDC, OPAQUE);
	SetBkColor(hGraphDC, RGB(0,0,0));
	SetTextColor(hGraphDC, color);
	SetTextAlign(hGraphDC, TA_TOP | TA_LEFT); 

	sprintf(str, "%6.4f", dataY);
	TextOut(hGraphDC, marginXL + 2, marginYT + 2, (LPCTSTR)str, lstrlen(str));

	SelectObject(hGraphDC, hOldFont);
	DeleteObject(hFont);

	DeleteDC(hGraphDC);
	ReleaseDC(hGraphWnd, hdc);

	return TRUE;
}

//  BOOL tagGraph::Print(float, COLORREF)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  BOOL tagGraph::Draw(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Draw the graph frame
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL tagGraph::Draw(void)
{
	HDC orgDC;
	HDC hGraphDC;
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	HFONT hFont, hOldFont;
	char str[256];

	orgDC = GetDC(hGraphWnd);
	hGraphDC = CreateCompatibleDC(orgDC);
	SelectObject(hGraphDC, hGraphBMP);	

//	hdc = hGraphDC;

	hBrush = CreateSolidBrush(RGB(0,0,0));
	hOldBrush = (HBRUSH)SelectObject(hGraphDC, hBrush);

	hPen = CreatePen(PS_NULL, 0, 0);
	hOldPen = (HPEN)SelectObject(hGraphDC, hPen);

	//  Draw the graph frame
	Rectangle(hGraphDC, 0, 0, SizeX + 1, SizeY + 1);
	SelectObject(hGraphDC, hOldPen);
	DeleteObject(hPen);
	hPen = CreatePen(PS_SOLID, 0, RGB(128, 128, 128));
	SelectObject(hGraphDC, (HBRUSH)GetStockObject(NULL_BRUSH));
	SelectObject(hGraphDC, hPen);

	Rectangle(hGraphDC,marginXL,marginYT,SizeX-marginXR,SizeY-marginYB);

	//  Draw the scale
	hFont = SetFont(hGraphDC, (LPCSTR)"Times New Roman", 15, 0, FW_NORMAL, FALSE, FALSE);
	hOldFont = (HFONT)SelectObject(hGraphDC, hFont);
	SetBkMode(hGraphDC, TRANSPARENT);
	SetTextColor(hGraphDC, RGB(0, 255, 0));

	SetTextAlign(hGraphDC, TA_BASELINE | TA_RIGHT); 

	sprintf(str, "%3.1f", dataYmax);
	TextOut(hGraphDC, marginXL - 2, marginYT + 5, (LPCTSTR)str, lstrlen(str));
	sprintf(str, "%3.1f", dataYmin);
	TextOut(hGraphDC, marginXL - 2, SizeY - marginYB + 5, (LPCTSTR)str, lstrlen(str));

	SetTextAlign(hGraphDC, TA_CENTER | TA_TOP); 

	sprintf(str, "%3.0f", dataXmin);
	TextOut(hGraphDC, marginXL, SizeY - marginYB+2, (LPCTSTR)str, lstrlen(str));
	sprintf(str, "%3.0f", dataXmax);
	TextOut(hGraphDC, SizeX-marginXR, SizeY - marginYB + 2, (LPCTSTR)str, lstrlen(str));

	//  In the case there is zero axis
	if (dataYmin < 0 && dataYmax > 0) {
		MoveToEx(hGraphDC, marginXL,(int)(-ScaleY * dataYmin + marginYT), NULL);
		LineTo(hGraphDC, SizeX - marginXR, (int)(-ScaleY * dataYmin + marginYT));
		SetTextAlign(hGraphDC, TA_BASELINE | TA_RIGHT); 
		sprintf(str, "0.0");
		TextOut(hGraphDC, marginXL - 2,
			(int)(-ScaleY * dataYmin + marginYT + 5), (LPCTSTR)str, lstrlen(str));
	}

	SelectObject(hGraphDC, hOldBrush);
	SelectObject(hGraphDC, hOldPen);
	SelectObject(hGraphDC, hOldFont);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	DeleteObject(hFont);

	BitBlt(orgDC, 0, 0, SizeX, SizeY, hGraphDC, 0, 0, SRCCOPY);

	DeleteDC(hGraphDC);
	ReleaseDC(hGraphWnd, orgDC);
	
	return TRUE;
}

//  BOOL tagGraph::Draw(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  BOOL tagGraph::Paint(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Paint the graph frame
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL tagGraph::Paint(void)
{
	HDC hdc;
	HDC hGraphDC;
	PAINTSTRUCT ps;
 
	hdc = BeginPaint(hGraphWnd, &ps);
	hGraphDC = CreateCompatibleDC(hdc);
	SelectObject(hGraphDC, hGraphBMP);	

	BitBlt(hdc, 0, 0, SizeX, SizeY, hGraphDC, 0, 0, SRCCOPY);

	DeleteDC(hGraphDC);
	EndPaint(hGraphWnd, &ps);
	
	return TRUE;
}

//  BOOL tagGraph::Paint(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  LRESULT CALLBACK GraphWndProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Procedure of the graph window
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK GraphWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInst;
	
	hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

	switch (msg) {
		case WM_CLOSE:
			break;

		default :
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0L;
}

//  LRESULT CALLBACK GraphWndProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////