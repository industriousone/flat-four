/**
 * @file curve.cxx
 *
 * A 2D curve.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id$
 */
#include <Core/core.h>
#include <StdLib/curve.h>
#include <vector>
#include <Memory/memory.h>
using namespace f4;


struct iCurve : public Curve
{
	~iCurve() { }

	void  addPoint(Float x, Float y);
	Float getValueAt(Float x);

private:
	struct Point
	{
		Float x, y;
	};

	typedef std::vector<Point> Points;
	Points m_points;

	DECL_SERVICE;
};

IMPL_SERVICE(iCurve, Curve)

//---------------------------------------------------------------------------

void iCurve::addPoint(Float x, Float y)
{
	Point pt;
	pt.x = x;
	pt.y = y;

	// Insert the point, keep the list sorted by x coordinate
	Points::iterator it;
	for (it = m_points.begin(); it != m_points.end(); ++it)
	{
		if ((*it).x > x)
		{
			m_points.insert(it, pt);
			return;
		}
	}

	m_points.push_back(pt);
}

//---------------------------------------------------------------------------

Float iCurve::getValueAt(Float x)
{
	for (int i = 0; i < (int)m_points.size(); ++i)
	{
		if (m_points[i].x > x)
		{
			if (i == 0)
				return 0;

			Float x1 = m_points[i - 1].x;
			Float y1 = m_points[i - 1].y;
			Float x2 = m_points[i].x;
			Float y2 = m_points[i].y;

			return (x - x1) / (x2 - x1) * (y2 - y1) + y1;
		}
	}

	return 0;
}

