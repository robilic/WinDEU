/*----------------------------------------------------------------------------*
 | This file is part of WinDEU, the port of DEU to Windows.                   |
 | WinDEU was created by the DEU team:                                        |
 |  Renaud Paquay, Raphael Quinet, Brendon Wyber and others...                |
 |                                                                            |
 | DEU is an open project: if you think that you can contribute, please join  |
 | the DEU team.  You will be credited for any code (or ideas) included in    |
 | the next version of the program.                                           |
 |                                                                            |
 | If you want to make any modifications and re-distribute them on your own,  |
 | you must follow the conditions of the WinDEU license. Read the file        |
 | LICENSE or README.TXT in the top directory.  If do not  have a copy of     |
 | these files, you can request them from any member of the DEU team, or by   |
 | mail: Raphael Quinet, Rue des Martyrs 9, B-4550 Nandrin (Belgium).         |
 |                                                                            |
 | This program comes with absolutely no warranty.  Use it at your own risks! |
 *----------------------------------------------------------------------------*

	Project WinDEU
	DEU team
	Jul-Dec 1994, Jan-Mar 1995

	FILE:         common.h

*/
#ifndef __common_h
#define __common_h

#ifndef __WIN32__
#ifndef __LARGE__
#error This application can only be compiled with the LARGE memory model
#endif
#endif

#ifdef __WIN32__
#pragma warn -sig
#endif

// If release version, undefines all debug and trace options
#ifdef RELEASE
#undef DEBUG
#undef TRACE
#define NDEBUG
#endif

/* the includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <dir.h>
#include <alloc.h>
#include <assert.h>

#include <owl\owldefs.h>
#include <owl\version.h>
#include <owl\module.h>
#include <owl\applicat.h>
#include <owl\dc.h>
#include <owl\menu.h>
#include <owl\window.h>
#include <owl\dialog.h>
#include <owl\decframe.h>
#include <owl\control.h>
#include <owl\window.h>
#include <owl\dialog.h>
#include <owl\static.h>
#include <owl\edit.h>
#include <owl\checkbox.h>
#include <owl\radiobut.h>
#include <owl\listbox.h>
#include <owl\validate.h>

class TMapDC;

#ifndef USHORT
typedef unsigned short USHORT;
typedef short SHORT;
#endif

#ifndef ULONG
typedef unsigned long ULONG;
typedef long LONG;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef min
#define min(a,b)	((a) < (b) ? (a) : (b))
#define max(a,b)	((a) > (b) ? (a) : (b))
#endif


/*
   the version information
*/

#define DEU_VERSION "5.23"  /* the version number */


/*
   syntactic sugar
*/
typedef int Bool;               /* Boolean data: true or false */


#endif