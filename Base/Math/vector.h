/**
 * @file vector.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: vector.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 *
 * @todo Port over remaining methods.
 *
 */
#if !defined(_F4_MATH_VECTOR_H_)
#define _F4_MATH_VECTOR_H_

#include "math.h"
ENTER_NAMESPACE(f4);


// --- C interface ---
MATH_API void Vec3Normalize(Vector3* v);
// --- end C ---


/**
 * A vector with X and Y components.
 * @ingroup f4_math
 */
struct Vector2
{
	Vector2() { }


	/**
	 * The vector components.
	 */
	Float x, y;


	/**
	 * Initializing constructor.
	 */
	Vector2(Float x, Float y)
	{
		this->x = x;
		this->y = y;
	}


	/**
	 * Initializing constructor.
	 */
	Vector2(const Vector2& v)
	{
		x = v.x;
		y = v.y;
	}
	 
	 
	/**
	 * Set all of the vector components at once.
	 */
	void set(Float x, Float y)
	{
		this->x = x;
		this->y = y;
	}

};




/**
 * A vector with X, Y, and Z components.
 * @ingroup f4_math
 */
struct Vector3
{
	Vector3() {}

	/**
	 * The vector components.
	 */
	Float x, y, z;

	
	/**
	 * Initializing constructor.
	 */
	Vector3(Float x, Float y, Float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	
	/**
	 * Initializing constructor.
	 */
	Vector3(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	 
	 
	/**
	 * Assignment operator.
	 */
	Vector3& operator = (const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}


	/**
	 * Set all of the vector components at once.
	 */
	void set(Float x, Float y, Float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}


	/**
	 * Zero out all components of the vector.
	 */
	void zero()
	{
		x = 0;
		y = 0;
		z = 0;
	}


	/**
	 * Treat the vector as an array of floats.
	 */
	operator Float*()
	{
		return &x;
	}

	
	/**
	 * Set the vector length to one.
	 */
	void normalize()
	{
		Vec3Normalize(this);
	}


	/**
	 * Vector addition.
	 */
	Vector3 operator + (const Vector3& v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}


	/**
	 * Vector subtraction.
	 */
	Vector3 operator - (const Vector3& v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}


	/**
	 * Vector negation.
	 */
	Vector3 operator - () const
	{
		return Vector3(-x, -y, -z);
	}

	 
	/**
	 * Vector scaling.
	 */
	Vector3 operator * (Float s) const
	{
		return Vector3(x * s, y * s, z * s);
	}


	/**
	 * Calculate a vector cross product.
	 */
	Vector3 cross(const Vector3& v) const
	{
		return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}


	/**
	 * Calculate a vector dot product.
	 */
	Float dot(const Vector3& v) const
	{
		return (x * v.x + y * v.y + z * v.z);
	}


	/**
	 * Get the length of the vector.
	 */
	Float length() const
	{
		return (sqrt(x*x + y*y + z*z));
	}

};


EXIT_NAMESPACE;
#endif
