///////////////////////////////////////////////////////////////////////////////////////////////////
//  DeviceControl.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
//  This source file includes functions for device control
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Programmer:		Masayuki HARA (Assitant Professor)
//  Affiliation:	Higuchi & Yamamoto Lab. (Advanced Mechatoronics Lab.)
//					School of Engineering, The University of Tokyo
//  Created date:	13.01.2009
//	Updated date:	29.11.2011
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  include

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <Mmsystem.h>

#include "resource.h"

#include "DeviceControl.h"
#include "GLUTTools.h"
#include "HapticTools.h"
#include "IOControl.h"
#include "MainGraph.h"
#include "RoboticsTools.h"
#include "SamplerTools.h"
#include "SerialCommunication.h"
#include "WinStyleMain.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//	define

#define DELAYED_NONE 0
#define DELAYED_MASTER 1
#define DELAYED_SLAVE 2

#define CUTOFF_FREQ 30.0

#define IRI_MODE 0
#define TIF_MODE 1
#define LOG_MODE 2
#define BMP_MODE 3

#define EXP_DURATION 300.0//180.001

#define DRIFT_MEASUREMENT 0.001//0.001//10.0

#define MANIPULATION_RANGE 300.0


///////////////////////////////////////////////////////////////////////////////////////////////////
//	variable

HANDLE hGraphicThread;
DWORD GraphicThreadID;
PrintThreadParmTag pParm;

LARGE_INTEGER fp;
LARGE_INTEGER scount;
LARGE_INTEGER ecount;
LARGE_INTEGER acount;

ULONG TimerCounter;
UINT ErrorCounter;
ULONG AnimationCounter;
ULONG AnimationCounter2;
ULONG AnimationCounter3;

HINSTANCE hInstGL;

double SamplingTime;
double TimeError;

int OpenGLFlag = 0;
int AnimationFlag = 0;
int AnimationFlag2 = 0;
int AnimationFlag3 = 0;
int ExpFlag;
int ExpMode;

char *LogFilePass = ".\\LogFiles\\";	
char FileName[256];

int RadioMov[3][2] = { {1, 0}, {1, 0}, {1, 0} };
int DeviceNum;
char DeviceCombo[2][16] = {"Geomagic Touch", "None"};

int RadioDev[3] = {1, 0, 0};
int DelayedDevice;

int RadioBox[2] = {1, 0};
int ComboNum;
char ComboBox[3][16] = {".xls", ".txt", ".dat"};

int DeviceMovement[3];

//  Variables for experiment
double dJointAngle[3];							//  Joint angle of motors [rad]
double dJointAnglePre[3];						//  Joint angle 1 step before [rad]
double dDesiredAngle[3];						//  Desired angle [rad]

double dAngularVelocity[3];						//  Anglular velocity [rad/s]
double dFilteredAngularVelocity[3];				//  Filtered angular velocity [rad/s]
double dFilteredAngularVelocityPre[3];			//  Filtered angular velocity (1 sampling before) [rad/s]

double dRealPosition[3];						//  Real position 1 step before [mm]
double dRealPositionPre[3];						//  Real position
double dDesiredPosition[3];						//  Desired position [mm]
double dDelayedPosition[5000][3];				//  Delayed desired position [mm]

double dPhantomAngle[3];						//  Joint angle of PHANToM Omni [rad]
double dGimbalAngle[3];							//  Gimbal angle [rad]
double dOrientation[3];							//  Orientation of the end effector [rad]

double dContactPosition[3] = {0.0, 0.0, -132.0};//  Position where a virtual hand is located [mm]
double dInitialPosition[3];						//  Initial position of PHANToM [mm]

double dRealVelocity[3];						//  Real velocity [mm/s]
double dFilteredRealVelocity[3];				//  Filtered velocity [mm/s]
double dFilteredRealVelocityPre[3];				//  Filtered velocity (1 sampling before) [mm/s]

double dOutputVoltage[4];						//  Output voltage [V]
double dInputVolt;								//  Input voltage [V]
double dAppliedForce[5000];						//  Applied force [N]
double dFilteredAppliedForce;					//  Filtered applied force [N]
double dAppliedForceVec[3];						//  Applied force vector [N]
double dContactForce[3];						//  Contact force [N]

double dForceAmplitude;

double dSliderPosition;							//  Slider position [mm]

//  Parameters for PID controller
double dPID_Kp[3];								//  P gain
double dPID_Ki[3];								//  I gain
double dPID_Kd[3];								//  D gain

//  Paramteres for compliance control in slave device
double dCompliance_Gn;
double dCompliance_G1;
double dCompliance_G2;
double dCompliance_G3;

double dCompliance_M;							//  Mass [kg]
double dCompliance_D;							//  Damper [Kg/s]
double dCompliance_K;							//  Stiffness [N/mm]

double dPositionError[3];						//  Position error [mm]
double dPositionErrorPre1[3];					//  Position error (1 sampling before) [mm]
double dPositionErrorPre2[3];					//  Position error (2 sampling before) [mm]

double dReferencePosition[3];					//  Reference position [mm]

//  LPF
double dLPF_Para1;								//  Parameter 1 for LPF
double dLPF_Para2;								//  Parameter 2 for LPF
double dLPF_Cutoff_f;							//  Corrected cutoff frequency [Hz]
double dLPF_Dummy;								//  Dummy data
double dLPF_DummyPre;							//  Dummy data (1 sampling before)

double dDelayTime;								//  Delay time [s]
int iHoldCount;									//  Wait count
ULONG DelayedCounter_M;							//  Delayed counter (for master device)
ULONG DelayedCounter_S;							//  Delayed counter (for slave device)

//  Log data
double TimerCounter_Log[SAVEDATAMAX];
double TimerCounter1_Log[SAVEDATAMAX];
double TimerCounter2_Log[SAVEDATAMAX];
double OutputVoltage_Log[3][SAVEDATAMAX];
double RealAngle_Log[3][SAVEDATAMAX];
double DesiredAngle_Log[3][SAVEDATAMAX];
double RealPosition_Log[3][SAVEDATAMAX];
double DesiredPosition_Log[3][SAVEDATAMAX];
double DesiredPosition1_Log[3][SAVEDATAMAX];
double DesiredPosition2_Log[3][SAVEDATAMAX];
double ContactPosition_Log[SAVEDATAMAX];
int ContactFlag_Log[SAVEDATAMAX];

int VisualFlag;

int ContactFlag;
int ConditionFlag;
double dHoldPosition;

int FirstLogFlag;
int SecondLogFlag;
ULONG TimerCounter1;
ULONG TimerCounter2;


///////////////////////////////////////////////////////////////////////////////////////////////////
//	function

void DeviceControlSampler(void);
void DrawSamplerStatus(HWND);
void DrawDeviceStatus(HWND);

DWORD WINAPI GraphicThread(LPVOID);

LRESULT CALLBACK ExpConfigDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL MakeExpConditionDlg(HINSTANCE);
LRESULT CALLBACK ExpConditionDlgProc(HWND, UINT, WPARAM, LPARAM);
void SetEditBoxInExpConfigDlg(HWND);
void LoadEditBoxInExpConfigDlg(HWND);
BOOL SaveExpParameter(void);
BOOL LoadExpParameter(void);

LRESULT CALLBACK SaveDataDlgProc(HWND, UINT, WPARAM, LPARAM);
void SetDefaultFileName(HWND);
char *ReadFileName(HWND);
BOOL SaveDataToFile(char *);


