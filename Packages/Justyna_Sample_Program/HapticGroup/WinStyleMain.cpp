///////////////////////////////////////////////////////////////////////////////////////////////////
//  WinStyleMain.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
//  This source file includes Windows style main function
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Programmer:		Masayuki HARA (Assitant Professor)
//  Affiliation:	Higuchi & Yamamoto Lab. (Advanced Mechatoronics Lab.)
//					School of Engineering, The University of Tokyo
//  Created date:	20.12.2008
//	Updated date:	29.11.2011
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  include

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

#include "resource.h"

#include "DeviceControl.h"
#include "GLUTItems.h"
#include "GLUTTools.h"
#include "HapticTools.h"
#include "IOControl.h"
#include "MainGraph.h"
#include "RoboticsTools.h"
#include "SamplerTools.h"
#include "SerialCommunication.h"
#include "WinStyleMain.h"

//  Headers for Metaseq
#include "GLMetaseq.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//	define

#define NON_SELECTED	0
#define GL_MODE			1
#define GLUT_MODE		2


///////////////////////////////////////////////////////////////////////////////////////////////////
//	variable

HWND hAppWnd;
HWND hGLWnd;
HINSTANCE hAppInst;

BOOL GraphReset;

int OpenMode = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////
//	function

ATOM InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

void DrawScreen(HWND);
void DrawTitle(HWND);

TCHAR szClassName[] = TEXT("Test Program for Haptic Device");


///////////////////////////////////////////////////////////////////////////////////////////////////
//  int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Main function (Windows style)
///////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;

	hAppInst = hCurInst;
	
	//  Register window
	if (!InitApp(hCurInst)) {
		return FALSE;
	}

	//  Create window
	if (!InitInstance(hCurInst, nCmdShow)) {
		return FALSE;
	}

#ifndef DEBUG
	//  Open driver of NI cards
	if (IO_OpenDevice() != 0) {
		MessageBox(hAppWnd, TEXT("Device cannot be opened"),	
			TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
	}
#endif

#ifndef DEBUG
	//  Start NI cards
	if (IO_StartDevice() != 0) {
		MessageBox(hAppWnd, TEXT("Device cannot be started"),	
			TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
	}
#endif

	//  Main loop
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
			break;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

#ifndef DEBUG
	//  Stop NI cards
	if (IO_StopDevice() != 0) {
		MessageBox(hAppWnd, TEXT("Device cannot be stopped"),	
			TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
	}
#endif

#ifndef DEBUG
	//  Close driver of NI cards
	if (IO_CloseDevice() != 0) {
		MessageBox(hAppWnd, TEXT("Device cannot be closed"),	
			TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
	}
#endif

	return (int)msg.wParam;
}

//  int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  ATOM InitApp(HINSTANCE)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Register window class
///////////////////////////////////////////////////////////////////////////////////////////////////
ATOM InitApp(HINSTANCE hInst)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;							//  Style of window class
	wc.lpfnWndProc = WndProc;									//  Name of window class
	wc.cbClsExtra = 0;											//  Structure of window class
	wc.cbWndExtra = 0;											//  Window instance
	wc.hInstance = hInst;										//  Instance
	wc.hIcon = (HICON)LoadImage(NULL,							//  Icon
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);
//	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));		//  Icon
	wc.hCursor = (HCURSOR)LoadImage(NULL,						//  Cursor
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		//  Configuration of background
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);				//  Name of menu
	wc.lpszClassName = (LPCTSTR)szClassName;					//  Name of class
	wc.hIconSm = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);

	return (RegisterClassEx(&wc));								//  Register window class
}

//  ATOM InitApp(HINSTANCE)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  BOOL InitInstance(HINSTANCE, int)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Create window
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
	//  Configuration of window handle
	hAppWnd = CreateWindow(
			szClassName,										//  Name of class
			TEXT("Test Program for Haptic Device"),				//  Name of title bar
			WS_OVERLAPPEDWINDOW,								//  Category of window
			CW_USEDEFAULT,										//  X-cordinate
			CW_USEDEFAULT,										//  Y-cordinate
			CW_USEDEFAULT,										//  Width
			CW_USEDEFAULT,										//  Height
			NULL,												//  Handle of parent window
			NULL,												//  Menue handle
			hInst,												//  Instance handle 
			NULL);												//  Data pointer

	//  If window is not created...
	if (!hAppWnd){
		return FALSE;
	}

	//  Display window
//	ShowWindow(hAppWnd, nCmdShow);
	ShowWindow(hAppWnd, SW_MAXIMIZE);

	//  Update window
	UpdateWindow(hAppWnd);

	return TRUE;
}

