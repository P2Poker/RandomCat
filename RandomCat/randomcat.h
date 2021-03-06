
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

// Collections
#include <string>
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







#ifdef min
	#define RANDOMCAT_TEMP_MIN min
	#undef min
#endif  // #ifdef min



#ifdef max
	#define RANDOMCAT_TEMP_MAX max
	#undef max
#endif  // #ifdef max







/// \brief Centralized, global, crypto-secure, multi-threaded, high-performance, true random number generator
/// \details Uses AES to perform randomization. Collects entropy from a variety of sources. Uses SHA to stir in collected entropy bytes.
class RandomCat
{
public :

							using											thisT									= RandomCat																;



							using											byte									= CryptoPP :: byte													;

	static				byte constexpr								BLANK_BYTE							= 0xCC																	;



							using											blockCipherT						= CryptoPP :: AES :: Encryption									;

							using											block									= std :: array <	byte									,
																																			blockCipherT :: MAX_BLOCKSIZE	>			;



							using											poolContainer						= CryptoPP :: SecBlock <	block														,
																																						CryptoPP :: AllocatorWithCleanup <	block	,
																																																		true	>	> ;

							using											size_type							= poolContainer :: size_type										;

	static				size_type constexpr						BLOCK_BYTE_COUNT					= blockCipherT :: MAX_BLOCKSIZE									;

	static				size_type constexpr						KEY_BYTE_COUNT						= blockCipherT :: MAX_KEYLENGTH									;

	static				size_type constexpr						SEED_BYTE_COUNT					= BLOCK_BYTE_COUNT + KEY_BYTE_COUNT								;



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



	static				size_type constexpr						MIN_BLOCK_COUNT					= 3																		;

	static				size_type constexpr						MAX_BLOCK_COUNT					= 65536 / BLOCK_BYTE_COUNT											;



							using											hashGeneratorT						= std :: conditional_t <	SEED_BYTE_COUNT * CHAR_BIT <= 224																			,
																																						CryptoPP :: SHA224																								,
																																						std :: conditional_t <	SEED_BYTE_COUNT * CHAR_BIT <= 256										,
																																														CryptoPP :: SHA256															,
																																														std :: conditional_t <	SEED_BYTE_COUNT * CHAR_BIT <= 384	,
																																																						CryptoPP :: SHA384						,
																																																						CryptoPP :: SHA512						>	>	> ;

	static				size_type constexpr						HASH_DIGEST_BYTE_COUNT			= hashGeneratorT :: DIGESTSIZE									;

							using											hash									= CryptoPP :: FixedSizeAlignedSecBlock <	byte							,
																																											HASH_DIGEST_BYTE_COUNT	> ;



							using											intRand								= int64_t																;

							using											uintRand								= uint64_t																;



	template < typename _T >
	static				bool constexpr								IS_FLOAT								= std :: is_floating_point_v < _T >								;

							template < typename _T >
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

	static				blockCipherT								blockCipher																										;



	static				poolContainer								pool																												;

	static				byte const *								pCurrByte																										;

	static				byte const *								pEndByte																											;

	static				block *										pCurrCipherBlock																								;

	static				block const *								pEndCipherBlock																								;



	static				std :: mutex								initMutex																										;

	static				std :: mutex								poolMutex																										;

	static				std :: mutex								cipherMutex																										;

	static				std :: recursive_mutex					threadMutex																										;

	static				std :: condition_variable				generateBlocksCV																								;



	// Block generation background thread
	static				std :: unique_ptr < std :: thread >	pBlockGenThread																								;



	static				bool											isBlockGenThreadFinished																					;

	static				bool											isInitialized																									;



public :

																			RandomCat							(																			) = delete ;

																			RandomCat							(	thisT const &			src										) = delete ;

																			RandomCat							(	thisT &&					src										) = delete ;

							thisT &										operator =							(	thisT const &			src										) = delete ;