///////////////////////////////////////////////////////////////////////////////////////////////////
//  void DeviceControlSampler(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Initialize sampler for device control
///////////////////////////////////////////////////////////////////////////////////////////////////
void DeviceControlSampler(void)
{
	QueryPerformanceCounter(&ecount);
	SamplingTime = (float)(1. * (ecount.LowPart - scount.LowPart) / fp.LowPart);
	TimeError = fabs(SamplingTime - SAMP);

	scount = ecount;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //  Program for device control
    ///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEBUG
	//  Get joint angle
	IO_GetJointAngle(dJointAngle);
#endif

	//  Get slider position
	dSliderPosition = 0.0;//RT_GetPotentiometerPosition();

	//  Calculate joint angular velocity
	for (unsigned int i = 0; i < 3; i++) {
		dAngularVelocity[i] = (dJointAngle[i] - dJointAnglePre[i]) / SAMP;
		dFilteredAngularVelocity[i] = (dJointAngle[i] - dJointAnglePre[i] 
		+ dFilteredAngularVelocityPre[i] / CUTOFF_FREQ) / (1.0 / CUTOFF_FREQ + SAMP); 
	}

	//  Get real position
	RT_CalculateDirectKinematics(dJointAngle, dRealPosition);

	//  Calculate real velocity
	for (unsigned int i = 0; i < 3; i++) {
		dRealVelocity[i] = (dRealPosition[i] - dRealPositionPre[i]) / SAMP;
		dFilteredRealVelocity[i] = (dRealPosition[i] - dRealPositionPre[i] 
		+ dFilteredRealVelocityPre[i] / CUTOFF_FREQ) / (1.0 / CUTOFF_FREQ + SAMP); 
	}
	
	//  Filter the applied force
	//dLPF_Dummy = dAppliedForce[DelayedCounter_M] + dLPF_Para1 * dLPF_DummyPre;
	//dFilteredAppliedForce = dLPF_Para2 * (dLPF_Dummy + dLPF_DummyPre);
	//dLPF_DummyPre = dLPF_Dummy;

	//  Get the applied force vector
	//RT_GetForceVector(dJointAngle, dFilteredAppliedForce, dAppliedForceVec);

	//  Get the applied force
	//dAppliedForce[DelayedCounter_M] = RT_GetSingleForce();

	//  Call the callback function for PHANToM Omni
	if (PhantomFlag == TRUE) {
		hdWaitForCompletion(hPhantomCallback, HD_WAIT_CHECK_STATUS);
	}

	//  Flag control for contact state
	if (dDesiredPosition[2] <= dContactPosition[2]) {
		if (ContactFlag == 0) {
			ContactFlag = 1;
		}
	} else {
		if (ContactFlag == 1) {
			ContactFlag = 0;
		}
	}

	//  Select the device movement
	//if (DeviceMovement[0] == MIRROR_MOV) {
	//	dDelayedPosition[DelayedCounter_S][0] = -(dDelayedPosition[DelayedCounter_S][0] - dLinkLength[2]) + dLinkLength[2];
	//}
	//if (DeviceMovement[1] == MIRROR_MOV) {
	//	dDelayedPosition[DelayedCounter_S][1] = (-dDelayedPosition[DelayedCounter_S][1]);
	//}
	//if (DeviceMovement[2] == MIRROR_MOV) {
	//	dDelayedPosition[DelayedCounter_S][2] = -(dDelayedPosition[DelayedCounter_S][2] + dLinkLength[1]) - dLinkLength[1];
	//}

	//dDelayedPosition[DelayedCounter_S][0] = dLinkLength[2];
	//dDelayedPosition[DelayedCounter_S][1] = dRealPosition[1];
	//dDelayedPosition[DelayedCounter_S][2] = dLinkLength[1];
	dDelayedPosition[DelayedCounter_S][0] = 100.0;

	//  Calculate the compliance factor
	//for (i = 0; i < 3; i++) {
	//	dPositionError[i] = dCompliance_G1 * dAppliedForceVec[i] + dCompliance_G2 * dPositionErrorPre1[i] 
	//	+ dCompliance_G3 * (dPositionErrorPre1[i] - dPositionErrorPre2[i]) / SAMP;
	//	dReferencePosition[i] = dDelayedPosition[DelayedCounter_S][i] + dPositionError[i];
	//	dPositionErrorPre2[i] = dPositionErrorPre1[i];
	//	dPositionErrorPre1[i] = dPositionError[i];
	//}

	//  Saturation of aplied force for safety
	//if (ContactFlag == 0) {
	//	if (dAppliedForceVec[2] >= 3.0) {
	//		dHoldPosition = dDesiredPosition[2];
	//		dDelayedPosition[DelayedCounter_S][2] = dHoldPosition;
	//		ContactFlag = 1;
	//	}
	//} else if (ContactFlag == 1) {
	//	if (dDesiredPosition[2] < dHoldPosition) {
	//		dDelayedPosition[DelayedCounter_S][2] = dHoldPosition;
	//	} else {
	//		ContactFlag = 0;
	//	}
	//}

	//  Set limitation of penetration into the hand
	if (dDelayedPosition[DelayedCounter_S][2] <= dContactPosition[2] - 5.0) {
		dDelayedPosition[DelayedCounter_S][2] = dContactPosition[2] - 5.0;
	}

	//  Calculate the desired angle
	//RT_CalculateInverseKinematics(dReferencePosition, dDesiredAngle);
	RT_CalculateInverseKinematics(dDelayedPosition[DelayedCounter_S], dDesiredAngle);
	//if (iHoldCount != 0) {
	//	RT_CalculateInverseKinematics(dDelayedPosition[a], dDesiredAngle);
	//} else {
	//	RT_CalculateInverseKinematics(dDelayedPosition[DelayedCounter_S], dDesiredAngle);
	//}

	//  PID control
	for (unsigned int i = 0; i < 2; i++) {
		dOutputVoltage[i] = dPID_Kp[i + 1] * (dDesiredAngle[i + 1] - dJointAngle[i + 1])
			- dPID_Kd[i + 1] * dAngularVelocity[i + 1];
	}
	//dOutputVoltage[0] = -dOutputVoltage[0];

	//  Limitation of output voltage
	for (unsigned int i = 0; i < 2; i++) {
		if (dOutputVoltage[i] > 9.8) {
			dOutputVoltage[i] = 9.8;
		} else if (dOutputVoltage[i] < -9.8) {
			dOutputVoltage[i] = -9.8;
		}
	}

	//  Convert from leader position [mm] to 0-5 [V]
	dOutputVoltage[2] = (dDesiredPosition[0] + MANIPULATION_RANGE / 2.0) / MANIPULATION_RANGE * 5.0;
	dOutputVoltage[3] = (dDesiredPosition[2] + MANIPULATION_RANGE / 2.0) / MANIPULATION_RANGE * 5.0;

	//  Limitation of output voltage
	for (unsigned int i = 2; i < 4; i++) {
		if (dOutputVoltage[i] > 5.0) {
			dOutputVoltage[i] = 5.0;
		} else if (dOutputVoltage[i] < 0.0) {
			dOutputVoltage[i] = 0.0;
		}
	}

	//  Set zero output [V]
	if (FirstLogFlag == 1 || SecondLogFlag == 1) {
		for (unsigned int i = 0; i < 4; i++) {
			dOutputVoltage[i] = 0.0;
		}
	}

#ifndef DEBUG
	//  Output voltage
	if (ExpMode == EXPERIMENT) {
		IO_OutputVoltage(dOutputVoltage);
	}
#endif

	//  Data buffer
	if (TimerCounter < (int)(EXP_DURATION / SAMP)) {//SAVEDATAMAX) {
		TimerCounter_Log[TimerCounter] = TimerCounter;

		for (unsigned int i = 0; i < 3; i++) {
			OutputVoltage_Log[i][TimerCounter] = dOutputVoltage[i];
			RealAngle_Log[i][TimerCounter] = dJointAngle[i];
			DesiredAngle_Log[i][TimerCounter] = dDesiredAngle[i];
			RealPosition_Log[i][TimerCounter] = dRealPosition[i];
			DesiredPosition_Log[i][TimerCounter] = dDesiredPosition[i];
		}

		ContactPosition_Log[TimerCounter] = dContactPosition[2];
		ContactFlag_Log[TimerCounter] = ContactFlag;
	}

	//  Drift measurement
	if (ExpMode != INITIALIZATION) {
		if (TimerCounter1 < (int)(DRIFT_MEASUREMENT / SAMP)) {
			TimerCounter1_Log[TimerCounter1] = TimerCounter1;
			for (unsigned int i = 0; i < 3; i++) {
				DesiredPosition1_Log[i][TimerCounter1] = dDesiredPosition[i];
			}
		} else {
			if (FirstLogFlag == 1) {
				ExternalInputByKeystroke('s');
				VisualFlag = 1;
			}
			FirstLogFlag = 2;
		}

		if (TimerCounter2 < (int)(DRIFT_MEASUREMENT / SAMP)) {
			TimerCounter2_Log[TimerCounter2] = TimerCounter2;
			for (unsigned int i = 0; i < 3; i++) {
				DesiredPosition2_Log[i][TimerCounter2] = dDesiredPosition[i];
			}
		} else {
			SecondLogFlag = 2;
		}
	} else {
		ExpFlag = 2;
		FirstLogFlag = 2;
		SecondLogFlag = 2;
	}

	//  Shift data
	for (unsigned int i = 0; i < 3; i++) {
		dJointAnglePre[i] = dJointAngle[i];
		dRealPositionPre[i] = dRealPosition[i];
		dDelayedPosition[DelayedCounter_S][i] = dDesiredPosition[i];
	}

	//  End of experiment
	if (ExpFlag == 1) {
		if (TimerCounter * SAMP > EXP_DURATION) {
			if (SecondLogFlag == 0) {
				//  Stop white noise
				PlaySound(NULL, NULL, 0);

				VisualFlag = 0;

				SecondLogFlag = 1;
			}
			//SendMessage(hAppWnd, WM_COMMAND, (WPARAM)ID_SAMPLER_OFF, 0L);
		}

		if (SecondLogFlag == 2) {
			Beep(440, 1000);
			SendMessage(hAppWnd, WM_COMMAND, (WPARAM)ID_SAMPLER_OFF, 0L);
		}
	} else if (ExpFlag == 2) {
		VisualFlag = 0;
	}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //  Program for device control
    ///////////////////////////////////////////////////////////////////////////////////////////////

	if (ExpFlag == 1 || ExpFlag == 2) {
		TimerCounter++;								//	Count up
	}

	if (FirstLogFlag == 1) {
		TimerCounter1++;
	}

	if (SecondLogFlag == 1) {
		TimerCounter2++;
	}

	//  Delayed counter up (Master device)
	if (DelayedDevice == DELAYED_MASTER) {
		DelayedCounter_M++;
		if (DelayedCounter_M > iHoldCount) {
			DelayedCounter_M = 0;
		}
	}

	//  Delayed counter up (Slave device)
	if (DelayedDevice == DELAYED_SLAVE) {
		DelayedCounter_S++;
		if (DelayedCounter_S > iHoldCount) {
			DelayedCounter_S = 0;
		}
	}

	if ((TimerCounter % GRAPH_MAX) == 0) {		//  Graph condition
		GraphReset = TRUE;
	}

	if ((TimerCounter % 1) == 0) {				//  OpenGL condition (1 rendering / 10 samplings)
		OpenGLFlag = 1;
	}

	if (AnimationFlag == ANIMATION_ON) {
		AnimationCounter++;
	}

	if (AnimationFlag2 == ANIMATION_ON) {
		AnimationCounter2++;
	}

	if (AnimationFlag3 == ANIMATION_ON) {
		AnimationCounter3++;
	}
}

