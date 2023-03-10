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

	FILE:         levels.h
*/
#ifndef __levels_h
#define __levels_h

/* the includes */
#ifndef __common_h
	#include "common.h"
#endif

#ifndef __windeu_h
	#include "windeu.h"	// DoomLevel, Default...
#endif

#ifndef __wstructs_h
	#include "wstructs.h"
#endif

#ifndef __things_h
	#include "things.h"
#endif

#ifndef __wads_h
	#include "wads.h"
#endif

struct TextureInfo
{
	SHORT Width;
	SHORT Height;
	char Name[9];
};

/* the external variables from levels.c */
extern MDirPtr Level;		/* master dictionary entry for the level */

extern SHORT NumThings;	/* number of things */
extern TPtr  Things;		/* things data */

extern SHORT NumLineDefs;	/* number of line defs */
extern LDPtr LineDefs;		/* line defs data */

extern SHORT NumSideDefs;	/* number of side defs */
extern SDPtr SideDefs;		/* side defs data */

extern SHORT NumVertexes;	/* number of vertexes */
extern VPtr  Vertexes;		/* vertex data */

extern SHORT NumSegs;		/* number of segments */
extern SEPtr Segs;		/* list of segments */
extern SEPtr LastSeg;		/* last segment in the list */

extern SHORT NumSSectors;	/* number of subsectors */
extern SSPtr SSectors;		/* list of subsectors */
extern SSPtr LastSSector;	/* last subsector in the list */

extern SHORT NumSectors;	/* number of sectors */
extern SPtr  Sectors;		/* sectors data */

extern SHORT NumWTexture;	/* number of wall textures */
extern TextureInfo **WTexture;	/* array of wall texture names */

extern SHORT NumFTexture;	/* number of floor/ceiling textures */
extern TextureInfo **FTexture;	/* array of texture names */

extern SHORT   MapMaxX;		/* maximum X value of map */
extern SHORT   MapMaxY;		/* maximum Y value of map */
extern SHORT   MapMinX;		/* minimum X value of map */
extern SHORT   MapMinY;		/* minimum Y value of map */

extern BOOL  MadeChanges;	/* made changes? */
extern BOOL  MadeMapChanges;	/* made changes that need rebuilding? */

/* from levels.cpp */
void ShowProgress (int);
void ReadLevelData (char *levelName);
void ForgetLevelData (void);
void SaveLevelData (char *);

void AddWTextureInfo (char *name);
void ReadWTextureInfo ();
void ForgetFTextureInfo ();

void AddWTextureInfo (char *startEntry, char *endEntry);
void ReadFTextureInfo ();
void ForgetWTextureInfo ();

void GetWallTextureSize (SHORT *, SHORT *, char *);

#endif
