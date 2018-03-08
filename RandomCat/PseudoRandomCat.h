
/* If base class not included yet, include base class first.
	RandomCat.h will re-include this header after the base class definition. */
#ifndef RANDOMCAT_RANDOMCAT_H

	// High-performance multi-threaded crypto-secure true RNG
	#include "randomcat.h"

#elif !defined RANDOMCAT_PSEUDORANDOMCAT_H  // Traditional macro guard
#define RANDOMCAT_PSEUDORANDOMCAT_H







class PseudoRandomCat :
	public RandomCat
{
} ;







#endif	/* #ifndef RANDOMCAT_RANDOMCAT_H
				#elif !defined RANDOMCAT_PSEUDORANDOMCAT_H */
