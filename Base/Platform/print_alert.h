/**
 * @file print_alert.h
 *
 * Display formatted messages in a popup window.
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: print_alert.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 *
 * @todo Linux: use a proper popup window, writes to stdout currently.
 *
 */
#if !defined(_F4_PLATFORM_PRINT_ALERT_H_)
#define _F4_PLATFORM_PRINT_ALERT_H_

#include <stdarg.h>
ENTER_NAMESPACE(f4);


/**
 * Write a formatted string to a popup message box, using @c print() style
 * codes. The total length of the formatted string must not exceed 1024
 * characters.
 * @ingroup f4_platform_msg
 */
void printAlert(const Char* caption, const Char* msg, ...);



/**
 * Write a formatted string to a popup message box, using @c print() style
 * codes. The total length of the formatted string must not exceed 1024
 * characters.
 * @ingroup f4_platform_msg
 */
void vprintAlert(const Char* caption, const Char* msg, va_list vargs);


EXIT_NAMESPACE;
#endif
