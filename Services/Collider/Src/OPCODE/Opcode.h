///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Main file for Opcode.dll.
 *	\file		Opcode.h
 *	\author		Pierre Terdiman
 *	\date		March, 20, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __OPCODE_H__
#define __OPCODE_H__

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compilation messages

// 379: not building a DLL here
//#if defined(OPCODEDISTRIB_EXPORTS)
//	#pragma message("Compiling OPCODE")
//#elif !defined(OPCODEDISTRIB_EXPORTS)
//	#pragma message("Using OPCODE")
//#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Preprocessor

// 379: not building a DLL here
#define OPCODE_API

//#ifdef OPCODEDISTRIB_EXPORTS
//	#define OPCODE_API __declspec(dllexport)
//#else
//	#define OPCODE_API __declspec(dllimport)
//#endif

#ifndef __ICECORE_H__

	// 379: no windows stuff in here, conflicts with the F4 symbols
//	#ifdef WIN32
//	#include <windows.h>
//	#include <windowsx.h>
//	#endif // WIN32

	// 379: redefine Win32 specific stuff
#if !defined(WIN32)
	#define __stdcall
	#define __forceinline inline
#endif

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <ctype.h>
	#include <assert.h>
// 379:	#include <shlobj.h>
// 379:	#include <mmsystem.h>
	#include <stdarg.h>
	#include <time.h>
	#include <float.h>

	#ifndef ASSERT
		#define	ASSERT	assert
	#endif

	#define	Log
	// 379: gcc doesn't like using zero as a function pointer
	//      was - #define SetIceError		false
	inline bool __SetIceError(const char* m, const char* c=0) { return false; }
	#define SetIceError   __SetIceError
	#define	EC_OUTOFMEMORY	"Out of memory"
	#define	Alignment
	#define ICECORE_API		OPCODE_API
	#define	override(baseclass)	virtual

	// IceCore includes
	#include "IceTypes.h"
	#include "IceFPU.h"
	#include "IceMemoryMacros.h"
	#include "IceContainer.h"
	#include "IceRandom.h"
#endif

#ifndef __ICEMATHS_H__
	// 379: fails on case-sensitive platforms - #include <Math.h>
	#include <math.h>

	#define ICEMATHS_API	OPCODE_API

	class HPoint;
	class Matrix3x3;
	class Matrix4x4;
	class Quat;
	class PRS;
	class PR;

	// IceMaths includes
	#include "IcePoint.h"
	#include "IceHPoint.h"
	#include "IceMatrix3x3.h"
	#include "IceMatrix4x4.h"
	#include "IceRay.h"
#endif


#ifndef __MESHMERIZER_H__

	#define MESHMERIZER_API	OPCODE_API

	// 379: gcc complains that this hasn't been defined, and it doesn't seem to
	//      be getting used anywhere, so...
#if 0
	enum CubeIndex;
#endif
	class Plane;
	class ProgressiveEigen;

	// Meshmerizer includes
	#include "IceTriangle.h"
	#include "IceAABB.h"
	#include "IceBoundingSphere.h"
#endif

	namespace Opcode
	{
		// Bulk-of-the-work
		#include "OPC_Settings.h"
		#include "OPC_Common.h"
		#include "OPC_TreeBuilders.h"
		#include "OPC_AABBTree.h"
		#include "OPC_OptimizedTree.h"
		#include "OPC_Model.h"
		#include "OPC_BVTCache.h"
		#include "OPC_Collider.h"
		#include "OPC_TreeCollider.h"
		#include "OPC_RayCollider.h"
		#include "OPC_SphereCollider.h"
	}

#endif // __OPCODE_H__
