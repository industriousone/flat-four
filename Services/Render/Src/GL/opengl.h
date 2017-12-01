/**
 * @file opengl.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: opengl.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */

#if defined(WIN32)
#define WINGDIAPI __declspec(dllimport)
#define APIENTRY  __stdcall
#define CALLBACK  __stdcall
typedef unsigned short wchar_t;
#endif

#include <GL/gl.h>
