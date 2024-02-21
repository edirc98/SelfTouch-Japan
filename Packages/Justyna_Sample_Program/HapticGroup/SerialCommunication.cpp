//////////////////////////////////////////////////////////////////////////////////////////////////
//  SerialCommunication.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
//  This source file includes functions for sampler
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Programmer:		Masayuki HARA (Postdoctoral Researcher)
//  Affiliation:	EPFL/STI/IMT/LSRO1(Robotic Systems Laboratory 1)
//					Swiss Federal Institute of Technology, Lausanne (EPFL)
//  Created date:	14.01.2009
//	Updated date:	26.01.2009
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//  include

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>

#include "resource.h"

#include "SerialCommunication.h"
#include "WinStyleMain.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//	define

#define COM_PORT_NUM 8
#define BAUD_RATE_NUM 11


///////////////////////////////////////////////////////////////////////////////////////////////////
//	variable

HANDLE hComm;										//  Handle of COM port
DCB setDCB;											//  DCB structure
COMMTIMEOUTS timeOut;								//  Timeout

char szComPortCombo[COM_PORT_NUM][16] = {			//  Number of COM port
	"COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8"
};

char szBaudRateCombo[BAUD_RATE_NUM][16] = {		//  Baud rate
	"1000000", "500000", "400000", "250000", "200000","115200", "57600", "38400", "31250", "19200", "9600"
};

char szComNumber[16];								//  Selected COM port
char szBaudRate[16];								//  Selected baud rate
char szMessage[256];								//  Message

int nAddrCom;
int nAddrBps;

int	ComMode = 0;									//  Open flag of COM port


