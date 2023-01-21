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

 	FILE:         nodes.cpp

	OVERVIEW
	========
	Source file for automatic builder for Nodes, Segs and SSectors.

   *------- PLEASE READ THE COMMENT AT THE END OF THIS FILE. -------*
   | If you use the algorithm or even some of the ideas taken from  |
   | this file, you must put a message in your program, so that the |
   | user knows that all or part of the algorithm comes from DEU.   |
   *------- PLEASE READ THE COMMENT AT THE END OF THIS FILE. -------*
*/

#include "common.h"
#pragma hdrstop

#ifndef __nodes_h
	#include "nodes.h"
#endif

#ifndef __levels_h
	#include "levels.h"
#endif

#ifndef __memory_h
	#include "memory.h"
#endif

#ifndef __objects_h
	#include "objects.h"
#endif

#ifndef __gfx_h
	#include "gfx.h"	// ComputeDist
#endif


/*
   find the point of intersection for two lines (return FALSE if there is none)
*/

BOOL ComputeIntersection (SHORT *x, SHORT *y, SEPtr seg1, SEPtr seg2)
{
	assert_vnum (seg1->start);
	assert_vnum (seg1->end);
	assert_vnum (seg2->start);
	assert_vnum (seg2->end);

	/* floating-point required because long integers cause errors */
	Vertex V1Start = Vertexes[seg1->start];
	Vertex V1End   = Vertexes[seg1->end];
	Vertex V2Start = Vertexes[seg2->start];
	Vertex V2End   = Vertexes[seg2->end];

	double x1  = V1Start.x;
	double y1  = V1Start.y;
	double dx1 = V1End.x - x1;
	double dy1 = V1End.y - y1;
	double x2  = V2Start.x;
	double y2  = V2Start.y;
	double dx2 = V2End.x - x2;
	double dy2 = V2End.y - y2;
	double d;

	d = dy1 * dx2 - dx1 * dy2;
	if (d != 0.0)
	{
		x1 = y1 * dx1 - x1 * dy1;
		x2 = y2 * dx2 - x2 * dy2;
		/* (*x, *y) = intersection */
		*x = (SHORT) ((dx1 * x2 - dx2 * x1) / d);
		*y = (SHORT) ((dy1 * x2 - dy2 * x1) / d);

		/* check if the intersection is not at one end of a Seg (vertex grid = 8*8) */
		if ( *x >= V1Start.x - 7  &&  *x <= V1Start.x + 7 &&
			 *y >= V1Start.y - 7  &&  *y <= V1Start.y + 7 )
		{
			return FALSE; /* not a real intersection point (round-off error in a previous operation) */
		}

		if ( *x >= V1End.x - 7   &&  *x <= V1End.x + 7 &&
			 *y >= V1End.y - 7   &&  *y <= V1End.y + 7 )
		{
			return FALSE; /* not a real intersection point (round-off error in a previous operation) */
		}

		if ( *x >= V2Start.x - 7  &&  *x <= V2Start.x + 7 &&
			 *y >= V2Start.y - 7  &&  *y <= V2Start.y + 7 )
		{
			return FALSE; /* not a real intersection point (round-off error in a previous operation) */
		}

		if ( *x >= V2End.x - 7  &&  *x <= V2End.x + 7 &&
			 *y >= V2End.y - 7  &&  *y <= V2End.y + 7 )
		{
			return FALSE; /* not a real intersection point (round-off error in a previous operation) */
		}
		return TRUE; /* intersection OK */
	}

	return FALSE; /* parallel lines */
}



/*
   choose a nodeline amongst the list of Segs
*/