							thisT &										operator =							(	thisT &&					src										) = delete ;

//	virtual	inline												  ~RandomPoolBase						(																			) ;



private :

	static				bool											Initialize							(	size_type				poolBlockCount							) ;

	static	inline	bool											IsBlockGenThreadRunning			(																			) ;

	static	inline	bool											IsBlockGenThreadReadyToJoin	(																			) ;

	static	inline	void											CreateBlockGenThread				(																			) ;

	static	inline	void											TerminateBlockGenThread			(																			) ;

	static				void											GenerateBlocks						(																			) ;



	static	inline	size_type									UnusedByteCount					(																			) ;

	static	inline	size_type									UnusedBlockCount					(																			) ;

	static	inline	size_type									UsedByteCount						(																			) ;

	static	inline	size_type									UsedBlockCount						(																			) ;



	static	inline	block &										SeedBlock							(																			) ;

	static	inline	copyBlock &									SeedCopyBlock						(																			) ;

	static	inline	void											NextSeedBlock						(																			) ;



public :

	template < typename _T = intRand >
	static	inline	_T												Next									(	bool						allowInfinity		= false			,
																														bool						allowNaN				= false			) ;

	template < typename _T = intRand >
	static	inline	_T												Next									(	_T							minValue									,
																														_T							maxValue									,
																														bool						allowInfinity		= false			,
																														bool						allowNaN				= false			) ;



	static				void											IncorporateEntropy				(	byte const *			input										,
																														size_type				length									) ;

	template < typename _T >
	static	inline	void											IncorporateEntropy				(	_T const &				input										) ;



	template < typename _T = intRand >
	static	inline	DIFF_T < _T > constexpr					CALC_RANGE							(	_T							minValue									,
																														_T							maxValue									,
																														bool						includeUpperBound	= true			) ;

	template < typename _T >
	static				uint64_t constexpr						MAX_RANGE							= ( sizeof ( _T ) < sizeof ( uint64_t ) ) ?
																															static_cast < uint64_t > ( static_cast < uint64_t > ( 1 ) << ( sizeof ( _T ) * CHAR_BIT ) ) :
																															static_cast < uint64_t > ( 0 )							;

	template < typename _DIFF_T = DIFF_T < intRand > >
	static	inline	_DIFF_T constexpr							MAX_RANDOM_VALUE					(	_DIFF_T					range										) ;



	template < typename RANGE_T >
	static	inline	void											CheckRangeUniqueCount			(	bool						uniqueValues							,
																														size_t					nCount									,
																														RANGE_T					range										) ;

	static	inline	void											CheckArrayLength					(	size_t					nCount									,
																														size_t					arrayLength								) ;







	template < typename _T = RandomCat :: intRand >
	class PseudoRandomCatSharedBase
	{
	public :

								using											result_type							= _T																		;

								using											DIFF_T								= RandomCat :: DIFF_T < result_type >							;

	} ;



	template <	typename	_T								= RandomCat :: intRand								,
					bool		RIJNDAEL_256_BIT_BLOCK	= false													,
					bool		_T_CONSTEXPR				= ( sizeof ( _T ) <= sizeof ( uint64_t ) )	>
	class PseudoRandomCatBase :
		public PseudoRandomCatSharedBase < _T >
	{
	public :
								using											blockCipherT						= CryptoPP :: AES :: Encryption									;

		static	inline	result_type constexpr					min									(																			) ;

		static	inline	result_type constexpr					max									(																			) ;

	} ;  /* class PseudoRandomCatBase <	_T		,
													false	,
													true	> */



	template < typename _T >
	class PseudoRandomCatBase <	_T		,
											false	,
											false	> :
		public PseudoRandomCatSharedBase < _T >
	{
	public :

								using											blockCipherT						= CryptoPP :: AES :: Encryption									;

		static	inline	result_type const &						min									(																			) ;

		static	inline	result_type const &						max									(																			) ;

	} ;  /* class PseudoRandomCatBase <	_T		,
													false	,
													false	> */



