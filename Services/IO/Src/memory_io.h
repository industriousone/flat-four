/**
 * @file memory_io.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: memory_io.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_IO_MEMORY_IO_H_)
#define F4_IO_MEMORY_IO_H_

ENTER_NAMESPACE(f4);


struct MemoryStream : public RawStream
{
	MemoryStream();
	virtual ~MemoryStream();

	virtual bool     initialize(void* buffer, unsigned size);

	virtual bool     eof();
	virtual unsigned read (void* buffer, unsigned num);
	virtual unsigned write(void* buffer, unsigned num);
	virtual bool     flush();
	virtual bool     seek(int offset, int origin);
	virtual unsigned tell();

private:
	UInt8*   m_buffer;
	unsigned m_size;
	unsigned m_current;
	DECL_SERVICE;
};


EXIT_NAMESPACE;
#endif
