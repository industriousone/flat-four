/**
 * @file motion_event.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: motion_event.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <System/motion_event.h>
#include <Memory/memory.h>
using namespace f4;


struct MotionEventImpl : public MotionEvent
{
	MotionEventImpl();
	virtual ~MotionEventImpl() { }

	virtual bool initialize(int x, int y, int buttons);

	virtual int  getX() const;
	virtual int  getY() const;
	virtual int  getRelX() const;
	virtual int  getRelY() const;
	virtual bool getButtonState(int button) const;

private:
	int m_x, m_y;
	int m_prevX, m_prevY;
	int m_buttons;
	DECL_SERVICE;
};

IMPL_SERVICE(MotionEventImpl, MotionEvent)

//---------------------------------------------------------------------------

MotionEventImpl::MotionEventImpl()
: m_x(0), m_y(0),
  m_prevX(0), m_prevY(0),
  m_buttons(0)
{
}

//---------------------------------------------------------------------------

bool MotionEventImpl::initialize(int x, int y, int buttons)
{
	m_prevX = m_x;
	m_prevY = m_y;
	m_x = x;
	m_y = y;
	m_buttons = buttons;
	return true;
}

//---------------------------------------------------------------------------

int MotionEventImpl::getX() const
{
	return m_x;
}

//---------------------------------------------------------------------------

int MotionEventImpl::getY() const
{
	return m_y;
}

//---------------------------------------------------------------------------

int MotionEventImpl::getRelX() const
{
	return (m_x - m_prevX);
}

//---------------------------------------------------------------------------

int MotionEventImpl::getRelY() const
{
	return (m_y - m_prevY);
}

//---------------------------------------------------------------------------

bool MotionEventImpl::getButtonState(int button) const
{
	if (button < 0 || button > 30) return false;
	return (m_buttons & (1 << button)) != 0;
}