//  void DeviceControlSampler(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void InitSampler(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Initialize sampler for device control
///////////////////////////////////////////////////////////////////////////////////////////////////
void InitSampler(void)
{
	//  Initialize timer and error counters
	TimerCounter = 0;
	TimerCounter1 = 0;
	TimerCounter2 = 0;
	ErrorCounter = 0;
	AnimationCounter = 0;
	AnimationCounter2 = 0;
	AnimationCounter3 = 0;

	ExpFlag = 0;
	VisualFlag = 0;
	FirstLogFlag = 1;
	SecondLogFlag = 0;
	ContactFlag = 0;

	DelayedCounter_M = 0;
	DelayedCounter_S = 0;

	dForceAmplitude = 1.0;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //  Initialize parameters for device control
    ///////////////////////////////////////////////////////////////////////////////////////////////
	for (unsigned int i = 0; i < 3; i++) {
		dJointAnglePre[i] = 0.0;
		dRealPositionPre[i] = 0.0;
		dPositionErrorPre1[i] = 0.0;
		dPositionErrorPre2[i] = 0.0;
		dFilteredAngularVelocityPre[i] = 0.0;
		dFilteredAngularVelocityPre[i] = 0.0;
	}

	//  Call the callback function for PHANToM Omni
	if (PhantomFlag == TRUE) {
		hdWaitForCompletion(hPhantomCallback, HD_WAIT_CHECK_STATUS);
	}

	for (unsigned int i = 0; i < iHoldCount; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			dDelayedPosition[i][j] = dDesiredPosition[j];
		}
		dAppliedForce[i] = 0.0;
	}

	//  Configure the parameters for compliance control
	dCompliance_Gn = dCompliance_M / SAMP + dCompliance_D + dCompliance_K * SAMP;
	dCompliance_G1 = SAMP / dCompliance_Gn;
	dCompliance_G2 = (dCompliance_M / SAMP + dCompliance_D) / dCompliance_Gn;
	dCompliance_G3 = dCompliance_M/ dCompliance_Gn;

	//  Configuration of LPF for the applied force
	dLPF_Cutoff_f = 2.0 / SAMP * tan(CUTOFF_FREQ * SAMP / 2.0);
	dLPF_Para1 = (2.0 - dLPF_Cutoff_f * SAMP) / (2.0 + dLPF_Cutoff_f * SAMP);
	dLPF_Para2 = dLPF_Cutoff_f * SAMP / (2.0 + dLPF_Cutoff_f * SAMP);
	dLPF_Dummy = 0.0;
	dLPF_DummyPre = 0.0;

	if (ExpMode != INITIALIZATION) {
		Beep(440, 1000);
	}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //  Initialize parameters for device control
    ///////////////////////////////////////////////////////////////////////////////////////////////
	//  Create new thread
	GraphReset = FALSE;
	if (WinGraph.Active) {
		WinGraph.Draw();
	}
	pParm.hWnd = hAppWnd;
	pParm.ThreadEnd	= FALSE;
	hGraphicThread = CreateThread(NULL, 0, 
		(LPTHREAD_START_ROUTINE)GraphicThread, (LPVOID)&pParm, CREATE_SUSPENDED, &GraphicThreadID);

	//  Configure priority of thread
	SetThreadPriority(hGraphicThread, THREAD_PRIORITY_BELOW_NORMAL);
	
	//  Resume thread
	ResumeThread(hGraphicThread);	

	//  Configure time
	QueryPerformanceFrequency(&fp);
	QueryPerformanceCounter(&scount);
	
	//  Start sampler
	SamplerOn(&DeviceControlSampler);
}  

//  void InitSampler(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void StopSampler(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Stop sampler for device control
///////////////////////////////////////////////////////////////////////////////////////////////////
void StopSampler(void)
{
	pParm.ThreadEnd = TRUE;
	SamplerOff();
}  

//  void StopSampler(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  DWORD WINAPI GraphicThread(LPVOID)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Procedure of graphic thread
///////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI GraphicThread(LPVOID Parm)
{
	PrintThreadParmTag *pParm;
	float *Data;

	Data = (float *)calloc(GRAPH_NUM, sizeof(float));
	pParm = (PrintThreadParmTag *)Parm;

	while (!pParm->ThreadEnd) {
		DrawSamplerStatus(hAppWnd);
		DrawDeviceStatus(hAppWnd);

		//  If graph dialog box is displayed...
		if (GraphReset) {	
			if (WinGraph.Active) {
				WinGraph.Draw();
			}

			GraphReset = FALSE;
		}

		if (WinGraph.Active) {
			for (unsigned int i = 0; i < GRAPH_NUM; i++) {
				Data[i] = (float)dAppliedForceVec[i] / 1.0;
			}
			
			WinGraph.Plot((float)(TimerCounter % GRAPH_MAX), Data, RGB(255, 255, 255));
		}
	}

	free(Data);

	SendMessage(pParm->hWnd, WM_ENDTHREAD, 0, 0L);

	return((DWORD)0);
} 

