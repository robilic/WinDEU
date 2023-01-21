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

	FILE:         levels.cpp

	OVERVIEW
	========
	Source file for level loading/saving functions
*/

#include "common.h"
#pragma hdrstop

#ifndef __levels_h
	#include "levels.h"
#endif

#ifndef __lprogdlg_h
	#include "lprogdlg.h"
#endif

#ifndef __gfx_h
	#include "gfx.h"	// ComputeAngle()
#endif

#ifndef __memory_h
	#include "memory.h"
#endif

#ifndef __objects_h
	#include "objects.h"	// IsLineDefInside()
#endif

#ifndef _names_h
	#include "names.h"		//  GetDoomLevelName()
#endif

#ifndef __nodes_h
	#include "nodes.h"		// CreateNodes()
#endif

#ifndef __things_h
	#include "things.h"
#endif

#ifndef __wads_h
	#include "wads.h"
#endif

#ifndef __wstructs_h
	#include "wstructs.h"
#endif

/* external function from objects.c */
extern Bool CreateNodes (NPtr *, SHORT *, SEPtr); /* SWAP - needs Vertexes */

/* the global data */
MDirPtr Level = NULL;			/* master dictionary entry for the level */

SHORT NumThings = 0;			/* number of things */
TPtr  Things;					/* things data */

SHORT NumLineDefs = 0;			/* number of line defs */
LDPtr LineDefs;					/* line defs data */

SHORT NumSideDefs = 0;			/* number of side defs */
SDPtr SideDefs;					/* side defs data */

SHORT NumVertexes = 0;			/* number of vertexes */
VPtr  Vertexes;					/* vertex data */

SHORT NumSectors = 0;			/* number of sectors */
SPtr  Sectors;					/* sectors data */

SHORT NumSegs = 0;				/* number of segments */
SEPtr Segs = NULL;				/* list of segments */
SEPtr LastSeg = NULL;			/* last segment in the list */

SHORT NumSSectors = 0;			/* number of subsectors */
SSPtr SSectors = NULL;			/* list of subsectors */
SSPtr LastSSector = NULL;		/* last subsector in the list */

SHORT NumNodes = 0;				/* number of Nodes */
NPtr  Nodes = NULL;				/* nodes tree */

SHORT NumWTexture = 0;			/* number of wall textures */
TextureInfo **WTexture = NULL;	/* array of wall texture info */

SHORT NumFTexture = 0;			/* number of floor/ceiling textures */
TextureInfo **FTexture = NULL;	/* array of texture info */

SHORT MapMaxX = -32767;			/* maximum X value of map */
SHORT MapMaxY = -32767;			/* maximum Y value of map */
SHORT MapMinX = 32767;			/* minimum X value of map */
SHORT MapMinY = 32767;			/* minimum Y value of map */
BOOL MadeChanges = FALSE;		/* made changes? */
BOOL MadeMapChanges = FALSE;	/* made changes that need rebuilding? */


//
// Modeless Dialog Box for showing progress while saving level data
//
static TLevelProgressDialog *pProgressDialog = NULL;

void ShowProgress (int objtype)
{
	assert (pProgressDialog != NULL);

	pProgressDialog->ShowNodesProgress (objtype);
}


/*
   read in the level data
*/

