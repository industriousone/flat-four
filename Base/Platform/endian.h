/**
 * @file endian.h
 *
 * Routines to convert between high- and low-endian formats.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: endian.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#if !defined(_F4_PLATFORM_ENDIAN_H_)
#define _F4_PLATFORM_ENDIAN_H_

ENTER_NAMESPACE(f4);

/**
 * @defgroup f4_platform_endian Endian Conversion
 * @ingroup f4_platform
 * @{
 */


//---------------------------------------------------------------------------
// Low-endian routines
//---------------------------------------------------------------------------
#if defined(i386) || defined(WIN32)

#define F4_SYSTEM_LITTLE_ENDIAN 1


/** Convert between native and LSB representations of 16 bit integers. */
inline UInt16 convertLSB(UInt16 x)	{ return x; }

/** Convert between native and LSB representations of 32 bit integers. */
inline UInt32 convertLSB(UInt32 x)	{ return x; }

/** Convert between native and LSB representations of 32 bit floating point values. */
inline Float  convertLSB(Float  x)	{ return x; }

/** Convert between native and MSB representations of 16 bit integers. */
inline UInt16 convertMSB(UInt16 x)	{ return ((x<<8)|(x>>8)); }

/** Convert between native and MSB representations of 32 bit integers. */
inline UInt32 convertMSB(UInt32 x)	{ return ((x<<24)|((x<<8)&0xFF0000)|((x>>8)&0xFF00)|(x>>24)); }

/** Convert between native and MSB representations of 32 bit floating point values. */
inline Float  convertMSB(Float  fx)	
{	
	UInt32 x = *((UInt32*)&fx);
	x = ((x<<24)|((x<<8)&0xFF0000)|((x>>8)&0xFF00)|(x>>24)); 
	return *((Float*)&x);
}



//---------------------------------------------------------------------------
// High-endian routines
//---------------------------------------------------------------------------
#else

#define F4_SYSTEM_BIG_ENDIAN 1


/** Convert between native and LSB representations of 16 bit integers. */
inline UInt16 convertLSB(UInt16 x)	{ return ((x<<8)|(x>>8)); }

/** Convert between native and LSB representations of 32 bit integers. */
inline UInt32 convertLSB(UInt32 x)	{ return ((x<<24)|((x<<8)&0xFF0000)|((x>>8)&0xFF00)|(x>>24)); }

/** Convert between native and LSB representations of 32 bit floating point values. */
inline Float  convertLSB(Float  x)	
{ 
	UInt32 x = *((UInt32*)&fx);
	x = ((x<<24)|((x<<8)&0xFF0000)|((x>>8)&0xFF00)|(x>>24)); 
	return *((Float*)&x);
}

/** Convert between native and MSB representations of 16 bit integers. */
inline UInt16 convertMSB(UInt16 x)	{ return x; }

/** Convert between native and MSB representations of 32 bit integers. */
inline UInt32 convertMSB(UInt32 x)	{ return x; }

/** Convert between native and MSB representations of 32 bit floating point values. */
inline Float  convertMSB(Float  x)	{ return x; }



#endif

/** @} */

EXIT_NAMESPACE;
#endif
