/**
 * @file file_server.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: file_server.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 *
 * @todo Add openLibrary() call to find and open a shared library.
 *
 */
#if !defined(_F4_CORE_FILE_SERVER_H_)
#define _F4_CORE_FILE_SERVER_H_

ENTER_NAMESPACE(f4);


/**
 * This interface provides a way to override the default behavior of
 * CoreService::runFile(), presumably to provide more a sophisticated
 * algorithm for locating the script file.
 * @ingroup f4_core
 */
struct CoreFileServer : public Service
{

	/**
	 * Load and execute a script file.
	 */
	virtual bool runFile(const Char* filename, Context* context = 0) = 0;


	/**
	 * Determine if a specified path (file or directory) exists.
	 */
	virtual bool pathExists(const Char* path) = 0;

};


EXIT_NAMESPACE;
#endif
