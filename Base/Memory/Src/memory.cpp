/**
 * @file memory.cpp
 *
 * @author  Jason Perkins (jason@379.com)
 * @author  ...with help from Paul Nettle (http://www.flipcode.com/askmid/)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: memory.cpp,v 1.3 2002/05/22 01:41:07 jason379 Exp $
 *
 * @todo Adds guard bands before and after each allocation, check for damage.
 *
 * @todo Check for allocator/deallocator mismatches.
 *
 * @todo More breakpoint functions.
 *
 * @todo Show total memory used (pooled + normal alloc)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <Platform/platform.h>
#include <Memory/memory.h>
#include <Memory/undef.h>
using namespace f4;



//---------------------------------------------------------------------------
// Replacement for the Win32 assert() which brings up a dialog box (which
// allocates memory, which I shouldn't be doing when memory runs out)
//---------------------------------------------------------------------------

#if defined(WIN32)
	#if defined(DEBUG)
	#define _assert(x)    if (!(x)) __asm { int 3 }
	#else
	#define _assert(x)    { }
	#endif
#else
	#include <assert.h>
	#define _assert      assert
#endif



//---------------------------------------------------------------------------
// Data and definitions for memory pooling
//---------------------------------------------------------------------------

static const int GRANULARITY   = sizeof(void*);  
static const int MAX_POOLED    = 1024*4;  // must be multiple of GRANULARITY
static const int PAGE_SIZE     = 1024*16; // must be multiple of MAX_POOLED

static const int INIT_TBL_SIZE = 8192;    // initial size of page table
static const int GROW_TBL_BY   = 1024;    // number to add when table is full

// The pool list - each entry points to the next free block in the pool.
static const int NUM_POOLS = (MAX_POOLED / GRANULARITY);
static void* _pools[NUM_POOLS];

// A "page" is a chunk of memory assigned to a particular pool.
struct PoolPage
{
	void* addr;
	int   pool;

	bool contains(void* ptr)
	{
		return (ptr >= addr && ptr < ((char*)addr + PAGE_SIZE));
	}
};

static PoolPage* _poolPages    = 0;   // the master list of pool memory
static unsigned  _numPoolPages = 0;
static unsigned  _maxPoolPages = 0;



//---------------------------------------------------------------------------
// Data and definitions for error checking
//---------------------------------------------------------------------------

static const UInt32 UNUSED_PATTERN   = 0xfeedface;  // used to init new allocs
static const UInt32 DELETE_PATTERN   = 0xdeadbeef;  // used to init deleted allocs

static const int HASH_BITS = 12;               // alloc info is stored in hash tbl
static const int HASH_SIZE = 1 << HASH_BITS;

static const int BREAK_ON_DEALLOC = 0x01;
static const int BREAK_ON_REALLOC = 0x02;

struct AllocInfo
{
	const char* sourceFile;
	unsigned    lineNumber;
	int         caller;
	size_t      requestedSize;
	size_t      actualSize;
	void*       reportedAddr;
	void*       actualAddr;
	unsigned    allocNumber;
	unsigned    flags;
	AllocInfo*  prev;
	AllocInfo*  next;
};

#if defined(DEBUG)

static AllocInfo*  _allocTable[HASH_SIZE];
static AllocInfo** _allocPages     = 0;
static unsigned    _numAllocPages  = 0;
static AllocInfo*  _nextFreeInfo   = 0;

// Cache the source file and line information for new and delete
static const char* _sourceFile     = 0;
static unsigned    _lineNumber     = 0;

// If the user has called breakOnAllocation(), the number on which to break
static unsigned    _allocBreakCount = 0;

#endif



//---------------------------------------------------------------------------
// Data and definitions for statistics collection
//---------------------------------------------------------------------------

static unsigned _numAllocations   = 0;
static unsigned _numPooledAllocs  = 0;
static unsigned _poolMemAllocated = 0;
static unsigned _currPoolMemUsed  = 0;
static unsigned _maxPoolMemUsed   = 0;



//---------------------------------------------------------------------------
// Error messages
//---------------------------------------------------------------------------

static const char* OUT_OF_MEMORY = "MemMgr: out of memory\n";
static const char* NOT_MINE      = "MemMgr: received pointer to unmanaged memory\n";



//---------------------------------------------------------------------------
// Local interface
//---------------------------------------------------------------------------

static void       _log(const char* msg, ...);
static void*      _doAlloc(size_t size);
static void       _doRelease(void* ptr);
static void       _createPools();
static void       _releasePools();
static void       _addMemoryToPool(int pool);
static void*      _createPoolPage(int pool);
static int        _findNearestPageSlot(void* ptr);
static AllocInfo* _createAllocInfo(const char* src, unsigned line, size_t size, int caller);
static void       _prepareAllocation(AllocInfo* info, void* ptr);
static void       _registerAllocation(AllocInfo* info);
static void       _unregisterAllocation(AllocInfo* info);
static AllocInfo* _findAllocInfo(void* ptr);
static void       _fillArea(void* ptr, unsigned size, unsigned pattern);



//---------------------------------------------------------------------------
// Initialization and cleanup is controlled by a static class
//---------------------------------------------------------------------------

struct MemMgrCtrlClass
{
	MemMgrCtrlClass()
	{
		_createPools();
	}

	~MemMgrCtrlClass()
	{
		_releasePools();
	}
};

static MemMgrCtrlClass sg_controller;




//---------------------------------------------------------------------------
// Breakpoint API
//---------------------------------------------------------------------------

#if defined(DEBUG)

void f4::breakOnAllocation(unsigned num)
{
	_allocBreakCount = num;
}

//---------------------------------------------------------------------------

void f4::breakOnDealloc(void* ptr)
{
	AllocInfo* info = _findAllocInfo(ptr);
	
	// You called breakOnDealloc for an invalid or unmanaged memory pointer
	_assert(info);

	info->flags |= BREAK_ON_DEALLOC;
}

//---------------------------------------------------------------------------

void f4::breakOnRealloc(void* ptr)
{
	AllocInfo* info = _findAllocInfo(ptr);

	// You called breakOnRealloc for an invalid of unmanaged memory pointer
	_assert(info);

	info->flags |= BREAK_ON_REALLOC;
}

#endif



//---------------------------------------------------------------------------
// Reporting API
//---------------------------------------------------------------------------

static void (*_printFunc)(const char* msg) = 0;

//---------------------------------------------------------------------------

void f4::setMemoryPrintFunc(void (*func)(const char*))
{
	_printFunc = func;
}

//---------------------------------------------------------------------------

void f4::printMemoryReport()
{
	_log("-- Memory Report --------------------------------------------------\n");

	_log("Total allocations: %ld\n",  _numAllocations);
	_log("Pooled allocations: %ld\n", _numPooledAllocs);
	_log("Pool mem allocated: %ldK (%ld pages)\n", _poolMemAllocated/1024, _poolMemAllocated/PAGE_SIZE);
	_log("Pool mem used: %ldK\n", _maxPoolMemUsed/1024);

#if defined(DEBUG)
	_log("\nCurrent allocations:\n");

	bool allocsFound = false;
	for (int i = 0; i < HASH_SIZE; ++i)
	{
		if (_allocTable[i])
		{
			allocsFound = true;
			for (AllocInfo* info = _allocTable[i]; info; info = info->next)
				_log("%s(%d): #%ld, %ld bytes\n", (info->sourceFile) ? info->sourceFile : "(unknown)", 
				                                   info->lineNumber, 
				                                   info->allocNumber, info->requestedSize);
		}
	}

	if (!allocsFound) _log("  No current allocations.\n");
#endif

	_log("-- End Memory Report ----------------------------------------------\n");
}




//---------------------------------------------------------------------------
// Allocation API
//---------------------------------------------------------------------------

void* f4::allocateMemory(const char* src, unsigned line, size_t size, int caller)
{
	++_numAllocations;

#if defined(DEBUG)
	if (caller == MEMMGR_NEW || caller == MEMMGR_NEW_ARRAY)
	{
		src = _sourceFile;  line = _lineNumber;
		_sourceFile = NULL; _lineNumber = 0;
	}

	// You called breakOnAllocation() for this allocation
	_assert(_numAllocations != _allocBreakCount);

	// Add this allocation to the master list. The size may be adjusted to
	// account for debug information, guard bands, etc.
	AllocInfo* info = _createAllocInfo(src, line, size, caller);
	size = info->actualSize;
#endif

	void* ptr = _doAlloc(size);
	if (!ptr)
	{
		_log(OUT_OF_MEMORY);
		return 0;
	}

#if defined(DEBUG)
	// Initialize the allocation and add it to the master list
	_prepareAllocation(info, ptr);
	_registerAllocation(info);

	// Debug info and guard bands may have been added to the allocation
	ptr  = info->reportedAddr;
	size = info->requestedSize;
#endif

	return ptr;
}

//---------------------------------------------------------------------------


void* f4::reallocateMemory(const char* src, unsigned line, void* ptr, size_t size)
{
	if (!ptr) return allocateMemory(src, line, size, MEMMGR_REALLOC);

#if defined(DEBUG)
	AllocInfo* info = _findAllocInfo(ptr);
	if (!info)
	{
		_log(NOT_MINE);
		return realloc(ptr, size);
	}

	// You called breakOnRealloc() for this allocation
	_assert((info->flags & BREAK_ON_REALLOC) == 0);

	// Cache information that needs to be carried over
	unsigned flags = info->flags;

	// In debug mode, I always allocate a new block and copy the data over.
	// This is the easiest way to set up the guard bands, etc.
	void* newPtr = allocateMemory(src, line, size, MEMMGR_REALLOC);
	if (!newPtr) return 0;
	memcpy(newPtr, ptr, (info->requestedSize > size) ? size : info->requestedSize);
	releaseMemory(src, line, ptr, MEMMGR_REALLOC);

	// Copy over the persistant information
	info = _findAllocInfo(newPtr);
	info->flags = flags;

	return newPtr;
#else
	// Is this a pooled block? If not, just call realloc()
	int slot = _findNearestPageSlot(ptr);
	if (slot == -1 || !_poolPages[slot].contains(ptr))
		return realloc(ptr, size);

	// Pooled memory must be copied to a new block
	void* newPtr = allocateMemory(src, line, size, MEMMGR_REALLOC);
	
	unsigned oldSize = (_poolPages[slot].pool + 1) * GRANULARITY;
	memcpy(newPtr, ptr, (size > oldSize) ? oldSize : size);

	_doRelease(ptr);
	return newPtr;
#endif
}

//---------------------------------------------------------------------------

void f4::releaseMemory(const char* src, unsigned line, void* ptr, int caller)
{
	if (!ptr) return;

#if defined(DEBUG)
	AllocInfo* info = _findAllocInfo(ptr);
	if (info)
	{
		// You called breakOnDealloc() for this allocation
		_assert((info->flags & BREAK_ON_DEALLOC) == 0);

		// Fill the allocation with a recognizable pattern before releasing it
		_fillArea(info->actualAddr, info->actualSize, DELETE_PATTERN);
		_unregisterAllocation(info);
	}
	else
		_log(NOT_MINE);
#endif

	_doRelease(ptr);
}

//---------------------------------------------------------------------------

#if defined(DEBUG)
void f4::cacheAllocatorInfo(const char* src, unsigned line)
{
	_sourceFile = src;
	_lineNumber = line;
}
#endif



//---------------------------------------------------------------------------
// Local functions
//---------------------------------------------------------------------------

void _log(const char* msg, ...)
{
	char buffer[1024];

	va_list vargs;
	va_start(vargs, msg);
	vsprintf(buffer, msg, vargs);
	va_end(vargs);
	if (_printFunc) _printFunc(buffer);
}

//---------------------------------------------------------------------------

void* _doAlloc(size_t size)
{
	// Requests for zero bytes must still return a valid address
	if (size == 0) size = 1;

	// Allocations bigger than MAX_POOLED bytes use malloc()
	if (size > (size_t)MAX_POOLED)
		return malloc(size);

	// Which pool does this fall into?
	int pool = (size - 1) / GRANULARITY;

	// Are there any free blocks left in the pool? If not, add a new 
	// blank page to the pool to make room. If there isn't enough room
	// to create a new page, fall back to malloc()
	if (!_pools[pool])
	{
		_addMemoryToPool(pool);
		if (!_pools[pool]) return malloc(size);
	}

	// Grab a free block out of the pool (and point the pool to the next one)
	void* ptr = _pools[pool];
	_pools[pool] = *((void**)ptr);

	// Update the memory statistics
	_numPooledAllocs++;
	_currPoolMemUsed += (pool + 1) * GRANULARITY;
	if (_currPoolMemUsed > _maxPoolMemUsed) _maxPoolMemUsed = _currPoolMemUsed;

	return ptr;
}

//---------------------------------------------------------------------------

void _doRelease(void* ptr)
{
	// Is this address in one of the memory pool? If not, just call free()
	int slot = _findNearestPageSlot(ptr);
	if (slot == -1 || !_poolPages[slot].contains(ptr))
	{
		free(ptr);
		return;
	}

	// Write a pointer to the next free block at this address, then add the
	// block back to it's pool
	int pool = _poolPages[slot].pool;
	*((void**)ptr) = _pools[pool];
	_pools[pool] = ptr;

	// Update the statistics
	_currPoolMemUsed -= (pool + 1) * GRANULARITY;
}

//---------------------------------------------------------------------------

void _createPools()
{
	// Zero out each pool's free memory pointer
	for (int p = 0; p < NUM_POOLS; ++p) _pools[p] = 0;

	// Create some free slots in the pooled memory page table
	_poolPages    = (PoolPage*)malloc(INIT_TBL_SIZE * sizeof(PoolPage));
	_maxPoolPages = INIT_TBL_SIZE;
}

//---------------------------------------------------------------------------

void _releasePools()
{
	for (unsigned p = 0; p < _numPoolPages; ++p) free(_poolPages[p].addr);
	free(_poolPages);
}

//---------------------------------------------------------------------------

void _addMemoryToPool(int pool)
{
	void* page = _createPoolPage(pool);
	if (!page) return;

	// Link all the blocks on the page into one big free memory list
	int blockSize = (pool + 1);
	int numBlocks = PAGE_SIZE / (blockSize * GRANULARITY);

	void** ptr = (void**)page;
	for (int i = 0; i < (numBlocks - 1); ++i)
	{
		*ptr = ptr + blockSize;
		ptr += blockSize;
	}
	*ptr = 0;

	// Set the free memory pointer to this new page
	_pools[pool] = page;
}

//---------------------------------------------------------------------------

void* _createPoolPage(int pool)
{
	// Make sure there is a free slot in the master page table
	if (_numPoolPages == _maxPoolPages)
	{
		PoolPage* temp = (PoolPage*)realloc(_poolPages, sizeof(PoolPage) * (_maxPoolPages + GROW_TBL_BY));
		if (!temp) return 0;

		_poolPages = temp;
		_maxPoolPages += GROW_TBL_BY;
	}

	// Allocate a new page
	void* page = malloc(PAGE_SIZE);
	if (!page) return 0;

	// Insert this page into the table, in address order
	int slot = _findNearestPageSlot(page);
	if (slot > -1)
		memmove(&_poolPages[slot+1], &_poolPages[slot], (_numPoolPages - slot) * sizeof(PoolPage));
	else
		slot = _numPoolPages;

	// Initialize the page
	++_numPoolPages;
	_poolPages[slot].addr = page;
	_poolPages[slot].pool = pool;

	// Update the statistics
	_poolMemAllocated += PAGE_SIZE;

	return page;
}

//---------------------------------------------------------------------------

int _findNearestPageSlot(void* ptr)
{
	// Binary search the page table
	unsigned start = 0;
	unsigned end   = _numPoolPages;

	while (start != end)
	{
		int slot = (end - start) / 2 + start;
		if (_poolPages[slot].contains(ptr))
			return slot;
		else if (_poolPages[slot].addr < ptr)
			start = slot + 1;
		else
			end = slot;
	}

	if (end == _numPoolPages)            // everything < this address
		return -1;

	if (_poolPages[end].contains(ptr))   // found the right page
		return end;

	if (_poolPages[end].addr < ptr)      // ended up on lower page
		return end + 1;

	return end;                          // ended up on higher page
}




//---------------------------------------------------------------------------
// Error checking API functions
//---------------------------------------------------------------------------
#if defined(DEBUG)

AllocInfo* _createAllocInfo(const char* src, unsigned line, size_t size, int caller)
{
	// If I've run out of AllocInfo objects, create a bunch more
	if (!_nextFreeInfo)
	{
		AllocInfo* page = (AllocInfo*)malloc(sizeof(AllocInfo) * 256);
		_assert(page);

		// Link 'em all together into one list of free info objects
		for (int i = 0; i < 255; ++i) page[i].next = &page[i + 1];
		page[255].next = 0;

		// Add the new page to the master list
		AllocInfo** temp = (AllocInfo**)realloc(_allocPages, (_numAllocPages + 1) * sizeof(AllocInfo*));
		_assert(temp);

		_allocPages = temp;
		_allocPages[_numAllocPages++] = page;

		// Set the free pointer to the front of the list
		_nextFreeInfo = page;
	}

	// Grab the first object off the list
	AllocInfo* info = _nextFreeInfo;
	_nextFreeInfo = _nextFreeInfo->next;

	// Initialize it and return
	info->sourceFile    = src;
	info->lineNumber    = line;
	info->caller        = caller;
	info->requestedSize = size;
	info->actualSize    = size;
	info->allocNumber   = _numAllocations;
	info->flags         = 0;

	return info;
}

//---------------------------------------------------------------------------

void _prepareAllocation(AllocInfo* info, void* ptr)
{
	// I've haven't added the guard bands code yet
	info->actualAddr   = ptr;
	info->reportedAddr = ptr;

	// Fill the user memory with a recognizable byte code
	if (info->caller != MEMMGR_CALLOC)
		_fillArea(info->reportedAddr, info->requestedSize, UNUSED_PATTERN);
}

//---------------------------------------------------------------------------

void _registerAllocation(AllocInfo* info)
{
	unsigned hash = ((unsigned)info->reportedAddr >> 4) & (HASH_SIZE - 1);

	// Standard insertion to the front of a doubly-linked list
	AllocInfo* nextAlloc = _allocTable[hash];
	info->next = nextAlloc;
	info->prev = 0;

	if (nextAlloc) nextAlloc->prev = info;
	_allocTable[hash] = info;
}

//---------------------------------------------------------------------------

void _unregisterAllocation(AllocInfo* info)
{
	// Standard removal of a node from a doubly-linked list
	if (!info->prev)
	{
		unsigned hash = ((unsigned)info->reportedAddr >> 4) & (HASH_SIZE - 1);
		_allocTable[hash] = info->next;
	}
	else
		info->prev->next = info->next;

	if (info->next)
		info->next->prev = info->prev;

	// Put this object back on the free list
	info->next = _nextFreeInfo;
	_nextFreeInfo = info;
}

//---------------------------------------------------------------------------

AllocInfo* _findAllocInfo(void* ptr)
{
	unsigned hash = ((unsigned)ptr >> 4) & (HASH_SIZE - 1);

	for (AllocInfo* info = _allocTable[hash]; info; info = info->next)
	{
		if (info->reportedAddr == ptr)
			return info;
	}

	return 0;
}

//---------------------------------------------------------------------------

void _fillArea(void* ptr, unsigned size, unsigned pattern)
{
	unsigned i;

	// For speed, write full ints as far as possible
	unsigned len = size / sizeof(unsigned);
	unsigned *up = (unsigned*)ptr;
	
	for (i = 0; i < len; ++i, ++up)
	{
		*up = pattern;
	}

	// Fill in the last bit with single bytes
	len = size % sizeof(unsigned);
	char* cp = (char*)up;
	char* pp = (char*)&pattern;

	for (i = 0; i < len; ++i, ++cp, ++pp)
	{
		*cp = *pp;
	}
}

#endif  // defined(DEBUG)
