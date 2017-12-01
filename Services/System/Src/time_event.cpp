/**
 * @file time_event.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: time_event.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <System/time_event.h>
#include <Memory/memory.h>
using namespace f4;


struct TimeEventImpl : public TimeEvent
{
	virtual bool initialize(Float elapsedTime);
	virtual Float getElapsed() const            { return m_elapsed; }
	virtual void  setElapsed(Float elapsed)     { m_elapsed = elapsed; }
private:
	Float m_elapsed;
	DECL_SERVICE;
};

IMPL_SERVICE(TimeEventImpl, TimeEvent)

//---------------------------------------------------------------------------

bool TimeEventImpl::initialize(Float elapsedTime)
{
	m_elapsed = elapsedTime;
	return true;
}