//  DWORD WINAPI GraphicThread(LPVOID)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void DrawSamplerStatus(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Display sampler status
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawSamplerStatus(HWND hWnd)
{
	HDC hdc;
	HFONT hFont, hFontOld;
	char str[256];

	hdc = GetDC(hAppWnd);	

	SetBkMode(hdc, OPAQUE);

	hFont = SetFont(hdc, (LPCSTR)"Time New Roman", 20, 0, FW_BOLD, FALSE, FALSE);
	hFontOld = (HFONT)SelectObject(hdc,	hFont);

	(HFONT)SelectObject(hdc, hFont);

	SetBkColor(hdc, RGB(0,0,0));

	SetTextColor(hdc, RGB(255, 255, 0));
	sprintf(str, "Current Time = %8.3f sec", TimerCounter * SAMP);
	TextOut(hdc, 10, 60,(LPCSTR)str, lstrlen(str));

	SetTextColor(hdc, RGB(0, 255, 0));
	sprintf(str, "Sampling Time = %5.3f msec", SamplingTime * 1000.0);
	TextOut(hdc, 10, 80, (LPCSTR)str, lstrlen(str));

	SetTextColor(hdc, RGB(255, 0, 0));
	sprintf(str, "Time Error = %5.3f msec", TimeError * 1000.0);
	TextOut(hdc, 10, 100, (LPCSTR)str, lstrlen(str));

	SetTextColor(hdc, RGB(255, 255, 0));
	sprintf(str, "(PD Measurement: Before Exp = %5.3f sec, After Exp = %5.3f sec)", TimerCounter1 * SAMP, TimerCounter2 * SAMP);
	TextOut(hdc, 300, 60,(LPCSTR)str, lstrlen(str));
	
//	SetTextColor(hdc, RGB(255, 0, 0));
//	sprintf(str, "Error Count = %u			", ErrorCounter);
//	TextOut(hdc, 10, 100, (LPCSTR)str, lstrlen(str));

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	
	ReleaseDC(hWnd, hdc);
}  

//  void DrawSamplerStatus(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void DrawDeviceStatus(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Display device status
///////////////////////////////////////////////////////////////////////////////////////////////////
void DrawDeviceStatus(HWND hWnd)
{
	HDC hdc;
	HFONT hFont, hFontOld;
	char str[256];
	unsigned int i = 0;

	hdc = GetDC(hAppWnd);	

	SetBkMode(hdc, OPAQUE);

	hFont = SetFont(hdc, (LPCSTR)"Time New Roman", 16, 0, FW_BOLD, FALSE, FALSE);
	hFontOld = (HFONT)SelectObject(hdc, hFont);

	(HFONT)SelectObject(hdc, hFont);

	SetBkColor(hdc, RGB(0,0,0));

	///////////////////////////////////////////////////////////////////////////////////////////////
	//  Experiment condition
	///////////////////////////////////////////////////////////////////////////////////////////////
	if (ExpFlag == 0) {
		sprintf(str, "Experiment = Not started           ");
	} else if (ExpFlag == 1) {
		sprintf(str, "Experiment = Started (Condition %d) ", ConditionFlag);
	} else if (ExpFlag == 2) {
		sprintf(str, "Experiment = Initialization        ");
	}
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 140 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	///////////////////////////////////////////////////////////////////////////////////////////////
	//  Device condition
	///////////////////////////////////////////////////////////////////////////////////////////////
	sprintf(str, "[Device Conditions]");
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 160 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	if (DeviceNum == DEV_PHANTOM) {
		sprintf(str, " Selected Master Device = Touch USB");
	} else {
		sprintf(str, " Selected Master Device = None     ");
	}
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 160 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	//sprintf(str, " Joint Angle 0 = %+3.3lf [rad] : Desiredr Angle 0 = %+3.3lf [rad]     ", 
	//	dJointAngle[0], dDesiredAngle[0]);
	//SetTextColor(hdc, RGB(255, 255, 255));
	//TextOut(hdc, 10, 160 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	sprintf(str, " Joint Angle 1 = %+3.3lf [rad] : Desired Angle 1 = %+3.3lf [rad]     ", 
		dJointAngle[1], dDesiredAngle[1]);
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 160 + 20 * (i++), (LPCSTR)str, lstrlen(str));
	
	sprintf(str, " Joint Angle 2 = %+3.3lf [rad] : Desired Angle 2 = %+3.3lf [rad]     ", 
		dJointAngle[2], dDesiredAngle[2]);
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 160 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	sprintf(str, " Follower Position X = %+3.3lf [mm] :  Leader Position X = %+3.3lf (Delayed: %+3.3lf) [mm]     ", 
		dRealPosition[0], dDesiredPosition[0], dDelayedPosition[DelayedCounter_S][0]);
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 160 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	sprintf(str, " Follower Position Y = %+3.3lf [mm] :  Leader Position Y = %+3.3lf (Delayed: %+3.3lf) [mm]     ", 
		dRealPosition[1], dDesiredPosition[1], dDelayedPosition[DelayedCounter_S][1]);
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 160 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	sprintf(str, " Follower Position Z = %+3.3lf [mm] :  Leader Position Z = %+3.3lf (Delayed: %+3.3lf) [mm]     ", 
		dRealPosition[2], dDesiredPosition[2], dDelayedPosition[DelayedCounter_S][2]);
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 160 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	sprintf(str, " Output Voltage = (%+3.3lf, %+3.3lf, %+3.3lf, %+3.3lf) [V]     ", 
		dOutputVoltage[0], dOutputVoltage[1], dOutputVoltage[2], dOutputVoltage[3]);
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 160 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	if (DeviceNum == DEV_PHANTOM) {
		sprintf(str, " Joint angle of Touch USB = (%+3.3lf, %+3.3lf, %+3.3lf) [deg]     ", 
			dPhantomAngle[0] * 180.0 / PI,
			dPhantomAngle[1] * 180.0 / PI, 
			(dPhantomAngle[2] - dPhantomAngle[1]) * 180.0 / PI);
		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, 10, 180 + 20 * (i++), (LPCSTR)str, lstrlen(str));

		sprintf(str, " Gimbal angle of Touch USB = (%+3.3lf, %+3.3lf, %+3.3lf) [deg]     ", 
			dGimbalAngle[0] * 180.0 / PI,
			dGimbalAngle[1] * 180.0 / PI, 
			dGimbalAngle[2] * 180.0 / PI);
		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, 10, 180 + 20 * (i++), (LPCSTR)str, lstrlen(str));

		sprintf(str, " Orientation of Touch USB = (%+3.3lf, %+3.3lf, %+3.3lf) [deg]     ", 
			dOrientation[0] * 180.0 / PI,
			dOrientation[1] * 180.0 / PI, 
			dOrientation[2] * 180.0 / PI);
		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, 10, 180 + 20 * (i++), (LPCSTR)str, lstrlen(str));
	}

	//sprintf(str, " Applied Force = %+3.3lf[N] (%+3.3lf, %+3.3lf, %+3.3lf)		", dFilteredAppliedForce, 
	//	dAppliedForceVec[0], dAppliedForceVec[1], dAppliedForceVec[2]);
	//SetTextColor(hdc, RGB(255, 255, 255));
	//TextOut(hdc, 10, 160 + (i++), (LPCSTR)str, lstrlen(str));


	//sprintf(str, " Contact Force = %+3.3lf[N] (%+3.3lf, %+3.3lf, %+3.3lf)		", dContactForce[2], 
	//	dContactForce[0], dContactForce[1], dContactForce[2]);
	//SetTextColor(hdc, RGB(255, 255, 255));
	//TextOut(hdc, 10, 160 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	///////////////////////////////////////////////////////////////////////////////////////////////
	//  Experimental condition
	///////////////////////////////////////////////////////////////////////////////////////////////
	sprintf(str, "[Experimental Conditions]");
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 200 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	if (DelayedDevice == DELAYED_NONE) {
		sprintf(str, " Delay Time = %3.3lf [s] (None: %3d, %3d)         ", iHoldCount * SAMP, 
			DelayedCounter_M, DelayedCounter_S);
	} if (DelayedDevice == DELAYED_MASTER) {
		sprintf(str, " Delay Time = %3.3lf [s] (Leader: %3d, %3d)       ", iHoldCount * SAMP, 
			DelayedCounter_M, DelayedCounter_S);
	}  if (DelayedDevice == DELAYED_SLAVE) {
		sprintf(str, " Delay Time = %3.3lf [s] (Follower: %3d, %3d)     ", iHoldCount * SAMP, 
			DelayedCounter_M, DelayedCounter_S);
	}
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 200 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	if (DeviceMovement[0] == NORMAL_MOV) {
		sprintf(str, " Follower Movement (X Direction ) = Normal movement");
	} else if (DeviceMovement[0] == MIRROR_MOV) {
		sprintf(str, " Follower Movement (X Direction ) = Mirror movement");
	}
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 200 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	if (DeviceMovement[1] == NORMAL_MOV) {
		sprintf(str, " Follower Movement (Y Direction ) = Normal movement");
	} else if (DeviceMovement[1] == MIRROR_MOV) {
		sprintf(str, " Follower Movement (Y Direction ) = Mirror movement");
	}
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 200 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	if (DeviceMovement[2] == NORMAL_MOV) {
		sprintf(str, " Follower Movement (Z Direction ) = Normal movement");
	} else if (DeviceMovement[2] == MIRROR_MOV) {
		sprintf(str, " Follower Movement (Z Direction ) = Mirror movement");
	}
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 200 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	sprintf(str, " Contact Position = %+3.3lf [mm]     ", dContactPosition[2]);
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 10, 200 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	if (ContactFlag == 0) {
		sprintf(str, " Contact Status = Not Contacted");
		SetTextColor(hdc, RGB(255, 255, 255));
	} else if (ContactFlag == 1) {
		sprintf(str, " Contact Status = Contacted    ");
		SetTextColor(hdc, RGB(255, 0, 0));
	} 
	TextOut(hdc, 10, 200 + 20 * (i++), (LPCSTR)str, lstrlen(str));

	//sprintf(str, " Contact Force = %+3.3lf[N]     ", dContactForce[2]);
	//SetTextColor(hdc, RGB(255, 255, 255));
	//TextOut(hdc, 10, 200 + 20 * (i++), (LPCSTR)str, lstrlen(str));


	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	
	ReleaseDC(hWnd,hdc);
}  

