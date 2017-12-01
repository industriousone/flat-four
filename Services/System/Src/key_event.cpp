/**
 * @file key_event.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: key_event.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <System/key_event.h>
#include <Memory/memory.h>
using namespace f4;

extern const Char* _keyNames[];

struct KeyEventImpl : public KeyEvent
{
	virtual bool   initialize(int code, bool pressed, bool repeat);

	virtual int getCode() const                 { return m_code;            }
	virtual const Char* getDescription() const  { return _keyNames[m_code]; }
	virtual bool isPressed() const              { return m_pressed;         }
	virtual bool isRepeat() const               { return m_repeat;          }

private:
	int  m_code;
	bool m_pressed;
	bool m_repeat;
	DECL_SERVICE;
};

IMPL_SERVICE(KeyEventImpl, KeyEvent)

//---------------------------------------------------------------------------

bool KeyEventImpl::initialize(int code, bool pressed, bool repeat)
{
	m_code = code;
	m_pressed = pressed;
	m_repeat = repeat;
	return true;
}
