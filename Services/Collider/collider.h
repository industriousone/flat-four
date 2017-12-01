/**
 * @file collider.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: collider.h,v 1.3 2002/05/15 10:40:04 jason379 Exp $
 */
#if !defined(_F4_COLLIDER_H_)
#define _F4_COLLIDER_H_

ENTER_NAMESPACE(f4);

struct BoundingShape;
struct ContactList;


/**
 * The collision tester.
 * @ingroup f4_collide
 */
struct CollisionService : public Service
{

	/**
	 * Test two shapes for intersection. 
	 * 
	 * @param s1,s2     The shapes to test.
	 * @param contacts  If the shapes intersect, this will return information
	 *                  about the contact points. May be @c NULL if contact
	 *                  information is not required.
	 *
	 * @return The number of contacts detected.
	 *
	 * @par Scripted as:
	 *      [ method ]
	 */
	virtual int test(BoundingShape* s1, BoundingShape* s2, ContactList* contacts) = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(CollisionService, test, RETURNS_INT)
	ARG_SERVICE(BoundingShape), ARG_SERVICE(BoundingShape), ARG_SERVICE(ContactList)
END_METHOD

EXPORTS(CollisionService)
	METHOD(CollisionService, test)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif

