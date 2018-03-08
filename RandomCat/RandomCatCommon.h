
/****************************************************************
   Header for RandomCat non-feature-specific global declarations,
   and third-party library includes
*****************************************************************/



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
	// #define WIN32_LEAN_AND_MEAN

	// Windows Header Files:
	#include <windows.h>

#endif  // #ifdef RANDOMCAT_WINDOWS
// No longer Windows-specific code







// If compiling for FIPS-compliant applications, , ensure RANDOMCAT_FIPS is defined, and enable Crypto++ compliance with FIPS 140-2
#ifdef CRYPTOPP_ENABLE_COMPLIANCE_WITH_FIPS_140_2
	#if ( CRYPTOPP_ENABLE_COMPLIANCE_WITH_FIPS_140_2 || defined fips || defined _fips || defined Fips || defined _Fips || defined FIPS || defined _FIPS || defined RANDOMCAT_FIPS )
		#ifndef RANDOMCAT_FIPS
			#define RANDOMCAT_FIPS
		#endif

		// Enable Crypto++ compliance with FIPS 140-2
		#if !CRYPTOPP_ENABLE_COMPLIANCE_WITH_FIPS_140_2
			#undef CRYPTOPP_ENABLE_COMPLIANCE_WITH_FIPS_140_2
			#define CRYPTOPP_ENABLE_COMPLIANCE_WITH_FIPS_140_2 1
		#endif
	#endif
#elif ( defined fips || defined _fips || defined Fips || defined _Fips || defined FIPS || defined _FIPS || defined RANDOMCAT_FIPS )  // !defined CRYPTOPP_ENABLE_COMPLIANCE_WITH_FIPS_140_2
	#ifndef RANDOMCAT_FIPS
		#define RANDOMCAT_FIPS
	#endif

	// Enable Crypto++ compliance with FIPS 140-2
	#define CRYPTOPP_ENABLE_COMPLIANCE_WITH_FIPS_140_2 1
#endif



// AES standard block cipher
#include <aes.h>  // Universal interface provided by the Crypto++ library

// OS-implemented cryptographic quality pseudo-RNG
#include <osrng.h>  // Universal interface provided by the Crypto++ library

// High-resolution timer
#include <hrtimer.h>  // Universal interface provided by the Crypto++ library
//#include <chrono>







// Exception handling
#include <stdexcept>
#include <sstream>  // Used for building exception messages

// Thread safety
#include <mutex>
#include <condition_variable>

// Smart pointers
#include <memory>

// Numbers
#include <cstdint>
#include <type_traits>
#include <limits>
#include <cmath>
//#include <random>

// Collections
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>







#endif  // #ifndef RANDOMCAT_RANDOMCATCOMMON_H