void ReadLevelData (char *levelname)
{
	MDirPtr dir;
	char name[9];
	SHORT n;
	SHORT m;
	SHORT val;
	SHORT OldNumVertexes;
	SHORT *VertexUsed;

	// find the various level information from the master directory
	strcpy (name, levelname);
	WorkMessage ("Reading data for level %s ...", name);
	Level = FindMasterDir (MasterDir, name);
	if ( Level == NULL )
		ProgError ("Level \"%s\"data not found", name);

	// get the number of Vertices
	dir = FindMasterDir (Level, "VERTEXES");
	if (dir != NULL)
	{
		assert (dir->dir.size / 4L < SHRT_MAX);
		OldNumVertexes = (SHORT) (dir->dir.size / 4L);
	}
	else
		OldNumVertexes = 0;

	// If there are vertexes, allocated memory for the 'use' tab
	if (OldNumVertexes > 0)
	{
		VertexUsed = (SHORT *)GetMemory (OldNumVertexes * sizeof(SHORT));
		for (n = 0; n < OldNumVertexes; n++)
			VertexUsed[n] = (SHORT)FALSE;
	}

	//
	// read in the Things data */
	//
	dir = FindMasterDir (Level, "THINGS");
	if (dir != NULL)
	{
		assert (dir->dir.size / 10L < SHRT_MAX);
		NumThings = (SHORT) (dir->dir.size / 10L);
	}
	else
		NumThings = 0;

	if (NumThings > 0)
	{
		Things = (TPtr)GetMemory (NumThings * sizeof (Thing));
		BasicWadSeek (dir->wadfile, dir->dir.start);
		for (n = 0; n < NumThings; n++)
		{
			Thing CurThing;
			BasicWadRead( dir->wadfile, &(CurThing.xpos), 2);
			BasicWadRead( dir->wadfile, &(CurThing.ypos), 2);
			BasicWadRead( dir->wadfile, &(CurThing.angle), 2);
			BasicWadRead( dir->wadfile, &(CurThing.type), 2);
			BasicWadRead( dir->wadfile, &(CurThing.when), 2);
			Things[n] = CurThing;
		}
	}

	//
	// read in the LineDef information
	//
	dir = FindMasterDir (Level, "LINEDEFS");
	if (dir != NULL)
	{
		assert (dir->dir.size / 14L < SHRT_MAX);
		NumLineDefs = (SHORT) (dir->dir.size / 14L);
	}
	else
		NumLineDefs = 0;

	if (NumLineDefs > 0)
	{
		LineDefs = (LDPtr)GetMemory (NumLineDefs * sizeof (LineDef));
		BasicWadSeek (dir->wadfile, dir->dir.start);
		for (n = 0; n < NumLineDefs; n++)
		{
			LineDef CurLD;
			BasicWadRead (dir->wadfile, &(CurLD.start), 2);
			BasicWadRead (dir->wadfile, &(CurLD.end), 2);
			BasicWadRead (dir->wadfile, &(CurLD.flags), 2);
			BasicWadRead (dir->wadfile, &(CurLD.type), 2);
			BasicWadRead (dir->wadfile, &(CurLD.tag), 2);
			BasicWadRead (dir->wadfile, &(CurLD.sidedef1), 2);
			BasicWadRead (dir->wadfile, &(CurLD.sidedef2), 2);
			LineDefs[n] = CurLD;

			VertexUsed[CurLD.start] = (SHORT)TRUE;
			VertexUsed[CurLD.end] = (SHORT)TRUE;
		}
	}

	// read in the SideDef information
	dir = FindMasterDir (Level, "SIDEDEFS");
	if (dir != NULL)
	{
		assert (dir->dir.size / 30L < SHRT_MAX);
		NumSideDefs = (SHORT) (dir->dir.size / 30L);
	}
	else
		NumSideDefs = 0;

	if (NumSideDefs > 0)
	{
		SideDefs = (SDPtr)GetMemory (NumSideDefs * sizeof (SideDef));
		BasicWadSeek (dir->wadfile, dir->dir.start);
		for (n = 0; n < NumSideDefs; n++)
		{
			SideDef CurSD;
			BasicWadRead (dir->wadfile, &(CurSD.xoff), 2);
			BasicWadRead (dir->wadfile, &(CurSD.yoff), 2);
			BasicWadRead (dir->wadfile, &(CurSD.tex1), 8);
			BasicWadRead (dir->wadfile, &(CurSD.tex2), 8);
			BasicWadRead (dir->wadfile, &(CurSD.tex3), 8);
			BasicWadRead (dir->wadfile, &(CurSD.sector), 2);
			SideDefs[n] = CurSD;
		}
	}

	// read in the Vertices which are all the corners of the level, but ignore the
	// Vertices not used in any LineDef (they usually are at the end of the list).
	NumVertexes = 0;
	for (n = 0; n < OldNumVertexes; n++)
		if (VertexUsed[ n])
			NumVertexes++;

	if (NumVertexes > 0)
	{
		Vertexes = (VPtr)GetMemory (NumVertexes * sizeof (Vertex));
		dir = FindMasterDir (Level, "VERTEXES");
		BasicWadSeek (dir->wadfile, dir->dir.start);
		MapMaxX = -32767;
		MapMaxY = -32767;
		MapMinX = 32767;
		MapMinY = 32767;
		m = 0;
		for (n = 0; n < OldNumVertexes; n++)
		{
			BasicWadRead (dir->wadfile, &val, 2);
			if (VertexUsed [n])
			{
				if (val < MapMinX)	MapMinX = val;
				if (val > MapMaxX)	MapMaxX = val;
				Vertexes[ m].x = val;
			}

			BasicWadRead( dir->wadfile, &val, 2);
			if (VertexUsed[ n])
			{
				if (val < MapMinY)		MapMinY = val;
				if (val > MapMaxY)		MapMaxY = val;
				Vertexes[ m].y = val;
				m++;
			}
		}

		if (m != NumVertexes)
			ProgError("Inconsistency in the Vertexes data\n");
	}

	if (OldNumVertexes > 0)
	{
		/* update the Vertex numbers in the LineDefs (not really necessary, but...) */
		m = 0;
		for (n = 0; n < OldNumVertexes; n++)
			if (VertexUsed[ n])
				VertexUsed[ n] = m++;

		for (n = 0; n < NumLineDefs; n++)
		{
			LineDefs[ n].start = VertexUsed[ LineDefs[ n].start];
			LineDefs[ n].end   = VertexUsed[ LineDefs[ n].end];
		}
		FreeMemory (VertexUsed);
	}

	/* ignore the Segs, SSectors and Nodes */

	/* read in the Sectors information */
	dir = FindMasterDir (Level, "SECTORS");
	if (dir != NULL)
	{	assert (dir->dir.size / 26L < SHRT_MAX);
		NumSectors = (SHORT) (dir->dir.size / 26L);
	}
	else
		NumSectors = 0;

	if (NumSectors > 0)
	{
		Sectors = (SPtr)GetMemory (NumSectors * sizeof (Sector));
		BasicWadSeek( dir->wadfile, dir->dir.start);
		for (n = 0; n < NumSectors; n++)
		{
			Sector CurS;
			BasicWadRead( dir->wadfile, &(CurS.floorh), 2);
			BasicWadRead( dir->wadfile, &(CurS.ceilh), 2);
			BasicWadRead( dir->wadfile, &(CurS.floort), 8);
			BasicWadRead( dir->wadfile, &(CurS.ceilt), 8);
			BasicWadRead( dir->wadfile, &(CurS.light), 2);
			BasicWadRead( dir->wadfile, &(CurS.special), 2);
			BasicWadRead( dir->wadfile, &(CurS.tag), 2);
			Sectors[n]= CurS;
		}
	}

   /* ignore the last entries (Reject & BlockMap) */
}



