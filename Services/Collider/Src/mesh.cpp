/**
 * @file mesh.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: mesh.cpp,v 1.3 2002/06/12 23:05:16 jason379 Exp $
 */
#include <ode/ode.h>
#include "OPCODE/Opcode.h"

#include <Core/core.h>
#include <Collider/mesh.h>
#include <Math/math.h>
#include <Memory/memory.h>
using namespace f4;
using namespace Opcode;


struct MeshImpl : public BoundingMesh
{
	MeshImpl();
	virtual ~MeshImpl();

	virtual bool initialize(int numVerts, int numTris);
	virtual void setVertexPosition(int i, Float x, Float y, Float z);
	virtual void setVertexIndex(int i, int vertexIndex);
	virtual bool build();

	// BoundingShape interface
	virtual void* getShapeData() { return &m_geom; }
	virtual void  setMatrix(const Matrix44& m);

private:
	dGeomID       m_geom;
	OPCODE_Model* m_model;

	Vector3* m_vertices;
	int      m_numVertices;

	UInt32*  m_indices;
	int      m_numIndices;

public:
	static int  _sphereTest(dGeomID o1, dGeomID o2, int flags, dContactGeom* contact, int skip);
	static void _opcodeCallback(udword triangleIndex, VertexPointers& triangle, udword userdata);

	DECL_SERVICE;
};

IMPL_SERVICE(MeshImpl, BoundingMesh)



//---------------------------------------------------------------------------
// Local stuff
//---------------------------------------------------------------------------

// Add support for OPCODE shapes to ODE
static int _meshTypeID = 0;
static dColliderFn* _getColliderFnFn(int num);
static void _getAABBFn(dGeomID o, dReal aabb[6]);
static int  _sphereTest(dGeomID o1, dGeomID o2, int flags, dContactGeom* contact, int skip);

// Shortcuts to get/set the mesh pointer from the ODE object
#define GETMESH(o)    (*(MeshImpl**)dGeomGetClassData(o))
#define SETMESH(o,m)  (*(MeshImpl**)dGeomGetClassData(o)) = m

// OPCODE collision detectors
static AABBSphereCollider SC;
static Matrix4x4 identity;


//---------------------------------------------------------------------------

MeshImpl::MeshImpl()
: m_geom(0),
  m_vertices(0), m_numVertices(0),
  m_indices(0),  m_numIndices(0)
{
	// If this is the first instance of a bounding mesh...
	if (_meshTypeID == 0)
	{
		// Register this new shape type with ODE
		dGeomClass gc;
		gc.bytes = sizeof(OPCODE_Model*);
		gc.collider = _getColliderFnFn;
		gc.aabb = _getAABBFn;
		gc.aabb_test = 0;
		gc.dtor = 0;
		_meshTypeID = dCreateGeomClass(&gc);

		// Setup the OPCODE colliders
		SC.SetFirstContact(true);
		SC.SetTemporalCoherence(false);
		SC.SetCallbackObj(&_opcodeCallback);

		identity.Identity();
	}

	m_geom = dCreateGeom(_meshTypeID);
	SETMESH(m_geom, this);

	m_model = new OPCODE_Model;
}

//---------------------------------------------------------------------------

MeshImpl::~MeshImpl()
{
	delete m_model;

	if (m_vertices)  free(m_vertices);
	if (m_indices)   free(m_indices);

	dGeomDestroy(m_geom);
}

//---------------------------------------------------------------------------

bool MeshImpl::initialize(int numVerts, int numTris)
{
	if (m_vertices) delete m_vertices;
	if (m_indices)  delete m_indices;

	m_vertices = (Vector3*)malloc(sizeof(Vector3) * numVerts);
	m_numVertices = numVerts;

	m_indices  = (UInt32*)malloc(sizeof(UInt32) * numTris * 3);
	m_numIndices = numTris * 3;

	return true;
}

//---------------------------------------------------------------------------

void MeshImpl::setVertexPosition(int i, Float x, Float y, Float z)
{
	if (i < 0 || i >= m_numVertices) return;
	m_vertices[i] = Vector3(x, y, z);
}

//---------------------------------------------------------------------------

void MeshImpl::setVertexIndex(int i, int vertexIndex)
{
	if (i < 0 || i >= m_numIndices) return;
	m_indices[i] = vertexIndex;
}

