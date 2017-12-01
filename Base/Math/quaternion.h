/**
 * @file quaternion.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: quaternion.h,v 1.3 2002/07/16 01:11:36 jason379 Exp $
 *
 * @todo Port over remaining methods.
 *
 */
#if !defined(_F4_MATH_QUATERNION_H_)
#define _F4_MATH_QUATERNION_H_

#include "math.h"
ENTER_NAMESPACE(f4);

struct Quaternion;

// --- C interface ---
MATH_API void QuatNormalize(Quaternion* q);
MATH_API void QuatSetAngleAxisRotation(Quaternion* q, Float angle, Float x, Float y, Float z);
MATH_API void QuatSetEulerRotation(Quaternion* q, Float h, Float p, Float r);
MATH_API void QuatSetMatrixRotation(Quaternion* q, Matrix44* m);
// --- end C ---


/**
 * A quaternion is a quaternion.
 * @ingroup f4_math
 */
struct Quaternion
{
	Quaternion() { }


	/**
	 * The quaternion components.
	 */
	Float w, x, y, z;


	/**
	 * Initializing constructor.
	 */
	Quaternion(Float w, Float x, Float y, Float z)
	{
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}


	/**
	 * Initializing constructor.
	 */
	Quaternion(const Quaternion& q)
		{
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
	}


	/**
	 * Assignment operator.
	 */
	Quaternion& operator = (const Quaternion& q)
	{
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
		return *this;
	}


	/**
	 * Set the quaternion components.
	 */
	void set(Float w, Float x, Float y, Float z)
	{
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}


	/**
	 * Sets to the identity matrix.
	 */
	void setIdentity()
	{
		w = 1.0;
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}


	/**
	 * Initialize the quaternion from angle-axis representation.
	 *
	 * @param angle   The angle, in degrees.
	 * @param x,y,z   The normalized axis components.
	 *
	 * @par C Interface:
	 *      @code
	 *      void QuatSetAngleAxisRotation(Quaternion* q, Float angle, Float x, Float y, Float z);
	 *      @endcode
	 */
	void setAngleAxisRotation(Float angle, Float x, Float y, Float z)
	{
		QuatSetAngleAxisRotation(this, angle, x, y, z);
	}


	/**
	 * Initialize the quaternion from a euler representation.
	 * @par C Interface:
	 *      @code
	 *      void QuatSetEulerRotation(Quaternion* q, Float h, Float p, Floar r);
	 *      @endcode
	 */
	void setEulerRotation(Float h, Float p, Float r)
	{
		QuatSetEulerRotation(this, h, p, r);
	}


	/**
	 * Initialize the quaternion from the rotational part of a matrix.
	 * @par C Interface:
	 *      @code
	 *      void QuatSetMatrixRotation(Quaternion* q, Matrix44* m);
	 *      @endcode
	 */
	void setMatrixRotation(const Matrix44& m)
	{
		QuatSetMatrixRotation(this, const_cast<Matrix44*>(&m));
	}


	/**
	 * Make this a unit quaternion.
	 *
	 * @par C Interface:
	 *      @code
	 *      void QuatNormalize(Quaternion* q);
	 *      @endcode
	 */
	void normalize()
	{
		QuatNormalize(this);
	}


	/**
	 * Concatonate two rotations.
	 */
	Quaternion operator * (const Quaternion& q) const
	{
		return Quaternion(w*q.w - x*q.x - y*q.y - z*q.z,
						  w*q.x + x*q.w + y*q.z - z*q.y,
						  w*q.y + y*q.w + z*q.x - x*q.z,
						  w*q.z + z*q.w + x*q.y - y*q.x);
	}


	/**
	 * Concatonate two rotations.
	 */
	void operator *= (const Quaternion& q)
	{
		*this = (*this) * q;
	}

	 
	/**
	 * Invert the quaternion (returns the opposite rotation).
	 */
	Quaternion inverse() const
	{
		return Quaternion(-w, x, y, z);
	}

};	



EXIT_NAMESPACE;
#endif