/*
   forget the level data
*/

void ForgetLevelData() /* SWAP! */
{
	/* forget the Things */
	NumThings = 0;
	if (Things)
		FreeMemory (Things);
	Things = NULL;

	/* forget the Vertices */
	NumVertexes = 0;
	if (Vertexes)
		FreeMemory (Vertexes);
	Vertexes = NULL;

	/* forget the LineDefs */
	NumLineDefs = 0;
	if (LineDefs)
		FreeMemory (LineDefs);
	LineDefs = NULL;

	/* forget the SideDefs */
	NumSideDefs = 0;
	if (SideDefs)
		FreeMemory (SideDefs);
	SideDefs = NULL;

	/* forget the Sectors */
	NumSectors = 0;
	if (Sectors)
		FreeMemory (Sectors);
	Sectors = NULL;

	// Don't do this
	//BUG: If we create a new level, ForgetLevelData is called, but
	//     we want the Level var. still to point to the level entry in the
	//     Master directory.
	//     Thanks to ??? for signaling the bug.
	// Level = NULL;
}



/*
   recursively save the Nodes data to a PWAD file
*/

void SaveNodes (FILE *file, NPtr node)
{
	// Safer to not assert
	// assert (node != NULL);
	if ( node == NULL )
		return;

	/* Nodes tree walk: save child1, save child2, save parent */
	if ((node->child1 & 0x8000) == 0)
	{
		SaveNodes (file, node->node1);
		node->child1 = node->node1->num;
	}
	if ((node->child2 & 0x8000) == 0)
	{
		SaveNodes (file, node->node2);
		node->child2 = node->node2->num;
	}
	WriteBytes (file, &(node->x), 2L);
	WriteBytes (file, &(node->y), 2L);
	WriteBytes (file, &(node->dx), 2L);
	WriteBytes (file, &(node->dy), 2L);
	WriteBytes (file, &(node->maxy1), 2L);
	WriteBytes (file, &(node->miny1), 2L);
	WriteBytes (file, &(node->minx1), 2L);
	WriteBytes (file, &(node->maxx1), 2L);
	WriteBytes (file, &(node->maxy2), 2L);
	WriteBytes (file, &(node->miny2), 2L);
	WriteBytes (file, &(node->minx2), 2L);
	WriteBytes (file, &(node->maxx2), 2L);
	WriteBytes (file, &(node->child1), 2L);
	WriteBytes (file, &(node->child2), 2L);
	node->num = NumNodes++;
}



/*
   forget the Nodes
*/

void ForgetNodes (NPtr node)
{
	// Safer to not assert
	// assert (node != NULL);
	if ( node != NULL )
	{
		if ((node->child1 & 0x8000) == 0)
			ForgetNodes (node->node1);
		if ((node->child2 & 0x8000) == 0)
			ForgetNodes (node->node2);
		FreeMemory (node);
	}
}



/*
   save the level data to a PWAD file
*/

