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

	FILE:         wstructs.h
*/
#ifndef __wstructs_h
#define __wstructs_h

#ifndef __common_h
	#include "common.h"
#endif

#ifndef __windeu_h
	#include "windeu.h"
#endif

/*
   *** IMPORTANT ***
   RP NOTE: Under Windows, the elements of an "huge" array MUST have
			a size equal to a power of 2. So, the Thing, SideDef, Vertex,
			LineDef and Sector struct are filled with dummy fields.
			Note this is only applicable to Windows 16-bit programs.
			32-bit programs doesn't have this limitation.

*/

/*
   this data structure contains the information about the THINGS

*/
struct Thing
{
	SHORT xpos;      /* x position */		/* size =  2 */
	SHORT ypos;      /* y position */       /* size =  4 */
	SHORT angle;     /* facing angle */     /* size =  6 */
	SHORT type;      /* thing type */       /* size =  8 */
	SHORT when;      /* appears when? */    /* size = 10 */
#ifndef __WIN32__
	LONG filler;							/* size = 16 = 2**4 */
#endif
};
typedef struct Thing HUGE *TPtr;



/*
   this data structure contains the information about the LINEDEFS
*/
struct LineDef
{
	SHORT start;     /* from this vertex ... */
	SHORT end;       /* ... to this vertex */
	SHORT flags;     /* see NAMES.C for more info */
	SHORT type;      /* see NAMES.C for more info */
	SHORT tag;       /* crossing this linedef activates the sector with the same tag */
	SHORT sidedef1;  /* sidedef */
	SHORT sidedef2;  /* only if this line adjoins 2 sectors */
#ifndef __WIN32__
	SHORT filler;		/* 2-byte filler to make 16-byte struct */
#endif
};
typedef struct LineDef HUGE *LDPtr;



/*
   this data structure contains the information about the SIDEDEFS
*/
struct SideDef
{
   SHORT xoff;      /* X offset for texture */
   SHORT yoff;      /* Y offset for texture */
   char tex1[8];  	/* texture name for the part above */
   char tex2[8];  	/* texture name for the part below */
   char tex3[8];  	/* texture name for the regular part */
   SHORT sector;    /* adjacent sector */
#ifndef __WIN32__
   SHORT filler;		/* 2-byte filler to make 32-byte struct */
#endif
};
typedef struct SideDef HUGE *SDPtr;



/*
   this data structure contains the information about the VERTEXES
*/
struct Vertex
{
   SHORT x;         /* X coordinate */
   SHORT y;         /* Y coordinate */
};
typedef struct Vertex HUGE *VPtr;


/*
   this data structure contains the information about the SEGS
*/
typedef struct Seg HUGE *SEPtr;
struct Seg
{
   SEPtr next;    	/* next Seg in list */
   SHORT start;     /* from this vertex ... */
   SHORT end;       /* ... to this vertex */
   USHORT angle;	/* angle (0 = east, 16384 = north, ...) */
   SHORT linedef;   /* linedef that this seg goes along*/
   SHORT flip;      /* true if not the same direction as linedef */
   USHORT dist; 	/* distance from starting point */
};




/*
   this data structure contains the information about the SSECTORS
*/
typedef struct SSector HUGE *SSPtr;
struct SSector
{
   SSPtr next;	  	/* next Sub-Sector in list */
   SHORT num;       /* number of Segs in this Sub-Sector */
   SHORT first;     /* first Seg */
};



/*
   this data structure contains the information about the NODES
*/
typedef struct Node HUGE *NPtr;
struct Node
{
   SHORT x, y;                      // starting point
   SHORT dx, dy;                    // offset to ending point
   SHORT miny1, maxy1, minx1, maxx1;// bounding rectangle 1
   SHORT miny2, maxy2, minx2, maxx2;// bounding rectangle 2
   SHORT child1, child2;            // Node or SSector (if high bit is set)
   NPtr node1, node2;               // pointer if the child is a Node
   SHORT num;                       // number given to this Node
};



/*
   this data structure contains the information about the SECTORS
*/
struct Sector
{
   SHORT floorh;    /* floor height */
   SHORT ceilh;     /* ceiling height */
   char floort[8];	/* floor texture */
   char ceilt[8]; 	/* ceiling texture */
   SHORT light;     /* light level (0-255) */
   SHORT special;   /* special behaviour (0 = normal, 9 = secret, ...) */
   SHORT tag;       /* sector activated by a linedef with the same tag */
#ifndef __WIN32__
   SHORT filler1;  /* 3 * 2-byte filler to make 32-byte struct */
   SHORT filler2;
   SHORT filler3;
#endif
};
typedef struct Sector HUGE *SPtr;



#endif
/* end of file */
