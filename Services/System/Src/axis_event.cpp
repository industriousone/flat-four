/**
 * @file axis_event.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: axis_event.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <System/axis_event.h>
#include <Memory/memory.h>
using namespace f4;


struct AxisEventImpl : public AxisEvent
{
	virtual bool initialize(int device, int axis, Float pos);
	
	virtual int getDevice() const       { return m_device; }
	virtual int getAxis() const         { return m_axis;   }
	virtual Float getPosition() const   { return m_position; }

private:
	int m_device;
	int m_axis;
	Float m_position;
	DECL_SERVICE;
};

IMPL_SERVICE(AxisEventImpl, AxisEvent)

//---------------------------------------------------------------------------

bool AxisEventImpl::initialize(int device, int axis, Float pos)
{
	m_device = device;
	m_axis   = axis;
	m_position = pos;
	return true;
}
