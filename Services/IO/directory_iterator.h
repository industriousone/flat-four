/**
 * @file directory_iterator.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: directory_iterator.h,v 1.2 2002/04/22 01:51:56 jason379 Exp $
 */
#if !defined(_F4_IO_DIRECTORY_ITERATOR_H_)
#define _F4_IO_DIRECTORY_ITERATOR_H_

ENTER_NAMESPACE(f4);


/**
 * Iterate through the contents of a directory.
 * @ingroup f4_io
 */
struct DirectoryIterator : public Service
{

	/**
	 * Get the name of the current directory entry.
	 */
	virtual const Char* getName() = 0;


	/**
	 * Move to the next entry in the directory.
	 *
	 * @return True if successful, false if the end of the directory listing
	 *         has been reached.
	 */
	virtual bool next() = 0;


	/**
	 * Restart the iterator.
	 */
	virtual bool rewind() = 0;

};



// --- begin scripting interface ---

SCRIPT_METHOD(DirectoryIterator, getName, RETURNS_STRING)
END_METHOD

SCRIPT_METHOD(DirectoryIterator, next, RETURNS_BOOL)
END_METHOD

SCRIPT_METHOD(DirectoryIterator, rewind, RETURNS_BOOL)
END_METHOD

EXPORTS(DirectoryIterator)
	RO_PROP_STRING(DirectoryIterator, name, getName)
	METHOD(DirectoryIterator, rewind)
	METHOD(DirectoryIterator, next)
END_EXPORTS

// --- end scripting interface ---


EXIT_NAMESPACE;
#endif
