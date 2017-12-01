/**
 * @file output_stream.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: output_stream.h,v 1.3 2002/04/25 01:17:36 jason379 Exp $
 */
#if !defined(_F4_IO_OUTPUT_STREAM_H_)
#define _F4_IO_OUTPUT_STREAM_H_

ENTER_NAMESPACE(f4);

struct Directory;
struct RawStream;


/**
 * A write-only data stream.
 * @ingroup f4_io
 */
struct OutputStream : public Service
{
	enum { LSB, MSB };

	/**
	 * Prepare the stream for use. Called by the I/O service as part of the
	 * stream setup code.
	 */
	virtual bool initialize(RawStream* raw, Directory* dir, const Char* name) = 0;


	/**
	 * Write raw bytes to the stream.
	 *
	 * @param buffer A buffer to hold the data to be written.
	 * @param num    The number of bytes to write to the stream.
	 *
	 * @return The number of bytes actually written.
	 */
	virtual unsigned write(void* buffer, unsigned num) = 0;

	
	/**
	 * Flush any buffered output to the stream.
	 */
	virtual bool flush() = 0;


	/**
	 * Set the byte ordering for higher level data objects. The default for
	 * new streams is @a STREAM_LSB.
	 *
	 * @param order @a STREAM_MSB or @a STREAM_LSB
	 */
	virtual void setByteOrder(int order) = 0;


	/**
	 * Write an 8-bit integer number.
	 */
	virtual bool writeInt8(UInt8 i) = 0;


	/**
	 * Write a 16-bit integer number.
	 */
	virtual bool writeInt16(UInt16 i) = 0;


	/**
	 * Read a 32-bit integer number.
	 */
	virtual bool writeInt32(UInt32 i) = 0;


	/**
	 * Read a single-precision floating point number.
	 */
	virtual bool writeFloat(Float f) = 0;


	/**
	 * Write a string to the stream.
	 * @par Scripted as:
	 *      <I>bool writeString(string)</I>
	 */
	virtual bool writeString(const Char* s) = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(OutputStream, flush, RETURNS_BOOL)
END_METHOD

SCRIPT_METHOD(OutputStream, setByteOrder, RETURNS_VOID)
	ARG_INT
END_METHOD

SCRIPT_METHOD(OutputStream, write, RETURNS_INT)
	ARG_PTR(void), ARG_INT
END_METHOD

SCRIPT_METHOD(OutputStream, writeFloat, RETURNS_BOOL)
	ARG_FLOAT
END_METHOD

SCRIPT_METHOD(OutputStream, writeInt8, RETURNS_BOOL)
	ARG_INT
END_METHOD

SCRIPT_METHOD(OutputStream, writeInt16, RETURNS_BOOL)
	ARG_INT
END_METHOD

SCRIPT_METHOD(OutputStream, writeInt32, RETURNS_BOOL)
	ARG_INT
END_METHOD

SCRIPT_METHOD(OutputStream, writeString, RETURNS_BOOL)
	ARG_STRING
END_METHOD

EXPORTS(OutputStream)
	METHOD(OutputStream, flush)
	METHOD(OutputStream, setByteOrder)
	METHOD(OutputStream, write)
	METHOD(OutputStream, writeFloat)
	METHOD(OutputStream, writeInt8)
	METHOD(OutputStream, writeInt16)
	METHOD(OutputStream, writeInt32)
	METHOD(OutputStream, writeString)
	CONST_INT(OutputStream, LSB)
	CONST_INT(OutputStream, MSB)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
