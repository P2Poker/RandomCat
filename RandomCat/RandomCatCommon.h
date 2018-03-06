#ifndef RANDOMCAT_RANDOMCATCOMMON_H
#define RANDOMCAT_RANDOMCATCOMMON_H



// If compiling for ARM, ensure RANDOMCAT_ARM is defined
#if ( defined arm || defined Arm || defined ARM || defined _arm || defined _Arm || defined _ARM ) && !defined RANDOMCAT_ARM
	#define RANDOMCAT_ARM
#endif



// If compiling for X86, ensure RANDOMCAT_X86 is defined
#if ( defined x86 || defined X86 || defined WIN32 ) && !defined RANDOMCAT_X86
	#define RANDOMCAT_X86
#endif



// If compiling for X64, ensure RANDOMCAT_X64 is defined
#if ( defined x64 || defined X64 ) && !defined RANDOMCAT_X64
	#define RANDOMCAT_X64
#endif







// If compiling for Windows, ensure RANDOMCAT_WINDOWS is defined
#if ( defined windows || defined _windows || defined Windows || defined _Windows || defined WINDOWS || defined _WINDOWS || \
		defined win32 || defined _win32 || defined Win32 || defined _Win32 || defined WIN32 || defined _WIN32 ) && !defined RANDOMCAT_WINDOWS
	#define RANDOMCAT_WINDOWS
#endif



// If compiling for OSX, ensure RANDOMCAT_OSX is defined
#if ( defined mac || defined _mac || defined Mac || defined _Mac || defined MAC || defined _MAC || \
		defined OSX || defined _OSX || defined macOSX || defined _macOSX || defined MacOSX || defined _MacOSX || defined MACOSX || defined _MACOSX ) && !defined RANDOMCAT_OSX
	#define RANDOMCAT_OSX
#endif



// If compiling for IOS, ensure RANDOMCAT_IOS is defined
#if ( defined ios || defined _ios || defined Ios || defined _Ios || defined IOS || defined _IOS ) && !defined RANDOMCAT_IOS
	#define RANDOMCAT_IOS
#endif



// If compiling for Android, ensure RANDOMCAT_ANDROID is defined
#if ( defined android || defined _android || defined Android || defined _Android || defined ANDROID || defined _ANDROID ) && !defined RANDOMCAT_ANDROID
	#define RANDOMCAT_ANDROID
#endif



// If compiling for Linux, ensure RANDOMCAT_LINUX is defined
#if ( defined linux || defined _linux || defined Linux || defined _Linux || defined LINUX || defined _LINUX || \
		defined unix || defined _unix || defined Unix || defined _Unix || defined UNIX || defined _UNIX || \
		defined RANDOMCAT_OSX || defined RANDOMCAT_IOS || defined RANDOMCAT_ANDROID  || !defined RANDOMCAT_WINDOWS ) && !defined RANDOMCAT_LINUX
	#define RANDOMCAT_LINUX
#endif







// Windows-specific code
#ifdef RANDOMCAT_WINDOWS



// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>



// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

// Windows Header Files:
#include <windows.h>



#endif  // #ifdef RANDOMCAT_WINDOWS
// No longer Windows-specific code



#endif  // #ifndef RANDOMCAT_RANDOMCATCOMMON_H
