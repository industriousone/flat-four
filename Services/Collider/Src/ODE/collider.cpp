/**
 * @file collider.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: collider.cpp,v 1.3 2002/05/15 10:40:04 jason379 Exp $
 */
#include <ode/ode.h>

#include <Core/core.h>
#include <Collider/collider.h>
#include <Collider/contact_list.h>
#include <Collider/shape.h>
#include <Math/math.h>
#include <Memory/memory.h>
using namespace f4;


struct CollisionImpl : public CollisionService
{
	virtual ~CollisionImpl() { }

	virtual int test(BoundingShape* s1, BoundingShape* s2, ContactList* contacts);

private:
	DECL_SERVICE;
};

IMPL_SERVICE(CollisionImpl, CollisionService)

//---------------------------------------------------------------------------

int CollisionImpl::test(BoundingShape* s1, BoundingShape* s2, ContactList* contacts)
{
	dGeomID g1 = *(dGeomID*)s1->getShapeData();
	dGeomID g2 = *(dGeomID*)s2->getShapeData();

	dContactGeom cInfo[3];
	int numContacts = dCollide(g1, g2, contacts ? 3 : 1, cInfo, sizeof(dContactGeom));

	if (contacts)
	{
		contacts->setNumContacts(numContacts);
		for (int c = 0; c < numContacts; ++c)
		{
			dContactGeom& ci = cInfo[c];
			contacts->setContact(c, Vector3(ci.pos[0], ci.pos[1], ci.pos[2]),
								 Vector3(ci.normal[0], ci.normal[1], ci.normal[2]), ci.depth);
		}
	}

	return numContacts;
}
