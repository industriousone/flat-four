/**
 * @file vb.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: vb.h,v 1.3 2002/06/02 18:55:02 jason379 Exp $
 *
 * @todo Need to publish/document execute() methods on all render objects.
 *
 */
#if !defined(_F4_RENDER_VB_H_)
#define _F4_RENDER_VB_H_

#include <Math/math.h>
#include "geometry.h"
ENTER_NAMESPACE(f4);

struct VBufIterator;


/**
 * A list of vertices.
 * @ingroup f4_render
 */
struct VertexBuffer : public Geometry
{
	enum { POSITION = 0x01, NORMAL = 0x02, TEXCOORD = 0x04, COLOR = 0x08 };
	enum { TRIANGLES, QUADS };


	/**
	 * Allocate and prepare memory for a new vertex buffer.
	 *
	 * @param format One or more of these: @c NORMAL, @c TEXCOORD, @c COLOR3.
	 *               @c POSITION may be specified but is always included.
	 *
	 * @param count  The initial number of vertices in the buffer.
	 */
	virtual bool initialize(int format, int count) = 0;


	/**
	 * Set the type of primitive stored in the buffer, which is TRIANGLES by
	 * default.
	 *
	 * @param type  One of @c TRIANGLES, @c QUADS.
	 */
	virtual void setPrimitiveType(int type) = 0;


	/**
	 * Resize the buffer. If the new size is smaller than the old one the
	 * renderer may release the excess memory, but is not required to do so.
	 */
	virtual bool resize(int count) = 0;


	/**
	 * Retrieve the number of vertices in the buffer.
	 */
	virtual int size() const = 0;


	/**
	 * Set the position of a vertex.
	 */
	virtual bool setPosition(int i, Float x, Float y, Float z) = 0;


	/**
	 * Set the normal of a vertex.
	 */
	virtual bool setNormal(int i, Float x, Float y, Float z) = 0;


	/**
	 * Set the texture coordinates of a vertex.
	 */
	virtual bool setTexCoord(int i, Float u, Float v) = 0;


	/**
	 * Set the color at a vertex.
	 */
	virtual bool setColor(int i, Float r, Float g, Float b, Float a) = 0;


	/**
	 * Get the position of a vertex.
	 */
	virtual const Vector3 getPosition(int i) = 0;


	/**
	 * Get the texture coordinates of a vertex.
	 */
	virtual const Vector3 getTexCoord(int i) = 0;


	// --- internal interface ---
	virtual void execute(VBufIterator* vbi) = 0;
	// --- internal interface ---

};



// --- begin scripting interface ---

SCRIPT_METHOD(VertexBuffer, getPosition, RETURNS_VECTOR)
	ARG_INT
END_METHOD

SCRIPT_METHOD(VertexBuffer, getTexCoord, RETURNS_VECTOR)
	ARG_INT
END_METHOD

SCRIPT_METHOD(VertexBuffer, initialize, RETURNS_BOOL)
	ARG_INT, ARG_INT
END_METHOD

SCRIPT_METHOD(VertexBuffer, resize, RETURNS_BOOL)
	ARG_INT
END_METHOD

SCRIPT_METHOD(VertexBuffer, setColor, RETURNS_BOOL)
	ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(VertexBuffer, setNormal, RETURNS_BOOL)
	ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(VertexBuffer, setPosition, RETURNS_BOOL)
	ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(VertexBuffer, setPrimitiveType, RETURNS_VOID)
	ARG_INT
END_METHOD

SCRIPT_METHOD(VertexBuffer, setTexCoord, RETURNS_BOOL)
	ARG_INT, ARG_FLOAT, ARG_FLOAT
END_METHOD

SCRIPT_METHOD(VertexBuffer, size, RETURNS_INT)
END_METHOD

EXPORTS(VertexBuffer)
	INTERFACE(Geometry)
	RO_PROP_INT(VertexBuffer, size, size)
	METHOD(VertexBuffer, getPosition)
	METHOD(VertexBuffer, getTexCoord)
	METHOD(VertexBuffer, initialize)
	METHOD(VertexBuffer, resize)
	METHOD(VertexBuffer, setColor)
	METHOD(VertexBuffer, setNormal)
	METHOD(VertexBuffer, setPosition)
	METHOD(VertexBuffer, setTexCoord)
	CONST_INT(VertexBuffer, COLOR)
	CONST_INT(VertexBuffer, NORMAL)
	CONST_INT(VertexBuffer, POSITION)
	CONST_INT(VertexBuffer, QUADS)
	CONST_INT(VertexBuffer, TEXCOORD)
	CONST_INT(VertexBuffer, TRIANGLES)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