void SaveLevelData (char *outfile)
{
	assert (Level != NULL);

	FILE       *file;
	MDirPtr     dir;
	ULONG       counter = 11;
	ULONG       size;
	SHORT       n;
	SHORT       i, j;
	BYTE  HUGE *data;
	LONG        dirstart;
	SHORT HUGE *blockptr;
	ULONG       blocksize;
	SHORT       blockcount;
	LONG        oldpos;
	BOOL        newnodes;
	ULONG       rejectsize;
	SHORT       oldNumVertexes;

	SELECT_WAIT_CURSOR();
	SAVE_WORK_MSG();

	WorkMessage ("Saving data to \"%s\"...", outfile);
	LogMessage (": Saving data to \"%s\"...\n", outfile);
	oldNumVertexes = NumVertexes;

	// Init. to NULL to destory it safely at the end of this function
	pProgressDialog = NULL;

	//
	// open the file
	//
	if ((file = fopen (outfile, "wb")) == NULL)
		ProgError ("Unable to open file \"%s\"", outfile);

	WriteBytes (file, "PWAD", 4L);     /* PWAD file */
	WriteBytes (file, &counter, 4L);   /* 11 entries */
	WriteBytes (file, &counter, 4L);   /* fix this up later */
	counter = 12L;
	dir = Level->next;

	//
	// output the things data
	//
	for (n = 0; n < NumThings; n++)
	{
		Thing CurTH = Things[n];
		WriteBytes (file, &(CurTH.xpos), 2L);
		WriteBytes (file, &(CurTH.ypos), 2L);
		WriteBytes (file, &(CurTH.angle), 2L);
		WriteBytes (file, &(CurTH.type), 2L);
		WriteBytes (file, &(CurTH.when), 2L);
		counter += 10L;
	}
	dir = dir->next;

	//
	// update MapMinX, MapMinY, MapMaxX, MapMaxY
	//
	MapMaxX = -32767;
	MapMaxY = -32767;
	MapMinX = 32767;
	MapMinY = 32767;
	for (n = 0; n < NumVertexes; n++)
	{
		Vertex ve = Vertexes[n];
		if (ve.x < MapMinX)   MapMinX = ve.x;
		if (ve.x > MapMaxX)   MapMaxX = ve.x;
		if (ve.y < MapMinY)   MapMinY = ve.y;
		if (ve.y > MapMaxY)   MapMaxY = ve.y;
	}

	//
	// do we need to rebuild the Nodes, Segs and SSectors?
	//
	if (MadeMapChanges &&
		(Expert ||
		 Confirm("Do you want to rebuild the NODES, SEGS, SSECTORS, "
				 "REJECT and BLOCKMAP ?\n"
				 "WARNING: You won't be able to use your level if you "
				 "don't do this...")))
	{
		SEPtr seglist;

		// Create progress dialog box (modeless)
		TFrameWindow *MainWindow = ((TApplication *)::Module)->GetMainWindow();
		pProgressDialog = new TLevelProgressDialog(MainWindow);
		pProgressDialog->Create();
		// Diable main window
		MainWindow->EnableWindow(FALSE);

		// Build the segment list
		seglist = NULL;
		for (n = 0; n < NumLineDefs; n++)
		{
			if ( (n & 15) == 0 )
				COOPERATE();	// Windows multi-tasking cooperation

			LineDef CurLD = LineDefs[n];
			assert_vnum (CurLD.start);
			assert_vnum (CurLD.end);
			Vertex VStart = Vertexes[CurLD.start];
			Vertex VEnd   = Vertexes[CurLD.end];

			assert_vsdnum (CurLD.sidedef1);
			if (CurLD.sidedef1 >= 0)
			{
				if (seglist)
				{
					LastSeg->next = (SEPtr)GetMemory (sizeof(Seg));
					LastSeg = LastSeg->next;
				}
				else
				{
					seglist = (SEPtr)GetMemory (sizeof(Seg));
					LastSeg = seglist;
				}
				LastSeg->next    = NULL;
				LastSeg->start   = CurLD.start;
				LastSeg->end     = CurLD.end;
				LastSeg->angle   = ComputeAngle (VEnd.x - VStart.x,
												 VEnd.y - VStart.y);
				LastSeg->linedef = n;
				LastSeg->flip    = 0;
				LastSeg->dist    = 0;
			}

			assert_vsdnum (CurLD.sidedef2);
			if (CurLD.sidedef2 >= 0)
			{
				if (seglist)
				{
					LastSeg->next = (SEPtr)GetMemory( sizeof( struct Seg));
					LastSeg       = LastSeg->next;
				}
				else
				{
					seglist = (SEPtr)GetMemory( sizeof( struct Seg));
					LastSeg = seglist;
				}
				LastSeg->next    = NULL;
				LastSeg->start   = CurLD.end;
				LastSeg->end     = CurLD.start;
				LastSeg->angle   = ComputeAngle(VStart.x - VEnd.x,
												VStart.y - VEnd.y);
				LastSeg->linedef = n;
				LastSeg->flip    = 1;
				LastSeg->dist    = 0;
			}
		} // End for
		ShowProgress (OBJ_VERTEXES);
		ShowProgress (OBJ_SIDEDEFS);

		LogMessage(": Starting Nodes builder...\n");
		LogMessage("\tNumber of Vertices: %d\n", NumVertexes);
		LogMessage("\tNumber of Segs:     %d\n", NumSegs);
		// Notify ("DEBUG: CreateNodes");

		if (CreateNodes (&Nodes, &n, seglist) == FALSE)
		{
			// Beep();
			// Beep();
			Beep();
			LogMessage("\nError: CreateNodes failed!\n\n");
			// Notify ("Error: Couldn't create the nodes.\n"
			//		"Your level may be unplayable!");
			// Beep();
			// Beep();
			// Beep();
			newnodes = TRUE;
		}
		else
		{
			LogMessage(": Nodes created OK.\n");
			LogMessage("\tNumber of Vertices: %d\n", NumVertexes);
			LogMessage("\tNumber of SideDefs: %d\n", NumSideDefs);
			LogMessage("\tNumber of Segs:     %d\n", NumSegs);
			LogMessage("\tNumber of SSectors: %d\n", NumSSectors);

			newnodes = TRUE;
		}
	}
	else
		newnodes = FALSE;

	//
	// output the LineDefs
	//
	for (n = 0; n < NumLineDefs; n++)
	{
		LineDef CurLD = LineDefs[n];
		WriteBytes (file, &(CurLD.start), 2L);
		WriteBytes (file, &(CurLD.end), 2L);
		WriteBytes (file, &(CurLD.flags), 2L);
		WriteBytes (file, &(CurLD.type), 2L);
		WriteBytes (file, &(CurLD.tag), 2L);
		WriteBytes (file, &(CurLD.sidedef1), 2L);
		WriteBytes (file, &(CurLD.sidedef2), 2L);
		counter += 14L;
	}
	dir = dir->next;

	//
	// output the SideDefs
	//
	for (n = 0; n < NumSideDefs; n++)
	{
		SideDef CurSD = SideDefs[n];
		WriteBytes (file, &(CurSD.xoff), 2L);
		WriteBytes (file, &(CurSD.yoff), 2L);
		WriteBytes (file, &(CurSD.tex1), 8L);
		WriteBytes (file, &(CurSD.tex2), 8L);
		WriteBytes (file, &(CurSD.tex3), 8L);
		WriteBytes (file, &(CurSD.sector), 2L);
		counter += 30L;
	}
	dir = dir->next;

	//
	// output the Vertices
	//
	if (MadeMapChanges)
	{
		// output the Vertices
		for (n = 0; n < NumVertexes; n++)
		{
			Vertex VCur = Vertexes[n];
			WriteBytes (file, &(VCur.x), 2L);
			WriteBytes (file, &(VCur.y), 2L);
			counter += 4L;
		}
	}
	else
	{
		// copy the Vertices
		size = dir->dir.size;
		counter += size;
		BasicWadSeek (dir->wadfile, dir->dir.start);
		CopyBytes (file, dir->wadfile->fileinfo, size);
	}
	dir = dir->next;

	if (newnodes)
	{
		SEPtr curse, oldse;
		SSPtr curss, oldss;

		// output and forget the Segments
		curse = Segs;
		while (curse)
		{
			WriteBytes (file, &(curse->start), 2L);
			WriteBytes (file, &(curse->end), 2L);
			WriteBytes (file, &(curse->angle), 2L);
			WriteBytes (file, &(curse->linedef), 2L);
			WriteBytes (file, &(curse->flip), 2L);
			WriteBytes (file, &(curse->dist), 2L);
			oldse = curse;
			curse = curse->next;
			FreeMemory (oldse);
			counter += 12L;
		}
		Segs = NULL;
		dir = dir->next;

		// output and forget the SSectors
		curss = SSectors;
		while (curss)
		{
			WriteBytes (file, &(curss->num), 2L);
			WriteBytes (file, &(curss->first), 2L);
			oldss = curss;
			curss = curss->next;
			FreeMemory (oldss);
			counter += 4L;
		}
		SSectors = NULL;
		dir = dir->next;

		// output the Nodes
		NumNodes = 0;
		SaveNodes (file, Nodes);
		counter += (ULONG) NumNodes * 28L;
		dir = dir->next;

		// forget the Nodes
		ForgetNodes(Nodes);
		Nodes = NULL;
	}
	else
	{
		// copy the Segs, SSectors and Nodes
		for (n = 0; n < 3; n++)
		{
			size = dir->dir.size;
			counter += size;
			BasicWadSeek (dir->wadfile, dir->dir.start);
			CopyBytes (file, dir->wadfile->fileinfo, size);
			dir = dir->next;
		}
	}

	//
	// output the Sectors
	//
	for (n = 0; n < NumSectors; n++)
	{
		Sector CurS = Sectors[n];
		WriteBytes (file, &(CurS.floorh), 2L);
		WriteBytes (file, &(CurS.ceilh), 2L);
		WriteBytes (file, &(CurS.floort), 8L);
		WriteBytes (file, &(CurS.ceilt), 8L);
		WriteBytes (file, &(CurS.light), 2L);
		WriteBytes (file, &(CurS.special), 2L);
		WriteBytes (file, &(CurS.tag), 2L);
		counter += 26L;
	}
	dir = dir->next;

	//
	// output REJECT data
	//
	if (newnodes)
	{
		// create and output the reject data
		pProgressDialog->ShowRejectControls();
		pProgressDialog->ShowRejectProgress(0);

		rejectsize = ((ULONG) NumSectors * (ULONG) NumSectors + 7L) / 8L;
		data = (BYTE HUGE *)GetMemory (rejectsize * sizeof(BYTE));

		/* RP: Done by GetMemory, but we never know */
		for (i = 0; i < rejectsize; i++)
			data[i] = (BYTE)0;

		for (i = 0; i < NumSectors; i++)
		{
			COOPERATE();	// Windows multi-tasking cooperation

			pProgressDialog->ShowRejectProgress((int) (((float) i / (float) NumSectors) * 100.0));

			for (j = 0; j < NumSectors; j++)
			{
/*
		if (Reject( i, j))
		   data[ (i * NumSectors + j) / 8] |= 1 <<
*/
			}
		}
		pProgressDialog->ShowRejectProgress(100);

		WriteBytes (file, data, rejectsize);
		counter += rejectsize;
		dir = dir->next;
		FreeMemory (data);
	}
	else
	{
		// copy the Reject data
		rejectsize = dir->dir.size;
		size = rejectsize;
		counter += size;
		BasicWadSeek (dir->wadfile, dir->dir.start);
		CopyBytes (file, dir->wadfile->fileinfo, size);
		dir = dir->next;
	}

	//
	// output BLOCKMAP data
	//
	if (newnodes)
	{
		SHORT mminx, mminy, mnumx, mnumy;

		// create and output the blockmap
		pProgressDialog->ShowBlockmapControls();
		pProgressDialog->ShowBlockmapProgress(0);

		mminx = (SHORT) (MapMinX / 8 - 8) * 8;
		WriteBytes (file, &mminx, 2L);

		mminy = (SHORT) (MapMinY / 8 - 8) * 8;
		WriteBytes (file, &mminy, 2L);

		mnumx = MapMaxX / 128 - mminx / 128 + 2;
		WriteBytes (file, &mnumx, 2L);

		mnumy = MapMaxY / 128 - mminy / 128 + 2;
		WriteBytes (file, &mnumy, 2L);

		counter += 8L;
		oldpos    = ftell (file);
		blocksize = (ULONG)mnumx * mnumy * sizeof(SHORT);
		blockptr  = (SHORT HUGE *)GetMemory (blocksize);
		WriteBytes (file, blockptr, blocksize);
		blocksize += 8L;
		counter   += blocksize - 7L;
		blockcount = mnumx * mnumy + 4;
		for (i = 0; i < mnumy; i++)
		{
			COOPERATE();	// Windows multi-tasking cooperation

			pProgressDialog->ShowBlockmapProgress((int) (((float) i / (float) mnumy) * 100.0) );

			for (j = 0; j < mnumx; j++)
			{
				COOPERATE();	// Windows multi-tasking cooperation

				blockptr [(LONG)mnumx * i + j] = blockcount;
				n = 0;
				WriteBytes (file, &n, 2L);
				counter += 2L;
				blocksize += 2L;
				blockcount++;
				for (n = 0; n < NumLineDefs; n++)
				{
					if ( (n & 15) == 0 )
						COOPERATE();	// Windows multi-tasking cooperation

					if (IsLineDefInside(n, mminx + j * 128,
										   mminy + i * 128,
										   mminx + 127 + j * 128,
										   mminy + 127 + i * 128))
					{
						WriteBytes (file, &n, 2L);
						counter += 2L;
						blocksize += 2L;
						blockcount++;
					}
				}
				n = -1;
				WriteBytes (file, &n, 2L);
				counter += 2L;
				blocksize += 2L;
				blockcount++;
			}
		}
		pProgressDialog->ShowBlockmapProgress(100);

		size = ftell (file);
		fseek (file, oldpos, SEEK_SET);
		WriteBytes (file, blockptr, (LONG)mnumx * mnumy * sizeof(SHORT));
		fseek (file, size, SEEK_SET);

		/*RP What is this? (protection!)*/
		/* removed, caused errors when adding level ExMx in DOOM2,   */
		/* because ExMx entries are further in the Master Directory  */
		/* than P2_END                                               */
		// if (FindMasterDir (dir, "P2_END"))
			counter--;
		FreeMemory (blockptr);
	}
	else
	{
		// copy the blockmap data
		blocksize = dir->dir.size;
		size    = blocksize;
		counter += size;
		BasicWadSeek (dir->wadfile, dir->dir.start);
		CopyBytes (file, dir->wadfile->fileinfo, size);
		dir = dir->next;
	}


	//
	// output the actual directory
	//
	dirstart = counter;
	counter = 12L;
	size = 0L;
	dir = Level;
	WriteBytes (file, &counter, 4L);
	WriteBytes (file, &size, 4L);
	WriteBytes (file, &(dir->dir.name), 8L);
	dir = dir->next;

	size = (ULONG) NumThings * 10L;
	WriteBytes (file, &counter, 4L);
	WriteBytes (file, &size, 4L);
	WriteBytes (file, "THINGS\0\0", 8L);
	counter += size;
	dir = dir->next;

	size = (ULONG) NumLineDefs * 14L;
	WriteBytes (file, &counter, 4L);
	WriteBytes (file, &size, 4L);
	WriteBytes (file, "LINEDEFS", 8L);
	counter += size;
	dir = dir->next;

	size = (ULONG) NumSideDefs * 30L;
	WriteBytes (file, &counter, 4L);
	WriteBytes (file, &size, 4L);
	WriteBytes (file, "SIDEDEFS", 8L);
	counter += size;
	dir = dir->next;

	if (MadeMapChanges)
		size = (ULONG) NumVertexes * 4L;
	else
		size = dir->dir.size;
	WriteBytes (file, &counter, 4L);
	WriteBytes (file, &size, 4L);
	WriteBytes (file, "VERTEXES", 8L);
	counter += size;
	dir = dir->next;

	if (newnodes)
		size = (ULONG) NumSegs * 12L;
	else
		size = dir->dir.size;
	WriteBytes (file, &counter, 4L);
	WriteBytes (file, &size, 4L);
	WriteBytes (file, "SEGS\0\0\0\0", 8L);
	counter += size;
	dir = dir->next;

	if (newnodes)
		size = (ULONG) NumSSectors * 4L;
	else
		size = dir->dir.size;
	WriteBytes (file, &counter, 4L);
	WriteBytes (file, &size, 4L);
	WriteBytes (file, "SSECTORS", 8L);
	counter += size;
	dir = dir->next;

	if (newnodes)
		size = (ULONG) NumNodes * 28L;
	else
		size = dir->dir.size;
	WriteBytes (file, &counter, 4L);
	WriteBytes (file, &size, 4L);
	WriteBytes (file, "NODES\0\0\0", 8L);
	counter += size;
	dir = dir->next;

	size = (ULONG) NumSectors * 26L;
	WriteBytes (file, &counter, 4L);
	WriteBytes (file, &size, 4L);
	WriteBytes (file, "SECTORS\0", 8L);
	counter += size;
	dir = dir->next;

	size = rejectsize;
	WriteBytes (file, &counter, 4L);
	WriteBytes (file, &size, 4L);
	WriteBytes (file, "REJECT\0\0", 8L);
	counter += size;
	dir = dir->next;

	size = blocksize;
	WriteBytes (file, &counter, 4L);
	WriteBytes (file, &size, 4L);
	WriteBytes (file, "BLOCKMAP", 8L);
	counter += size;
	dir = dir->next;

	// fix up the directory start information
	if (fseek (file, 8L, SEEK_SET))
		ProgError ("Error writing to PWAD file");
	WriteBytes (file, &dirstart, 4L);

	// close the file
	fclose (file);
	file = NULL;

	NumSegs = 0;
	NumSSectors = 0;
	NumNodes = 0;

	//
	// delete the vertices added by the Nodes builder
	//
	if (NumVertexes != oldNumVertexes)
	{
		assert (NumVertexes > oldNumVertexes);
		NumVertexes = oldNumVertexes;
		Vertexes = (VPtr)ResizeMemory (Vertexes, NumVertexes * sizeof (Vertex));
	}

	//
	// the file is now up to date
	//
	MadeChanges = FALSE;
	if (newnodes)
		MadeMapChanges = FALSE;

	//
	// update pointers in Master Directory
	//
	OpenPatchWad (outfile);

	//
	// this should free the old "*.BAK" file
	//
	CloseUnusedWadFiles();

	//
	// Destory progress dialog box
	//
	if ( pProgressDialog != NULL )
	{
		// Re-enable main window before closing modeless dialog box
		// to avoid Windows giving focus to another application.
		TFrameWindow *MainWindow = ((TApplication *)::Module)->GetMainWindow();
		MainWindow->EnableWindow(TRUE);
		MainWindow->ShowWindow(SW_SHOW);
		// Close dialog box.
		pProgressDialog->CloseWindow();
		delete pProgressDialog;
	}

	UNSELECT_WAIT_CURSOR();
	RESTORE_WORK_MSG();
}



