#include "RandomCat.h"



// Windows-specific code
#ifdef RANDOMCAT_WINDOWS



// DLL-specific code
#ifdef RANDOMCAT_EXPORTS



BOOL APIENTRY DllMain (	HMODULE	hModule					,
								DWORD		ul_reason_for_call	,
								LPVOID	lpReserved				)
{
	switch ( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH :
	case DLL_THREAD_ATTACH :
	case DLL_THREAD_DETACH :
	case DLL_PROCESS_DETACH :
		break ;
	}

	return TRUE ;
}



#endif  // #ifdef RANDOMCAT_EXPORTS
// No longer DLL-specific code



#endif  // #ifdef RANDOMCAT_WINDOWS
// No longer Windows-specific code