//  void DrawDeviceStatus(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void ExternalInputByKeystroke(WPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  External input by keystrokes
///////////////////////////////////////////////////////////////////////////////////////////////////
void ExternalInputByKeystroke(WPARAM wParam)
{
	char str[64];
	BYTE str_r[1024];

	switch(wParam) {

		case 's':
			if (ExpFlag == 0){
				TimerCounter = 0;
				PlaySound("white.wav", NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
				ExpFlag = 1;
			} else if (ExpFlag == 1){
				ExpFlag = 0;
			}
			break;

		case 'f':
			if (FirstLogFlag == 0) {
				TimerCounter1 = 0;
				Beep(440, 1000);
				FirstLogFlag = 1;
			} else if (FirstLogFlag == 1) {
				FirstLogFlag = 0;
			}
			break;

		case 'c':
			if (ExpMode == INITIALIZATION) {
				dContactPosition[2] = dRealPosition[2];
			}
			break;

		case 'i':
			if (ExpMode == INITIALIZATION) {
				dInitialPosition[0] = dDesiredPosition[0] - dLinkLength[1];
				dInitialPosition[1] = dDesiredPosition[1] + dLinkLength[0];
				dInitialPosition[2] = dDesiredPosition[2] + dLinkLength[2];
			}
			break;

		case 'a':
			if (AnimationFlag == ANIMATION_OFF) {
				AnimationFlag = ANIMATION_ON;
			} else if (AnimationFlag == ANIMATION_ON) {
				AnimationFlag = ANIMATION_OFF;
			}
			break;

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

		default:
			break;
	}

}

//	void ExternalInputByKeystroke(WPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL MakeExpConfigDlg(HINSTANCE)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Make the dialog box for configuring experimental conditions
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL MakeExpConfigDlg(HINSTANCE hInst)
{
	hInstGL = hInst;
	if (DialogBox(hInst, 
		MAKEINTRESOURCE(IDD_EXP_CONFIG), hAppWnd, (DLGPROC)ExpConfigDlgProc) == IDCANCEL) {
		return FALSE;
	}

	return TRUE;
}

//	BOOL MakeExpConfigDlg(HINSTANCE)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	LRESULT CALLBACK ExpConfigDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Procedure of dialog box for configuring experimental conditions
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ExpConfigDlgProc(HWND hDlgWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hParent;
	
	hParent = GetParent(hDlgWnd);

	switch (msg) {
		case WM_INITDIALOG:
			//  Set the values in edit boxes
			SetEditBoxInExpConfigDlg(hDlgWnd);

			//  Set the radioboxes
			SendMessage(GetDlgItem(hDlgWnd,	IDC_RADIO_NONE), BM_SETCHECK, (WPARAM)RadioDev[0], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_RADIO_MASTER), BM_SETCHECK, (WPARAM)RadioDev[1], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_RADIO_SLAVE), BM_SETCHECK, (WPARAM)RadioDev[2], 0L);

			SendMessage(GetDlgItem(hDlgWnd,	IDC_RADIO_X_NORMAL), BM_SETCHECK, (WPARAM)RadioMov[0][0], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_RADIO_X_MIRROR), BM_SETCHECK, (WPARAM)RadioMov[0][1], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_RADIO_Y_NORMAL), BM_SETCHECK, (WPARAM)RadioMov[1][0], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_RADIO_Y_MIRROR), BM_SETCHECK, (WPARAM)RadioMov[1][1], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_RADIO_Z_NORMAL), BM_SETCHECK, (WPARAM)RadioMov[2][0], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_RADIO_Z_MIRROR), BM_SETCHECK, (WPARAM)RadioMov[2][1], 0L);
			
			//  Set the combo box
			for (unsigned i = 0; i < 2; i++) {
				ComboBox_AddString(GetDlgItem(hDlgWnd, IDC_DEV_COMBO), DeviceCombo[i]);
			}
			ComboBox_SetCurSel(GetDlgItem(hDlgWnd, IDC_DEV_COMBO), DeviceNum);
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDC_SAVE_PID:
					LoadEditBoxInExpConfigDlg(hDlgWnd);
					SaveExpParameter();
					break;

				case IDC_LOAD_PID:
					LoadExpParameter();
					SetEditBoxInExpConfigDlg(hDlgWnd);
					break;

				case IDC_DEV_COMBO:
					DeviceNum = ComboBox_GetCurSel(GetDlgItem(hDlgWnd, IDC_DEV_COMBO));
					SetEditBoxInExpConfigDlg(hDlgWnd);
					break;

				case IDC_RADIO_NONE:
				case IDC_RADIO_MASTER:
				case IDC_RADIO_SLAVE:
					if(IsDlgButtonChecked(hDlgWnd, IDC_RADIO_NONE) == BST_CHECKED){
						RadioDev[0] = 1;
						DelayedDevice = DELAYED_NONE;
					} else {
						RadioDev[0] = 0;
					}
					if(IsDlgButtonChecked(hDlgWnd, IDC_RADIO_MASTER) == BST_CHECKED){
						RadioDev[1] = 1;
						DelayedDevice = DELAYED_MASTER;
					} else {
						RadioDev[1] = 0;
					}
					if(IsDlgButtonChecked(hDlgWnd, IDC_RADIO_SLAVE) == BST_CHECKED){
						RadioDev[2] = 1;
						DelayedDevice = DELAYED_SLAVE;
					} else {
						RadioDev[2] = 0;
					}
					break;

				case IDC_RADIO_X_NORMAL:
				case IDC_RADIO_X_MIRROR:
					if(IsDlgButtonChecked(hDlgWnd, IDC_RADIO_X_NORMAL) == BST_CHECKED){
						RadioMov[0][0] = 1;
						DeviceMovement[0] = NORMAL_MOV;
					} else {
						RadioMov[0][0] = 0;
					}
					if(IsDlgButtonChecked(hDlgWnd, IDC_RADIO_X_MIRROR) == BST_CHECKED){
						RadioMov[0][1] = 1;
						DeviceMovement[0] = MIRROR_MOV;
					} else {
						RadioMov[0][1] = 0;
					}
					break;

				case IDC_RADIO_Y_NORMAL:
				case IDC_RADIO_Y_MIRROR:
					if(IsDlgButtonChecked(hDlgWnd, IDC_RADIO_Y_NORMAL) == BST_CHECKED){
						RadioMov[1][0] = 1;
						DeviceMovement[1] = NORMAL_MOV;
					} else {
						RadioMov[1][0] = 0;
					}
					if(IsDlgButtonChecked(hDlgWnd, IDC_RADIO_Y_MIRROR) == BST_CHECKED){
						RadioMov[1][1] = 1;
						DeviceMovement[1] = MIRROR_MOV;
					} else {
						RadioMov[1][1] = 0;
					}
					break;

				case IDC_RADIO_Z_NORMAL:
				case IDC_RADIO_Z_MIRROR:
					if(IsDlgButtonChecked(hDlgWnd, IDC_RADIO_Z_NORMAL) == BST_CHECKED){
						RadioMov[2][0] = 1;
						DeviceMovement[2] = NORMAL_MOV;
					} else {
						RadioMov[2][0] = 0;
					}
					if(IsDlgButtonChecked(hDlgWnd, IDC_RADIO_Z_MIRROR) == BST_CHECKED){
						RadioMov[2][1] = 1;
						DeviceMovement[2] = MIRROR_MOV;
					} else {
						RadioMov[2][1] = 0;
					}
					break;

				case IDC_RESET:
					for (unsigned i = 0; i < 3; i++) {
						dPID_Kp[i] = 0.0;
						dPID_Ki[i] = 0.0;
						dPID_Kd[i] = 0.0;
					}
					dPhantomStiffness = 0.0;
					SetEditBoxInExpConfigDlg(hDlgWnd);
					LoadEditBoxInExpConfigDlg(hDlgWnd);
					break;

				case IDC_INITIALIZATION:
					ExpMode = INITIALIZATION;
					for (unsigned i = 0; i < 3; i++) {
						dPID_Kp[i] = 0.0;
						dPID_Ki[i] = 0.0;
						dPID_Kd[i] = 0.0;
					}
					dPhantomStiffness = 0.0;
					SetEditBoxInExpConfigDlg(hDlgWnd);
					LoadEditBoxInExpConfigDlg(hDlgWnd);
					SendMessage(hDlgWnd, WM_CLOSE, 0, 0L);
					break;

				case IDOK:
					ExpMode = EXPERIMENT;
					LoadEditBoxInExpConfigDlg(hDlgWnd);
					SendMessage(hDlgWnd, WM_CLOSE, 0, 0L);
					MakeExpConditionDlg(hInstGL);
					break;

				case IDCANCEL:
					SendMessage(hDlgWnd, WM_CLOSE, 0, 0L);
					break;

				default:
					return FALSE;

			}
			break;

		case WM_CLOSE:
			InvalidateRect(hParent, NULL, TRUE);
			EndDialog(hDlgWnd, IDOK);
			break;

		default:
			return FALSE;

	}

	return TRUE;
}

