#ifndef RANDOMCAT_RANDOMCAT_H
#define RANDOMCAT_RANDOMCAT_H



// Common declarations and third-party library headers
#include "randomcatcommon.h"







/// \brief Centralized, global, crypto-secure, multi-threaded, high-performance, true random number generator
/// \details Uses AES to perform randomization. Collects entropy from a variety of sources. Uses SHA to fold in collected entropy.
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



protected :

																			RandomCat							(																	) = default ;

public :

																			RandomCat							(	thisT const &			src								) = delete ;

																			RandomCat							(	thisT &&					src								) = delete ;

							thisT &										operator =							(	thisT const &			src								) = delete ;

							thisT &										operator =							(	thisT &&					src								) = delete ;

	///*virtual*/ inline	~RandomPoolBase ( ) ;



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



protected :

	template < typename _T = uintRand >
	static	inline	_T												Next									(																	) ;

	template < typename _T = uintRand >
	static	inline	_T												Next									(	_T							minValue							,
																														_T							maxValue							) ;



	template < typename _T = float >
	static	inline	_T												NextFloat							(	bool						allowInfinity		= false	,
																														bool						allowNaN				= false	) ;

	template < typename _T = float >
	static	inline	_T												NextFloat							(	_T							minValue							,
																														_T							maxValue							,
																														bool						allowInfinity		= false	,
																														bool						allowNaN				= false	) ;



public :

	static				void											IncorporateEntropy				(	byte const *			input								,
																														size_type				length							) ;

	template < typename _T >
	static	inline	void											IncorporateEntropy				(	_T const &				input								) ;

	template < typename Sseq >
	inline				void											seed									(	Sseq &					q									) const ;

	inline				void											discard								(	unsigned long long	z									) ;



private :

	template < typename RANGE_T >
	static	inline	void											CheckRangeUniqueCount			(	bool						uniqueValues					,
																														size_t					nCount							,
																														RANGE_T					range								) ;

	static	inline	void											CheckArrayLength					(	size_t					nCount							,
																														size_t					arrayLength						) ;
} ;







/* Traditional re-seedable pseudo RNG,
	based off of RandomCat's design, and implementing RandomCat's interface */
#include "pseudorandomcat.h"



#endif  // #ifndef RANDOMCAT_RANDOMCAT_H
