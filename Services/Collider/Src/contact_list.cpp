/**
 * @file contact_list.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: contact_list.cpp,v 1.3 2002/05/24 19:05:31 jason379 Exp $
 */
#include <Core/core.h>
#include <Collider/contact_list.h>
#include <Math/math.h>
#include <vector>
#include <assert.h>
#include <Memory/memory.h>
using namespace f4;


struct ContactListImpl : public ContactList
{
	virtual void setNumContacts(int num);
	virtual void setContact(int i, const Vector3& pos, const Vector3& normal, Float depth);
	virtual int  size() const;

	virtual const Vector3& getPosition(int i) const;
	virtual const Vector3& getNormal(int i) const;
	virtual Float getDepth(int i) const;

private:
	struct Contact
	{
		Vector3 pos;
		Vector3 normal;
		Float   depth;
	};

	typedef std::vector<Contact> Contacts;
	Contacts m_contacts;

	DECL_SERVICE;
};

IMPL_SERVICE(ContactListImpl, ContactList)

//---------------------------------------------------------------------------

void ContactListImpl::setNumContacts(int num)
{
	m_contacts.resize(num);
}

//---------------------------------------------------------------------------

void ContactListImpl::setContact(int i, const Vector3& pos, const Vector3& normal, Float depth)
{
	if (i < 0 || i >= (int)m_contacts.size()) return;

	m_contacts[i].pos = pos;
	m_contacts[i].normal = normal;
	m_contacts[i].depth = depth;
}

//---------------------------------------------------------------------------

int ContactListImpl::size() const
{
	return m_contacts.size();
}

//---------------------------------------------------------------------------

const Vector3& ContactListImpl::getPosition(int i) const
{
	assert(i >= 0 && i < (int)m_contacts.size());
	return m_contacts[i].pos;
}

//---------------------------------------------------------------------------

const Vector3& ContactListImpl::getNormal(int i) const
{
	assert(i >= 0 && i < (int)m_contacts.size());
	return m_contacts[i].normal;
}

//---------------------------------------------------------------------------

Float ContactListImpl::getDepth(int i) const
{
	assert(i >= 0 && i < (int)m_contacts.size());
	return m_contacts[i].depth;
}