SEPtr FindNodeLine (SEPtr seglist)
{
	SHORT splits;
#ifdef OLD_ALGORITHM
	SHORT minsplits = 32767;
#endif /* OLD_ALGORITHM */
	SHORT mindiff = 32767;
	SHORT num1, num2;
	SEPtr nodeline, bestnodeline;
	SEPtr curseg;
	LONG  x, y;
	LONG  dx, dy;
	LONG  a, b, c, d;
	SHORT dummyx, dummyy;
	SHORT coop;
	/* ***DEBUG*** */
	static SEPtr lastnodeline = NULL;

	/* find nodeline - brute force: try with all Segs */
	bestnodeline = NULL;
	for (nodeline = seglist ; nodeline ; nodeline = nodeline->next)
	{
		COOPERATE();	// Windows multi-tasking cooperation

		assert_vnum(nodeline->start);
		assert_vnum(nodeline->end);
		/* compute x, y, dx, dy */
		Vertex VNStart = Vertexes[nodeline->start];
		Vertex VNEnd   = Vertexes[nodeline->end];

		x = VNStart.x;
		y = VNStart.y;
		dx = VNEnd.x - x;
		dy = VNEnd.y - y;

		/* compute number of splits */
		if (dx == 0 || dy == 0)
			splits = 0;
		else
			splits = 1; /* small penalty for oblique lines */
		num1 = 0;
		num2 = 0;
		for (curseg = seglist, coop=0; curseg; curseg = curseg->next, coop++)
		{
			if ( (coop & (16-1)) == 0 )
				COOPERATE();	// Windows multi-tasking cooperation

			if (curseg == nodeline)
			{
				num1++;
				continue;
			}
			/* you love maths, don't you? */
			assert_vnum(curseg->start);
			assert_vnum(curseg->end);
			Vertex VCStart = Vertexes[curseg->start];
			Vertex VCEnd   = Vertexes[curseg->end];

			a = ((LONG)VCStart.x - x) * dy;
			b = ((LONG)VCStart.y - y) * dx;
			c = ((LONG)VCEnd.x   - x) * dy;
			d = ((LONG)VCEnd.y   - y) * dx;

			if ((a != b) && (c != d) && ((a > b) != (c > d)) &&
				ComputeIntersection (&dummyx, &dummyy, nodeline, curseg))
			{
				splits++; /* one more split */
				num1++;
				num2++;
			}
			else if ( (a > b) ||
					  ( (a == b) && (c >  d) ) ||
					  ( (a == b) && (c == d) &&
						((dx > 0) == ((VCEnd.x - VCStart.x) > 0)) &&
						((dy > 0) == ((VCEnd.y - VCStart.y) > 0)) ))
			{
				num1++; /* one more Seg on the first (right) side */
			}
			else
				num2++; /* one more Seg on the second (left) side */
#ifdef OLD_ALGORITHM
			if (splits > minsplits)
				break;  /* don't waste time */
#else
			if (max( num1, num2) + SplitFactor * splits > mindiff)
				break;  /* don't waste time */
#endif /* OLD_ALGORITHM */
		}

		/* there must be at least one Seg on each side */
		if (num1 > 0 && num2 > 0)
		{
#ifdef OLD_ALGORITHM
			/* now, num1 = difference in number of Segs between two sides */
			if (num1 > num2)
				num1 = num1 - num2;
			else
				num1 = num2 - num1;
			/* minimal number of splits = candidate for nodeline */
			if (splits < minsplits || (splits == minsplits && num1 < mindiff))
			{
				minsplits = splits; /* minimal number of splits */
				mindiff = num1; /* minimal difference between the two sides */
				bestnodeline = nodeline; /* save the nodeline */
			}
#else
			/* now, num1 = rating for this nodeline */
			num1 = max( num1, num2) + SplitFactor * splits;
			/* this nodeline is better than the previous one */
			if (num1 < mindiff)
			{
				mindiff = num1; /* save the rating */
				bestnodeline = nodeline; /* save the nodeline */
			}
#endif /* OLD_ALGORITHM */
		}
	}

	/* ***DEBUG*** */
	if (bestnodeline && bestnodeline == lastnodeline)
		ProgError ("Nodeline picked twice (this is a BUG!)");

	lastnodeline = nodeline;

	return bestnodeline;
}



