/**
 * @file render_visitor.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2002 by 379, Inc.
 * @version $Id: render_visitor.cpp,v 1.1 2002/06/19 01:39:59 jason379 Exp $
 */
#include <Core/core.h>
#include <Scene/render_visitor.h>
#include <Scene/entity.h>
#include <Scene/camera.h>
#include <Scene/pose.h>
#include <System/system.h>
#include <Math/math.h>
#include <vector>
#include <Memory/memory.h>
using namespace f4;


struct iRenderVisitor : public RenderVisitor
{
	iRenderVisitor();
	virtual ~iRenderVisitor() { }

	virtual void render(Entity* root);

	virtual void setPose(Pose* pose);
	virtual void setCamera(Camera* camera);

	virtual RenderService* getRenderService()   { return m_rs; }

	virtual void queue(Visual* visual);

	// Visitor interface
	virtual void visitEntity(Entity* ent);
	virtual void visitAttribute(Attribute* at);

private:
	Ref<RenderService> m_rs;     // services used to start/stop a frame
	Ref<SystemService> m_ss;

	Ref<Camera> m_camera;

	// As the entity tree is traversed, each Pose will push it's current world
	// matrix onto the state tree, which passes it into the render. See
	// pushState(), popState(), and setPose() below.
	typedef std::vector<const Matrix44*> MatrixStack;
	MatrixStack m_matrices;

	DECL_SERVICE;
};

IMPL_SERVICE(iRenderVisitor, RenderVisitor)

//---------------------------------------------------------------------------

iRenderVisitor::iRenderVisitor()
{
	m_rs = (RenderService*)Core->createA("Render.RenderService");
	m_ss = (SystemService*)Core->createA("System.SystemService");
}

//---------------------------------------------------------------------------

void iRenderVisitor::render(Entity* root)
{
	// Seed the matrix stack
	static Matrix44 identity = Matrix44::identity();

	m_matrices.clear();
	m_matrices.push_back(&identity);

	// Reset the renderer
	m_rs->start();

	// Apply the active camera, if one has been provided
	if (m_camera) m_camera->apply(this);

	// Run the render pass
	visitEntity(root);

	// Display the results
	m_rs->finish();
	m_ss->swapBuffers();
}


//---------------------------------------------------------------------------

void iRenderVisitor::setPose(Pose* pose)
{
	// Grab the world matrix from the Pose. Store it at the top of the
	// matrix stack, and make it current in the renderer as well.
	const Matrix44* matrix = &pose->getWorldMatrix();
	m_matrices.back() = matrix;
	m_rs->setWorldMatrix(*matrix);
}

//---------------------------------------------------------------------------

void iRenderVisitor::setCamera(Camera* camera)
{
	m_camera = camera;
	camera->apply(this);
}

//---------------------------------------------------------------------------

void iRenderVisitor::queue(Visual* visual)
{
	m_rs->queue(visual);
}

//---------------------------------------------------------------------------

void iRenderVisitor::visitEntity(Entity* ent)
{
	// Make a new copy of the current matrix at the top of the stack
	m_matrices.push_back(m_matrices.back());

	ent->visit(this);

	// Remove the top item from the matrix stack and activate the new
	// current matrix
	m_matrices.pop_back();
	m_rs->setWorldMatrix(*m_matrices.back());
}

//---------------------------------------------------------------------------

void iRenderVisitor::visitAttribute(Attribute* at)
{

	at->render(this);
}

