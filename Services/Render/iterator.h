/**
 * @file iterator.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: iterator.h,v 1.3 2002/06/12 23:05:16 jason379 Exp $
 */
#if !defined(_F4_RENDER_ITERATOR_H_)
#define _F4_RENDER_ITERATOR_H_

#include "vb.h"
ENTER_NAMESPACE(f4);


/**
 * A list of indices into a vertex buffer.
 * @ingroup f4_render
 */
struct VBufIterator : public Geometry
{
	enum { TRIANGLES, QUADS };

	/**
	 * Allocate memory for the list of indices.
	 *
	 * @type vb    The list of vertices to be indexed.
	 * @type size  The number of indices to store in the iterator.
	 */
	virtual bool initialize(VertexBuffer* vb, int size) = 0;


	/**
	 * Set the primitive type. The default is @c TRIANGLES.
	 *
	 * @param type   One of @c TRIANGLES or @c QUADS.
	 */
	virtual void setPrimitiveType(int type) = 0;


	/**
	 * Set a vertex index at a position in the buffer.
	 */
	virtual void set(int i, int vertexIndex) = 0;


	/**
	 * Get the number of indices stored in the buffer.
	 */
	virtual int size() const = 0;


	/**
	 * Get the type of primitives contained in the buffer.
	 */
	virtual int getPrimitiveType() const = 0;


	/**
	 * Retrieve a pointer to the index buffer. Be careful with it!
	 */
	virtual UInt32* getData() = 0;


	/**
	 * Retrieve a reference to the associated vertex buffer.
	 */
	virtual VertexBuffer* getVertexBuffer() = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(VBufIterator, getPrimitiveType, RETURNS_INT)
END_METHOD

SCRIPT_METHOD(VBufIterator, getVertexBuffer, RETURNS_SERVICE)
END_METHOD

SCRIPT_METHOD(VBufIterator, initialize, RETURNS_BOOL)
	ARG_SERVICE(VertexBuffer), ARG_INT
END_METHOD

SCRIPT_METHOD(VBufIterator, set, RETURNS_VOID)
	ARG_INT, ARG_INT
END_METHOD

SCRIPT_METHOD(VBufIterator, setPrimitiveType, RETURNS_VOID)
	ARG_INT
END_METHOD

EXPORTS(VBufIterator)
	INTERFACE(Geometry)
	RW_PROP_INT(VBufIterator, primitiveType, getPrimitiveType, setPrimitiveType)
	METHOD(VBufIterator, getVertexBuffer)
	METHOD(VBufIterator, initialize)
	METHOD(VBufIterator, set)
	CONST_INT(VBufIterator, TRIANGLES)
	CONST_INT(VBufIterator, QUADS)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
