// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef RANDOMCAT_RANDOMCATCOMMON_H
#define RANDOMCAT_RANDOMCATCOMMON_H



// If compiling for Windows, ensure RANDOMCAT_WINDOWS is defined
#if ( defined _WINDOWS || defined WIN32 ) && !defined RANDOMCAT_WINDOWS
	#define RANDOMCAT_WINDOWS
#endif  // #if ( defined _WINDOWS || defined WIN32 ) && !defined RANDOMCAT_WINDOWS



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
