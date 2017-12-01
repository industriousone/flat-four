/**
 * @file mass_helper.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: mass_helper.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_PHYSICS_MASS_HELPER_H_)
#define _F4_PHYSICS_MASS_HELPER_H_

ENTER_NAMESPACE(f4);


/**
 * An interface for calculating the mass and inertia of complex objects.
 * @ingroup f4_physics
 */
struct MassHelper : public Service
{

	/**
	 * Set up for a solid box with the given dimensions.
	 */
	virtual void setSolidCylinder(Float mass, Float radius, Float width) = 0;


	/**
	 * Adjust the center of mass to a new position relative to the parent space.
	 */
	virtual void translate(Float x, Float y, Float z) = 0;


	/**
	 * Add mass B to mass A.
	 */
	virtual void add(MassHelper* b) = 0;


	// --- internal interface ---
	virtual void* getMassData() = 0;
	// --- internal interface ---

};



// --- begin scripting interface ---

SCRIPT_METHOD(MassHelper, add, RETURNS_VOID)
	ARG_SERVICE(MassHelper)
END_METHOD

SCRIPT_METHOD(MassHelper, setSolidCylinder, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(MassHelper, translate, RETURNS_VOID)
	ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

EXPORTS(MassHelper)
	METHOD(MassHelper, add)
	METHOD(MassHelper, setSolidCylinder)
	METHOD(MassHelper, translate)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
