///////////////////////////////////////////////////////////////////////////////////////////////////
//  HapticTools.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
//  This source file includes functions for PHANToM Omni
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Programmer:		Masayuki HARA (Assitant Professor)
//  Affiliation:	Higuchi & Yamamoto Lab. (Advanced Mechatoronics Lab.)
//					School of Engineering, The University of Tokyo
//  Created date:	09.02.2010
//	Updated date:	09.02.2010
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

#include "resource.h"

#include "DeviceControl.h"
#include "HapticTools.h"
#include "IOControl.h"
#include "MainGraph.h"
#include "RoboticsTools.h"
#include "SamplerTools.h"
#include "WinStyleMain.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//	define


///////////////////////////////////////////////////////////////////////////////////////////////////
//	variables

//  Variables for PHANToM Omni
HHD hHD;
HDCallbackCode hPhantomCallback;
BOOL PhantomFlag = FALSE;

double dPhantomInertia = 0.0;
double dPhantomViscosity = 0.0;
double dPhantomStiffness;

double dPhantomPositionError[3];
double dPhantomPositionErrorPre[3];
double dPhantomVelocity[3];
double dFilteredPhantomVelocity[3];
double dFilteredPhantomVelocityPre[3];

double dPhantomCutoffFreq = 30.0;


///////////////////////////////////////////////////////////////////////////////////////////////////
//	functions

HDCallbackCode HDCALLBACK PhantomCallbackFunction(void *);


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Functions for PHANToM Omni
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  BOOL HT_InitializePhantom(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Initialize the connected PHANToM Omni
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL HT_InitializePhantom(void)
{
	//  Open the device
	hHD = hdInitDevice(HD_DEFAULT_DEVICE);
	if (HD_DEVICE_ERROR(hdGetError())) {
		return FALSE;
	}
	
	//  Allow the force output
	hdEnable(HD_FORCE_OUTPUT);

	//  Start the scheduler of PHANToM Omni
	hdStartScheduler();
	if (HD_DEVICE_ERROR(hdGetError())) {
		return FALSE;
	}

	//  Set the callback function
	hPhantomCallback = hdScheduleAsynchronous(PhantomCallbackFunction, 
		0, HD_DEFAULT_SCHEDULER_PRIORITY);

	return TRUE;
}

//  BOOL HT_InitializePhantom(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  BOOL HT_ClosePhantom(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Close the connected PHANToM Omni
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL HT_ClosePhantom(void)
{
	// Stop the scheduler of PHANToM Omni
	hdStopScheduler();

	//  unschedule the callbacks
	hdUnschedule(hPhantomCallback);

	//  Stop the device
	hdDisableDevice(hHD);

	//  1 sec wait for stabilizing
	Sleep(1000);

	//  Check the error
	if (HD_DEVICE_ERROR(hdGetError())) {
		return FALSE;
	}

	return TRUE;
}

//  BOOL HT_ClosePhantom(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//  HDCallbackCode HDCALLBACK PhantomCallbackFunction(void *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Callback function for PHANToM Omni
///////////////////////////////////////////////////////////////////////////////////////////////////
HDCallbackCode HDCALLBACK PhantomCallbackFunction(void *data)
{
    hduVector3Dd pos;								//  Position of PHANToM Omni
	hduVector3Dd force;								//  Force of PHANToM Omni
	hduVector3Dd angle;								//  Joint angle of PHANToM Omni
	hduVector3Dd gangle;							//  Gimbal angle of PHANToM Omni
	hduVector3Dd direction(0, 0, 1);

	//  Bigin the frame
	hdBeginFrame(hdGetCurrentDevice());

	//  Get the position of PHANToM Omni
	hdGetDoublev(HD_CURRENT_POSITION, pos);

	//  Get the joint angle of PHANToM Omni (based on the base frame)
	hdGetDoublev(HD_CURRENT_JOINT_ANGLES, angle);
	for (unsigned int i = 0; i < 3; i++) {
		dPhantomAngle[i] = angle[i];
	}

	//  Get the gimbal angle of PHANToM Omni
	hdGetDoublev(HD_CURRENT_GIMBAL_ANGLES, gangle);
	for (unsigned int i = 0; i < 3; i++) {
		dGimbalAngle[i] = gangle[i];
	}

	//  Calculate the desired position of slave device
//	dDesiredPosition[0] = (pos[2] - dInitialPosition[0]) + dLinkLength[1];
//	dDesiredPosition[1] = (pos[0] - dInitialPosition[1]) + dLinkLength[0];
//	dDesiredPosition[2] = (pos[1] - dInitialPosition[2]) - dLinkLength[2];

	dDesiredPosition[2] = (pos[1] + 25.0) - dLinkLength[2];
	dDesiredPosition[0] = pos[2] + dLinkLength[1];
	dDesiredPosition[1] = pos[0] + dLinkLength[0];
//	dDesiredPosition[2] = pos[1] - dLinkLength[2];

//	dDesiredPosition[0] = pos[0];
//	dDesiredPosition[1] = pos[1];
//	dDesiredPosition[2] = pos[2];

	//  Caluculate the position error and velocity
	dPhantomPositionError[0] = - dDesiredPosition[1];
	dPhantomPositionError[1] = dContactPosition[2] - dDesiredPosition[2];
	dPhantomPositionError[2] = - dDesiredPosition[0];
	for (unsigned int i = 0; i < 3; i++) {
		dPhantomVelocity[i] = (dPhantomPositionError[i] - dPhantomPositionErrorPre[i]) / SAMP;
		dFilteredPhantomVelocity[i] = (dPhantomPositionError[i] - dPhantomPositionErrorPre[i]
		+ dFilteredPhantomVelocityPre[i] / dPhantomCutoffFreq) / (1.0 / dPhantomCutoffFreq + SAMP); 
	}

	//  Set the desired force
	if (dDesiredPosition[2] < dContactPosition[2]) {
//		force[0] = dPhantomViscosity * dPhantomVelocity[0];
		force[1] = dPhantomStiffness * dPhantomPositionError[1] + dPhantomViscosity * dFilteredPhantomVelocity[1];
//		force[2] = dPhantomViscosity * dPhantomVelocity[2];
	} else {
		force[1] = 0.0;
	}
	dContactForce[0] = force[2];
	dContactForce[1] = force[0];
	dContactForce[2] = force[1];
/*
	force[0] = dForceAmplitude * pow(-1.0, DeviceMovement[1]) * dAppliedForceVec[1];
	force[1] = dForceAmplitude * pow(-1.0, DeviceMovement[2]) * dAppliedForceVec[2];
	force[2] = dForceAmplitude * -pow(-1.0, DeviceMovement[0]) * dAppliedForceVec[0];
*/

	//  Shift data
	for (unsigned int i = 0; i < 3; i++) {
		dPhantomPositionErrorPre[i] = dPhantomPositionError[i];
		dFilteredPhantomVelocityPre[i] = dFilteredPhantomVelocity[i];
	}

	//  Generate the desired force
	hdSetDoublev(HD_CURRENT_FORCE, force);

	//  End the frame
	hdEndFrame(hdGetCurrentDevice());

    return HD_CALLBACK_CONTINUE;
}

//  HDCallbackCode HDCALLBACK PhantomCallbackFunction(void *)
///////////////////////////////////////////////////////////////////////////////////////////////////