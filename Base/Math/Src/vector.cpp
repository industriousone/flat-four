/**
 * @file vector.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: vector.cpp,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#include <Core/core.h>
#include <Math/math.h>
using namespace f4;


void f4::Vec3Normalize(Vector3* v)
{
	Float len = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);

	// If len is too small, set to arbitrary vector
	if (len > 0.000001)
	{
		v->x /= len;
		v->y /= len;
		v->z /= len;
	}
	else
	{
		v->x = 1.0;
		v->y = 0.0;
		v->z = 0.0;
	}

}
