/**
 * @file click_event.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: click_event.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <System/click_event.h>
#include <Memory/memory.h>
using namespace f4;


struct ClickEventImpl : public ClickEvent
{
	ClickEventImpl();
	virtual ~ClickEventImpl() { }

	virtual bool initialize(int button, bool pressed, int x, int y);

	virtual int  getButton() const  { return m_button;  }
	virtual bool isPressed() const  { return m_pressed; }
	virtual int  getX() const       { return m_x;       }
	virtual int  getY() const       { return m_y;       }

private:
	int  m_button;
	bool m_pressed;
	int  m_x, m_y;
	DECL_SERVICE;
};

IMPL_SERVICE(ClickEventImpl, ClickEvent)

//---------------------------------------------------------------------------

ClickEventImpl::ClickEventImpl()
: m_button(0),
  m_pressed(false),
  m_x(0), m_y(0)
{
}

//---------------------------------------------------------------------------

bool ClickEventImpl::initialize(int button, bool pressed, int x, int y)
{
	m_button  = button;
	m_pressed = pressed;
	m_x       = x;
	m_y       = y;
	return true;
}