	template < typename _T >
	class PseudoRandomCatBase <	_T		,
											true	,
											true	> :
		public PseudoRandomCatSharedBase < _T >
	{
	public :

								using											blockCipherT						= void																	;

		static	inline	result_type constexpr					min									(																			) ;

		static	inline	result_type constexpr					max									(																			) ;

	} ;  /* class PseudoRandomCatBase <	_T		,
													true	,
													true	> */



	template < typename _T >
	class PseudoRandomCatBase <	_T		,
											true	,
											false	> :
		public PseudoRandomCatSharedBase < _T >
	{
	public :

								using											blockCipherT						= void																	;

		static	inline	result_type const &						min									(																			) ;

		static	inline	result_type const &						max									(																			) ;

	} ;  /* class PseudoRandomCatBase <	_T		,
													true	,
													false	> */



} ;  // class RandomCat







/// \brief Constructable wrapper class that provides non-static methods replicating all RandomCat functionality
/// \details This class does not define or implement any behavior, but simply provides non-static methods that invoke their respective static counterparts in the RandomCat class.
template < typename _T = RandomCat :: intRand >
class RandomCatObject
{
public :

							using											thisT									= RandomCatObject														;

							using											result_type							= _T																		;



	static	inline	result_type									min									(																			) ;

	static	inline	result_type									max									(																			) ;



				inline	void											seed									(																			) ;

				inline	void											seed									(	result_type				value										) ;

				template < typename Sseq >
				inline	void											seed									(	Sseq &					q											) ;

				inline	void											discard								(	unsigned long long	z											) ;



				inline	result_type									operator ( )						(																			) ;
} ;







/// \brief Thread-safe, high-performance, manually seedable/re-seedable, highly unpredictable pseudo-random number generator
/// \details Based off of RandomCat's design, and implementing RandomCatObject's interface.
///  Uses AES to perform randomization. Uses SHA to convert generic seed bytes into an AES key and cipher block bytes.
///  Useful in situations that require repeatable random sequences by reusing a given seed value.
///  PseudoRandomCat should not be used in security-sensitive contexts; use RandomCat or RandomCatObject instead.
template <	typename	_T								= RandomCat :: intRand	,
				bool		RIJNDAEL_256_BIT_BLOCK	= false						>
