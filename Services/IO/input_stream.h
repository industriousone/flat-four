/**
 * @file input_stream.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: input_stream.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_IO_INPUT_STREAM_H_)
#define _F4_IO_INPUT_STREAM_H_

ENTER_NAMESPACE(f4);

struct Directory;
struct IOFilter;
struct RawStream;


/**
 * A read-only data stream.
 * @ingroup f4_io
 */
struct InputStream : public Service
{
	enum { LSB, MSB };
	enum { START, CURRENT, END };


	/**
	 * Prepare the stream for use. Called by the I/O service as part of the
	 * stream setup code.
	 */
	virtual bool initialize(RawStream* raw, Directory* dir, const Char* name) = 0;

	
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


	/**
	 * Returns true when reading reaches the end of the file.
	 */
	virtual bool eof() = 0;


	/**
	 * Set the byte ordering for higher level data objects. The default for
	 * new streams is @a STREAM_LSB.
	 *
	 * @param order @a STREAM_MSB or @a STREAM_LSB
	 */
	virtual void setByteOrder(int order) = 0;


	/**
	 * Read an 8-bit integer number.
	 */
	virtual UInt8 readInt8() = 0;


	/**
	 * Read a 16-bit integer number.
	 */
	virtual UInt16 readInt16() = 0;


	/**
	 * Read a 32-bit integer number.
	 */
	virtual UInt32 readInt32() = 0;


	/**
	 * Read a single-precision floating point number.
	 */
	virtual Float readFloat() = 0;


	/**
	 * Read a zero-terminated string.
	 */
	virtual std::string readString() = 0;


	/**
	 * Read a fixed-length string.
	 */
	virtual std::string readFixedString(int length) = 0;


	/**
	 * Read an array of 32-bit integers.
	 */
	virtual void readIntegers(UInt32* ints, int num) = 0;


	/**
	 * Read an array of single-precision floats.
	 */
	virtual void readFloats(Float* floats, int num) = 0;


	/**
	 * Read an array of Vector3.
	 */
	virtual void readVectors(Vector3* vectors, int num) = 0;


	/**
	 * Create an in-memory image of the file that can be accessed using the
	 * same InputStream interface.
	 *
	 * @param size  The number of bytes, starting at the current file position,
	 *              that should be buffered.
	 */
	virtual InputStream* createMemoryFile(unsigned size) = 0;


	/**
	 * Get the name of the file (or whatever) associated with this stream.
	 */
	virtual const Char* getSourceName() = 0;


	/**
	 * Get an interface to the directory containing this data source.
	 */
	virtual Directory* getDirectory() = 0;


	/**
	 * Apply a filter to the input data. Filters are applied in the reverse
	 * order that they are added to the stream.
	 */
	virtual void addFilter(IOFilter* filter) = 0;

};



// --- begin scripting interface ---

EXIT_NAMESPACE;
#include "directory.h"
ENTER_NAMESPACE(f4);

SCRIPT_METHOD(InputStream, addFilter, RETURNS_VOID)
	ARG_SERVICE(IOFilter)
END_METHOD

SCRIPT_METHOD(InputStream, createMemoryFile, RETURNS_SERVICE)
	ARG_INT
END_METHOD

SCRIPT_METHOD(InputStream, getDirectory, RETURNS_SERVICE)
END_METHOD

SCRIPT_METHOD(InputStream, getSourceName, RETURNS_STRING)
END_METHOD

SCRIPT_METHOD(InputStream, initialize, RETURNS_BOOL)
	ARG_SERVICE(RawStream), ARG_SERVICE(Directory), ARG_STRING
END_METHOD

SCRIPT_METHOD(InputStream, read, RETURNS_INT)
	ARG_PTR(void), ARG_INT
END_METHOD

SCRIPT_METHOD(InputStream, readFixedString, RETURNS_STRING)
	ARG_INT
END_METHOD

SCRIPT_METHOD(InputStream, readFloat, RETURNS_FLOAT)
END_METHOD

SCRIPT_METHOD(InputStream, readInt8, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(InputStream, readInt16, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(InputStream, readInt32, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(InputStream, readString, RETURNS_STRING)
END_METHOD

SCRIPT_METHOD(InputStream, seek, RETURNS_BOOL)
	ARG_INT, ARG_INT
END_METHOD

SCRIPT_METHOD(InputStream, setByteOrder, RETURNS_VOID)
	ARG_INT
END_METHOD

SCRIPT_METHOD(InputStream, tell, RETURNS_INT)
END_METHOD

EXPORTS(InputStream)
	METHOD(InputStream, createMemoryFile)
	METHOD(InputStream, getDirectory)
	METHOD(InputStream, getSourceName)
	METHOD(InputStream, initialize)
	METHOD(InputStream, read)
	METHOD(InputStream, readFixedString)
	METHOD(InputStream, readFloat)
	METHOD(InputStream, readInt8)
	METHOD(InputStream, readInt16)
	METHOD(InputStream, readInt32)
	METHOD(InputStream, readString)
	METHOD(InputStream, seek)
	METHOD(InputStream, setByteOrder)
	METHOD(InputStream, tell)
	CONST_INT(InputStream, LSB)
	CONST_INT(InputStream, MSB)
	CONST_INT(InputStream, START)
	CONST_INT(InputStream, CURRENT)
	CONST_INT(InputStream, END)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
