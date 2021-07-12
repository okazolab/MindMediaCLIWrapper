#pragma once

//#include "afxwin.h"
#include "GenericDeviceInterface.h"

// Global constants:
#define NX_CHANNELS			38
#define TIMER_ID			12345

// Type definitions:
typedef DWORD				(*DLL_TEST)	(void);
typedef void				(*DLL_ShowData)	(int nSamples, int nChan, float* fData);
typedef int					(*DLL_INIT)	(DLL_ShowData fFunc, int nSearchMode, long long lDeviceSerialNumber);
//typedef int					(__stdcall* DLL_INIT)	(DLL_ShowData fFunc, int nSearchMode, long long lDeviceSerialNumber);
//typedef int					(__cdecl* DLL_INIT)	(DLL_ShowData fFunc, int nSearchMode, long long lDeviceSerialNumber);
// 
// 
//typedef int					(*DLL_INIT)	(System::IntPtr fFunc, int nSearchMode, long long lDeviceSerialNumber);

typedef int					(*DLL_START) (DWORD* dwSamplerate);
typedef int					(*DLL_STOP)	(void);
typedef DWORD				(*DLL_GETINTERFACE)	(void);
typedef bool				(*DLL_GETDEVICEINFO) (DEVICE_INFO* devInf);
typedef bool				(*DLL_GETCHANNELINFO) (int nIndex, CHANNEL_INFO* chanInf);
typedef void				(*DLL_SHOWAUTHENTICATION) (void);