/*
   Move a Seg into a list and update the bounding box
*/

void StoreInSegList (SEPtr seg, SEPtr *seglist, SEPtr *slistend)
{
	if (*seglist)
	{
		assert ((*slistend) != NULL);
		(*slistend)->next = seg;
		*slistend = (*slistend)->next;
	}
	else
	{
		*seglist = seg;
		*slistend = *seglist;
	}

	assert ((*slistend) != NULL);
	(*slistend)->next = NULL;
}



/*
   compute the bounding box (limits on X, Y) for a list of Segs
*/

void ComputeBoundingBox(SEPtr seglist, SHORT *minx, SHORT *maxx,
									   SHORT *miny, SHORT *maxy)
{
	SEPtr curseg;

	*maxx = -32767;
	*maxy = -32767;
	*minx = 32767;
	*miny = 32767;
	SHORT coop;
	for (curseg = seglist, coop = 0; curseg; curseg = curseg->next, coop++)
	{
		if ( (coop & (32-1)) == 0 )
			COOPERATE();	// Windows multi-tasking cooperation

		Vertex VCur;
		SHORT x, y;

		assert_vnum(curseg->start);
		VCur = Vertexes[curseg->start];
		x = VCur.x;
		y = VCur.y;

		if (x < *minx)        *minx = x;
		if (x > *maxx)        *maxx = x;
		if (y < *miny)        *miny = y;
		if (y > *maxy)        *maxy = y;

		assert_vnum(curseg->end);
		VCur = Vertexes[curseg->end];
		x = VCur.x;
		y = VCur.y;

		if (x < *minx)        *minx = x;
		if (x > *maxx)        *maxx = x;
		if (y < *miny)        *miny = y;
		if (y > *maxy)        *maxy = y;
	}
}



/*
   create a SSector from a list of Segs
*/

SHORT CreateSSector (SEPtr seglist)
{
	COOPERATE();	// Windows multi-tasking cooperation

	/* update the SSectors list */
	NumSSectors++;
	if (SSectors)
	{
		LastSSector->next = (SSPtr)GetMemory (sizeof(SSector));
		LastSSector = LastSSector->next;
	}
	else
	{
		SSectors = (SSPtr)GetMemory (sizeof(SSector));
		LastSSector = SSectors;
	}
	LastSSector->next = NULL;

	/* number of first Segment in this SubSector */
	LastSSector->first = NumSegs;

	/* update the Segs list */
	if (Segs == NULL)
		Segs = seglist;
	else
		LastSeg->next = seglist;
	NumSegs++;

	//BUG: in the for loop: If LastSeg == NULL, we access LastSeg->next anyway
	// So added this instruction
	if ( seglist != NULL )
		for (LastSeg = seglist; LastSeg->next; LastSeg = LastSeg->next)
			NumSegs++;

	/* total number of Segments in this SubSector */
	LastSSector->num = NumSegs - LastSSector->first;

	/* while the user is waiting... */
	ShowProgress (OBJ_SSECTORS);

	/* return the number of this SubSector */
	return NumSSectors - 1;
}



/*
   create all Nodes from a list of Segs
*/