//	LRESULT CALLBACK ExpConfigDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL MakeExpConditionDlg(HINSTANCE)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Make the dialog box for the experimental conditions
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL MakeExpConditionDlg(HINSTANCE hInst)
{
	if (DialogBox(hInst, 
		MAKEINTRESOURCE(IDD_EXP_CONDITION), hAppWnd, (DLGPROC)ExpConditionDlgProc) == IDCANCEL) {
		return FALSE;
	}

	return TRUE;
}

//	BOOL MakeExpConditionDlg(HINSTANCE)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	LRESULT CALLBACK ExpConditionDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Procedure of dialog box for the experimental conditions
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ExpConditionDlgProc(HWND hDlgWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {

		case WM_COMMAND:
			switch (LOWORD(wParam)) {

				case IDC_CONDITION_1:
					ConditionFlag = 1;
					//dPhantomStiffness = 1.0;
					iHoldCount = (int)0.0;
					DelayedDevice = DELAYED_NONE;
					SendMessage(hDlgWnd, WM_CLOSE, 0, 0L);
					break;

				case IDC_CONDITION_2:
					ConditionFlag = 2;
					//dPhantomStiffness = 1.0;
					iHoldCount = (int)(dDelayTime / SAMP);
					DelayedDevice = DELAYED_SLAVE;
					SendMessage(hDlgWnd, WM_CLOSE, 0, 0L);
					break;

				case IDC_CONDITION_3:
					ConditionFlag = 3;
					//dPhantomStiffness = 1.0;
					iHoldCount = (int)0.0;
					DelayedDevice = DELAYED_NONE;
					SendMessage(hDlgWnd, WM_CLOSE, 0, 0L);
					break;

				case IDC_CONDITION_4:
					ConditionFlag = 4;
					//dPhantomStiffness = 1.0;
					iHoldCount = (int)(dDelayTime / SAMP);
					DelayedDevice = DELAYED_SLAVE;
					SendMessage(hDlgWnd, WM_CLOSE, 0, 0L);
					break;

				default:
					return FALSE;

			}
			break;

		case WM_CLOSE:
			EndDialog(hDlgWnd, IDOK);
			break;

		default:
			return FALSE;

	}

	return TRUE;
}

//	LRESULT CALLBACK ExpConfigDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void SetExpConfigDlg(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Set the edit boxes in the dialog box for configuring experimental conditions
///////////////////////////////////////////////////////////////////////////////////////////////////
void SetEditBoxInExpConfigDlg(HWND hDlgWnd)
{
	char str[256];

	//  Set the values in edit boxes
	sprintf(str, "%5.2e", dPID_Kp[0]);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_PGAIN_0), (LPCTSTR)str);
	sprintf(str, "%5.2e", dPID_Kp[1]);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_PGAIN_1), (LPCTSTR)str);
	sprintf(str, "%5.2e", dPID_Kp[2]);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_PGAIN_2), (LPCTSTR)str);

	sprintf(str, "%5.2e", dPID_Ki[0]);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_IGAIN_0), (LPCTSTR)str);
	sprintf(str, "%5.2e", dPID_Ki[1]);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_IGAIN_1), (LPCTSTR)str);
	sprintf(str, "%5.2e", dPID_Ki[2]);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_IGAIN_2), (LPCTSTR)str);

	sprintf(str, "%5.2e", dPID_Kd[0]);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_DGAIN_0), (LPCTSTR)str);
	sprintf(str, "%5.2e", dPID_Kd[1]);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_DGAIN_1), (LPCTSTR)str);
	sprintf(str, "%5.2e", dPID_Kd[2]);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_DGAIN_2), (LPCTSTR)str);

	sprintf(str, "%5.2e", dCompliance_M);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_COMP_INERTIA), (LPCTSTR)str);
	sprintf(str, "%5.2e", dCompliance_D);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_COMP_VISCOSITY), (LPCTSTR)str);
	sprintf(str, "%5.2e", dCompliance_K);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_COMP_STIFFNESS), (LPCTSTR)str);

	sprintf(str, "%5.2e", dPhantomStiffness);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_STIFFNESS), (LPCTSTR)str);

	if (DeviceNum == DEV_PHANTOM) {
		sprintf(str, "%5.2e", dPhantomInertia);
		SetWindowText(GetDlgItem(hDlgWnd, IDC_INERTIA), (LPCTSTR)str);
		sprintf(str, "%5.2e", dPhantomViscosity);
		SetWindowText(GetDlgItem(hDlgWnd, IDC_VISCOSITY), (LPCTSTR)str);
		sprintf(str, "%5.2e", dPhantomStiffness);
		SetWindowText(GetDlgItem(hDlgWnd, IDC_STIFFNESS), (LPCTSTR)str);
	}

	sprintf(str, "%5.3f", dDelayTime);
	SetWindowText(GetDlgItem(hDlgWnd, IDC_DELAY_TIME), (LPCTSTR)str);
}