//---------------------------------------------------------------------------

bool MeshImpl::build()
{
	// Create an OPCODE model from the (hopefully initialized!) triangle list
	OPCODECREATE oc;
	oc.NbTris    = m_numIndices / 3;
	oc.NbVerts   = m_numVertices;
	oc.Tris      = m_indices;
	oc.Verts     = (const Point*)(m_vertices);
	oc.Rules     = SPLIT_COMPLETE | SPLIT_SPLATTERPOINTS;
	oc.NoLeaf    = true;
	oc.Quantized = true;

	return m_model->Build(oc);
}

//---------------------------------------------------------------------------

int MeshImpl::_sphereTest(dGeomID o1, dGeomID o2, int flags, dContactGeom* contact, int skip)
{
	// Convert the abstract ODE objects to useful types
	MeshImpl* mesh = GETMESH(o1);

	int cl = dGeomGetClass(o2);

	const dReal* center = dGeomGetPosition(o2);
	dReal radius = dGeomSphereGetRadius(o2);

	Point pt(center[0], center[1], center[2]);
	Sphere sphere(pt, radius);

	// Collide the objects
	CollisionFaces faces;
	SC.SetDestination(&faces);

	SC.SetUserData((udword)mesh);
	SC.Collide(sphere, mesh->m_model, identity, identity);

	int n = faces.GetNbFaces();
	if (n > 0)
	{
		CollisionFace* f = faces.GetFaces();
		for (int i = 0; i < n; ++i)
		{
			// Locate the triangle that was hit
			UInt32* indices = &mesh->m_indices[f[i].mFaceID * 3];
			
			const Vector3& p1 = mesh->m_vertices[indices[0]];
			const Vector3& p2 = mesh->m_vertices[indices[1]];
			const Vector3& p3 = mesh->m_vertices[indices[2]];

			// Find the point of intersection from the barycentric coordinates
			Float mU = f[i].mU;
			Float mV = f[i].mV;
			Float mW = 1.0 - mU - mV;
			
			contact->pos[0] = (p1.x * mW) + (p2.x * mU) + (p3.x * mV);
			contact->pos[1] = (p1.y * mW) + (p2.y * mU) + (p3.y * mV);
			contact->pos[2] = (p1.z * mW) + (p2.z * mU) + (p3.z * mV);
			contact->pos[3] = 0;

			// Calculate the surface normal
			Vector3 normal = -(p2 - p1).cross(p3 - p1);
			normal.normalize();

			contact->normal[0] = normal.x;
			contact->normal[1] = normal.y;
			contact->normal[2] = normal.z;
			contact->normal[3] = 0;

			// Calculate the penetration depth
			contact->depth = radius - sqrtf(f[i].mDistance);

			// Fill in the contact bodies
			contact->g1 = o1;
			contact->g2 = o2;
			
			contact = (dContactGeom*)((UInt8*)contact) + skip;
		}

		return n;
	}

	return 0;
}

//---------------------------------------------------------------------------

void MeshImpl::_opcodeCallback(udword triangleIndex, VertexPointers& triangle, udword userdata)
{
	MeshImpl* mesh = (MeshImpl*)userdata;
	UInt32* indices = &mesh->m_indices[triangleIndex * 3];
	
	triangle.Vertex[0] = (const Point*)&mesh->m_vertices[indices[0]];
	triangle.Vertex[1] = (const Point*)&mesh->m_vertices[indices[1]];
	triangle.Vertex[2] = (const Point*)&mesh->m_vertices[indices[2]];
}

//---------------------------------------------------------------------------

void MeshImpl::setMatrix(const Matrix44& m)
{
}

//---------------------------------------------------------------------------

dColliderFn* _getColliderFnFn(int num)
{
	// Get the class IDs for the stuff I can collide with...ugly
	dGeomID id = dCreateSphere(0, 1);
	int sphereClass = dGeomGetClass(id);
	dGeomDestroy(id);

	if (num == sphereClass)
		return &MeshImpl::_sphereTest;

	return 0;
}

//---------------------------------------------------------------------------

// This function must be provided, but is never actually called.
void _getAABBFn(dGeomID o, dReal aabb[6])
{
	dInfiniteAABB(o, aabb);
}

