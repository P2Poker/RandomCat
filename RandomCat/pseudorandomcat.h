
/* If base class not included yet, include base class first.
	RandomCat.h will re-include this header after the base class definition. */
#ifndef RANDOMCAT_RANDOMCAT_H

	// High-performance multi-threaded crypto-secure true RNG
	#include "randomcat.h"

#elif !defined RANDOMCAT_PSEUDORANDOMCAT_H  // Traditional macro guard
#define RANDOMCAT_PSEUDORANDOMCAT_H







/// \brief Thread-safe, high-performance, manually seedable, highly unpredictable pseudo-random number generator
/// \details Uses AES to perform randomization. Uses SHA to convert generic seed bytes into an AES key and cipher block bytes.
///  Useful in situations that require repeatable random sequences by reusing a given seed value. Should not be used in security-sensitive contexts.
class PseudoRandomCat :
	public RandomCat
{
} ;







#endif	/* #ifndef RANDOMCAT_RANDOMCAT_H
				#elif !defined RANDOMCAT_PSEUDORANDOMCAT_H */