class PseudoRandomCat :
	public RandomCat :: PseudoRandomCatBase <	_T								,
															RIJNDAEL_256_BIT_BLOCK	>
{
public :

							using											thisT									= PseudoRandomCat														;

							using											baseT									= RandomCat :: PseudoRandomCatBase <	_T								,
																																										RIJNDAEL_256_BIT_BLOCK	> ;



							using											byte									= CryptoPP :: byte													;

							using											size_type							= RandomCat :: size_type											;



	static				size_type constexpr						KEY_BYTE_COUNT						= RandomCat :: KEY_BYTE_COUNT										;

	static				size_type constexpr						BLOCK_BYTE_COUNT					= RandomCat :: BLOCK_BYTE_COUNT									;

	static				size_type constexpr						SEED_BYTE_COUNT					= KEY_BYTE_COUNT + BLOCK_BYTE_COUNT								;

							using											poolContainer						= CryptoPP :: FixedSizeAlignedSecBlock <	byte					,
																																											BLOCK_BYTE_COUNT	> ;

							using											seedT									= CryptoPP :: FixedSizeAlignedSecBlock <	byte					,
																																											SEED_BYTE_COUNT	> ;

							using											copyPoolContainer					= RandomCat :: copyBlock											;

							using											copySeed								= std :: conditional_t <	SEED_BYTE_COUNT % sizeof ( size_type )							,
																																						std :: array <	byte					,
																																											SEED_BYTE_COUNT	>								,
																																						std :: array <	size_type										,
																																											SEED_BYTE_COUNT / sizeof ( size_type )	>	> ;



							using											hashGeneratorT						= std :: conditional_t <	SEED_BYTE_COUNT * CHAR_BIT <= 224																			,
																																						CryptoPP :: SHA224																								,
																																						std :: conditional_t <	SEED_BYTE_COUNT * CHAR_BIT <= 256										,
																																														CryptoPP :: SHA256															,
																																														std :: conditional_t <	SEED_BYTE_COUNT * CHAR_BIT <= 384	,
																																																						CryptoPP :: SHA384						,
																																																						CryptoPP :: SHA512						>	>	> ;

	static				size_type constexpr						HASH_DIGEST_BYTE_COUNT			= hashGeneratorT :: DIGESTSIZE									;

							using											hash									= CryptoPP :: FixedSizeAlignedSecBlock <	byte							,
																																											HASH_DIGEST_BYTE_COUNT	> ;



private :

							poolContainer								pool																												;

							blockCipherT								blockCipher																										;

							result_type const							minResult																										;
							result_type const							maxResult																										;

							DIFF_T const								maxRandomValue																									;

							bool const									fullRange																										;

							byte											nCurrByte																										;



public :

				inline													PseudoRandomCat					(																			) ;

	explicit	inline													PseudoRandomCat					(	uint64_t					seedValue								) ;

	explicit	inline													PseudoRandomCat					(	int64_t					seedValue								) ;

	explicit	inline													PseudoRandomCat					(	double					seedValue								) ;

//	explicit	inline													PseudoRandomCat					(	long double				seedValue								) ;

				inline													PseudoRandomCat					(	byte const *			seedInput								,
																														size_type				length									) ;

				template < typename Sseq >
	explicit	inline													PseudoRandomCat					(	Sseq &					q											) ;

				inline													PseudoRandomCat					(	result_type				_minResult								,
																														result_type				_maxResult								) ;

				inline													PseudoRandomCat					(	result_type				_minResult								,
																														result_type				_maxResult								,
																														uint64_t					seedValue								) ;

				inline													PseudoRandomCat					(	result_type				_minResult								,
																														result_type				_maxResult								,
																														int64_t					seedValue								) ;

				inline													PseudoRandomCat					(	result_type				_minResult								,
																														result_type				_maxResult								,
																														double					seedValue								) ;

				//inline													PseudoRandomCat					(	result_type				_minResult								,
				//																										result_type				_maxResult								,
				//																										long double				seedValue								) ;

				inline													PseudoRandomCat					(	result_type				_minResult								,
																														result_type				_maxResult								,
																														byte const *			seedInput								,
																														size_type				length									) ;

				template < typename Sseq >
				inline													PseudoRandomCat					(	result_type				_minResult								,
																														result_type				_maxResult								,
																														Sseq &					q											) ;



				inline	result_type const &						MinResult							(																			) const ;

				inline	result_type const &						MaxResult							(																			) const ;



				inline	void											seed									(																			) ;

				inline	void											seed									(	uint64_t					value										) ;

				inline	void											seed									(	int64_t					value										) ;

				inline	void											seed									(	double					value										) ;

				//inline	void											seed									(	long double				value										) ;

				inline	void											seed									(	byte const *			input										,
																														size_type				length									) ;

							template < typename Sseq >
							void											seed									(	Sseq &					q											) ;

				inline	void											discard								(	unsigned long long	z											) ;



				inline	result_type									Next									(																			) ;

				inline	result_type &								Next									(	result_type &			dest										) ;

				inline	result_type									Next									(	result_type				_minResult								,
																														result_type				_maxResult								) ;

				inline	result_type &								Next									(	result_type &			dest										,
																														result_type				_minResult								,
																														result_type				_maxResult								) ;

				inline	result_type									operator ( )						(																			) ;

				inline	result_type &								operator ( )						(	result_type &			dest										) ;

				inline	result_type									operator ( )						(	result_type				_minResult								,
																														result_type				_maxResult								) ;

				inline	result_type &								operator ( )						(	result_type &			dest										,
																														result_type				_minResult								,
																														result_type				_maxResult								) ;



private :

				inline	byte											BytesAvailable						(																			) const ;

				inline	byte											BytesUsed							(																			) const ;
} ;







