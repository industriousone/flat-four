/**
 * @file reader_writer.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: reader_writer.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_IO_READER_WRITER_H_)
#define _F4_IO_READER_WRITER_H_

ENTER_NAMESPACE(f4);

struct InputStream;


/**
 * A generic interface for performing object level I/O.
 * @ingroup f4_data
 */
struct ReaderWriter : public Service
{

	/**
	 * Load an object from a stream. The type of service that is returned
	 * is implicitly defined by the type of the ReaderWriter that is called.
	 */
	virtual Service* read(InputStream* stream) = 0;


	/**
	 * Write an object to a stream.
	 */
	virtual bool write(Service* src, OutputStream* stream) = 0;

};


// --- begin scripting interface ---

SCRIPT_METHOD(ReaderWriter, read, RETURNS_SERVICE)
	ARG_SERVICE(InputStream)
END_METHOD

SCRIPT_METHOD(ReaderWriter, write, RETURNS_BOOL)
	ARG_SERVICE(Service), ARG_SERVICE(OutputStream)
END_METHOD

EXPORTS(ReaderWriter)
	METHOD(ReaderWriter, read)
	METHOD(ReaderWriter, write)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
