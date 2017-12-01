/**
 * @file curve.h
 *
 * A 2D curve.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id$
 */
#if !defined(_F4_STDLIB_CURVE_H_)
#define _F4_STDLIB_CURVE_H_

ENTER_NAMESPACE(f4);


/**
 * A 2D curve, with interpolation between points.
 * 
 * @ingroup f4_stdlib
 */
struct Curve : public Service
{
	
	/**
	 * Add a data point to the curve.
	 */
	virtual void addPoint(Float x, Float y) = 0;


	/**
	 * Retrieve a point on the curve.
	 */
	virtual Float getValueAt(Float x) = 0;

};


// --- begin scripting interface ---

EXPORTS(Curve)
END_EXPORTS

// --- end scripting interface ---

EXIT_NAMESPACE;
#endif