template < typename _T >
inline PseudoRandomCat < _T > :: PseudoRandomCat ( ) :
	minResult		( min ( )	) ,
	maxResult		( max ( )	) ,
	maxRandomValue	( max ( )	) ,
	fullRange		( true		)
{
	seed ( ) ;
}



template < typename _T >
inline PseudoRandomCat < _T > :: PseudoRandomCat ( uint64_t const seedValue ) :
	minResult		( min ( )	) ,
	maxResult		( max ( )	) ,
	maxRandomValue	( max ( )	) ,
	fullRange		( true		)
{
	seed ( seedValue ) ;
}



template < typename _T >
inline PseudoRandomCat < _T > :: PseudoRandomCat ( int64_t const seedValue ) :
	minResult		( min ( )	) ,
	maxResult		( max ( )	) ,
	maxRandomValue	( max ( )	) ,
	fullRange		( true		)
{
	seed ( seedValue ) ;
}



template < typename _T >
inline PseudoRandomCat < _T > :: PseudoRandomCat ( double const seedValue ) :
	minResult		( min ( )	) ,
	maxResult		( max ( )	) ,
	maxRandomValue	( max ( )	) ,
	fullRange		( true		)
{
	seed ( seedValue ) ;
}



//template < typename _T >
//inline PseudoRandomCat < _T > :: PseudoRandomCat ( long double const seedValue ) :
//	minResult		( min ( )	) ,
//	maxResult		( max ( )	) ,
//	maxRandomValue	( max ( )	) ,
//	fullRange		( true		)
//{
//	seed ( seedValue ) ;
//}



template < typename _T >
inline PseudoRandomCat < _T > :: PseudoRandomCat (	byte const * const	seedInput	,
																	size_type const		length		) :
	minResult		( min ( )	) ,
	maxResult		( max ( )	) ,
	maxRandomValue	( max ( )	) ,
	fullRange		( true		)
{
	seed (	seedInput	,
				length		) ;
}



template < typename _T >
template < typename Sseq >
inline PseudoRandomCat < _T > :: PseudoRandomCat ( Sseq & q ) :
	minResult		( min ( )	) ,
	maxResult		( max ( )	) ,
	maxRandomValue	( max ( )	) ,
	fullRange		( true		)
{
	seed ( q ) ;
}



template < typename _T >
inline PseudoRandomCat < _T > :: PseudoRandomCat (	result_type	_minResult	,
																	result_type	_maxResult	) :
	minResult	( _minResult															) ,
	maxResult	( _maxResult															) ,
	fullRange	( ( _minResult == min ( ) ) && ( _maxResult == max ( ) )	)
{
	seed ( ) ;
}



template < typename _T >
inline PseudoRandomCat < _T > :: PseudoRandomCat (	result_type		_minResult	,
																	result_type		_maxResult	,
																	uint64_t const	seedValue	) :
	minResult	( _minResult															) ,
	maxResult	( _maxResult															) ,
	fullRange	( ( _minResult == min ( ) ) && ( _maxResult == max ( ) )	)
{
	seed ( seedValue ) ;
}



template < typename _T >
inline PseudoRandomCat < _T > :: PseudoRandomCat (	result_type		_minResult	,
																	result_type		_maxResult	,
																	int64_t const	seedValue	) :
	minResult	( _minResult															) ,
	maxResult	( _maxResult															) ,
	fullRange	( ( _minResult == min ( ) ) && ( _maxResult == max ( ) )	)
{
	seed ( seedValue ) ;
}



