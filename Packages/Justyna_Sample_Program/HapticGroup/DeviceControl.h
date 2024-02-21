//////////////////////////////////////////////////////////////////////////////////////////////////
//  DeviceControl.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Header of DeviceControl.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Programmer:		Masayuki HARA (Assitant Professor)
//  Affiliation:	Higuchi & Yamamoto Lab. (Advanced Mechatoronics Lab.)
//					School of Engineering, The University of Tokyo
//  Created date:	13.01.2009
//	Updated date:	15.01.2009
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  include



///////////////////////////////////////////////////////////////////////////////////////////////////
//  define

#define SAVEDATAMAX 600000							//  Maximum number of save data

#define NORMAL_MOV 0
#define MIRROR_MOV 1

#define DEFAULT 0
#define INITIALIZATION 1
#define EXPERIMENT 2

#define ANIMATION_OFF 0
#define ANIMATION_ON 1

//#define DEBUG


///////////////////////////////////////////////////////////////////////////////////////////////////
//  variable

extern int DeviceNum;

extern double dRealPosition[3];
extern double dRealPositionPre[3];
extern double dDesiredPosition[3];

extern double dPhantomAngle[3];
extern double dGimbalAngle[3];
extern double dOrientation[3];

extern double dContactPosition[3];
extern double dInitialPosition[3];

extern double dAppliedForceVec[3];
extern double dAppliedForceVecPre[3];
extern double dContactForce[3];

extern int DeviceMovement[3];

extern double dForceAmplitude;

extern double dSliderPosition;

extern int ExpMode;
extern int TrainingFlag;
extern int AnimationFlag;
extern int AnimationFlag2;
extern int AnimationFlag3;

extern int ExpFlag;
extern int VisualFlag;
extern int ConditionFlag;

extern ULONG TimerCounter;
extern ULONG AnimationCounter;
extern ULONG AnimationCounter2;
extern ULONG AnimationCounter3;


///////////////////////////////////////////////////////////////////////////////////////////////////
//  function

void InitSampler(void);
void StopSampler(void);

BOOL InitSaveData(HINSTANCE);
void ExternalInputByKeystroke(WPARAM);

BOOL MakeExpConfigDlg(HINSTANCE);