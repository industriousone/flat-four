/**
 * @file quaternion.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: quaternion.cpp,v 1.3 2002/04/22 20:22:07 jason379 Exp $
 */
#include <Core/core.h>
#include <Math/quaternion.h>
using namespace f4;


//---------------------------------------------------------------------------

void f4::QuatNormalize(Quaternion* q)
{
	Float len = sqrt(q->x*q->x + q->y*q->y + q->z*q->z);

	// If len is too small, set to an arbitrary orientation
	if (len > 0.000001)
	{
		q->w /= len;
		q->x /= len;
		q->y /= len;
		q->z /= len;
	}
	else
	{
		q->w = 1;
		q->x = 0;
		q->y = 0;
		q->z = 0;
	}
}

//---------------------------------------------------------------------------

void f4::QuatSetAngleAxisRotation(Quaternion* q, Float angle, Float x, Float y, Float z)
{
	angle = degreesToRadians(-angle) / 2;
	Float scale = sin(angle);

	q->w = cos(angle);
	q->x = x * scale;
	q->y = y * scale;
	q->z = z * scale;
}

//---------------------------------------------------------------------------

void f4::QuatSetEulerRotation(Quaternion* q, Float h, Float p, Float r)
{
	// Convert from F4/OpenGL system of +X=right, +Y=up, +Z=out to the 
	// "standard" (?) +X=right, +Y=in, +Z=up
	Float ah = -degreesToRadians(r) / 2.0;
	Float ap = -degreesToRadians(h) / 2.0;
	Float ar = -degreesToRadians(p) / 2.0;

	Float ch = cos(ah);
	Float sh = sin(ah);
	Float cp = cos(ap);
	Float sp = sin(ap);
	Float cr = cos(ar);
	Float sr = sin(ar);

	q->w = ch * cp * cr + sh * sp * sr;
	q->x = ch * cp * sr + sh * sp * cr;
	q->y = ch * sp * cr + sh * cp * sr;
	q->z = sh * cp * cr + ch * sp * sr;
}

//---------------------------------------------------------------------------

void f4::QuatSetMatrixRotation(Quaternion* q, Matrix44* m)
{
	Float trace = (*m)[0][0] + (*m)[1][1] + (*m)[2][2];
	if (trace > 0)
	{
		Float root = (Float)sqrt(trace + 1);
		q->w = root * 0.5f;

		root = 0.5f / root;
		q->x = ((*m)[2][1] - (*m)[1][2]) * root;
		q->y = ((*m)[0][2] - (*m)[2][0]) * root;
		q->z = ((*m)[1][0] - (*m)[0][1]) * root;
	}
	else
	{
		int next[3] = { 1, 2, 0 };
		Vector3 vec;

		int i = 0;
		if ((*m)[1][1] > (*m)[0][0]) i = 1;
		if ((*m)[2][2] > (*m)[i][i]) i = 2;

		int j = next[i];
		int k = next[j];

		Float root = (Float)sqrt((*m)[i][i] - (*m)[j][j] - (*m)[k][k] + 1);

		vec[i] = 0.5f * root;
		root = 0.5f / root;
		vec[j] = ((*m)[j][i] + (*m)[i][j]) * root;
		vec[k] = ((*m)[k][i] + (*m)[i][k]) * root;
		q->w   = ((*m)[k][j] - (*m)[j][k]) * root;

		q->x = vec.x;
		q->y = vec.y;
		q->z = vec.z;
	}
}