/*
   function used by qsort to sort the texture names
*/
int _USERENTRY SortTextures (const void *a, const void *b)
{
	return strcmp ( (*(TextureInfo **)a)->Name, (*(TextureInfo **)b)->Name );
}


/*
   read in the wall texture names in entry "dirname"
*/

void AddWTextureInfo (char *dirname)
{
	ULONG numtex;

	// Add empty texture name at first place
	if ( WTexture == NULL )
	{
		WTexture = (TextureInfo **)GetMemory (sizeof(TextureInfo *));

		WTexture[0] = (TextureInfo *)GetMemory (sizeof(TextureInfo));
		strcpy (WTexture[0]->Name, "-");
		WTexture[0]->Width  = 0;
		WTexture[0]->Height = 0;
		NumWTexture = 1;
	}

	// Search for texture entry
	MDirPtr dir = FindMasterDir (MasterDir, dirname);
	if ( dir == NULL )
	{
		Notify ("Couldn't find wall texture entry: \"%s\"", dirname);
		return;
	}

	BasicWadSeek (dir->wadfile, dir->dir.start);
	BasicWadRead (dir->wadfile, &numtex, 4);

	// Alloc. array of TextureInfo Pointers
	assert (WTexture != NULL);
	assert (numtex + NumWTexture <= SHRT_MAX);

	SHORT OldNumWTexture = NumWTexture;
	NumWTexture += (SHORT)numtex;
	WTexture = (TextureInfo **)
		ResizeMemory (WTexture, NumWTexture * sizeof(TextureInfo *));

	// read in the offsets for texture names and info
	LONG HUGE *offsets = (LONG HUGE *)GetMemory (numtex * 4);
	BasicWadRead (dir->wadfile, offsets, 4L * numtex);

	// Read the texture info
	for (SHORT n = OldNumWTexture; n < NumWTexture; n++)
	{
		LONG dummy_long;

		BasicWadSeek (dir->wadfile, dir->dir.start + offsets[n - OldNumWTexture]);

		// Init. texture info
		WTexture[n] = (TextureInfo *)GetMemory (sizeof(TextureInfo));

		BasicWadRead (dir->wadfile, &WTexture[n]->Name, 8);
		WTexture[n]->Name[8] = '\0';
		BasicWadRead (dir->wadfile, &dummy_long, 4L);	// dummy read
		BasicWadRead (dir->wadfile, &WTexture[n]->Width, 2L);
		BasicWadRead (dir->wadfile, &WTexture[n]->Height, 2L);
	}
	FreeMemory (offsets);
}


