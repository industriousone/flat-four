/**
 * @file event.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: event.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#if !defined(_F4_CORE_EVENT_H_)
#define _F4_CORE_EVENT_H_

ENTER_NAMESPACE(f4);


/**
 * An abstract event description.
 * @ingroup f4_core
 */
struct Event : public Service
{
	
	/**
	 * Get the time that the event was generated.
	 */
	virtual Double getTimestamp()
	{
		return m_timestamp;
	}


	/**
	 * Set the event timestamp.
	 */
	virtual void setTimestamp(Double ts)
	{
		m_timestamp = ts;
	}

private:
	Double m_timestamp;
};


// --- begin scripting interface ---

SCRIPT_METHOD(Event, getTimestamp, RETURNS_DOUBLE)
END_METHOD

SCRIPT_METHOD(Event, setTimestamp, RETURNS_VOID)
	ARG_DOUBLE
END_METHOD

EXPORTS(Event)
	RW_PROP_DOUBLE(Event, timestamp, getTimestamp, setTimestamp)
END_EXPORTS

// --- end scripting interface ---

EXIT_NAMESPACE;
#endif
