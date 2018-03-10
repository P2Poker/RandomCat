
// Traditional header macro guard
#ifndef RANDOMCAT_RANDOMCAT_H
#define RANDOMCAT_RANDOMCAT_H







// If compiling for ARM, ensure RANDOMCAT_ARM is defined
#if ( defined arm || defined _arm || defined Arm || defined _Arm || defined ARM || defined _ARM ) && !defined RANDOMCAT_ARM
	#define RANDOMCAT_ARM
#endif



// If compiling for X86, ensure RANDOMCAT_X86 is defined
#if ( defined x86 || defined _x86 || defined X86 || defined _X86 || defined WIN32 ) && !defined RANDOMCAT_X86
	#define RANDOMCAT_X86
#endif



// If compiling for X64, ensure RANDOMCAT_X64 is defined
#if ( defined x64 || defined _x64 || defined X64 || defined _X64 || !defined RANDOMCAT_X86 ) && !defined RANDOMCAT_X64
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







/// \brief Centralized, global, crypto-secure, multi-threaded, high-performance, true random number generator
/// \details Uses AES to perform randomization. Collects entropy from a variety of sources. Uses SHA to fold in collected entropy bytes.
class RandomCat
{
public :

							using											thisT									= RandomCat														;



							using											byte									= CryptoPP :: byte											;

	static				byte constexpr								BLANK_BYTE							= 0xCC															;



							using											blockCipherT						= CryptoPP :: AES :: Encryption							;

							using											block									= std :: array <	byte									,
																																			blockCipherT :: MAX_BLOCKSIZE	>	;



							using											poolContainer						= CryptoPP :: SecBlock <	block														,
																																						CryptoPP :: AllocatorWithCleanup <	block	,
																																																		true	>	> ;

							using											size_type							= poolContainer :: size_type								;

	static				size_type constexpr						BLOCK_BYTE_COUNT					= blockCipherT :: MAX_BLOCKSIZE							;

	static				size_type constexpr						KEY_BYTE_COUNT						= blockCipherT :: MAX_KEYLENGTH							;

	static				size_type constexpr						SEED_BYTE_COUNT					= BLOCK_BYTE_COUNT + KEY_BYTE_COUNT						;



							using											key									= CryptoPP :: FixedSizeAlignedSecBlock <	byte				,
																																											KEY_BYTE_COUNT	> ;

							using											seed									= CryptoPP :: FixedSizeAlignedSecBlock <	byte					,
																																											SEED_BYTE_COUNT	> ;



							using											copyBlock							= std :: conditional_t <	BLOCK_BYTE_COUNT % sizeof ( size_type )							,
																																						block																			,
																																						std :: array <	size_type											,
																																											BLOCK_BYTE_COUNT / sizeof ( size_type )	>	> ;

							using											copyKey								= std :: conditional_t <	KEY_BYTE_COUNT % sizeof ( size_type )							,
																																						std :: array <	byte				,
																																											KEY_BYTE_COUNT	>									,
																																						std :: array <	size_type										,
																																											KEY_BYTE_COUNT / sizeof ( size_type )	>	> ;

							using											copySeed								= std :: conditional_t <	SEED_BYTE_COUNT % sizeof ( size_type )							,
																																						std :: array <	byte					,
																																											SEED_BYTE_COUNT	>								,
																																						std :: array <	size_type										,
																																											SEED_BYTE_COUNT / sizeof ( size_type )	>	> ;



	static				size_type constexpr						MIN_BLOCK_COUNT					= 3																;

	static				size_type constexpr						MAX_BLOCK_COUNT					= 65536 / BLOCK_BYTE_COUNT									;



							using											hashGeneratorT						= std :: conditional_t <	SEED_BYTE_COUNT <= ( 224 / CHAR_BIT ) + ( ( 224 % CHAR_BIT ) ? 1 : 0 )																			,
																																						CryptoPP :: SHA224																																				,
																																						std :: conditional_t <	SEED_BYTE_COUNT <= ( 256 / CHAR_BIT ) + ( ( 256 % CHAR_BIT ) ? 1 : 0 )										,
																																														CryptoPP :: SHA256																											,
																																														std :: conditional_t <	SEED_BYTE_COUNT <= ( 384 / CHAR_BIT ) + ( ( 384 % CHAR_BIT ) ? 1 : 0 )	,
																																																						CryptoPP :: SHA384																		,
																																																						CryptoPP :: SHA512																		>	>	> ;

	static				size_type constexpr						HASH_DIGEST_BYTE_COUNT			= hashGeneratorT :: DIGESTSIZE							;

							using											hash									= CryptoPP :: FixedSizeAlignedSecBlock <	byte							,
																																											HASH_DIGEST_BYTE_COUNT	> ;



							using											intRand								= int64_t														;

							using											uintRand								= uint64_t														;



	template < typename _T = uintRand >
	static				bool constexpr								IS_FLOAT								= std :: is_floating_point_v < _T >						;

							template < typename _T = uintRand >
							using											DIFF_T								= std :: conditional_t <	IS_FLOAT < _T > == false																																	,
																																						std :: conditional_t <	std :: is_signed_v < _T >		,
																																														std :: make_unsigned_t < _T >	,
																																														_T										>																							,
																																						std :: conditional_t <	sizeof ( _T ) == sizeof ( uint32_t )																				,
																																														uint32_t																														,
																																														std :: conditional_t <	sizeof ( _T ) == sizeof ( uint64_t )											,
																																																						uint64_t																					,
																																																						std :: conditional_t <	sizeof ( _T ) == sizeof ( long double )	,
																																																														_T														,
																																																														_T														>	>	>	> ;



private :

