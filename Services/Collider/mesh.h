/**
 * @file mesh.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: mesh.h,v 1.2 2002/06/02 18:55:02 jason379 Exp $
 */
#if !defined(_F4_COLLIDER_MESH_H_)
#define _F4_COLLIDER_MESH_H_

#include "shape.h"
ENTER_NAMESPACE(f4);


/**
 * A bounding mesh, composed of triangles. The mesh is built like an indexed
 * vertex buffer: provide a list of vertices, then specify triangles as
 * indices into the vertex list. This example makes a flat plane out of
 * two triangles:
 * @code
 * // C++
 * BoundingMesh* mesh = (BoundingMesh*)Core->createA("Collider.BoundingMesh");
 * mesh->initialize(4, 2);      // 4 vertices, 2 triangles
 * mesh->setVertexPosition(0, -100, 0, -100);
 * mesh->setVertexPosition(1, -100, 0,  100);
 * mesh->setVertexPosition(2,  100, 0,  100);
 * mesh->setVertexPosition(3,  100, 0, -100);
 * mesh->setVertexIndex(0, 0);
 * mesh->setVertexIndex(1, 1);
 * mesh->setVertexIndex(2, 2);
 * mesh->setVertexIndex(3, 0);
 * mesh->setVertexIndex(4, 2);
 * mesh->setVertexIndex(5, 3);
 * mesh->build();
 *
 * -- Lua
 * local mesh = Collider.BoundingMesh(4,2)
 * mesh:setVertexPosition(0, -100, 0, -100)
 * mesh:setVertexPosition(1, -100, 0,  100)
 * mesh:setVertexPosition(2,  100, 0,  100)
 * mesh:setVertexPosition(3,  100, 0, -100)
 * mesh:setVertexIndex(0, 0)
 * mesh:setVertexIndex(1, 1)
 * mesh:setVertexIndex(2, 2)
 * mesh:setVertexIndex(3, 0)
 * mesh:setVertexIndex(4, 2)
 * mesh:setVertexIndex(5, 3)
 * mesh:build()
 * @endcode
 *
 * @ingroup f4_collide
 */
struct BoundingMesh : public BoundingShape
{

	/**
	 * Specify the number of vertices and triangles in the mesh.
	 */
	virtual bool initialize(int numVerts, int numTris) = 0;


	/**
	 * Specify the position of a vertex.
	 */
	virtual void setVertexPosition(int i, Float x, Float y, Float z) = 0;


	/**
	 * Specify a vertex index in the triangle list.
	 */
	virtual void setVertexIndex(int i, int vertexIndex) = 0;


	/**
	 * Once all of the vertices and indices have been initialized, call this
	 * method to build the bounding box hierarchy.
	 */
	virtual bool build() = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(BoundingMesh, build, RETURNS_BOOL)
END_METHOD

SCRIPT_METHOD(BoundingMesh, initialize, RETURNS_BOOL)
	ARG_INT, ARG_INT
END_METHOD

SCRIPT_METHOD(BoundingMesh, setVertexIndex, RETURNS_VOID)
	ARG_INT, ARG_INT
END_METHOD

SCRIPT_METHOD(BoundingMesh, setVertexPosition, RETURNS_VOID)
	ARG_INT, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT
END_METHOD

EXPORTS(BoundingMesh)
	INTERFACE(BoundingShape)
	METHOD(BoundingMesh, build)
	METHOD(BoundingMesh, initialize)
	METHOD(BoundingMesh, setVertexIndex)
	METHOD(BoundingMesh, setVertexPosition)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
