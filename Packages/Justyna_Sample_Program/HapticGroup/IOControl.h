//////////////////////////////////////////////////////////////////////////////////////////////////
//  IOControl.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Header of IOControl.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Programmer:		Masayuki HARA (Assitant Professor)
//  Affiliation:	Higuchi & Yamamoto Lab. (Advanced Mechatoronics Lab.)
//					School of Engineering, The University of Tokyo
//  Created date:	26.01.2009
//	Updated date:	10.04.2009
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  include



///////////////////////////////////////////////////////////////////////////////////////////////////
//  define

#define AO_DEV_NUM 1							//  NI-DAQ device number for analog output
#define AI_DEV_NUM 0							//  NI-DAQ device number for analog input
#define CTR_DEV_NUM 1							//  NI-DAQ device number for counter
#define DO_DEV_NUM 0							//  NI-DAQ device number for digital output
#define DI_DEV_NUM 0							//  NI-DAQ device number for digital input

#define AO_PORT_NUM 4							//  Analog-output port number
#define AI_PORT_NUM 0							//  Aalog-input port number
#define CTR_PORT_NUM 2							//  Countr port number
#define DO_PORT_NUM 0							//  Digital-output port number
#define DI_PORT_NUM 0							//  Digital-inputt port number

#define DO_LOW 0								//  Digital low output
#define DO_HIGH 1								//  Digital hight output

#define DI_LOW 0								//  Digital low input
#define DI_HIGH 1								//  Digital hight input


///////////////////////////////////////////////////////////////////////////////////////////////////
//  variable



///////////////////////////////////////////////////////////////////////////////////////////////////
//  function

int IO_OpenDevice(void);						//  Open NI-DAQ devices (Create tasks)
int IO_StartDevice(void);						//  Start NI-DAQ devices

int IO_StopDevice(void);						//  Stop NI-DAQ devices
int IO_CloseDevice(void);						//  Close NI-DAQ devices

void IO_OutputVoltage(double *);				//  Output analog voltage
void IO_OutputZeroVoltage(void);				//  Output zero voltage for all the opened AO ports

void IO_GetAnalogVoltage(double *input);		//  Get analog voltage

void IO_GetAngleFromEnc(double *);				//  Get angle vector from encoders
void IO_GetJointAngle(double *);				//  Get joint angle

void IO_SetDigitalOutput(int *);				//  Set ditgital output
void IO_SetDigitalOutputAllLow(void);			//  Set low value for all digital-output ports
void IO_SetDigitalOutputAllHigh(void);			//  Set high value for all digital-output ports
void IO_GetDigitalInput(int *);					//  Get ditgital input