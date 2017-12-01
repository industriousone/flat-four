/**
 * @file render.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: render.cpp,v 1.3 2002/05/07 00:55:51 jason379 Exp $
 *
 * @todo Dynamically load OpenGL, display message if it is not installed.
 *       Perhaps provide an option to specify a GL lib name.
 *
 */
#include "opengl.h"

#include <vector>

#include <Core/core.h>
#include <Render/render.h>
#include <Render/visual.h>
#include <System/display_event.h>
#include <Math/math.h>
#include <Memory/memory.h>
using namespace f4;

DECLARE_SERVICE(Texture)


struct RenderImpl : public RenderService
{
	RenderImpl();
	virtual ~RenderImpl();

	virtual void start();
	virtual void finish();

	virtual void setProjectionMatrix(const Matrix44& m);
	virtual void setProjection2D(Float left, Float top, Float right, Float bottom);
	virtual void setProjection3D(Float fov, Float aspect, Float nearClip, Float farClip);

	virtual void setViewMatrix(const Matrix44& m);
	virtual void setWorldMatrix(const Matrix44& m);

	virtual const Matrix44& getViewMatrix() const;
	virtual const Matrix44& getWorldMatrix() const;

	virtual void queue(Visual* v);

	virtual void setBackgroundColor(Float r, Float g, Float b);

	virtual Texture* grabScreen(int left, int top, int right, int bottom) const;

	// EventListener interface
	virtual bool receive(Event* e);

private:
	int m_viewWidth, m_viewHeight;
	Float m_bgColor[3];

	// Rendering queue
	Matrix44 m_viewMatrix;
	const Matrix44* m_worldMatrix;

	struct QueuedVisual
	{
		Visual* visual;
		const Matrix44* matrix;
	};

	typedef std::vector<QueuedVisual> VisualQueue;
	VisualQueue m_vizQueue;

	int  m_finishCount;
	void _flush();

	DECL_SERVICE;
};

IMPL_SINGLETON_SERVICE(RenderImpl, RenderService)


//---------------------------------------------------------------------------
// Message text
//---------------------------------------------------------------------------

static const Char* START_MSG = "Initializing OpenGL Rendering service\n";
static const Char* STOP_MSG  = "Closing OpenGL Render service\n";



//---------------------------------------------------------------------------

RenderImpl::RenderImpl()
: m_viewWidth(0),
  m_viewHeight(0),
  m_worldMatrix(0),
  m_finishCount(0)
{
	Core->print(START_MSG);
	Core->addEventListener(this);

	m_bgColor[0] = 0;
	m_bgColor[1] = 0;
	m_bgColor[2] = 0;
}

//---------------------------------------------------------------------------

RenderImpl::~RenderImpl()
{
	Core->print(STOP_MSG);
	Core->removeEventListener(this);
}

//---------------------------------------------------------------------------

void RenderImpl::start()
{
	glViewport(0, 0, m_viewWidth, m_viewHeight);
	glClearColor(m_bgColor[0], m_bgColor[1], m_bgColor[2], 1);

	// Prepare the frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1, 1, 1, 1);

	// Start with empty view and world matrices
	m_viewMatrix.setIdentity();
	m_worldMatrix = 0;
}

//---------------------------------------------------------------------------

void RenderImpl::finish()
{
	_flush();

	// Prevent the driver from buffering up too many instructions, resulting
	// display lag. The Linux GL/DRI drivers are particularly bad about this.
	// This flushes the buffers every other time finish() is called.
	if (++m_finishCount & 0x01)
		glFinish();
}

//---------------------------------------------------------------------------

void RenderImpl::setProjectionMatrix(const Matrix44& m)
{
	_flush();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//---------------------------------------------------------------------------

void RenderImpl::setProjection2D(Float left, Float top, Float right, Float bottom)
{
	_flush();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//---------------------------------------------------------------------------

void RenderImpl::setProjection3D(Float fov, Float aspect, Float nearClip, Float farClip)
{
	_flush();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	Float yMax = nearClip * tan(fov * PI / 360.0);
	Float yMin = -yMax;
	Float xMin = yMin * aspect;
	Float xMax = yMax * aspect;

	glFrustum(xMin, xMax, yMin, yMax, nearClip, farClip);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//---------------------------------------------------------------------------

void RenderImpl::setViewMatrix(const Matrix44& m)
{
	_flush();
	m_viewMatrix = m;
}

//---------------------------------------------------------------------------

void RenderImpl::setWorldMatrix(const Matrix44& m)
{
	m_worldMatrix = &m;
}

//---------------------------------------------------------------------------

const Matrix44& RenderImpl::getViewMatrix() const
{
	return m_viewMatrix;
}

//---------------------------------------------------------------------------

const Matrix44& RenderImpl::getWorldMatrix() const
{
	return *m_worldMatrix;
}

//---------------------------------------------------------------------------

void RenderImpl::queue(Visual* v)
{
	if (!v) return;

	int i = m_vizQueue.size();
	m_vizQueue.resize(i + 1);
	m_vizQueue[i].visual = v;
	m_vizQueue[i].matrix = m_worldMatrix;
}

//---------------------------------------------------------------------------

void RenderImpl::setBackgroundColor(Float r, Float g, Float b)
{
	m_bgColor[0] = r;
	m_bgColor[1] = g;
	m_bgColor[2] = b;
}

//---------------------------------------------------------------------------

Texture* RenderImpl::grabScreen(int left, int top, int right, int bottom) const
{
	if (m_viewWidth == 0 || m_viewHeight == 0) return 0;
	
	int width = right - left;
	int height = bottom - top;

	void* pixels = malloc(width * height * 3);
	glReadBuffer(GL_FRONT);
	glReadPixels(left, top, right, bottom, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	Texture* t = CREATEA_LOCAL(Texture);
	t->initialize(width, height, 3, 1, pixels);
	return t;
}

//---------------------------------------------------------------------------

bool RenderImpl::receive(Event* e)
{
	if (e->isA("DisplayEvent"))
	{
		DisplayEvent* de = static_cast<DisplayEvent*>(e);
		m_viewWidth  = de->getWidth();
		m_viewHeight = de->getHeight();
	}

	return false;
}

//---------------------------------------------------------------------------

/*
 * Empty the geometry queues and clear all cached data. Note that this will
 * change completely when I implement the state tree.
 */
void RenderImpl::_flush()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m_viewMatrix);

	m_worldMatrix = 0;

	VisualQueue::iterator i;
	for (i = m_vizQueue.begin(); i != m_vizQueue.end(); ++i)
	{
		const QueuedVisual& qv = (*i);

		if (qv.matrix != m_worldMatrix)
		{
			m_worldMatrix = qv.matrix;
			glLoadMatrixf((*m_worldMatrix) * m_viewMatrix);
		}

		qv.visual->execute();
	}

	m_vizQueue.clear();
	m_worldMatrix = 0;
}

