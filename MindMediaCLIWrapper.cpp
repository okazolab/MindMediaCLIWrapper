// <copyright company="Okazolab Ltd">
// Copyright (c) 2021 All Rights Reserved
// </copyright>
// <author> OkazoLab team</author>
// <summary>Class representing a Sample entity</summary>

#include <msclr/marshal_cppstd.h>
#include <iostream>
#include <msclr\marshal_cppstd.h>
#include "GenericDeviceInterface.h"
#include "MindMediaCLIWrapper.h"
#include <string.h>
#include "atlstr.h"

using namespace System;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;
using namespace System::Diagnostics;


public ref class MindMediaEngine
{
protected:
	// Protected class members
	DLL_TEST				TestDLL;
	DLL_INIT				InitGenericDevice;
	DLL_START				StartGenericDevice;
	DLL_STOP				StopGenericDevice;
	DLL_GETINTERFACE		GetGenericDeviceInterface;
	DLL_GETCHANNELINFO		GetGenericDeviceChannelInfo;
	DLL_GETDEVICEINFO		GetGenericDeviceInfo;
	DLL_SHOWAUTHENTICATION	ShowGenericDeviceAuthentication;
	HICON					m_hIcon;
	HINSTANCE				m_hDLL;						// The DLL handle obtained through LoadLibrary()
	LPCWSTR					m_csPathName;				// The path of the DLL(GenericDeviceDLL.dll)
	bool					m_bSystemOK;
	
	public:

	
	property bool IsConnected;

	MindMediaEngine::MindMediaEngine(System::String^ LibraryLocation)
	{
		/// Load the function available in the GenericDeviceInterfaceDLL
		std::string standardString = marshal_as<std::string>(LibraryLocation);
		LoadDllFunctions(standardString);
		IsConnected = false;
	}

	// Load the Generic DLL supplied by MindMedia BV
	void MindMediaEngine::LoadDllFunctions(std::string FullPathLibraryFile)
	{		
///#ifdef x64_SYSTEM	// DLL for 64-Bit platform
///		m_csPathName = L"GenericDeviceInterfaceDLL_x64.dll"; //The DLL has to be in te same PATH as the DEMO-app
///#else					// DLL for 32-Bit platform
///		m_csPathName = L"GenericDeviceInterfaceDLL.dll"; //The DLL has to be in te same PATH as the DEMO-app
///#endif	

		std::wstring stemp = s2ws(FullPathLibraryFile);
		LPCWSTR result = stemp.c_str();

		m_hDLL = LoadLibrary(result);
		if (m_hDLL != NULL)
		{	// Handle successfully created, now create the function calls
			InitGenericDevice = (DLL_INIT)GetProcAddress(m_hDLL, "InitGenericDevice");

			StartGenericDevice = (DLL_START)GetProcAddress(m_hDLL, "StartGenericDevice");
			StopGenericDevice = (DLL_STOP)GetProcAddress(m_hDLL, "StopGenericDevice");
			GetGenericDeviceInterface = (DLL_GETINTERFACE)GetProcAddress(m_hDLL, "GetInterfaceVersion");
			GetGenericDeviceChannelInfo = (DLL_GETCHANNELINFO)GetProcAddress(m_hDLL, "GetChannelInfo");
			GetGenericDeviceInfo = (DLL_GETDEVICEINFO)GetProcAddress(m_hDLL, "GetDeviceInfo");
			ShowGenericDeviceAuthentication = (DLL_SHOWAUTHENTICATION)GetProcAddress(m_hDLL, "ShowAuthenticationWindow");
		}
	}

	~MindMediaEngine()
	{
		//delete EFactory;
	}

	std::wstring s2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
	
	
	System::String^ MindMediaEngine::Connect(IntPtr ProcessDataCallback, int SearchMode, long long DeviceSerialNumber)
	{
		IsConnected=false;
		
		DLL_ShowData CallbackFuntionNativePointer = static_cast<DLL_ShowData>(ProcessDataCallback.ToPointer());

		int nRet = InitGenericDevice(CallbackFuntionNativePointer, SearchMode, DeviceSerialNumber);

		switch (nRet)
		{	// Print returncode to GUI
		case IS_OK:
			IsConnected=true;
			return "OK";
			break;

		case IS_NO_VALID_SERIAL: // Device not authenticated
			// Show authentication dialog
			ShowGenericDeviceAuthentication();
			return "System not authenticated";
			break;

		default:
			// Something went wrong, check te return code for more details or check the generated log-file
			return "Cannot connect to device (error code " + nRet + ")!";
			break;
		};
		//return "";
	}

	void ShowAuthenticationDialog()
	{
		ShowGenericDeviceAuthentication();
	}

	void MindMediaEngine::getDeviceInfo([Out] int% ChannelNumber,[Out] String^% DeviceName,array<String^>^% ChannelNames)
	{
		//return 16;
		DEVICE_INFO	diDeviceInfo;
		CHANNEL_INFO	ciTemp;
		if (GetGenericDeviceInfo(&diDeviceInfo))
			{ 
			ChannelNumber= diDeviceInfo.dwNumberOfChannels;					
			DeviceName = gcnew String(diDeviceInfo.cName);
			ChannelNames = gcnew array<String^>(ChannelNumber);
			for (int i = 0; i < ChannelNumber; i++)
				{
				GetGenericDeviceChannelInfo(i, &ciTemp);
				ChannelNames[i]= gcnew String(ciTemp.cName);
				}
			}
	}

	System::String^ MindMediaEngine::Start(UInt32 SamplingRate)
	{
		int	nRetVal;
		DWORD lSamplingRate = SamplingRate;
		nRetVal = StartGenericDevice(&lSamplingRate);
		switch (nRetVal)
		{	// Did we have a succesfull start?
		case IS_OK: // Yes, Device started;
			// Fix samplerate input
			return "OK";
			break;

		case IS_NO_VALID_SERIAL: // Device not authenticated

			return "System not authenticated";
			break;

		default: // No; Could not start the device
			// Update dialog output string
			return "Could not start system";
			break;
		};
		return "";
	}

	System::String^ MindMediaEngine::Stop()
	{
		int nRetVal = StopGenericDevice();
		//gch.Free();
		switch (nRetVal)
		{	// Did we have a succesfull start?
		case IS_OK: // Yes, Device started;
			// Fix samplerate input
			return "OK";
			break;

		case IS_NO_VALID_SERIAL: // Device not authenticated

			return "System not authenticated";
			break;

		default: // No; Could not start the device
			// Update dialog output string
			return "Could not start system";
			break;
		};
		return "";
	}

	
};