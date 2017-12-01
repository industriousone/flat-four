/**
 * @file memory.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @author  ...with help from Paul Nettle (http://www.flipcode.com/askmid/)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @date    Portions copyright (C) 2000 by Paul Nettle
 * @version $Id: memory.h,v 1.3 2002/06/12 23:05:15 jason379 Exp $
 *
 * @todo Look at Doug Lea's version: gee.cs.oswego.edu/dl/html/malloc.html
 *
 */
#if !defined(_F4_MEMORY_H_)
#define _F4_MEMORY_H_

#if !defined(MEMORY_API)
#define MEMORY_API F4_IMPORT
#endif

#if defined(__GNUC__)
#define __need_size_t
#include <stddef.h>
#endif

ENTER_NAMESPACE(f4);



/**
 * Specify a print function. The memory manager will call this function
 * whenever it has a message to display.
 * @ingroup f4_memory
 */
MEMORY_API void setMemoryPrintFunc(void (*func)(const char*));


/**
 * Display a report of memory statistics and current allocations (debug builds
 * only). Call setMemoryPrintFunc() before using this function. This function
 * will be called automatically when the program exits.
 * @ingroup f4_memory
 */
MEMORY_API void printMemoryReport();


/**
 * Break program execution when a particular allocation is made.
 * @ingroup f4_memory
 */
MEMORY_API void breakOnAllocation(unsigned num);


/**
 * Break program execution when an allocation is released.
 * @ingroup f4_memory
 */
MEMORY_API void breakOnDealloc(void* ptr);


/**
 * Break program execution when an allocation is reallocated.
 * @ingroup f4_memory
 */
MEMORY_API void breakOnRealloc(void* ptr);



//---------------------------------------------------------------------------
// Internal interface - don't use these directly
//---------------------------------------------------------------------------

enum AllocationTypes {
	MEMMGR_NEW,
	MEMMGR_NEW_ARRAY,
	MEMMGR_MALLOC,
	MEMMGR_CALLOC,
	MEMMGR_REALLOC,
	MEMMGR_DELETE,
	MEMMGR_DELETE_ARRAY,
	MEMMGR_FREE
};


MEMORY_API void* allocateMemory(const char* src, unsigned line, size_t size, int caller);
MEMORY_API void* reallocateMemory(const char* src, unsigned line, void* ptr, size_t size);
MEMORY_API void  releaseMemory(const char* src, unsigned line, void* ptr, int caller);
MEMORY_API void  cacheAllocatorInfo(const char* src, unsigned line);


EXIT_NAMESPACE;



//---------------------------------------------------------------------------
// Replacement memory operators
//---------------------------------------------------------------------------

inline void* operator new(size_t size)
{
	return f4::allocateMemory(0, 0, size, f4::MEMMGR_NEW);
}

inline void* operator new[](size_t size)
{
	return f4::allocateMemory(0, 0, size, f4::MEMMGR_NEW_ARRAY);
}

inline void* operator new(size_t size, const char* src, int line)
{
	return f4::allocateMemory(src, line, size, f4::MEMMGR_NEW);
}

inline void* operator new[](size_t size, const char* src, int line)
{
	return f4::allocateMemory(src, line, size, f4::MEMMGR_NEW_ARRAY);
}

inline void operator delete(void* ptr)
{
	f4::releaseMemory(0, 0, ptr, f4::MEMMGR_DELETE);
}

inline void operator delete[](void* ptr)
{
	f4::releaseMemory(0, 0, ptr, f4::MEMMGR_DELETE_ARRAY);
}


#endif // !defined(_F4_MEMORY_H_)


//---------------------------------------------------------------------------
// Operator macro cleverness thanks to Paul Nettle
//---------------------------------------------------------------------------

#undef new
#undef delete
#undef malloc
#undef calloc
#undef realloc
#undef free

#if defined(DEBUG)
#define new              (f4::cacheAllocatorInfo(__FILE__,__LINE__),false) ? 0 : new
#define malloc(size)      f4::allocateMemory(__FILE__,__LINE__,size,MEMMGR_MALLOC)
#define calloc(size)      f4::allocateMemory(__FILE__,__LINE__,size,MEMMGR_CALLOC)
#define realloc(ptr,size) f4::reallocateMemory(__FILE__,__LINE__,ptr,size)
#define free(ptr)         f4::releaseMemory(__FILE__,__LINE__,ptr,MEMMGR_FREE)
#else
#define malloc(size)      f4::allocateMemory(0,0,size,MEMMGR_MALLOC)
#define calloc(size)      f4::allocateMemory(0,0,size,MEMMGR_CALLOC)
#define realloc(ptr,size) f4::reallocateMemory(0,0,ptr,size)
#define free(ptr)         f4::releaseMemory(0,0,ptr,MEMMGR_FREE)
#endif