BOOL CreateNodes (NPtr *node_r, SHORT *ssector_r, SEPtr seglist)
{
	NPtr         node;
	SEPtr        segs1, segs2;
	static SEPtr nodeline, curseg;
	static LONG  a, b, c, d;
	static SEPtr lastseg1, lastseg2;

	/* new Node */
	node = (NPtr)GetMemory (sizeof(Node));

	/* find the best nodeline */
	nodeline = FindNodeLine (seglist);

	/* nodeline could not be found: return a SSector */
	if (nodeline == NULL)
	{
		*node_r = NULL;
		*ssector_r = CreateSSector(seglist) | 0x8000;
		return FALSE;
	}

	/* compute x, y, dx, dy */
	assert_vnum (nodeline->start);
	assert_vnum (nodeline->end);
	Vertex VStart = Vertexes[nodeline->start];
	Vertex VEnd   = Vertexes[nodeline->end];
	node->x = VStart.x;
	node->y = VStart.y;
	node->dx = VEnd.x - node->x;
	node->dy = VEnd.y - node->y;

	/* split seglist in segs1 and segs2 */
	segs1 = NULL;
	segs2 = NULL;
	while (seglist)
	{
		COOPERATE();	// Windows multi-tasking cooperation

		curseg  = seglist;
		seglist = seglist->next;

		assert_vnum (curseg->start);
		assert_vnum (curseg->end);
		Vertex VStart = Vertexes[curseg->start];
		Vertex VEnd   = Vertexes[curseg->end];

		/* now, where is that old book about analytic geometry? */
		a = (LONG) (VStart.x - node->x) * (LONG) (node->dy);
		b = (LONG) (VStart.y - node->y) * (LONG) (node->dx);
		c = (LONG) (VEnd.x   - node->x) * (LONG) (node->dy);
		d = (LONG) (VEnd.y   - node->y) * (LONG) (node->dx);

		// check if starting vertex is on the right side of the nodeline,
		// or if starting vertex is on the nodeline and ending vertex on the
		//  right side,
		// or if both are on the nodeline and the Seg has the same orientation
		//  as the nodeline.
		if ( (a > b) || ((a == b) && (c > d)) || ((a == b) && (c == d) &&
			 ((node->dx > 0) == ((VEnd.x - VStart.x) > 0)) &&
			 ((node->dy > 0) == ((VEnd.y - VStart.y) > 0))))
		{
			/* the starting Vertex is on the first side (right) of the nodeline */
			StoreInSegList (curseg, &segs1, &lastseg1);
			if (c < d)
			{
				SHORT newx, newy;

				/* the ending Vertex is on the other side: split the Seg in two */
				if (ComputeIntersection (&newx, &newy, nodeline, curseg))
				{
					Vertex VLast = Vertexes[lastseg1->start];

					InsertObject (OBJ_VERTEXES, -2, newx, newy);
					StoreInSegList ((SEPtr)GetMemory (sizeof (Seg)), &segs2, &lastseg2);
					lastseg2->start   = NumVertexes - 1;
					lastseg2->end     = lastseg1->end;
					lastseg2->angle   = lastseg1->angle;
					lastseg2->linedef = lastseg1->linedef;
					lastseg2->flip    = lastseg1->flip;
					lastseg2->dist    = lastseg1->dist +
										   ComputeDist (newx - VLast.x,
														newy - VLast.y);
					lastseg1->end = NumVertexes - 1;
					ShowProgress (OBJ_VERTEXES);
				}
			}
		}
		else
		{
			/* the starting Vertex is on the second side (left) of the nodeline */
			StoreInSegList (curseg, &segs2, &lastseg2);
			if (c > d)
			{
				SHORT newx, newy;

				/* the ending Vertex is on the other side: split the Seg in two */
				if (ComputeIntersection (&newx, &newy, nodeline, curseg))
				{
					Vertex VLast = Vertexes[lastseg2->start];

					InsertObject (OBJ_VERTEXES, -2, newx, newy);
					StoreInSegList ((SEPtr)GetMemory (sizeof (Seg)), &segs1, &lastseg1);
					lastseg1->start = NumVertexes - 1;
					lastseg1->end = lastseg2->end;
					lastseg1->angle = lastseg2->angle;
					lastseg1->linedef = lastseg2->linedef;
					lastseg1->flip = lastseg2->flip;
					lastseg1->dist = lastseg2->dist +
									 ComputeDist (newx - VLast.x,
												  newy - VLast.y);
					lastseg2->end = NumVertexes - 1;
					ShowProgress (OBJ_VERTEXES);
				}
			}
		}
	}

	/* now, we should have all the Segs in segs1 and segs2 (seglist is empty) */
	if (segs1 == NULL || segs2 == NULL)
		ProgError("Could not split the Segs list (this is a BUG!)");

	/* compute bounding box limits for segs1 */
	ComputeBoundingBox (segs1, &(node->minx1), &(node->maxx1),
							   &(node->miny1), &(node->maxy1));

	/* create Nodes or SSectors from segs1 */
	CreateNodes (&(node->node1), &(node->child1), segs1);

	/* compute bounding box limits for segs2 */
	ComputeBoundingBox (segs2, &(node->minx2), &(node->maxx2),
							   &(node->miny2), &(node->maxy2));

	/* create Nodes or SSectors from segs2 */
	CreateNodes (&(node->node2), &(node->child2), segs2);

	/* this Node is OK */
	*node_r = node;
	*ssector_r = 0;
	return TRUE;
}


