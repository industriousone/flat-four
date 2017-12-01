/**
 * @file collision_visitor.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: collision_visitor.cpp,v 1.1 2002/06/19 01:39:59 jason379 Exp $
 */
#include <Core/core.h>
#include <Scene/collision_visitor.h>
#include <Scene/entity.h>
#include <Collider/collider.h>
#include <Physics/physics.h>
#include <vector>
#include <Memory/memory.h>
using namespace f4;

static const int HASH_TBL_SIZE = 1021;    // must be prime


/*
 * The collision pass can actually fire off any number of traversals. The main
 * traversal, the "query" pass, looks for surfaces that have moved, with the
 * assumption that surfaces that aren't moving can't collide with anything.
 * Whenever it encounters a moved surface, it starts a new "test" traversal,
 * looking for intersections with the queried surface.
 *
 * Care must be taken to ensure that every collision is reported only once.
 * This can happen when two moving surfaces are tested against one another.
 * The first surface will register contact with the second, and when the
 * second surface is queried and tested, it will register the same contact
 * again. To avoid this, each moved-moved test is stored in a hash table.
 */

struct iCollisionVisitor : public CollisionVisitor
{
	iCollisionVisitor();
	virtual ~iCollisionVisitor() { }

	virtual void collide(Entity* root);
	virtual void hasMoved();
	virtual void setPhysics(PhysicsObject* physics);
	virtual void test(Surface* surf);

	// Visitor interface
	virtual void visitEntity(Entity* ent);
	virtual void visitAttribute(Attribute* at);

private:
	// The services and contacts used for all collision tests
	Ref<CollisionService> m_cs;
	Ref<PhysicsService>   m_ps;
	Ref<ContactList>      m_contacts;

	// Keep track of the root node of the entity tree, so I know where to
	// start all of the "test" passes. This will be set to whatever entity
	// is first encountered by traverseEntity()
	Ref<Entity> m_root;

	// Maintain some information at each level of the tree, for both the
	// "query" and "test" passes.
	struct State
	{
		State(Entity* e, bool m)  { entity=e; hasMoved=m; }

		Ref<Entity> entity;
		bool        hasMoved;
	};

	typedef std::vector<State> Stack;
	Stack  m_queryStack, m_testStack;
	Stack* m_currentStack;

	// Keep track of both objects involved in the collision. If obj1.surface
	// is null, then I'm in "query" mode, looking for a moved surface to test.
	// If it is not null, then I'm in "test" mode, looking for intersections
	// with the given surface.
	struct CollidingObject
	{
		Ref<Entity>  entity;
		Ref<Surface> surface;
		Ref<PhysicsObject> physics;
	};
	CollidingObject  m_obj1, m_obj2;
	CollidingObject* m_nowTouching;

	// This hash table ensures contacts are reported only once for any pair.
	// Only tests between two moving objects have this problem. To consolidate
	// memory, all pairs are stored in a dynamic array and referenced by index.
	bool _checkCache(Surface* s1, Surface* s2);

	struct Pairing
	{
		Pairing(Surface* s1, Surface* s2, int n) { surface1=s1; surface2=s2; next=n; }

		Surface* surface1;
		Surface* surface2;
		int      next;
	};

	typedef std::vector<Pairing> Pairings;
	Pairings m_pairings;

	int m_pairLookup[HASH_TBL_SIZE];

	// Process a contact between two surfaces
	void _doContact();

	DECL_SERVICE;
};

IMPL_SERVICE(iCollisionVisitor, CollisionVisitor)



//---------------------------------------------------------------------------

iCollisionVisitor::iCollisionVisitor()
{
	m_cs       = (CollisionService*)Core->createA("Collider.CollisionService");
	m_contacts = (ContactList*)Core->createA("Collider.ContactList");
	m_ps       = (PhysicsService*)Core->createA("Physics.PhysicsService");
}

//---------------------------------------------------------------------------

void iCollisionVisitor::collide(Entity* root)
{
	// Clear out any cached data
	for (int i = 0; i < HASH_TBL_SIZE; ++i)
		m_pairLookup[i] = -1;
	m_pairings.clear();

	// Set up for a "query" pass
	m_obj1.entity  = 0;
	m_obj1.surface = 0;
	m_obj1.physics = 0;
	m_obj2.entity  = 0;
	m_obj2.surface = 0;
	m_obj2.physics = 0;
	m_currentStack = &m_queryStack;
	m_nowTouching  = &m_obj1;
	m_root = 0;

	// Run the test
	visitEntity(root);
}

//---------------------------------------------------------------------------

void iCollisionVisitor::hasMoved()
{
	m_currentStack->back().hasMoved = true;
}

//---------------------------------------------------------------------------