///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL SC_COMPortOpen(char *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Open COM port
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SC_COMPortOpen(char *szComNum)
{
	//  Close COM port (just in case)
	SC_COMPortClose();

	//  Open the resourse for serial communication
	hComm = CreateFile(szComNum, 
		GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//  If the resourse couldn't be opened,
	if (hComm == INVALID_HANDLE_VALUE) {
		//  Close COM port
		SC_COMPortClose();
		return FALSE;
	}

	return TRUE;
}

//	BOOL SC_COMPortOpen(char *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL SC_COMPortSet(char *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Configure COM port
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SC_COMPortSet(char *szBaudRate)
{
	int ret;

	//  Get DCB structure
	GetCommState(hComm, &setDCB);

	//  Configure COM port
	setDCB.BaudRate = atoi(szBaudRate);			//  Baud rate
	setDCB.ByteSize = 8;						//  Bit length = 8 bit
	setDCB.Parity = NOPARITY;					//  Parity = Non-parity
	setDCB.StopBits = ONESTOPBIT;				//  Stop bit = 1 stop bit
	
	//  Configure DCB structure
	ret = SetCommState(hComm, &setDCB);

	// If the configureation was failed
	if (ret == 0) {
		//  Close COM port
		SC_COMPortClose();
		return FALSE;
	}

	return TRUE;
}

//	BOOL SC_COMPortSet(char *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL SC_TimeoutSet(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Configure timeout
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SC_TimeoutSet(void)
{
	int ret;

	//  Get timeout
	ret = GetCommTimeouts(hComm, &timeOut);

	//  Configure timeout
	timeOut.ReadIntervalTimeout = 500;			//  Wait time for loading one character [ms]
	timeOut.ReadTotalTimeoutMultiplier = 500;	//  Time for loading one character [ms]
	timeOut.ReadTotalTimeoutConstant = 500;		//  Constant time for loading [ms]
	timeOut.WriteTotalTimeoutMultiplier = 500;	//  Time for writing one character [ms]
	timeOut.WriteTotalTimeoutConstant = 500;	//  Constant time for writing[ms]

	//  Configure timeout
	ret = SetCommTimeouts(hComm, &timeOut);

	//  If the configureation was failed
	if (ret == 0) {
		//  Close COM port
		SC_COMPortClose();
		return FALSE;
	}

	return TRUE;
}

//	BOOL SC_TimeoutSet(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	BOOL SC_COMPortClose(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Close COM port
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SC_COMPortClose(void)
{
	//  Close handle of COM port
	if (CloseHandle(hComm) == FALSE) {
		return FALSE;
	}

	return TRUE;
}


//	BOOL SC_COMPortClose(void)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Function for configuration dialog box
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//	void SC_COMPortSetDlgInit(HINSTANCE)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Call configuration dialog box for serial communication
///////////////////////////////////////////////////////////////////////////////////////////////////
void SC_COMPortSetDlgInit(HINSTANCE hInst)
{
	//  Call dialog box
	DialogBox(hInst, MAKEINTRESOURCE(IDD_COM_PORT_SET),hAppWnd, (DLGPROC)SC_COMPortSetDlgProc);
}

//	void SC_COMPortSetDlgInit(HINSTANCE)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	LRESULT CALLBACK SC_COMPortSetDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Callback function for configuration dialog box
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK SC_COMPortSetDlgProc(HWND hDlgWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hComPortCombo;
	static HWND hBaudRateCombo;
	int i;
	
	HWND hParent;
	
	//  Get handle of parent window
	hParent = GetParent(hDlgWnd);

	//  Dialog box procedure
	switch(msg){

		case WM_INITDIALOG:
			//  Get handle of combo box for COM port configuration
			hComPortCombo = GetDlgItem(hDlgWnd, IDC_COM_COMBO);

			//  Add string in combo box for COM port configuration
			for (i = 0; i < COM_PORT_NUM; i++) {
				ComboBox_AddString(hComPortCombo, szComPortCombo[i]);
			}

			//  Initialize combo box for COM port configuration
			ComboBox_SetCurSel(hComPortCombo, nAddrCom);

			//  Get handle of combo box for baud rate configuration
			hBaudRateCombo = GetDlgItem(hDlgWnd, IDC_BAUDRATE_COMBO);

			//  Add string in combo box for baud configuration
			for (i = 0; i < BAUD_RATE_NUM; i++) {
				ComboBox_AddString(hBaudRateCombo, szBaudRateCombo[i]);
			}

			//  Initialize combo box for baud configuration
			ComboBox_SetCurSel(hBaudRateCombo, nAddrBps);

			break;

		case WM_COMMAND:

			switch(LOWORD(wParam)){

				case IDC_COM_OPEN:
					wsprintf(szMessage,
						"COM Port Open?\nPort Number : %s\nBaud Rate : %s", 
						szComPortCombo[nAddrCom], szBaudRateCombo[nAddrBps]);

					if (MessageBox(hDlgWnd, szMessage, "COM Port Open",
						MB_YESNO | MB_ICONQUESTION) == IDYES){

						//  If COM port was already opened
						if (ComMode == 1) {
							MessageBox(hDlgWnd, 
								"COM Port Already Opened", "Caution!!", MB_OK | MB_ICONEXCLAMATION);
							break;
						}

						//  Open COM port
						if (SC_COMPortOpen(szComPortCombo[nAddrCom]) == FALSE) {
							MessageBox(hDlgWnd,
								"Fail to COM Port Open", "Caution!!", MB_OK | MB_ICONEXCLAMATION);

							//  Close COM port
							SC_COMPortClose();

							break;
						}

						//  Configure DCB structure
						if (SC_COMPortSet(szBaudRateCombo[nAddrBps]) == FALSE) {
							MessageBox(hDlgWnd,
								"Fail to COM Set", "Caution!!", MB_OK | MB_ICONEXCLAMATION);
							
							//  Close COM port
							SC_COMPortClose();

							break;
						}

						//  Configure timeout
						if (SC_TimeoutSet() == FALSE) {
							MessageBox(hDlgWnd,
								"Fail to Timeout Set", "Caution!!", MB_OK | MB_ICONEXCLAMATION);

							//  Close COM port
							SC_COMPortClose();

							break;
						}

						//  Stand COM port open flag
						ComMode = 1;

						InvalidateRect(hParent, NULL, TRUE);
						EndDialog(hDlgWnd, IDCANCEL);
					}

					break;

				case IDC_COM_CLOSE:
					if (MessageBox(hDlgWnd, 
						"COM Port Close?", "COM Port Close", MB_YESNO | MB_ICONQUESTION) == IDYES) {

						//  Close COM port
						if (SC_COMPortClose() == FALSE) {
							MessageBox(hDlgWnd,
								"Fail to COM Port Close.", "Caution!!", MB_OK | MB_ICONEXCLAMATION);

							break;
						}

						//  Down COM port open flag
						ComMode = 0;

						InvalidateRect(hParent, NULL, TRUE);
						EndDialog(hDlgWnd, IDCANCEL);
					}

					break;

				case IDC_DEFAULT_SET:
					nAddrCom = 2;
					nAddrBps = 7;

					//  Initialize combo boxes
					ComboBox_SetCurSel(hComPortCombo, nAddrCom);
					ComboBox_SetCurSel(hBaudRateCombo, nAddrBps);

					wsprintf(szMessage,
						"COM Port Open?\nPort Number : %s\nBaud Rate : %s", 
						szComPortCombo[nAddrCom], szBaudRateCombo[nAddrBps]);

					if(MessageBox(hDlgWnd, 
						szMessage, "COM Port Open", MB_YESNO | MB_ICONQUESTION) == IDYES){

						//  If COM port was already opened
						if (ComMode == 1) {
							MessageBox(hDlgWnd, 
								"COM Port Already Opened", "Caution!!", MB_OK | MB_ICONEXCLAMATION);

							break;
						}

						//  Open COM port
						if (SC_COMPortOpen(szComPortCombo[nAddrCom]) == FALSE) {
							MessageBox(hDlgWnd,
								"Fail to COM Port Open", "Caution!!", MB_OK | MB_ICONEXCLAMATION);
	
							//  Close COM port
							SC_COMPortClose();

							break;
						}

						//  Configure DCB structure
						if (SC_COMPortSet(szBaudRateCombo[nAddrBps]) == FALSE) {
							MessageBox(hDlgWnd,
								"Fail to COM Set", "Caution!!", MB_OK | MB_ICONEXCLAMATION);

							//  Close COM port
							SC_COMPortClose();

							break;
						}

						//  Configure timeout
						if (SC_TimeoutSet() == FALSE) {
							MessageBox(hDlgWnd,
								"Fail to Timeout Set", "Caution!!", MB_OK | MB_ICONEXCLAMATION);

							//  Close COM port
							SC_COMPortClose();

							break;
						}

						//  Stand COM port open flag
						ComMode = 1;

						InvalidateRect(hParent, NULL, TRUE);
						EndDialog(hDlgWnd, IDCANCEL);
					}

					break;

				case IDC_COM_COMBO:
					nAddrCom = ComboBox_GetCurSel(hComPortCombo);
					
					break;

				case IDC_BAUDRATE_COMBO:
					nAddrBps = ComboBox_GetCurSel(hBaudRateCombo);
					
					break;

				case IDCANCEL:
					InvalidateRect(hParent, NULL, TRUE);
					EndDialog(hDlgWnd, IDCANCEL);

					break;

				default:
					return FALSE;

			}

			break;

		default:
			return FALSE;

	}

	return TRUE;
}


//	LRESULT CALLBACK SC_COMPortSetDlgProc(HWND, UINT, WPARAM, LPARAM)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Communication tools
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//	void SC_SendOneByteData(BYTE)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Send 1 byte data
///////////////////////////////////////////////////////////////////////////////////////////////////
void SC_SendOneByteData(BYTE wData)
{
	DWORD dwWritten;

	//  Write file function
	WriteFile(hComm, &wData, 1, &dwWritten, NULL);
}

//	void SC_SendOneByteData(BYTE)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void SC_SendStringData(UINT, BYTE *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Send string data
///////////////////////////////////////////////////////////////////////////////////////////////////
void SC_SendStringData(UINT num, BYTE *wData)
{
	DWORD dwWritten;

	//  Write file function
	WriteFile(hComm, wData, num, &dwWritten, NULL);
}

//	void SC_SendStringData(UINT, BYTE *)
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//	void SC_ReceiveStringData(BYTE *)
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Receive string data
///////////////////////////////////////////////////////////////////////////////////////////////////
void SC_ReceiveStringData(BYTE *wData)
{
	DWORD dwError;									//  Error information
	COMSTAT ComStat;								//  State of device
	DWORD dwCount;									//  Byte number of received data
	DWORD dwRead;

	ClearCommError(hComm, &dwError, &ComStat);
	dwCount = ComStat.cbInQue;

	//  Write file function
	ReadFile(hComm, wData, dwCount, &dwRead, NULL);
}

//	void SC_ReceiveStringData(UINT, BYTE *)
///////////////////////////////////////////////////////////////////////////////////////////////////