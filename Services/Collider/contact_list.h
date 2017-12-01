/**
 * @file contact_list.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: contact_list.h,v 1.3 2002/06/02 18:55:02 jason379 Exp $
 */
#if !defined(_F4_COLLIDER_CONTACT_LIST_H_)
#define _F4_COLLIDER_CONTACT_LIST_H_

ENTER_NAMESPACE(f4);


/**
 * A list of contact point information.
 * @ingroup f4_collide
 */
struct ContactList : public Service
{

	/**
	 * Allocate space for the contacts.
	 */
	virtual void setNumContacts(int num) = 0;


	/**
	 * Initialize a contact.
	 */
	virtual void setContact(int i, const Vector3& pos, const Vector3& normal, Float depth) = 0;


	/**
	 * Get the number of contacts stored in the list.
	 */
	virtual int size() const = 0;


	/**
	 * Get a contact position.
	 */
	virtual const Vector3& getPosition(int i) const = 0;


	/**
	 * Get a contact normal.
	 */
	virtual const Vector3& getNormal(int i) const = 0;


	/**
	 * Get a contact's penetration depth.
	 */
	virtual Float getDepth(int i) const = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(ContactList, getPosition, RETURNS_VECTOR)
	ARG_INT
END_METHOD

SCRIPT_METHOD(ContactList, getNormal, RETURNS_VECTOR)
	ARG_INT
END_METHOD

SCRIPT_METHOD(ContactList, getDepth, RETURNS_FLOAT)
	ARG_INT
END_METHOD

SCRIPT_METHOD(ContactList, setContact, RETURNS_VOID)
	ARG_INT, ARG_VECTOR, ARG_VECTOR, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(ContactList, setNumContacts, RETURNS_VOID)
	ARG_INT
END_METHOD

SCRIPT_METHOD(ContactList, size, RETURNS_INT)
END_METHOD

EXPORTS(ContactList)
	RO_PROP_INT(ContactList, size, size)
	METHOD(ContactList, getPosition)
	METHOD(ContactList, getNormal)
	METHOD(ContactList, getDepth)
	METHOD(ContactList, setContact)
	METHOD(ContactList, setNumContacts)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