/*
   read in the wall texture names
*/

void ReadWTextureInfo()
{
	SELECT_WAIT_CURSOR();
	SAVE_WORK_MSG();
	WorkMessage ("Reading Wall texture info ...");

	// Forget previous list of texture
	ForgetWTextureInfo();
	assert (WTexture == NULL);
	assert (NumWTexture == 0);

	// First texture file
	AddWTextureInfo ("TEXTURE1");

	// No Texture2 entry in DOOM2
	if (FindMasterDir (MasterDir, "TEXTURE2") != NULL)
		AddWTextureInfo ("TEXTURE2");

	// sort the names
	qsort (WTexture, NumWTexture, sizeof(TextureInfo *), SortTextures);

	RESTORE_WORK_MSG();
	UNSELECT_WAIT_CURSOR();
}



/*
   forget the wall texture names
*/

void ForgetWTextureInfo()
{
	SHORT n;

	/* forget all names */
	for (n = 0; n < NumWTexture; n++)
		FreeMemory (WTexture[n]);

	/* forget the array */
	NumWTexture = 0;
	if ( WTexture != NULL )
		FreeMemory (WTexture);
}


/*
   read in the floor/ceiling texture names between entry 'entry1' and 'entry2'
*/

void AddFTextureInfo (char *startEntry, char *endEntry)
{
	MDirPtr dir;
	ULONG   numtex;
	SHORT   OldNumFTexture;

	// count the names
	dir = FindMasterDir (MasterDir, startEntry);
	if ( dir == NULL )
		Notify ("Couldn't find the Floor/Ceiling texture entry \"%s\"",
				startEntry);

	dir = dir->next;
	for (numtex = 0L;
		 dir != NULL  &&  strcmp(dir->dir.name, endEntry) != 0 ;
		 numtex++)
	{
		dir = dir->next;
	}

	// Alloc array of TextureInfo pointers
	OldNumFTexture = NumFTexture;
	if ( FTexture == NULL )
	{
		assert (numtex < SHRT_MAX);
		NumFTexture = (SHORT)numtex;
		FTexture = (TextureInfo **)GetMemory (NumFTexture * sizeof(TextureInfo *));
	}
	else
	{
		assert (NumFTexture + numtex < SHRT_MAX);
		NumFTexture += (SHORT)numtex;
		FTexture = (TextureInfo **)
			ResizeMemory (FTexture, NumFTexture * sizeof(TextureInfo *));
	}

	/* get the actual names from master dir. */
	dir = FindMasterDir (MasterDir, startEntry);
	assert (dir != NULL);	// We found it 15 lines upper!

	dir = dir->next;
	for (SHORT n = OldNumFTexture; n < NumFTexture; n++)
	{
		// Alloc new texture info
		FTexture[n] = (TextureInfo *)GetMemory (sizeof(TextureInfo));

		FTexture[n]->Width  = 64;
		FTexture[n]->Height = 64;
		strncpy (FTexture[n]->Name, dir->dir.name, 8);
		FTexture[n]->Name[8] = '\0';

		dir = dir->next;
	}
}



