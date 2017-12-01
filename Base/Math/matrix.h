/**
 * @file matrix.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: matrix.h,v 1.3 2002/05/15 10:36:34 jason379 Exp $
 *
 * @todo Port over remaining methods.
 *
 */
#if !defined(_F4_MATH_MATRIX_H_)
#define _F4_MATH_MATRIX_H_

#include "vector.h"
ENTER_NAMESPACE(f4);

struct Quaternion;
struct Vector3;

// --- C interface ---
MATH_API bool Mat44Inverse(Matrix44* m, const Matrix44* src);
MATH_API void Mat44Multiply(Matrix44* m, const Matrix44* m1, const Matrix44* m2);
MATH_API void Mat44SetAngleAxisRotation(Matrix44* m, Float angle, Float x, Float y, Float z);
MATH_API void Mat44SetEulerRotation(Matrix44* m, Float h, Float p, Float r);
MATH_API void Mat44SetQuaternion(Matrix44* m, const Quaternion* q);
MATH_API void Mat44Transform(Vector3* r, const Matrix44* m, const Vector3* v);
// --- end C ---



/**
 * A 4x4 matrix.
 * @ingroup f4_math
 */
struct Matrix44
{
	Matrix44() { }


	/**
	 * Access the matrix contents as a flat array.
	 */
	operator Float*() const
	{
		return (Float*)&(m_entry[0][0]);
	}


	/**
	 * Access a matrix row as an array.
	 */
	Float* operator[](int row) const
	{
		return (Float*)&(m_entry[row][0]);
	}


	/**
	 * Sets to the identity matrix.
	 */
	void setIdentity()
	{
		m_entry[0][0] = 1.0;  m_entry[0][1] = 0.0;  m_entry[0][2] = 0.0;  m_entry[0][3] = 0.0;
		m_entry[1][0] = 0.0;  m_entry[1][1] = 1.0;  m_entry[1][2] = 0.0;  m_entry[1][3] = 0.0;
		m_entry[2][0] = 0.0;  m_entry[2][1] = 0.0;  m_entry[2][2] = 1.0;  m_entry[2][3] = 0.0;
		m_entry[3][0] = 0.0;  m_entry[3][1] = 0.0;  m_entry[3][2] = 0.0;  m_entry[3][3] = 1.0;
	}


	/**
	 * Returns an identity matrix.
	 */
	static Matrix44 identity()
	{
		Matrix44 m;
		m.setIdentity();
		return m;
	}


	/**
	 * Set the rotational part of the matrix from a quaternion representation.
	 *
	 * @par C Interface:
	 *      @code
	 *      void Mat44SetQuaternion(Matrix44* m, const Quaternion* q);
	 *      @endcode
	 */
	void setQuaternion(const Quaternion& q)
	{
		Mat44SetQuaternion(this, &q);
	}


	/**
	 * Sets the rotational part of the matrix from euler angles. 
	 *
	 * @param h     Heading, or rotation about Y axis
	 * @param p     Pitch, or rotation about X axis
	 * @param r     Roll, or rotation about Z axis.
	 *
	 * @par C Interface:
	 *		@code
	 *		void Mat44SetEulerRotation(Matrix44* m, Float h, Float p, Float r);
	 *		@endcode
	 */
	void setEulerRotation(Float h, Float p, Float r)
	{
		Mat44SetEulerRotation(this, h, p, r);
	}


	/**
	 * Set the rotational part of the matrix from an angle-axis representation.
	 * 
	 * @param angle    The rotation in degrees.
	 * @param x,y,z    The normalized rotation axis.
	 *
	 * @par C Interface:
	 *      @code
	 *      void Mat44SetAngleAxisRotation(Matrix44* m, Float angle, Float x, Float y, Float z);
	 *      @endcode
	 */
	void setAngleAxisRotation(Float angle, Float x, Float y, Float z)
	{
		Mat44SetAngleAxisRotation(this, angle, x, y, z);
	}


	/**
	 * Set the positional part of the matrix.
	 */
	void setTranslation(Float x, Float y, Float z)
	{
		m_entry[3][0] = x;
		m_entry[3][1] = y;
		m_entry[3][2] = z;
	}


	/**
	 * Set the positional part of the matrix.
	 */
	void setTranslation(const Vector3& t)
	{
		m_entry[3][0] = t.x;
		m_entry[3][1] = t.y;
		m_entry[3][2] = t.z;
	}


	/**
	 * Matrix multiplication. 
	 * @par C Interface:
	 *      @code
	 *      void Mat44Multiply(Matrix44* result, Matrix44* m1, Matrix44* m2);
	 *      @endcode
	 */
	Matrix44 operator * (const Matrix44& m) const
	{
		Matrix44 result;
		Mat44Multiply(&result, this, &m);
		return result;
	}


	/**
	 * Vector transformation.
	 * @par C Interface:
	 *      @code
	 *      void Mat44Transform(Vector3* r, const Matrix44* m, const Vector3* v);
	 *      @endcode
	 */
	Vector3 operator * (const Vector3& v) const
	{
		Vector3 result;
		Mat44Transform(&result, this, &v);
		return result;
	}


	/**
	 * Calculate the inverse matrix. There is also a C interface:
	 * @code
	 * bool Mat44Inverse(Matrix44* dst, Matrix44* src);
	 * @endcode
	 */
	Matrix44 inverse() const
	{
		Matrix44 result;
		Mat44Inverse(&result, this);
		return result;
	}


private:
	Float m_entry[4][4];
};


EXIT_NAMESPACE;
#endif
