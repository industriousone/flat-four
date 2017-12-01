/**
 * @file raw_stream.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: raw_stream.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_IO_RAW_STREAM_H_)
#define _F4_IO_RAW_STREAM_H_

ENTER_NAMESPACE(f4);


/**
 * A "raw stream" provides a generic interface to a particular type of I/O.
 * For example, reading a file from a disk file is one kind of I/O, while 
 * reading a file contained in a .zip archive is another. The raw stream
 * provides the ability to translate between the stored data format and
 * raw bytes, and is used by the InputStream service to get data in and out.
 * @ingroup f4_io
 */
struct RawStream : public Service
{

	/**
	 * Returns true when at the end of the stream.
	 */
	virtual bool eof() = 0;


	/**
	 * Read raw bytes from the stream.
	 *
	 * @param buffer A buffer to hold the data as it as read.
	 * @param num    The number of bytes to read into the buffer.
	 *
	 * @return The number of bytes actually read.
	 */
	virtual unsigned read(void* buffer, unsigned num) = 0;


	/**
	 * Write raw bytes into a stream.
	 *
	 * @param buffer A buffer holding the bytes to be written.
	 * @param num    The number of bytes to be written.
	 *
	 * @return The number of bytes actually written. Will be zero if the 
	 *         stream does not support writing.
	 */
	virtual unsigned write(void* buffer, unsigned num) = 0;


	/**
	 * Flush any buffered output to the stream.
	 */
	virtual bool flush() = 0;


	/**
	 * Move the read/write position.
	 *
	 * @param offset The desired new offset from @a origin.
	 * @param origin One of the constants @a FILE_START, @a FILE_CURRENT, or 
	 *               @a FILE_END.
	 * 
	 * @return True if successful, false on error.
	 */
	virtual bool seek(int offset, int origin) = 0;


	/**
	 * Return the current read/write position, relative to the start of the file.
	 */
	virtual unsigned tell() = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(RawStream, eof, RETURNS_BOOL)
END_METHOD

SCRIPT_METHOD(RawStream, flush, RETURNS_BOOL)
END_METHOD

SCRIPT_METHOD(RawStream, read, RETURNS_INT)
	ARG_PTR(void), ARG_INT
END_METHOD

SCRIPT_METHOD(RawStream, write, RETURNS_INT)
	ARG_PTR(void), ARG_INT
END_METHOD

SCRIPT_METHOD(RawStream, seek, RETURNS_BOOL)
	ARG_INT, ARG_INT
END_METHOD

SCRIPT_METHOD(RawStream, tell, RETURNS_INT)
END_METHOD

EXPORTS(RawStream)
	METHOD(RawStream, eof)
	METHOD(RawStream, flush)
	METHOD(RawStream, read)
	METHOD(RawStream, write)
	METHOD(RawStream, seek)
	METHOD(RawStream, tell)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