template < typename _T >
inline PseudoRandomCat < _T > :: PseudoRandomCat (	result_type		_minResult	,
																	result_type		_maxResult	,
																	double const	seedValue	) :
	minResult	( _minResult															) ,
	maxResult	( _maxResult															) ,
	fullRange	( ( _minResult == min ( ) ) && ( _maxResult == max ( ) )	)
{
	seed ( seedValue ) ;
}



//template < typename _T >
//inline PseudoRandomCat < _T > :: PseudoRandomCat (	result_type			_minResult	,
//																	result_type			_maxResult	,
//																	long double const	seedValue	) :
//	minResult	( _minResult															) ,
//	maxResult	( _maxResult															) ,
//	fullRange	( ( _minResult == min ( ) ) && ( _maxResult == max ( ) )	)
//{
//	seed ( seedValue ) ;
//}



template < typename _T >
inline PseudoRandomCat < _T > :: PseudoRandomCat (	result_type				_minResult	,
																	result_type				_maxResult	,
																	byte const * const	seedInput	,
																	size_type const		length		) :
	minResult	( _minResult															) ,
	maxResult	( _maxResult															) ,
	fullRange	( ( _minResult == min ( ) ) && ( _maxResult == max ( ) )	)
{
	seed (	seedInput	,
				length		) ;
}



template < typename _T >
template < typename Sseq >
inline PseudoRandomCat < _T > :: PseudoRandomCat (	result_type	_minResult	,
																	result_type	_maxResult	,
																	Sseq &		q				) :
	minResult	( _minResult															) ,
	maxResult	( _maxResult															) ,
	fullRange	( ( _minResult == min ( ) ) && ( _maxResult == max ( ) )	)
{
	seed ( q ) ;
}



template <	typename	_T								,
				bool		RIJNDAEL_256_BIT_BLOCK	>
inline PseudoRandomCatBase <	_T								,
										RIJNDAEL_256_BIT_BLOCK	,
										true							> :: result_type const & PseudoRandomCatBase <	_T								,
																																		RIJNDAEL_256_BIT_BLOCK	,
																																		true							> :: min ( )
{
	return std :: numeric_limits < _T > :: lowest ( ) ;
}



template <	typename	_T								,
				bool		RIJNDAEL_256_BIT_BLOCK	>
inline PseudoRandomCatBase <	_T								,
										RIJNDAEL_256_BIT_BLOCK	,
										true							> :: result_type const & PseudoRandomCatBase <	_T								,
																																		RIJNDAEL_256_BIT_BLOCK	,
																																		true							> :: max ( )
{
	return std :: numeric_limits < _T > :: max ( ) ;
}



template <	typename	_T								,
				bool		RIJNDAEL_256_BIT_BLOCK	>
inline PseudoRandomCatBase <	_T								,
										RIJNDAEL_256_BIT_BLOCK	,
										false							> :: result_type const & PseudoRandomCatBase <	_T								,
																																		RIJNDAEL_256_BIT_BLOCK	,
																																		false							> :: min ( )
{
	static result_type result ( std :: numeric_limits < _T > :: lowest ( ) ) ;

	return result ;
}



template <	typename	_T								,
				bool		RIJNDAEL_256_BIT_BLOCK	>
inline PseudoRandomCatBase <	_T								,
										RIJNDAEL_256_BIT_BLOCK	,
										false							> :: result_type const & PseudoRandomCatBase <	_T								,
																																		RIJNDAEL_256_BIT_BLOCK	,
																																		false							> :: max ( )
{
	static result_type result ( std :: numeric_limits < _T > :: max ( ) ) ;

	return result ;
}



template < typename _T >
inline PseudoRandomCat < _T > :: result_type const & PseudoRandomCat < _T > :: MinResult ( ) const
{
	return minResult ;
}



