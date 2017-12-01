/**
 * @file display_event.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: display_event.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
#include <System/display_event.h>
#include <Memory/memory.h>
using namespace f4;


struct DisplayEventImpl : public DisplayEvent
{
	virtual bool initialize(int width, int height);
	virtual int  getWidth() const;
	virtual int  getHeight() const;

private:
	int m_width, m_height;
	DECL_SERVICE;
};

IMPL_SERVICE(DisplayEventImpl, DisplayEvent)

//---------------------------------------------------------------------------

bool DisplayEventImpl::initialize(int width, int height)
{
	m_width = width;
	m_height = height;
	return true;
}

//---------------------------------------------------------------------------

int DisplayEventImpl::getWidth() const
{
	return m_width;
}

//---------------------------------------------------------------------------

int DisplayEventImpl::getHeight() const
{
	return m_height;
}