//  BOOL InitInstance(HINSTANCE, int)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Procedure of main window
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInst;
	HMENU hMenu;
	MENUITEMINFO MenuInfo;

	HDErrorInfo error;

	//  Get menu items
	hMenu = (HMENU)GetMenu(hWnd);

	//  Get instance
	hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

	switch (msg) {
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case ID_FILE_EXIT:
					SendMessage(hWnd, WM_CLOSE, 0, 0L);
					break;

				case ID_FILE_SAVE_DATA:
					if (InitSaveData(hInst) == FALSE) {
						MessageBox(hWnd, TEXT("Dialog box for save data cannot be created"),	
							TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
						break;
					}
					break;

				case ID_SAMPLER_ON:
					if (OpenMode != 0) {
						MessageBox(hWnd, TEXT("Program is already running"),	
							TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
						break;
					}

					
					//  Open the dialogbox for the experimental configuration
					if (OpenMode != 0 || MakeExpConfigDlg(hInst) == FALSE) {
						MessageBox(hWnd, TEXT("Dialog box for configuration cannot be created"),	
							TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
						break;
					}

					//  Open PHANToM Omni
					switch (DeviceNum) {
						case DEV_PHANTOM:		//  Open PHANToM Omni
							if (PhantomFlag == FALSE) {
								if (HT_InitializePhantom() == TRUE) {				
									PhantomFlag = TRUE;
									Sleep(1000);				//  1 sec wait for stabilizing
								} else {
									MessageBox(hWnd, TEXT("PHANToM Omni cannot be opened"),
										TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
									PhantomFlag = FALSE;
								}
							}
							break;

						case DEV_NONE:			//  No device
						default:
							break;
					}

#ifndef DEBUG
					IO_OutputZeroVoltage();							//  0V output just in case
					Sleep(1000);									//  Wait for data to stabilize
#endif

					OpenMode = 1;
					DrawTitle(hWnd);
					InitSampler();
					break;

				case ID_SAMPLER_OFF:
					if (OpenMode != 0) {
						StopSampler();

						PlaySound(NULL, NULL, 0);

#ifndef DEBUG
						//  0V-output just in case
						IO_OutputZeroVoltage();
						IO_OutputZeroVoltage();
						Sleep(1000);
#endif

						//  Close PHANToM Omni
						switch (DeviceNum) {
							case DEV_PHANTOM:		//  Close PHANToM Omni
								if (PhantomFlag == TRUE) {
									if (HT_ClosePhantom() == FALSE) {
										MessageBox(hWnd, TEXT("PHANToM Omni cannot be closed"),	
											TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
									}
								}
								PhantomFlag = FALSE;
								break;

							case DEV_NONE:			//  No device
							default:
								break;
						}

						if (ExpMode == EXPERIMENT) {
							//  Display dialog box for saving data
							if (InitSaveData(hInst) == FALSE) {
								MessageBox(hWnd, TEXT("Dialog box for save data cannot be created"),	
								TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
								break;
							}
						}

					} else {
						MessageBox(hWnd, TEXT("Program is not running"),	
							TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
					}

					OpenMode = 0;
					break;

				case ID_CONFIGURATION:
					if (OpenMode != 0 || MakeExpConfigDlg(hInst) == FALSE) {
						MessageBox(hWnd, TEXT("Dialog box for configuration cannot be created"),	
							TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
						break;
					}
					break;

				case ID_CALIBRATION:
					if (OpenMode != 0) {
						MessageBox(hWnd, TEXT("Program is already running"),	
							TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
						break;
					}

					//  Calibration start
					RT_InitSamplerForCalibration();
					MessageBox(hWnd, TEXT("Caribration is done."),	
							TEXT("End of Caribration"), MB_OK | MB_ICONINFORMATION);
					break;

				case ID_ZERO_VOLT_OUTPUT:
#ifndef DEBUG
					IO_OutputZeroVoltage();
					Sleep(1000);
#endif

					MessageBox(hWnd, TEXT("Zero volt output"),	
						TEXT("Zero volt output"), MB_OK | MB_ICONINFORMATION);
					break;

				case ID_INIT_NI_CARD:
#ifndef DEBUG
					//  Stop NI cards
					if (IO_StopDevice() != 0) {
						MessageBox(hAppWnd, TEXT("Device cannot be stopped"),	
							TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
					}
					Sleep(250);
#endif

#ifndef DEBUG
					//  Close driver of NI cards
					if (IO_CloseDevice() != 0) {
						MessageBox(hAppWnd, TEXT("Device cannot be closed"),	
							TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
					}
					Sleep(250);
#endif

#ifndef DEBUG
					//  Open driver of NI cards
					if (IO_OpenDevice() != 0) {
						MessageBox(hAppWnd, TEXT("Device cannot be opened"),	
							TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
					}
					Sleep(250);
#endif

#ifndef DEBUG
					//  Start NI cards
					if (IO_StartDevice() != 0) {
						MessageBox(hAppWnd, TEXT("Device cannot be started"),	
							TEXT("Caution!!"), MB_OK | MB_ICONEXCLAMATION);
					}
					Sleep(250);
#endif

					MessageBox(hWnd, TEXT("NI cards are initialized"),
						TEXT("Initialize NI cards"), MB_OK | MB_ICONINFORMATION);
					break;

				case IDM_SERIAL_COM_CONFIG:
					if (OpenMode != 0) {
						MessageBox(hAppWnd, 
							"Can't configure COM port", "Caution!!", MB_OK | MB_ICONEXCLAMATION);
						break;
					}

					//  Call configuration dialog box for serial communication
					SC_COMPortSetDlgInit(hInst);
					
					break;	

				case ID_GRAPH:
					MenuInfo.fMask = MIIM_STATE;
					MenuInfo.hbmpChecked = NULL;
					if (WinGraph.Active) {
						MenuInfo.fState = MFS_UNCHECKED;
						SetMenuItemInfo(hMenu, ID_GRAPH, FALSE, &MenuInfo);
						WinGraph.Active = FALSE;
						WinGraph.Clear();
					} else {
						MenuInfo.fState = MFS_CHECKED;
						SetMenuItemInfo(hMenu, ID_GRAPH, FALSE, &MenuInfo);
						WinGraph.Active = TRUE;
						WinGraph.Init(hAppWnd, hInst, SW_SHOW, GRAPH_NUM, (float)0.0, (float)GRAPH_MAX,
								(float)-1.0, (float)1.0, "Graph Plot", 600, 300);//1024 - 10, 300 - 30);
					}
					break;

				case ID_OPENGL_DISPLAY:
					//  Configure view data
					GLUT_InitViewData();	

					//  Configure line of sight
					GLUT_InitLineOfSight();

					//  Configure camerawork
					GLUT_ConfigureCamera(GLUT_DOLLY, GLUT_DIF, 0.0, GLUT_WindowViewData);

					//  Load MQO models
					mqoInit();
					for (unsigned int i = 0; i < GLUT_MODEL_NUM; i++) {
						glut_mqo_model[i] = mqoCreateModel(glut_mqo_model_name[i], 0.1);
					}
					
					//  Start rendering based on GLUT library
					GLUT_Draw3DSpace(GLUT_WindowViewData);

					break;

				case ID_OPENGL_CONFIGURATION:
					//  Configure view data
					GLUT_InitViewData();
							
					//  Configure line of sight
					GLUT_InitLineOfSight();

					//  Configure camera angle
					GLUT_ConfigureCamera(GLUT_DOLLY, GLUT_DIF, 0.0, GLUT_WindowViewData);

					//  Create dialogbox
					GLUT_MakeOpenGLConfigDlg(hWnd);

					break;

				case ID_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, (DLGPROC)AboutDlgProc);
					break;

				default:
					break;
			}
			break;

		case WM_PAINT:
			DrawScreen(hWnd);
			break;

		case WM_CHAR:
			if (OpenMode == 1) {
				ExternalInputByKeystroke(wParam);
			}
			break;

		case WM_RBUTTONDOWN:
			if (OpenMode == 1) {
				SendMessage(hWnd, WM_COMMAND, (WPARAM)ID_SAMPLER_OFF, 0L);
			}
			break;

		case WM_CLOSE:
			if (MessageBox(hWnd, TEXT("End of Program?"), TEXT("End of Program"),
				MB_YESNO | MB_ICONQUESTION) == IDYES){
					DestroyWindow(hWnd);
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return (DefWindowProc(hWnd, msg, wParam, lParam));
	}

	return 0;
}

//  LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  LRESULT CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Procedure of about dialog box
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hParent;
    static HWND hDlg;
    static HBITMAP hBmp;

	//  Get handle of parent window
	hParent = GetParent(hWnd);

	switch (msg) {
		case WM_INITDIALOG:
			hDlg = GetDlgItem(hWnd, IDC_LOGO);
			hBmp=(HBITMAP)LoadImage(hAppInst, (LPCSTR)IDB_BITMAP1, 
				IMAGE_BITMAP, 0, 0, 0/*LR_LOADFROMFILE*/);
			SendMessage(hDlg, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					InvalidateRect(hParent, NULL, TRUE);	//  Add rectangle area
					EndDialog(hWnd, MSG_OK);				//  Destroy dialog box
					break;

				default:
					break;
			}
			break;

		case WM_CLOSE:
		  EndDialog(hWnd, WM_CLOSE);
		  break;

		default:
			return FALSE;
	}

	return TRUE;
}

//  LRESULT CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void DrawScreen(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Draw screen
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawScreen(HWND AppWnd)
{
	HDC hdc;
	PAINTSTRUCT	ps;

	//	Configuration of paintting
	hdc = BeginPaint(AppWnd, &ps);
	SetBkMode(hdc, TRANSPARENT);
	EndPaint(AppWnd, &ps);
}

//	void DrawScreen(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void DrawTitle(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Draw tilte on the window
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawTitle(HWND AppWnd)
{
	HDC hdc;
	HFONT hFont, hFontOld;
	TCHAR str[] = TEXT("Test Program for Haptic Device");
	int iStepLeft = 300;
	int iStepCenter = 550;
	int iStepRight = 800;
	int iStepVertical = 50;

	//  Get device context
	hdc = GetDC(hAppWnd);	

	//	Configuration
	SetBkMode(hdc, OPAQUE);	
	hFont = SetFont(hdc, (LPCTSTR)"Time New Roman", 30, 0, FW_BOLD, TRUE, TRUE);
	hFontOld = (HFONT)SelectObject(hdc,	hFont);
	(HFONT)SelectObject(hdc, hFont);

	SetBkColor(hdc,	RGB(0,0,0));

	SetTextColor(hdc, RGB(255,255,255));
	TextOut(hdc, 10, 10, str, lstrlen(str));

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
}

//	void DrawTitle(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	HFONT SetFont(HDC, LPCTSTR, int, int, int, BOOL, BOOL)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Set font of characters
///////////////////////////////////////////////////////////////////////////////////////////////////
HFONT SetFont(HDC hdc, LPCTSTR face, int h, int angle, int fnWeight, BOOL bItalic, BOOL bUnderline)
{
	HFONT	hFont;

	hFont = CreateFont(h, 0, angle, 0, fnWeight, bItalic, bUnderline, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, face);

	return	hFont;

}

//	HFONT SetFont(HDC, LPCTSTR, int, int, int, BOOL, BOOL)
///////////////////////////////////////////////////////////////////////////////////////////////////