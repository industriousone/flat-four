/**
 * @file exports.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: exports.cpp,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#include <Core/core.h>
using namespace f4;

DECLARE_SERVICE(RenderService)
DECLARE_SERVICE(Shader)
DECLARE_SERVICE(Sprite)
DECLARE_SERVICE(Texture)
DECLARE_SERVICE(VBufIterator)
DECLARE_SERVICE(VertexBuffer)
DECLARE_SERVICE(Visual)

PACKING_LIST
	SERVICE(RenderService)
	SERVICE(Shader)
	SERVICE(Sprite)
	SERVICE(Texture)
	SERVICE(VBufIterator)
	SERVICE(VertexBuffer)
	SERVICE(Visual)
END_PACKING_LIST