/*
   read in the floor/ceiling texture names
*/

void ReadFTextureInfo()
{
	SELECT_WAIT_CURSOR();
	SAVE_WORK_MSG();
	WorkMessage ("Reading Floor/Ceiling texture info...");

	// Forget previous list of texture
	ForgetFTextureInfo();
	assert (FTexture == NULL);
	assert (NumFTexture == 0);

	// Read Floor/ceiling texture info
	AddFTextureInfo ("F1_START", "F1_END");

	if (FindMasterDir (MasterDir, "F2_START") != NULL)
		AddFTextureInfo ("F2_START", "F2_END");

	// DOOM 2 only
	if (FindMasterDir (MasterDir, "F3_START") != NULL)
		AddFTextureInfo ("F3_START", "F3_END");

	// sort the names
	qsort (FTexture, NumFTexture, sizeof(TextureInfo *), SortTextures);

	RESTORE_WORK_MSG();
	UNSELECT_WAIT_CURSOR();
}



/*
   forget the floor/ceiling texture names
*/

void ForgetFTextureInfo()
{
   SHORT n;

   /* forget all names */
   for (n = 0; n < NumFTexture; n++)
	  FreeMemory (FTexture[n]);

   /* forget the array */
   NumFTexture = 0;
   if ( FTexture != NULL )		FreeMemory (FTexture);
}


/*
   Function to get the size of a wall texture
*/

void GetWallTextureSize (SHORT *xsize_r, SHORT *ysize_r, char *texname)
{
	assert (WTexture!=NULL);

	for (SHORT ti = 0 ; ti < NumWTexture ; ti++ )
	{
		assert (WTexture[ti] != NULL);

		if ( strnicmp (WTexture[ti]->Name, texname, 8) == 0 )
		{
			*xsize_r = WTexture[ti]->Width;
			*ysize_r = WTexture[ti]->Height;
			break ;
		}
	}

	if ( ti == NumWTexture )
	{
		Notify ("Couldn't find Wall texture info: \"%s\" (BUG)!", texname);
	}
}


/* end of file */
