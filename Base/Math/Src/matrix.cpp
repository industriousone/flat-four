/**
 * @file matrix.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: matrix.cpp,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#include <Core/core.h>
#include <Math/matrix.h>
using namespace f4;


void f4::Mat44SetQuaternion(Matrix44* m, const Quaternion* q)
{
    Float x  = 2.0 * q->x;
    Float y  = 2.0 * q->y;
    Float z  = 2.0 * q->z;
    Float wx = x * q->w;
    Float wy = y * q->w;
    Float wz = z * q->w;
    Float xx = x * q->x;
    Float xy = y * q->x;
    Float xz = z * q->x;
    Float yy = y * q->y;
    Float yz = z * q->y;
    Float zz = z * q->z;

    (*m)[0][0] = 1.0 - (yy+zz);
    (*m)[0][1] = xy - wz;
    (*m)[0][2] = xz + wy;
    (*m)[1][0] = xy + wz;
    (*m)[1][1] = 1.0 - (xx+zz);
    (*m)[1][2] = yz - wx;
    (*m)[2][0] = xz - wy;
    (*m)[2][1] = yz + wx;
    (*m)[2][2] = 1.0 - (xx+yy);
}

//---------------------------------------------------------------------------

void f4::Mat44SetAngleAxisRotation(Matrix44* m, Float angle, Float x, Float y, Float z)
{
	Vector3 axis(x, y, z);
	Float   radians = degreesToRadians(angle);

	Float s = sin(radians);
	Float c = cos(radians);
	Float t = 1.0f - c;

	(*m)[0][0] = t * axis.x * axis.x + c;
	(*m)[0][1] = t * axis.x * axis.y + s * axis.z;
	(*m)[0][2] = t * axis.x * axis.z - s * axis.y;
   
	(*m)[1][0] = t * axis.y * axis.x - s * axis.z;
	(*m)[1][1] = t * axis.y * axis.y + c;
	(*m)[1][2] = t * axis.y * axis.z + s * axis.x;

	(*m)[2][0] = t * axis.z * axis.x + s * axis.y;
	(*m)[2][1] = t * axis.z * axis.y - s * axis.x;
	(*m)[2][2] = t * axis.z * axis.z + c;
}

//---------------------------------------------------------------------------

// From Steve Baker's PLIB
void f4::Mat44SetEulerRotation(Matrix44* m, Float h, Float p, Float r)
{
	// SB uses +Z = up
	Float t = h;
	h = r;
	r = t;

	Float ch, sh;
	if (h == 0.0)
	{
		ch = 1.0;
		sh = 0.0;
	}
	else
	{
		h = degreesToRadians(h);
		sh = sin(h);
		ch = cos(h);
	}

	Float cp, sp;
	if (p == 0.0)
	{
		cp = 1.0;
		sp = 0.0;
	}
	else
	{
		p = degreesToRadians(p);
		sp = sin(p);
		cp = cos(p);
	}
	
	Float cr, sr, srsp, srcp, crsp;
	if (r == 0.0)
	{
		cr = 1.0;
		sr = 0.0;
		srsp = 0.0;
		srcp = 0.0;
		crsp = sp;
	}
	else
	{
		r = degreesToRadians(r);
		sr = sin(r);
		cr = cos(r);
		srsp = sr * sp;
		crsp = cr * sp;
		srcp = sr * cp;
	}

	(*m)[0][0] = ch * cr - sh * srsp;
	(*m)[1][0] = -sh * cp;
	(*m)[2][0] = sr * ch + sh * crsp;

	(*m)[0][1] = cr * sh + srsp * ch;
	(*m)[1][1] = ch * cp;
	(*m)[2][1] = sr * sh - crsp * ch;

	(*m)[0][2] = -srcp;
	(*m)[1][2] = sp;
	(*m)[2][2] = cr * cp;
}

//---------------------------------------------------------------------------

void f4::Mat44Multiply(Matrix44* result, const Matrix44* m1, const Matrix44* m2)
{
	for (int i = 0; i < 4; ++i)
	{
		(*result)[0][i] = (*m1)[0][0] * (*m2)[0][i] +
		                  (*m1)[0][1] * (*m2)[1][i] +
						  (*m1)[0][2] * (*m2)[2][i] +
						  (*m1)[0][3] * (*m2)[3][i];

		(*result)[1][i] = (*m1)[1][0] * (*m2)[0][i] +
		                  (*m1)[1][1] * (*m2)[1][i] +
						  (*m1)[1][2] * (*m2)[2][i] +
						  (*m1)[1][3] * (*m2)[3][i];

		(*result)[2][i] = (*m1)[2][0] * (*m2)[0][i] +
		                  (*m1)[2][1] * (*m2)[1][i] +
						  (*m1)[2][2] * (*m2)[2][i] +
						  (*m1)[2][3] * (*m2)[3][i];

		(*result)[3][i] = (*m1)[3][0] * (*m2)[0][i] +
		                  (*m1)[3][1] * (*m2)[1][i] +
					  	  (*m1)[3][2] * (*m2)[2][i] +
						  (*m1)[3][3] * (*m2)[3][i];
	}
}

//---------------------------------------------------------------------------

// From OpenSceneGraph
bool f4::Mat44Inverse(Matrix44* dst, const Matrix44* src)
{
	Float* a = (Float*)(*src);
	Float* b = (Float*)(*dst);

    int     n = 4;
    int     i, j, k;
    int     r[ 4], c[ 4], row[ 4], col[ 4];
    float   m[ 4][ 4*2], pivot, max_m, tmp_m, fac;

    /* Initialization */
    for ( i = 0; i < n; i ++ )
    {
        r[ i] = c[ i] = 0;
        row[ i] = col[ i] = 0;
    }

    /* Set working matrix */
    for ( i = 0; i < n; i++ )
    {
        for ( j = 0; j < n; j++ )
        {
            m[ i][ j] = a[ i * n + j];
            m[ i][ j + n] = ( i == j ) ? 1.0 : 0.0 ;
        }
    }

    /* Begin of loop */
    for ( k = 0; k < n; k++ )
    {
        /* Choosing the pivot */
        for ( i = 0, max_m = 0; i < n; i++ )
        {
            if ( row[ i]  ) continue;
            for ( j = 0; j < n; j++ )
            {
                if ( col[ j] ) continue;
                tmp_m = fabs( m[ i][j]);
                if ( tmp_m > max_m)
                {
                    max_m = tmp_m;
                    r[ k] = i;
                    c[ k] = j;
                }
            }
        }
        row[ r[k] ] = col[ c[k] ] = 1;
        pivot = m[ r[ k] ][ c[ k] ];

        if ( fabs( pivot) <= 1e-20)
            return false;

        /* Normalization */
        for ( j = 0; j < 2*n; j++ )
        {
            if ( j == c[ k] )
                m[ r[ k]][ j] = 1.0;
            else
                m[ r[ k]][ j] /=pivot;
        }

        /* Reduction */
        for ( i = 0; i < n; i++ )
        {
            if ( i == r[ k] )
                continue;

            for ( j=0, fac = m[ i][ c[k]];j < 2*n; j++ )
            {
                if ( j == c[ k] )
                    m[ i][ j] =0.0;
                else
                    m[ i][ j] -=fac * m[ r[k]][ j];
            }
        }
    }

    /* Assign invers to a matrix */
    for ( i = 0; i < n; i++ )
        for ( j = 0; j < n; j++ )
            row[ i] = ( c[ j] == i ) ? r[j] : row[ i];

    for ( i = 0; i < n; i++ )
        for ( j = 0; j < n; j++ )
            b[ i * n +  j] = m[ row[ i]][j + n];

    return true;                 // It worked
}

//---------------------------------------------------------------------------

void f4::Mat44Transform(Vector3* r, const Matrix44* m, const Vector3* v)
{
	r->x = v->x*(*m)[0][0] + v->y*(*m)[1][0] + v->z*(*m)[2][0] + (*m)[3][0];
	r->y = v->x*(*m)[0][1] + v->y*(*m)[1][1] + v->z*(*m)[2][1] + (*m)[3][1];
	r->z = v->x*(*m)[0][2] + v->y*(*m)[1][2] + v->z*(*m)[2][2] + (*m)[3][2];
}
