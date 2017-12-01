/**
 * @file filter.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: filter.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_IO_FILTER_H_)
#define _F4_IO_FILTER_H_

#include "raw_stream.h"
ENTER_NAMESPACE(f4);


/**
 * Perform translation on data as to passes to/from a stream.
 * @ingroup f4_io
 */
struct IOFilter : public RawStream
{

	/**
	 * Specify the stream where this filter gets/puts it's data.
	 */
	virtual void setTarget(RawStream* target) = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(IOFilter, setTarget, RETURNS_VOID)
	ARG_SERVICE(RawStream)
END_METHOD

EXPORTS(IOFilter)
	INTERFACE(RawStream)
	METHOD(IOFilter, setTarget)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
