/**
 * @file action.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: action.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <StdLib/action.h>
#include <Memory/memory.h>
using namespace f4;


struct ActionImpl : public Action
{
	virtual bool initialize(const Char* actionName, Float value);

	virtual const Char* getName() const { return m_name;  }
	virtual Float getValue() const      { return m_value; }

private:
	const Char* m_name;
	Float m_value;
	DECL_SERVICE;
};

IMPL_SERVICE(ActionImpl, Action)

//---------------------------------------------------------------------------

bool ActionImpl::initialize(const Char* actionName, Float value)
{
	m_name = actionName;
	m_value = value;
	return true;
}
