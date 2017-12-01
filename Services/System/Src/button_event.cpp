/**
 * @file button_event.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: button_event.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <System/button_event.h>
#include <Memory/memory.h>
using namespace f4;


struct ButtonEventImpl : public ButtonEvent
{
	virtual bool initialize(int device, int button, bool pressed);

	virtual int  getDevice() const  { return m_device;  }
	virtual int  getButton() const  { return m_button;  }
	virtual bool isPressed() const  { return m_pressed; }

private:
	int  m_device;
	int  m_button;
	bool m_pressed;
	DECL_SERVICE;
};

IMPL_SERVICE(ButtonEventImpl, ButtonEvent)

//---------------------------------------------------------------------------

bool ButtonEventImpl::initialize(int device, int button, bool pressed)
{
	m_device  = device;
	m_button  = button;
	m_pressed = pressed;
	return true;
}
