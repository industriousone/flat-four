/**
 * @file math.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: math.h,v 1.4 2002/06/19 01:39:59 jason379 Exp $
 *
 * @todo Finish porting math objects.
 *
 * @todo Create .dox page for this lib.
 *
 * @todo Add SIMD optimizations. Watch http://libsimd.sourceforge.net/
 *
 */
#if !defined(_F4_MATH_H_)
#define _F4_MATH_H_

#include <math.h>

#if !defined(MATH_API)
#define MATH_API F4_IMPORT
#endif

ENTER_NAMESPACE(f4);

#if defined(PI)
#undef PI
#endif
const Float PI = 3.14169265f;

/**
 * Convert between degrees and radians.
 * @ingroup f4_math
 */
inline Float degreesToRadians(Float degrees) 
{ 
	return degrees * PI / 180.0f;
}


/**
 * Convert between radians and degrees.
 * @ingroup f4_math
 */
inline Float radiansToDegrees(Float radians) 
{
	return radians * 180.0f / PI;
}

EXIT_NAMESPACE;

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

#endif