	static				blockCipherT								blockCipher																								;



	static				poolContainer								pool																										;

	static				byte const *								pCurrByte																								;

	static				byte const *								pEndByte																									;

	static				block *										pCurrCipherBlock																						;

	static				block const *								pEndCipherBlock																						;



	static				std :: mutex								initMutex																								;

	static				std :: mutex								poolMutex																								;

	static				std :: mutex								cipherMutex																								;

	static				std :: recursive_mutex					threadMutex																								;

	static				std :: condition_variable				generateBlocksCV																						;



	// Block generation background thread
	static				std :: unique_ptr < std :: thread >	pBlockGenThread																						;



	static				bool											isBlockGenThreadFinished																			;

	static				bool											isInitialized																							;



public :

																			RandomCat							(																	) = delete ;

																			RandomCat							(	thisT const &			src								) = delete ;

																			RandomCat							(	thisT &&					src								) = delete ;

							thisT &										operator =							(	thisT const &			src								) = delete ;

							thisT &										operator =							(	thisT &&					src								) = delete ;

//	virtual	inline												  ~RandomPoolBase						(																	) ;



private :

	static				bool											Initialize							(	size_type				poolBlockCount					) ;

	static	inline	bool											IsBlockGenThreadRunning			(																	) ;

	static	inline	bool											IsBlockGenThreadReadyToJoin	(																	) ;

	static	inline	void											CreateBlockGenThread				(																	) ;

	static	inline	void											TerminateBlockGenThread			(																	) ;

	static				void											GenerateBlocks						(																	) ;



	static	inline	size_type									UnusedByteCount					(																	) ;

	static	inline	size_type									UnusedBlockCount					(																	) ;

	static	inline	size_type									UsedByteCount						(																	) ;

	static	inline	size_type									UsedBlockCount						(																	) ;



	static	inline	block &										SeedBlock							(																	) ;

	static	inline	copyBlock &									SeedCopyBlock						(																	) ;

	static	inline	void											NextSeedBlock						(																	) ;



public :

	template < typename _T = uintRand >
	static	inline	_T												Next									(	bool						allowInfinity		= false	,
																														bool						allowNaN				= false	) ;

	template < typename _T = uintRand >
	static	inline	_T												Next									(	_T							minValue							,
																														_T							maxValue							,
																														bool						allowInfinity		= false	,
																														bool						allowNaN				= false	) ;



	static				void											IncorporateEntropy				(	byte const *			input								,
																														size_type				length							) ;

	template < typename _T >
	static	inline	void											IncorporateEntropy				(	_T const &				input								) ;



private :

	template < typename _T = uintRand >
	static	inline	DIFF_T < _T > constexpr					CALC_RANGE							(	_T							minValue							,
																														_T							maxValue							,
																														bool						includeUpperBound	= true	) ;

	template < typename _T = uintRand >
	static				size_t const								MAX_RANGE							= ( sizeof ( _T ) < sizeof ( size_t ) ) ?
																															static_cast < size_t > ( pow ( static_cast < size_t > ( 2 ) , static_cast < size_t > ( sizeof ( _T ) * CHAR_BIT ) ) ) :
																															static_cast < size_t > ( 0 )						;

	template < typename _DIFF_T = DIFF_T < uintRand > >
	static	inline	_DIFF_T constexpr							MAX_RANDOM_VALUE					(	_DIFF_T					range								) ;



	template < typename RANGE_T >
	static	inline	void											CheckRangeUniqueCount			(	bool						uniqueValues					,
																														size_t					nCount							,
																														RANGE_T					range								) ;

	static	inline	void											CheckArrayLength					(	size_t					nCount							,
																														size_t					arrayLength						) ;
} ;







/// \brief Constructable wrapper class that provides non-static methods replicating all RandomCat functionality
/// \details This class does not define or implement any behavior, but simply provides non-static methods that invoke their respective static counterparts in the RandomCat class.
class RandomCatObject
{
public :

							using											thisT									= RandomCatObject												;

				template < typename Sseq >
				inline	void											seed									(	Sseq &					q									) const ;

				inline	void											discard								(	unsigned long long	z									) ;
} ;







/// \brief Thread-safe, high-performance, manually seedable/re-seedable, highly unpredictable pseudo-random number generator
/// \details Based off of RandomCat's design, and implementing RandomCatObject's interface.
///  Uses AES to perform randomization. Uses SHA to convert generic seed bytes into an AES key and cipher block bytes.
///  Useful in situations that require repeatable random sequences by reusing a given seed value. Should not be used in security-sensitive contexts; use RandomCat or RandomCatObject instead.
class PseudoRandomCat
{
public :

							using											thisT									= PseudoRandomCat												;

				template < typename Sseq >
				inline	void											seed									(	Sseq &					q									) const ;

				inline	void											discard								(	unsigned long long	z									) ;
} ;







#endif  // #ifndef RANDOMCAT_RANDOMCAT_H
// End of traditional header macro guard
