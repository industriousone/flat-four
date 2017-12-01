/**
 * @file entity.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: entity.cpp,v 1.7 2002/07/16 01:11:36 jason379 Exp $
 */
#include <Core/core.h>
#include <Scene/entity.h>
#include <Scene/collision_visitor.h>
#include <string>
#include <vector>
#include <Memory/memory.h>
using namespace f4;


struct EntityImpl : public Entity
{
	EntityImpl();
	virtual ~EntityImpl() { }

	virtual bool initialize(const Char* name);

	virtual void setName(const Char* name);
	virtual const Char* getName() const;

	virtual void addAttribute(Attribute* a);
	virtual void removeAttribute(Attribute* a);
	virtual void insertAttribute(Attribute* a, Attribute* before);
	virtual void replaceAttribute(Attribute* aOld, Attribute* aNew);
	virtual Attribute* queryAttribute(const Char* type);

	virtual void visit(Visitor* v);
	virtual void touch(CollisionVisitor* cv);

private:
	std::string m_name;

	typedef std::vector< Ref<Attribute> > Attributes;
	Attributes m_attributes;

	DECL_SERVICE;
};

IMPL_SERVICE(EntityImpl, Entity)

//---------------------------------------------------------------------------

EntityImpl::EntityImpl()
{
}

//---------------------------------------------------------------------------

bool EntityImpl::initialize(const Char* name)
{
	m_name = name;
	return true;
}

//---------------------------------------------------------------------------

void EntityImpl::setName(const Char* name)
{
	m_name = name;
}

//---------------------------------------------------------------------------

const Char* EntityImpl::getName() const
{
	return m_name.c_str();
}

//---------------------------------------------------------------------------

void EntityImpl::addAttribute(Attribute* a)
{
	if (!a) return;

	m_attributes.push_back(a);
	a->connect(this);
}

//---------------------------------------------------------------------------

void EntityImpl::removeAttribute(Attribute* a)
{
	Attributes::iterator it;
	for (it = m_attributes.begin(); it != m_attributes.end(); ++it)
	{
		if ((*it) == a)
		{
			a->disconnect();
			m_attributes.erase(it);
			return;
		}
	}
}

//---------------------------------------------------------------------------

void EntityImpl::insertAttribute(Attribute* a, Attribute* before)
{
	Attributes::iterator it;
	for (it = m_attributes.begin(); it != m_attributes.end(); ++it)
	{
		if ((*it) == before)
		{
			m_attributes.insert(it, a);
			a->connect(this);
			return;
		}
	}

	// 'before' was not found, add 'a' to the end of the list
	m_attributes.push_back(a);
	a->connect(this);
}

//---------------------------------------------------------------------------

void EntityImpl::replaceAttribute(Attribute* aOld, Attribute* aNew)
{
	Attributes::iterator it;
	for (it = m_attributes.begin(); it != m_attributes.end(); ++it)
	{
		if ((*it) == aOld)
		{
			// Remove the old attribute
			aOld->disconnect();

			// Insert the new one
			(*it) = aNew;
			aNew->connect(this);
			return;
		}
	}

	// 'aOld' was not found, add 'aNew' to the end of the list
	m_attributes.push_back(aNew);
	aNew->connect(this);
}

//---------------------------------------------------------------------------

Attribute* EntityImpl::queryAttribute(const Char* type)
{
	Attributes::iterator it;
	for (it = m_attributes.begin(); it != m_attributes.end(); ++it)
	{
		if ((*it)->isA(type))
			return (*it);
	}

	return 0;
}

//---------------------------------------------------------------------------

void EntityImpl::visit(Visitor* v)
{
	Attributes::iterator it;
	for (it = m_attributes.begin(); it != m_attributes.end(); ++it)
		v->visitAttribute(*it);
}

//---------------------------------------------------------------------------

void EntityImpl::touch(CollisionVisitor* cv)
{
	Attributes::iterator it;
	for (it = m_attributes.begin(); it != m_attributes.end(); ++it)
		(*it)->touch(cv);
}