void iCollisionVisitor::setPhysics(PhysicsObject* physics)
{
	m_nowTouching->physics = physics;
}

//---------------------------------------------------------------------------

void iCollisionVisitor::test(Surface* surf)
{
	// If I'm in "query" mode, and this surface has moved, then store the 
	// surface and start a test pass.
	if (!m_obj1.surface)
	{
		if (m_queryStack.back().hasMoved)
		{
			// Set up test mode
			m_obj1.entity  = m_queryStack.back().entity;
			m_obj1.surface = surf;
			m_obj1.physics = 0;
			m_currentStack = &m_testStack;

			// Start the traversal at the top of the tree
			this->visitEntity(m_root);

			// Return to query mode
			m_obj1.entity  = 0;
			m_obj1.surface = 0;
			m_currentStack = &m_queryStack;
		}
	}

	// If in "test" mode, check to see if the surfaces intersect
	else
	{
		// Can't hit yourself...
		if (m_obj1.surface == surf)
			return;

		m_obj2.entity  = m_testStack.back().entity;
		m_obj2.surface = surf;

		// If both surfaces are in motion, check the test cache to see if I
		// have already run this test before. If so, bail out.
		if (m_testStack.back().hasMoved && _checkCache(m_obj1.surface, m_obj2.surface))
			return;

		// Compare all of the bounding shapes in each surface
		BoundingShape* shape1;
		BoundingShape* shape2;
		for (int i = 0; i < m_obj1.surface->countBoundingShapes(); ++i)
		{
			shape1 = m_obj1.surface->getBoundingShape(i);

			for (int j = 0; j < m_obj2.surface->countBoundingShapes(); ++j)
			{
				shape2 = m_obj2.surface->getBoundingShape(j);

				if (m_cs->test(shape1, shape2, m_contacts) > 0)
					_doContact();
			}
		}
	}
}

//---------------------------------------------------------------------------

/* 
 * At each level of the tree, I need to know a) which entity I am currently
 * processing, and b) whether or not that entity has moved. I use the stack
 * to keep track of this information.
 */
void iCollisionVisitor::visitEntity(Entity* ent)
{
	// Remember the root node of the tree, so I know where to start all
	// subsequent "test" passes
	if (!m_root) 
		m_root = ent;

	bool moved = (m_currentStack->empty()) ? false : m_currentStack->back().hasMoved;
 	m_currentStack->push_back(State(ent, moved));

	ent->visit(this);

	m_currentStack->pop_back();
}

//---------------------------------------------------------------------------

void iCollisionVisitor::visitAttribute(Attribute* at)
{
	at->collide(this);
}

//---------------------------------------------------------------------------

bool iCollisionVisitor::_checkCache(Surface* s1, Surface* s2)
{
	// I need to make sure I always use the same surface as the lookup
	// key, no matter what order the parameters arrive. To keep things
	// easy I just use the pointer with the lowest value.
	if (s1 > s2)
	{
		Surface* swap = s1;
		s1 = s2;
		s2 = swap;
	}

	// Iterate though the bucket for this surface and look for a matching pair
	int bucket = (unsigned)s1 % HASH_TBL_SIZE;
	for (int i = m_pairLookup[bucket]; i >= 0; i = m_pairings[i].next)
	{
		if (m_pairings[i].surface1 == s1 && m_pairings[i].surface2 == s2)
			return true;
	}

	// Pairing does not exist, add it now
	m_pairings.push_back(Pairing(s1, s2, m_pairLookup[bucket]));
	m_pairLookup[bucket] = m_pairings.size() - 1;
	return false;
}

//---------------------------------------------------------------------------

void iCollisionVisitor::_doContact()
{
	// Call touch() on both entities
	m_obj1.entity->touch(this);
	m_nowTouching = &m_obj2;
	m_obj2.entity->touch(this);
	m_nowTouching = &m_obj1;

	// If neither object provided physics information, I'm done
	if (!m_obj1.physics && !m_obj2.physics)
		return;

	// Apply physics response, if appropriate. If only one surface has
	// physical properties, use those to resolve the collision. Otherwise,
	// create a custom set of properties that merge the two
	ContactPhysics* cp1 = m_obj1.surface->getContactPhysics();
	ContactPhysics* cp2 = m_obj2.surface->getContactPhysics();

	ContactPhysics* cp;
	if (!cp1 && !cp2)
		return;

	else if (cp1 && !cp2)
		cp = cp1;

	else if (!cp1 && cp2)
		cp = cp2;
	
	else
	{
		// Hack: just use the first set of properties, to get something running
		cp = cp1;
	}

	// Apply the collision constraint
	m_ps->resolveCollision(m_obj1.physics, m_obj2.physics, m_contacts, cp);
}