template < typename _T >
inline PseudoRandomCat < _T > :: result_type const & PseudoRandomCat < _T > :: MaxResult ( ) const
{
	return maxResult ;
}



template < typename _T >
inline void PseudoRandomCat < _T > :: seed ( )
{
	static size_type constexpr DEFAULT_SEED_BYTE_COUNT ( HASH_DIGEST_BYTE_COUNT + 8 ) ;

	CryptoPP :: FixedSizeAlignedSecBlock <	byte							,
														DEFAULT_SEED_BYTE_COUNT	> seedValue ;

	// TODO : Use RandomCat to generate random bytes for the seedValue.

	seed (	seedValue					,
				DEFAULT_SEED_BYTE_COUNT	) ;
}



template < typename _T >
inline void PseudoRandomCat < _T > :: seed ( uint64_t const value )
{
	CryptoPP :: FixedSizeAlignedSecBlock <	byte					,
														sizeof ( value )	> valueBytes ;

	*reinterpret_cast < uint64_t * > ( valueBytes.data ( ) ) = 0 ;

	uint64_t mask ( ( 1 << CHAR_BIT ) - 1 ) ;

	for ( byte * pCurrByte ( valueBytes.data ( ) + ( sizeof ( value ) - 1 ) ) ;
		pCurrByte >= valueBytes.data ( ) ; pCurrByte -- )
	{
		*pCurrByte = static_cast < byte > ( ( value & mask ) >> ( CHAR_BIT * ( ( sizeof ( value ) - 1 ) - ( pCurrByte - valueBytes.data ( ) ) ) ) ) ;
		mask <<= CHAR_BIT ;
	}

	seed (	valueBytes			,
				sizeof ( value )	) ;
}



template < typename _T >
inline void PseudoRandomCat < _T > :: seed ( int64_t const value )
{
	seed ( static_cast < uint64_t > ( value ) ) ;
}



template < typename _T >
inline void PseudoRandomCat < _T > :: seed ( double const value )
{
}



//template < typename _T >
//inline void PseudoRandomCat < _T > :: seed ( long double const value )
//{
//}



template < typename _T >
inline void PseudoRandomCat < _T > :: seed (	byte const * const	input		,
															size_type const		length	)
{
	hashGeneratorT hashGenerator ;
	hashGenerator.Update (	input		,
									length	) ;

	seedT hashResult ;

	if ( SEED_BYTE_COUNT < HASH_DIGEST_BYTE_COUNT )
		hashGenerator.TruncatedFinal (	hashResult			,
													SEED_BYTE_COUNT	) ;
	else
		hashGenerator.Final ( hashResult ) ;

	if ( SEED_BYTE_COUNT > HASH_DIGEST_BYTE_COUNT )
		memset (	hashResult.data ( ) + HASH_DIGEST_BYTE_COUNT	,
					0															,
					SEED_BYTE_COUNT - HASH_DIGEST_BYTE_COUNT		) ;

	*reinterpret_cast < copyBlock * > ( pool.data ( ) ) = *reinterpret_cast < copyBlock * > ( hashResult.data ( ) ) ;

	blockCipher.SetKey (	hashResult.data ( ) + BLOCK_BYTE_COUNT	,
								KEY_BYTE_COUNT									) ;

	if ( BLOCK_BYTE_COUNT > HASH_DIGEST_BYTE_COUNT )
		;  // TODO : Encrypt the pool to eliminate any unhashed bytes

	nCurrByte = 0 ;
}