/*
   IF YOU ARE WRITING A DOOM EDITOR OR ANOTHER ADD-ON, PLEASE READ THIS:

   I spent a lot of time writing the Nodes builder.  There may be some bugs in
   it, but most of the code is OK.  If you steal any ideas from this program,
   put a prominent message in your own editor (i.e. it must be displayed when
   the program starts or in an "about" box) to make it CLEAR that some
   original ideas were taken from DEU.  You need not credit me.  Just credit
   DEU and its contributors.  Thanks.

   While everyone was talking about LineDefs, I had the idea of taking only
   the Segs into account, and creating the Segs directly from the SideDefs.
   Also, dividing the list of Segs in two after each call to CreateNodes makes
   the algorithm faster.  I use several other tricks, such as looking at the
   two ends of a Seg to see on which side of the nodeline it lies or if it
   should be split in two.  I took me a lot of time and efforts to do this.

   I give this algorithm to whoever wants to use it, but with this condition:
   if your program uses SOME of the IDEAS from DEU or the whole ALGORITHM, you
   MUST tell it to the user.  And if you post a message with all or parts of
   this algorithm in it, please post THIS NOTICE also.  I don't want to speak
   legalese; I hope that you understand me...  I kindly give the sources of my
   program to you: please be kind with me...

   If you need more information about this, here is my E-mail address:
   quinet@montefiore.ulg.ac.be (Rapha‰l Quinet).

   Short description of the algorithm:
	 1 - Create one Seg for each SideDef: pick each LineDef in turn.  If it
	 has a "first" SideDef, then create a normal Seg.  If it has a
	 "second" SideDef, then create a flipped Seg.
	 2 - Call CreateNodes with the current list of Segs.  The list of Segs is
	 the only argument to CreateNodes.
	 3 - Save the Nodes, Segs and SSectors to disk.  Start with the leaves of
	 the Nodes tree and continue up to the root (last Node).

   CreateNodes does the following:
	 1 - Pick a nodeline amongst the Segs (minimize the number of splits and
	 keep the tree as balanced as possible).
	 2 - Move all Segs on the right of the nodeline in a list (segs1) and
	 move all Segs on the left of the nodeline in another list (segs2).
	 3 - If the first list (segs1) contains references to more than one
	 Sector or if the angle between two adjacent Segs is greater than
	 180°, then call CreateNodes with this (smaller) list.  Else, create
	 a SubSector with all these Segs.
	 4 - Do the same for the second list (segs2).
	 5 - Return the new node (its two children are already OK).

   Each time CreateSSector is called, the Segs are put in a global list.
   When there is no more Seg in CreateNodes' list, then they are all in the
   global list and ready to be saved to disk.

   Note: now that the nice guys at Id software have released their algorithm,
   I have changed the way CreateNodes work.  Instead of checking if the Segs
   list should be split, I try to find a nodeline.  If I found one, then I
   split the list of Segs and call CreateNodes on both lists.  Else, I just
   return a SSector which contains the list of Segs.
*/


/* end of file */