//	void SetExpConfigDlg(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void LoadExpConfigDlg(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Load the edit boxes in the dialog box for configuring experimental conditions
///////////////////////////////////////////////////////////////////////////////////////////////////
void LoadEditBoxInExpConfigDlg(HWND hDlgWnd)
{
	char str[256];
	float dummy;

	//  Load the values in edit boxes
	GetWindowText(GetDlgItem(hDlgWnd, IDC_PGAIN_0), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dPID_Kp[0] = (double)dummy;
	GetWindowText(GetDlgItem(hDlgWnd, IDC_PGAIN_1), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dPID_Kp[1] = (double)dummy;
	GetWindowText(GetDlgItem(hDlgWnd, IDC_PGAIN_2), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dPID_Kp[2] = (double)dummy;

	GetWindowText(GetDlgItem(hDlgWnd, IDC_IGAIN_0), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dPID_Ki[0] = (double)dummy;
	GetWindowText(GetDlgItem(hDlgWnd, IDC_IGAIN_1), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dPID_Ki[1] = (double)dummy;
	GetWindowText(GetDlgItem(hDlgWnd, IDC_IGAIN_2), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dPID_Ki[2] = (double)dummy;

	GetWindowText(GetDlgItem(hDlgWnd, IDC_DGAIN_0), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dPID_Kd[0] = (double)dummy;
	GetWindowText(GetDlgItem(hDlgWnd, IDC_DGAIN_1), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dPID_Kd[1] = (double)dummy;
	GetWindowText(GetDlgItem(hDlgWnd, IDC_DGAIN_2), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dPID_Kd[2] = (double)dummy;

	GetWindowText(GetDlgItem(hDlgWnd, IDC_COMP_INERTIA), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dCompliance_M = (double)dummy;
	GetWindowText(GetDlgItem(hDlgWnd, IDC_COMP_VISCOSITY), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dCompliance_D = (double)dummy;
	GetWindowText(GetDlgItem(hDlgWnd, IDC_COMP_STIFFNESS), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dCompliance_K = (double)dummy;

	GetWindowText(GetDlgItem(hDlgWnd, IDC_STIFFNESS), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	dPhantomStiffness = (double)dummy;

	if (DeviceNum == DEV_PHANTOM) {
		GetWindowText(GetDlgItem(hDlgWnd, IDC_INERTIA), str, sizeof(str));
		sscanf(str, "%f", &dummy);
		dPhantomInertia = (double)dummy;
		GetWindowText(GetDlgItem(hDlgWnd, IDC_VISCOSITY), str, sizeof(str));
		sscanf(str, "%f", &dummy);
		dPhantomViscosity = (double)dummy;
		GetWindowText(GetDlgItem(hDlgWnd, IDC_STIFFNESS), str, sizeof(str));
		sscanf(str, "%f", &dummy);
		dPhantomStiffness = (double)dummy;
	}

	GetWindowText(GetDlgItem(hDlgWnd, IDC_DELAY_TIME), str, sizeof(str));
	sscanf(str, "%f", &dummy);
	/*iHoldCount = (int)(dummy / SAMP)*/;
	dDelayTime = (double)dummy;
}


//	void LoadExpConfigDlg(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL SaveExpParameter(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Save the experimental parameters
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SaveExpParameter(void)
{
	FILE *fp;

	//  File open
	if ((fp = fopen("ExpParameter.txt", "wt")) == NULL) {
		return FALSE;
	}

	//  Save the experimental paramteres
	for (unsigned i = 0; i < 3; i++) {
		fprintf(fp, "%lf	", dPID_Kp[i]);
	}
	fprintf(fp, "\n");

	for (unsigned i = 0; i < 3; i++) {
		fprintf(fp, "%lf	", dPID_Ki[i]);
	}
	fprintf(fp, "\n");

	for (unsigned i = 0; i < 3; i++) {
		fprintf(fp, "%lf	", dPID_Kd[i]);
	}
	fprintf(fp, "\n");

	fprintf(fp, "%lf	", dCompliance_M);
	fprintf(fp, "%lf	", dCompliance_D);
	fprintf(fp, "%lf	", dCompliance_K);
	fprintf(fp, "\n");

	fprintf(fp, "%lf	", dPhantomStiffness);
	fprintf(fp, "\n");

	fprintf(fp, "%lf	", dPhantomInertia);
	fprintf(fp, "%lf	", dPhantomViscosity);
	fprintf(fp, "%lf	", dPhantomStiffness);
	fprintf(fp, "\n");

	fprintf(fp, "%lf	", dDelayTime);

	//  File close
	fclose(fp);

	return TRUE;
}

//	BOOL SaveExpParameter(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL LoadExpParameter(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Load the experimental parameters
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL LoadExpParameter(void)
{
	FILE *fp;

	//  File open
	if ((fp = fopen("ExpParameter.txt", "rt")) == NULL) {
		return FALSE;
	}

	//  Save the experimental paramteres
	for (unsigned i = 0; i < 3; i++) {
		fscanf(fp, "%lf", &dPID_Kp[i]);
	}

	for (unsigned i = 0; i < 3; i++) {
		fscanf(fp, "%lf", &dPID_Ki[i]);
	}

	for (unsigned i = 0; i < 3; i++) {
		fscanf(fp, "%lf", &dPID_Kd[i]);
	}

	fscanf(fp, "%lf", &dCompliance_M);
	fscanf(fp, "%lf", &dCompliance_D);
	fscanf(fp, "%lf", &dCompliance_K);

	fscanf(fp, "%lf", &dPhantomStiffness);

	fscanf(fp, "%lf", &dPhantomInertia);
	fscanf(fp, "%lf", &dPhantomViscosity);
	fscanf(fp, "%lf", &dPhantomStiffness);

	fscanf(fp, "%lf", &dDelayTime);

	//  File close
	fclose(fp);

	return TRUE;
}

//	BOOL LoadExpParameter(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL InitSaveData(HINSTANCE)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Initialize for saving data
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL InitSaveData(HINSTANCE hInst)
{
	if (DialogBox(hInst, 
		MAKEINTRESOURCE(IDD_SAVE_DATA), hAppWnd, (DLGPROC)SaveDataDlgProc) == IDCANCEL) {
		return FALSE;
	}

	return TRUE;
}

//	BOOL InitSaveData(HINSTANCE)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	LRESULT CALLBACK SaveDataDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Procedure of dialog box for saving data
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK SaveDataDlgProc(HWND hDlgWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hParent;
	
	hParent = GetParent(hDlgWnd);

	switch (msg) {
		case WM_INITDIALOG:
			ComboNum = 0;
			SendMessage(GetDlgItem(hDlgWnd,	IDC_RADIO1), BM_SETCHECK, (WPARAM)RadioBox[0], 0L);
			SendMessage(GetDlgItem(hDlgWnd,	IDC_RADIO2), BM_SETCHECK, (WPARAM)RadioBox[1], 0L);
			for (unsigned i = 0; i < 3; i++) {
				ComboBox_AddString(GetDlgItem(hDlgWnd, IDC_COMBO1), ComboBox[i]);
			}
			ComboBox_SetCurSel(GetDlgItem(hDlgWnd, IDC_COMBO1), ComboNum);
			SetDefaultFileName(hDlgWnd);
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDC_RADIO1:
				case IDC_RADIO2:
					if(IsDlgButtonChecked(hDlgWnd, IDC_RADIO1) == BST_CHECKED){
						RadioBox[0] = 1;
					} else {
						RadioBox[0] = 0;
					}
					if(IsDlgButtonChecked(hDlgWnd, IDC_RADIO2) == BST_CHECKED){
						RadioBox[1] = 1;
					} else {
						RadioBox[1] = 0;
					}
					break;

				case IDC_COMBO1:
					ComboNum = ComboBox_GetCurSel(GetDlgItem(hDlgWnd, IDC_COMBO1));
					break;

				case IDC_SAVE:
					if (SaveDataToFile(ReadFileName(hDlgWnd)) == FALSE) {
						MessageBox(hAppWnd, TEXT("Save Data is not Done"),	
							TEXT("Caution"), MB_OK | MB_ICONEXCLAMATION);
						break;
					}
					InvalidateRect(hParent, NULL, TRUE);
					EndDialog(hDlgWnd, IDOK);
					MessageBox(hAppWnd, TEXT("Save Data is Completed"),	
							TEXT("End of Save Data"), MB_OK | MB_ICONINFORMATION);
					break;

				case IDC_CANCEL:
					SendMessage(hDlgWnd, WM_CLOSE, 0, 0L);
					break;

				default:
					return FALSE;

			}
			break;

		case WM_CLOSE:
			InvalidateRect(hParent, NULL, TRUE);
			EndDialog(hDlgWnd, IDOK);
			MessageBox(hAppWnd, TEXT("Program is stopped"),	
				TEXT("End of Program"), MB_OK | MB_ICONINFORMATION);
			break;

		default:
			return FALSE;

	}

	return TRUE;
}

//	LRESULT CALLBACK SaveDataDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void SetDefaultFileName(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Set default name to static edit box
///////////////////////////////////////////////////////////////////////////////////////////////////
void SetDefaultFileName(HWND hDlgWnd)
{
	char str[256];
	time_t standard_time;
	struct tm *current_time;

	time(&standard_time);
	
	current_time = localtime(&standard_time);
	sprintf(str, "logdata_20%02d-%02d-%02d.%02d.%02d.%02d",
		current_time->tm_year - 100, current_time->tm_mon, current_time->tm_mday,
		current_time->tm_hour, current_time->tm_min, current_time->tm_sec);

	SetWindowText(GetDlgItem(hDlgWnd, IDC_EDIT1), (LPCTSTR)str);
}

//  void SetDefaultFileName(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  void ReadFileName(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Read file name from edit box
///////////////////////////////////////////////////////////////////////////////////////////////////
char *ReadFileName(HWND hDlgWnd)
{
	char str[256];

	time_t standard_time;
	struct tm *current_time;

	if (RadioBox[0] == 1 && RadioBox[1] == 0) {
		GetWindowText(GetDlgItem(hDlgWnd, IDC_EDIT1), str, sizeof(str));
		sprintf(FileName, "%s%s", str, ComboBox[ComboNum]);
	} else if (RadioBox[0] == 0 && RadioBox[1] == 1) {
		GetWindowText(GetDlgItem(hDlgWnd, IDC_EDIT2), str, sizeof(str));
		time(&standard_time);
		current_time = localtime(&standard_time);	
		sprintf(FileName, "%s_C%d_20%02d-%02d-%02d.%02d.%02d.%02d%s", str, ConditionFlag,
			current_time->tm_year - 100, current_time->tm_mon, current_time->tm_mday,
			current_time->tm_hour, current_time->tm_min, current_time->tm_sec, ComboBox[ComboNum]);
	}

	return FileName;
}

//  void ReadFileName(HWND)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL SaveDataToFile(char *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Save data to file
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SaveDataToFile(char *filename)
{
	FILE *log_file;
	char str[256];

	strcpy(str, LogFilePass);
	strcat(str, filename);

	FILE *log_file1, *log_file2;
	char str1[256];
	char str2[256];

	sprintf(str1, "%s(Drift1)%s", LogFilePass, filename);
	sprintf(str2, "%s(Drift2)%s", LogFilePass, filename);

	//  File open
	if ((log_file = fopen(str, "wt")) == NULL) {
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//	Save title of variable
	///////////////////////////////////////////////////////////////////////////////////////////////
	fprintf(log_file, "Time	");

	for (unsigned i = 0; i < 4; i++) {
		fprintf(log_file, "Output Voltage[%d]	", i);
	}

	for (unsigned i = 0; i < 3; i++) {
		fprintf(log_file, "Real Angle[%d]	", i);
	}

	for (unsigned i = 0; i < 3; i++) {
		fprintf(log_file, "Desired Angle[%d]	", i);
	}

	for (unsigned i = 0; i < 3; i++) {
		fprintf(log_file, "Real Position[%d]	", i);
	}

	for (unsigned i = 0; i < 3; i++) {
		fprintf(log_file, "Desired Position[%d]	", i);
	}

	fprintf(log_file, "Contact Position	");
	fprintf(log_file, "Contact Flag	");

	fprintf(log_file, "\n");

	///////////////////////////////////////////////////////////////////////////////////////////////
	//	Save variables
	///////////////////////////////////////////////////////////////////////////////////////////////
	for (unsigned i = 1; ((i < TimerCounter) && (i < (int)(EXP_DURATION / SAMP))); i++) {//SAVEDATAMAX)); i++) {
		if (TimerCounter_Log[i] != NULL) {
			fprintf(log_file, "%lf	", TimerCounter_Log[i] * SAMP);

			for (unsigned j = 0; j < 3; j++) {
				fprintf(log_file, "%lf	", OutputVoltage_Log[j][i]);
			}

			for (unsigned j = 0; j < 3; j++) {
				fprintf(log_file, "%lf	", RealAngle_Log[j][i]);
			}

			for (unsigned j = 0; j < 3; j++) {
				fprintf(log_file, "%lf	", DesiredAngle_Log[j][i]);
			}

			for (unsigned j = 0; j < 3; j++) {
				fprintf(log_file, "%lf	", RealPosition_Log[j][i]);
			}

			for (unsigned j = 0; j < 3; j++) {
				fprintf(log_file, "%lf	", DesiredPosition_Log[j][i]);
			}

			fprintf(log_file, "%lf	", ContactPosition_Log[i]);
			fprintf(log_file, "%d	", ContactFlag_Log[i]);

			fprintf(log_file, "\n");
		}
	}

	//  File close
	fclose(log_file);

	//  File open
	if ((log_file1 = fopen(str1, "wt")) == NULL) {
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//	Save title of variable
	///////////////////////////////////////////////////////////////////////////////////////////////
	fprintf(log_file1, "Time	");

	for (unsigned i = 0; i < 3; i++) {
		fprintf(log_file1, "Desired Position[%d]	", i);
	}

	fprintf(log_file1, "\n");

	///////////////////////////////////////////////////////////////////////////////////////////////
	//	Save variables
	///////////////////////////////////////////////////////////////////////////////////////////////
	for (unsigned i = 1; ((i < TimerCounter1) && (i < (int)(10.0 / SAMP))); i++) {
		if (TimerCounter2_Log[i] != NULL) {
			fprintf(log_file1, "%lf	", TimerCounter1_Log[i] * SAMP);

			for (unsigned j = 0; j < 3; j++) {
				fprintf(log_file1, "%lf	", DesiredPosition1_Log[j][i]);
			}

			fprintf(log_file1, "\n");
		}
	}

	//  File close
	fclose(log_file1);

	//  File open
	if ((log_file2 = fopen(str2, "wt")) == NULL) {
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//	Save title of variable
	///////////////////////////////////////////////////////////////////////////////////////////////
	fprintf(log_file2, "Time	");

	for (unsigned i = 0; i < 3; i++) {
		fprintf(log_file2, "Desired Position[%d]	", i);
	}

	fprintf(log_file2, "\n");

	///////////////////////////////////////////////////////////////////////////////////////////////
	//	Save variables
	///////////////////////////////////////////////////////////////////////////////////////////////
	for (unsigned i = 1; ((i < TimerCounter2) && (i < (int)(10.0 / SAMP))); i++) {
		if (TimerCounter2_Log[i] != NULL) {
			fprintf(log_file2, "%lf	", TimerCounter2_Log[i] * SAMP);

			for (unsigned j = 0; j < 3; j++) {
				fprintf(log_file2, "%lf	", DesiredPosition2_Log[j][i]);
			}

			fprintf(log_file2, "\n");
		}
	}

	//  File close
	fclose(log_file2);

	return TRUE;
}

//	BOOL SaveDataToFile(char *)
///////////////////////////////////////////////////////////////////////////////////////////////////