template < typename _T >
template < typename Sseq >
void PseudoRandomCat < _T > :: seed ( Sseq & q )
{
	static size_type constexpr SEQUENCE_RESULT_COUNT ( ( HASH_DIGEST_BYTE_COUNT / sizeof ( uint32_t ) ) + 2 ) ;

	CryptoPP :: FixedSizeAlignedSecBlock <	byte														,
														SEQUENCE_RESULT_COUNT * sizeof ( uint32_t )	> sequenceResult ;

	q.generate (	reinterpret_cast < uint32_t * > ( sequenceResult.data ( ) )									,
						reinterpret_cast < uint32_t * > ( sequenceResult.data ( ) ) + SEQUENCE_RESULT_COUNT	) ;

	uint32_t mask , value ;
	byte * pCurrByte ;

	for ( byte * pCurrResult ( sequenceResult.data ( ) + ( ( SEQUENCE_RESULT_COUNT - 1 ) * sizeof ( uint32_t ) ) ) ;
			pCurrResult >= sequenceResult.data ( ) ;
			pCurrResult -= sizeof ( uint32_t ) )
	{
		mask = ( 1 << CHAR_BIT ) - 1 ;
		value = *reinterpret_cast < uint32_t * > ( pCurrResult ) ;

		for ( pCurrByte = pCurrResult + ( sizeof ( value ) - 1 ) ;
				pCurrByte >= pCurrResult ; pCurrByte -- )
		{
			*pCurrByte = static_cast < byte > ( ( value & mask ) >> ( CHAR_BIT * ( ( sizeof ( value ) - 1 ) - ( pCurrByte - pCurrResult ) ) ) ) ;
			mask <<= CHAR_BIT ;
		}
	}

	seed (	sequenceResult.data ( )								,
				SEQUENCE_RESULT_COUNT * sizeof ( uint32_t )	) ;
}



template < typename _T >
inline void PseudoRandomCat < _T > :: discard ( unsigned long long z )
{
	for ( ; z > 0 ; z -- )
		Next ( ) ;
}



template < typename _T >
inline typename PseudoRandomCat < _T > :: result_type PseudoRandomCat < _T > :: Next ( )
{
	result_type result ;
}



template < typename _T >
inline typename PseudoRandomCat < _T > :: result_type & PseudoRandomCat < _T > :: Next ( result_type & dest )
{
	result_type result ;
}



template < typename _T >
inline typename PseudoRandomCat < _T > :: result_type PseudoRandomCat < _T > :: Next (	result_type	_minResult	,
																													result_type	_maxResult	)
{
}



template < typename _T >
inline typename PseudoRandomCat < _T > :: result_type & PseudoRandomCat < _T > :: Next (	result_type &	dest			,
																														result_type		_minResult	,
																														result_type		_maxResult	)
{
}



template < typename _T >
inline typename PseudoRandomCat < _T > :: result_type PseudoRandomCat < _T > :: operator ( ) ( )
{
	return Next ( ) ;
}



template < typename _T >
inline typename PseudoRandomCat < _T > :: result_type & PseudoRandomCat < _T > :: operator ( ) ( result_type & dest )
{
	return Next ( dest ) ;
}



template < typename _T >
inline typename PseudoRandomCat < _T > :: result_type PseudoRandomCat < _T > :: operator ( ) (	result_type	_minResult	,
																																result_type	_maxResult	)
{
	return Next (	_minResult	,
						_maxResult	) ;
}



template < typename _T >
inline typename PseudoRandomCat < _T > :: result_type & PseudoRandomCat < _T > :: operator ( ) (	result_type &	dest			,
																																	result_type		_minResult	,
																																	result_type		_maxResult	)
{
	return Next (	dest			,
						_minResult	,
						_maxResult	) ;
}







#ifdef RANDOMCAT_TEMP_MIN
	#define min( a , b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
	#undef RANDOMCAT_TEMP_MIN
#endif  // #ifdef RANDOMCAT_TEMP_MIN



#ifdef RANDOMCAT_TEMP_MAX
	#define max( a , b ) ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
	#undef RANDOMCAT_TEMP_MAX
#endif  // #ifdef RANDOMCAT_TEMP_MAX







#endif  // #ifndef RANDOMCAT_RANDOMCAT_H
// End of traditional header macro guard
