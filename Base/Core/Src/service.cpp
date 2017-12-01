/**
 * @file service.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: service.cpp,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#include <string.h>

#include <Core/core.h>
#include <Script/script.h>
#include <Memory/memory.h>
using namespace f4;


struct ServiceImpl : public Service
{
	DECL_SERVICE;
};

IMPL_SERVICE(ServiceImpl, Service)



//---------------------------------------------------------------------------
// Local stuff
//---------------------------------------------------------------------------

static bool _findInterface(const Char* i, ExportInfo* x);
extern ScriptService* _script;     // defined in core.cpp



//---------------------------------------------------------------------------

void f4::_serviceReference(Service* s)
{
	// If the script system holds the only reference to this object it may
	// think that it can garbage collect it. Notify the script system that
	// the C++ environment has staked a claim to this object as well
	if (s->m_scriptData && s->m_referenceCount == 1)
		_script->reference(s, s->m_scriptData);

	++s->m_referenceCount;
}

//---------------------------------------------------------------------------

void f4::_serviceRelease(Service* s)
{
	--s->m_referenceCount;

	// If the C++ code has dropped all of it's references to the object,
	// notify the script system that it's okay to garbage collect
	if (s->m_scriptData && s->m_referenceCount == 1)
		_script->release(s, s->m_scriptData);

	if (s->m_referenceCount <= 0)
		delete s;
}

//---------------------------------------------------------------------------

const Char* f4::_serviceTypeName(Service* s)
{
	if (s->m_scriptData) 
	{
		const Char* n = _script->getTypeName(s);
		if (n) return n;
	}

	return s->getTypeInfo()->name;
}

//---------------------------------------------------------------------------

bool f4::_serviceIsA(Service* s, const Char* i)
{
	// Check the common case quick
	ServiceInfo* si = s->getTypeInfo();
	if (strcmp(si->name, i) == 0) return true;

	// If it's a scripted service, check there
	if (s->m_scriptData)
	{
		if (_script->isA(s, i))
			return true;
	}

	// Search the exports list for the interface
	bool found = _findInterface(i, si->x);

	// Last chance...
	return (found || strcmp(i, "Service") == 0);
}

//---------------------------------------------------------------------------

bool _findInterface(const Char* name, ExportInfo* x)
{
	for (int i = 0; x[i].name; ++i)
	{
		if (x[i].type == F4_X_INTERFACE)
		{
			if (!strcmp(name, x[i].name))
				return true;

			if (_findInterface(name, (ExportInfo*)x[i].addr1))
				return true;
		}
	}

	return false;
}
