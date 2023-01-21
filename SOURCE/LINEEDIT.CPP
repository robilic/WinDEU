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

	FILE:         lineedit.cpp

	OVERVIEW
	========
	Source file for implementation of TLineDefEditDialog (TDialog).
*/

#include "common.h"
#pragma hdrstop

#include "windeu.h"

#include "lineedit.h"

#ifndef __OWL_LISTBOX_H
	#include <owl\listbox.h>
#endif

#ifndef __OWL_EDIT_H
	#include <owl\edit.h>
#endif

#ifndef __OWL_RADIOBUT_H
	#include <owl\radiobut.h>
#endif

#ifndef __OWL_CHECKBOX_H
	#include <owl\checkbox.h>
#endif

#ifndef __OWL_VALIDATE_H
	#include <owl\validate.h>
#endif

#ifndef __cnflddlg_h
	#include "cnflddlg.h"
#endif

#ifndef __names_h
	#include "names.h"
#endif

#ifndef __newers_h
	#include "newers.h"		// new...
#endif

#ifndef __memory_h
	#include "memory.h"
#endif

#ifndef __cnflddlg_h
	#include "cnflddlg.h"
#endif

#ifndef __viewbmp_h
	#include "viewbmp.h"
#endif

// HELP context IDs
#include "windeuhl.h"


/////////////////////////////////////////////////////////////////////
//
// DOOM1 & HERETIC LineDef types
//
/////////////////////////////////////////////////////////////////////
const SHORT D1_DoorsTab[] =
{
	// DOOM1/HERETIC
	0,	// Normal
	1, 26, 27, 28, 63, 29, 90, 4, 31, 32, 34, 33, 61, 103, 50, 86,
	2, 46, 42, 75, 3, 76, 16,
	-1
};

const SHORT D1_CeilingsTab[] =
{
	// DOOM1/HERETIC
	43, 41, 72, 44, 40,
	-1
};

const SHORT D1_RaiseFloorsTab[] =
{
	// DOOM1/HERETIC
	69, 18, 20, 22, 95, 47, 5, 14, 30, 93, 59, 66, 67, 15, 92,
	58, 96, 64, 101, 91, 24, 65, 94, 56,
	-1
};

const SHORT D1_LowerFloorsTab[] =
{
	// DOOM1/HERETIC
	45, 60, 102, 82, 83, 19, 38, 70, 71, 98, 36, 23, 84, 68, 37,
	9, 21,
	-1
};

const SHORT D1_MovingThingsTab[] =
{
	// DOOM1/HERETIC
	62, 88, 10, 77, 6, 73, 74, 57, 87, 53, 89, 54, 7, 8,
	-1
};

const SHORT D1_SpecialTab[] =
{
	// DOOM1/HERETIC
	48, 11, 52, 51, 97, 39, 81, 13, 79, 35, 80, 12, 104, 17,
	-1
};

#define LD_DOORS		0
#define LD_CEILINGS		1
#define LD_RAISEFLOORS	2
#define LD_LOWERFLOORS	3
#define LD_MOVINGTHINGS	4
#define LD_SPECIAL      5

const SHORT *D1_LineDefSetTab[] =
{
	D1_DoorsTab,
	D1_CeilingsTab,
	D1_RaiseFloorsTab,
	D1_LowerFloorsTab,
	D1_MovingThingsTab,
	D1_SpecialTab
};

// const NB_LINEDEFSET = sizeof(LineDefSetTab) / sizeof (int *);
const NB_LINEDEFSET = 6;


/////////////////////////////////////////////////////////////////////
//
// DOOM2 new LineDefs
//
/////////////////////////////////////////////////////////////////////
const SHORT D2_DoorsTab[] =
{
	// DOOM1/HERETIC
	0,	// Normal
	1, 26, 27, 28, 63, 29, 90, 4, 31, 32, 34, 33, 61, 103, 50, 86,
	2, 46, 42, 75, 3, 76, 16,
	// DOOM2
	99, 100, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
	116, 117, 118, 133, 134, 135, 136, 137,
	-1
};

const SHORT D2_CeilingsTab[] =
{
	// DOOM1/HERETIC
	43, 41, 72, 44, 40,
	// DOOM2
	25, 49, 85, 141,
	-1
};

const SHORT D2_RaiseFloorsTab[] =
{
	// DOOM1/HERETIC
	69, 18, 20, 22, 95, 47, 5, 14, 30, 93, 59, 66, 67, 15, 92,
	58, 96, 64, 101, 91, 24, 65, 94, 56,
	// DOOM2
	55, 119, 128, 129, 130, 131, 132, 140,
	-1
};

const SHORT D2_LowerFloorsTab[] =
{
	// DOOM1/HERETIC
	45, 60, 102, 82, 83, 19, 38, 70, 71, 98, 36, 23, 84, 68, 37,
	9, 21,
	-1
};

const SHORT D2_MovingThingsTab[] =
{
	// DOOM1/HERETIC
	62, 88, 10, 77, 6, 73, 74, 57, 87, 53, 89, 54, 7, 8,
	// DOOM2
	120, 121, 122, 123, 127,
	-1
};

const SHORT D2_SpecialTab[] =
{
	// DOOM1/HERETIC
	48, 11, 52, 51, 97, 39, 81, 13, 79, 35, 80, 12, 104, 17,
	// DOOM2
	125, 126, 124, 78, 138, 139,
	-1
};

const SHORT *D2_LineDefSetTab[] =
{
	D2_DoorsTab,
	D2_CeilingsTab,
	D2_RaiseFloorsTab,
	D2_LowerFloorsTab,
	D2_MovingThingsTab,
	D2_SpecialTab
};


inline const SHORT *GetLineDefTab(int i)
{
	if ( DoomVersion == 2 )
		return D2_LineDefSetTab[i];
	else
		return D1_LineDefSetTab[i];
}


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TLineDefEditDialog, TDialog)
//{{TLineDefEditDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_DOORS, DoorsClicked),
	EV_BN_CLICKED(IDC_CEILINGS, CeilingsClicked),
	EV_BN_CLICKED(IDC_LOWER_FLOORS, LowerFloorsClicked),
	EV_BN_CLICKED(IDC_RAISE_FLOORS, RaiseFloorsClicked),
	EV_BN_CLICKED(IDC_MOVING_THINGS, MovingThingsClicked),
	EV_BN_CLICKED(IDC_SPECIAL, SpecialClicked),
	EV_BN_CLICKED(IDC_SD1_CLEAR_ABOVE, SD1ClearAboveClicked),
	EV_BN_CLICKED(IDC_SD1_CLEAR_BELOW, SD1ClearBelowClicked),
	EV_BN_CLICKED(IDC_SD1_CLEAR_NORMAL, SD1ClearNormalClicked),
	EV_BN_CLICKED(IDC_SD1_TO_NORMAL, SD1ToNormalClicked),
	EV_BN_CLICKED(IDC_SD1_TO_ABOVE, SD1ToAboveClicked),
	EV_BN_CLICKED(IDC_SD1_TO_BELOW, SD1ToBelowClicked),
	EV_BN_CLICKED(IDC_SD2_CLEAR_ABOVE, SD2ClearAboveClicked),
	EV_BN_CLICKED(IDC_SD2_CLEAR_BELOW, SD2ClearBelowClicked),
	EV_BN_CLICKED(IDC_SD2_CLEAR_NORMAL, SD2ClearNormalClicked),
	EV_BN_CLICKED(IDC_SD2_TO_ABOVE, SD2ToAboveClicked),
	EV_BN_CLICKED(IDC_SD2_TO_BELOW, SD2ToBelowClicked),
	EV_BN_CLICKED(IDC_SD2_TO_NORMAL, SD2ToNormalClicked),
	EV_LBN_SELCHANGE(IDC_LINEDEF_LIST, LineDefSelChange),
	EV_LBN_DBLCLK(IDC_TEXTURE_LIST, TextureListDBLClick),
	EV_EN_KILLFOCUS(IDC_SD1, SD1Killfocus),
	EV_EN_KILLFOCUS(IDC_SD2, SD2Killfocus),
	EV_LBN_SELCHANGE(IDC_TEXTURE_LIST, TextureSelchange),
	EV_BN_CLICKED(IDC_SD1_NEW, SD1NewClicked),
	EV_BN_CLICKED(IDC_SD2_NEW, SD2NewClicked),
	EV_BN_CLICKED(IDC_STAG_NEW, SectorTagNewClicked),
	EV_BN_CLICKED(IDOK, CmOk),
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONDBLCLK,
	EV_BN_CLICKED(IDC_ABOVEUNP, AboveUnpClicked),
	EV_BN_CLICKED(IDC_BELOWUNP, BelowUnpClicked),
	EV_BN_CLICKED(IDC_BLOCKSOUND, BlockSoundClicked),
	EV_BN_CLICKED(IDC_IMPASSABLE, ImpassableClicked),
	EV_BN_CLICKED(IDC_MONCROSS, MonCrossClicked),
	EV_BN_CLICKED(IDC_NOTSHOWN, NotShownClicked),
	EV_BN_CLICKED(IDC_SECRET, SecretClicked),
	EV_BN_CLICKED(IDC_SHOWN, ShownClicked),
	EV_BN_CLICKED(IDC_TWOSIDED, TwoSidedClicked),
//{{TLineDefEditDialogRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TLineDefEditDialog Implementation}}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
TLineDefEditDialog::TLineDefEditDialog (TWindow* parent, SelPtr Selected, TResId resId, TModule* module):
	TDialog(parent, resId, module)
{
	assert (Selected != NULL);
	assert (Selected->objnum >= 0  && Selected->objnum < NumLineDefs);

	LineDefSet = -1;
	TextureName[0] = '\0';
	SelLineDefs = Selected;
	CurLineDef = LineDefs[Selected->objnum];
	memset(&ConfirmData, 0, sizeof(ConfirmData));

	// Crete object for texture view, but do not create dialog box.
	pWTextureDialog = NULL;

	// Create objects for controls
	pAboveUnpCheck     = newTCheckBox(this, IDC_ABOVEUNP, 0);
	pBelowUnpCheck     = newTCheckBox(this, IDC_BELOWUNP, 0);
	pBlockSoundCheck   = newTCheckBox(this, IDC_BLOCKSOUND, 0);
	pImpassableCheck   = newTCheckBox(this, IDC_IMPASSABLE, 0);
	pMonsterCrossCheck = newTCheckBox(this, IDC_MONCROSS, 0);
	pNotShownCheck     = newTCheckBox(this, IDC_NOTSHOWN, 0);
	pSecretCheck       = newTCheckBox(this, IDC_SECRET, 0);
	pShownCheck        = newTCheckBox(this, IDC_SHOWN, 0);
	pTwoSidedCheck     = newTCheckBox(this, IDC_TWOSIDED, 0);

	pSectorTagEdit = newTEdit(this, IDC_SECTOR_TAG, 9);
	pVertex1Edit   = newTEdit(this, IDC_VERTEX2, 9);
	pVertex2Edit   = newTEdit(this, IDC_VERTEX1, 9);

	pSDEdit[0] = newTEdit(this, IDC_SD1, 9);
	pSDEdit[1] = newTEdit(this, IDC_SD2, 9);

	pSDXPosEdit[0]     = newTEdit(this, IDC_SD1_XPOS, 9);
	pSDYPosEdit[0]     = newTEdit(this, IDC_SD1_YPOS, 9);
	pSDSectorEdit[0]   = newTEdit(this, IDC_SD1_SECTOR, 9);
	pSDAboveStatic[0]  = newTStatic(this, IDC_SD1_ABOVE, 9);
	pSDBelowStatic[0]  = newTStatic(this, IDC_SD1_BELOW, 9);
	pSDNormalStatic[0] = newTStatic(this, IDC_SD1_NORMAL, 9);

	pSDXPosEdit[1]     = newTEdit(this, IDC_SD2_XPOS, 9);
	pSDYPosEdit[1]     = newTEdit(this, IDC_SD2_YPOS, 9);
	pSDSectorEdit[1]   = newTEdit(this, IDC_SD2_SECTOR, 9);
	pSDAboveStatic[1]  = newTStatic(this, IDC_SD2_ABOVE, 9);
	pSDBelowStatic[1]  = newTStatic(this, IDC_SD2_BELOW, 9);
	pSDNormalStatic[1] = newTStatic(this, IDC_SD2_NORMAL, 9);

	pTextureList = newTListBox(this, IDC_TEXTURE_LIST);
	pLineDefList = newTListBox(this, IDC_LINEDEF_LIST);

	pSDToAbove[0]     = newTButton(this, IDC_SD1_TO_ABOVE);
	pSDToBelow[0]     = newTButton(this, IDC_SD1_TO_BELOW);
	pSDToNormal[0]    = newTButton(this, IDC_SD1_TO_NORMAL);
	pSDClearAbove[0]  = newTButton(this, IDC_SD1_CLEAR_ABOVE);
	pSDClearBelow[0]  = newTButton(this, IDC_SD1_CLEAR_BELOW);
	pSDClearNormal[0] = newTButton(this, IDC_SD1_CLEAR_NORMAL);

	pSDToAbove[1]     = newTButton(this, IDC_SD2_TO_ABOVE);
	pSDToBelow[1]     = newTButton(this, IDC_SD2_TO_BELOW);
	pSDToNormal[1]    = newTButton(this, IDC_SD2_TO_NORMAL);
	pSDClearAbove[1]  = newTButton(this, IDC_SD2_CLEAR_ABOVE);
	pSDClearBelow[1]  = newTButton(this, IDC_SD2_CLEAR_BELOW);
	pSDClearNormal[1] = newTButton(this, IDC_SD2_CLEAR_NORMAL);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
TLineDefEditDialog::~TLineDefEditDialog ()
{
	Destroy();
	delete pWTextureDialog;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SetupWindow ()
{
	TDialog::SetupWindow();
	::CenterWindow (this);

	InitLineDefSet();
	SetLineDefList();
	SetTextureList();
	SetLineDef();
	SetSideDef(0);
	SetSideDef(1);

	// Setup validators for edit controls
	pSectorTagEdit->SetValidator (new TRangeValidator (-1, NumSectors-1));
	pSDEdit[0]->SetValidator (new TRangeValidator (-1, NumSideDefs-1));
	pSDEdit[1]->SetValidator (new TRangeValidator (-1, NumSideDefs-1));
	pVertex1Edit->SetValidator (new TRangeValidator (-1, NumVertexes-1));
	pVertex2Edit->SetValidator (new TRangeValidator (-1, NumVertexes-1));

	pSDSectorEdit[0]->SetValidator (new TRangeValidator (-1, NumSectors-1));
	pSDSectorEdit[1]->SetValidator (new TRangeValidator (-1, NumSectors-1));
	pSDXPosEdit[0]->SetValidator (new TRangeValidator (-32767, 32767));
	pSDYPosEdit[0]->SetValidator (new TRangeValidator (-999, 999));
	pSDXPosEdit[1]->SetValidator (new TRangeValidator (-32767, 32767));
	pSDYPosEdit[1]->SetValidator (new TRangeValidator (-999, 999));
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::CmOk ()
{
	// Cannot close if edit controls not valid
	if ( ! CanClose() )
		return;

	SHORT ldnum = SelLineDefs->objnum;

	// Get CurLineDef info from controls
	GetLineDef ();

	// Check that we have two different SideDef numbers
	if (CurLineDef.sidedef1 != -1  &&
		CurLineDef.sidedef1 == CurLineDef.sidedef2)
	{
		MessageBox ("The two SideDef numbers MUST be different !",
					"Error",
					MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Did the user made changes ?
	LineDef HUGE *pLineDef = &LineDefs[ldnum];
	if ( memcmp (&CurLineDef, pLineDef, sizeof (CurLineDef)) != 0 )
		MadeChanges = TRUE;

	if (CurLineDef.start    != pLineDef->start    ||
		CurLineDef.end      != pLineDef->end      ||
		CurLineDef.sidedef1 != pLineDef->sidedef1 ||
		CurLineDef.sidedef2 != pLineDef->sidedef2 )
	{
		MadeMapChanges = TRUE;
	}

	*pLineDef = CurLineDef;

	// Save SideDef 1
	if ( CurLineDef.sidedef1 >= 0 )
	{
		GetSideDef (0);
		SideDefs[CurLineDef.sidedef1] = CurSD[0];
	}

	// Save SideDef 2
	if ( CurLineDef.sidedef2 >= 0 )
	{
		GetSideDef (1);
		SideDefs[CurLineDef.sidedef2] = CurSD[1];
	}

	// Copy to the other selected LineDef's
	if ( SelLineDefs->next != NULL )
	{
		SET_HELP_CONTEXT(Confirming_copy_of_LineDef_attributes);
		if (TConfirmLineDefDialog(this, &ConfirmData).Execute() == IDOK)
		{
			//
			BOOL CopySD1Number = FALSE;
			BOOL CopySD2Number = FALSE;

			// Check that we can copy SideDef data from first LineDef
			if ( CurLineDef.sidedef1 < 0  &&  ConfirmData.pSDCheck[0] == TRUE)
			{
				if ( Expert )
					CopySD1Number = TRUE;
				else
					CopySD1Number = Confirm(
						"The first SideDef number of the LineDef %d is "
						"\"-1\" (meaning there's no SideDef).\nDo you want "
						"WinDEU to copy this number to the others "
						"selected LineDefs (meaning they will have no "
						"first SideDef) ?");
			}

			if ( CurLineDef.sidedef2 < 0  &&  ConfirmData.pSDCheck[1] == TRUE )
			{
				if ( Expert )
					CopySD2Number = TRUE;
				else
					CopySD2Number = Confirm(
						"The second SideDef number of the LineDef %d is "
						"\"-1\" (meaning there's no SideDef).\nDo you want "
						"WinDEU to copy this number to the others "
						"selected LineDefs (meaning they will have no "
						"second SideDef) ?");
/*
				Notify ("Cannot copy the %s SideDef data from the LineDef %d "
						"to the other selected LineDefs, because LineDef %d "
						"doesn't have such a SideDef.\n"
						"The copy process will continue normaly, except for "
						"this SideDef data.",
						CurLineDef.sidedef1 < 0 ? "first" : "second",
						SelLineDefs->objnum, SelLineDefs->objnum);
*/
			}

			// Copy data to each other selected LineDef
			for (SelPtr LdPtr = SelLineDefs->next ;
				 LdPtr != NULL ;
				 LdPtr = LdPtr->next)
			{
				LineDef HUGE *pSelLineDef = &LineDefs[LdPtr->objnum];
				LineDef LineDefBefore = *pSelLineDef;

				if ( ConfirmData.pTypeCheck )
					pSelLineDef->type = CurLineDef.type;

				if ( ConfirmData.pSectorCheck )
					pSelLineDef->tag = CurLineDef.tag;

				if ( ConfirmData.pVertex1Check )
					pSelLineDef->start = CurLineDef.start;

				if ( ConfirmData.pVertex2Check )
					pSelLineDef->end = CurLineDef.end;

				// Copy SideDef 1 data (or number if -1)
				if ( CopySD1Number )
				{
					LineDefs[LdPtr->objnum].sidedef1 = CurLineDef.sidedef1;
					MadeChanges = TRUE;
					MadeMapChanges = TRUE;
				}
				else
					CopySDData (0, LdPtr->objnum, ConfirmData);

				// Copy SideDef 2 data (or number if -1)
				if ( CopySD2Number )
				{
					LineDefs[LdPtr->objnum].sidedef2 = CurLineDef.sidedef2;
					MadeChanges = TRUE;
					MadeMapChanges = TRUE;
				}
				else
					CopySDData (1, LdPtr->objnum, ConfirmData);

				// Copy each individual flag
				if ( ConfirmData.pFlagsCheck )
				{
					SHORT lflags = pSelLineDef->flags;
					SHORT cflags = CurLineDef.flags;
					if ( ConfirmData.pImpassableCheck )
						if ( cflags & 0x01 )	lflags |= 0x01;
						else					lflags &= ~0x01;

					if ( ConfirmData.pMonCrossCheck )
						if ( cflags & 0x02 )	lflags |= 0x02;
						else					lflags &= ~0x02;

					if ( ConfirmData.pTwoSidedCheck )
						if ( cflags & 0x04 )	lflags |= 0x04;
						else					lflags &= ~0x04;

					if ( ConfirmData.pAboveUnpCheck )
						if ( cflags & 0x08 )	lflags |= 0x08;
						else					lflags &= ~0x08;

					if ( ConfirmData.pBelowUnpCheck )
						if ( cflags & 0x10 )	lflags |= 0x10;
						else					lflags &= ~0x10;

					if ( ConfirmData.pSecretCheck )
						if ( cflags & 0x20 )	lflags |= 0x20;
						else					lflags &= ~0x20;

					if ( ConfirmData.pBlockSoundCheck )
						if ( cflags & 0x40 )	lflags |= 0x40;
						else					lflags &= ~0x40;

					if ( ConfirmData.pNotShownCheck )
						if ( cflags & 0x80 )	lflags |= 0x80;
						else					lflags &= ~0x80;

					if ( ConfirmData.pShownCheck )
						if ( cflags & 0x100 )	lflags |= 0x100;
						else					lflags &= ~0x100;


					pSelLineDef->flags = lflags;
				}

				// Did we made changes?
				if ( memcmp (pSelLineDef, &LineDefBefore, sizeof (CurLineDef)) != 0 )
					MadeChanges = TRUE;

				if (LineDefBefore.start    != pSelLineDef->start    ||
					LineDefBefore.end      != pSelLineDef->end      ||
					LineDefBefore.sidedef1 != pSelLineDef->sidedef1 ||
					LineDefBefore.sidedef2 != pSelLineDef->sidedef2 )
				{
					MadeMapChanges = TRUE;
				}
			}
		}
		RESTORE_HELP_CONTEXT();
	}

	// Close Dialog box
	TDialog::CmOk();
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  Get the the LineDef data from the Controls (to CurLineDef)
void TLineDefEditDialog::GetLineDef ()
{
	char str[10];

	// Get LineDef info and setup Confirm data flags
	pSectorTagEdit->GetText (str, 6);
	ConfirmData.pSectorCheck = (CurLineDef.tag != atoi (str));
	CurLineDef.tag = atoi (str);

	pSDEdit[0]->GetText (str, 6);
	// May already be TRUE (if New SideDef), so use |=
	ConfirmData.pSDCheck[0] |= (CurLineDef.sidedef1 != atoi (str));
	CurLineDef.sidedef1 = atoi (str);

	pSDEdit[1]->GetText (str, 6);
	// May already be TRUE (if New SideDef), so use |=
	ConfirmData.pSDCheck[1] |= (CurLineDef.sidedef2 != atoi (str));
	CurLineDef.sidedef2 = atoi (str);

	pVertex1Edit->GetText (str, 6);
	ConfirmData.pVertex1Check = (CurLineDef.start != atoi (str));
	CurLineDef.start = atoi (str);

	pVertex2Edit->GetText (str, 6);
	ConfirmData.pVertex2Check = (CurLineDef.end != atoi (str));
	CurLineDef.end = atoi (str);

	// Get LineDef flags
	SHORT flags = 0;
	if ( pImpassableCheck->GetCheck() == BF_CHECKED )	flags |= 0x01;
	if ( pMonsterCrossCheck->GetCheck() == BF_CHECKED ) flags |= 0x02;
	if ( pTwoSidedCheck->GetCheck() == BF_CHECKED )   	flags |= 0x04;
	if ( pAboveUnpCheck->GetCheck() == BF_CHECKED )   	flags |= 0x08;
	if ( pBelowUnpCheck->GetCheck() == BF_CHECKED )   	flags |= 0x10;
	if ( pSecretCheck->GetCheck() == BF_CHECKED )   	flags |= 0x20;
	if ( pBlockSoundCheck->GetCheck() == BF_CHECKED )   flags |= 0x40;
	if ( pNotShownCheck->GetCheck() == BF_CHECKED )   	flags |= 0x80;
	if ( pShownCheck->GetCheck() == BF_CHECKED )   		flags |= 0x100;
	CurLineDef.flags = flags;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  Get SideDef 'sdnum' from Controls (to CurSD[sdnum])
void TLineDefEditDialog::GetSideDef (int sdnum)
{
	assert (sdnum == 0  ||  sdnum == 1);
	assert ( (sdnum == 0 && CurLineDef.sidedef1 < NumSideDefs) ||
			 (sdnum == 1 && CurLineDef.sidedef2 < NumSideDefs) );

	// Clear SideDef info
	//DEBUG: Removed because of use of ConfirmData !!!!
	// CurSD[sdnum].xoff = 0;
	// CurSD[sdnum].yoff = 0;
	// CurSD[sdnum].sector = 0;
	// memset (CurSD[sdnum].tex1, 0, 8);
	// memset (CurSD[sdnum].tex2, 0, 8);
	// memset (CurSD[sdnum].tex3, 0, 8);

	// Get SideDef info from controls
	char str[9];
	pSDXPosEdit[sdnum]->GetText (str, 6);
	ConfirmData.pSDXOfsCheck[sdnum] = (CurSD[sdnum].xoff != (SHORT)atoi (str));
	// May already be true (new SideDef), so use |=
	ConfirmData.pSDCheck[sdnum] |= ConfirmData.pSDXOfsCheck[sdnum];
	CurSD[sdnum].xoff = (SHORT)atoi (str);

	pSDYPosEdit[sdnum]->GetText (str, 6);
	ConfirmData.pSDYOfsCheck[sdnum] = (CurSD[sdnum].yoff != (SHORT)atoi (str));
	// May already be true (new SideDef), so use |=
	ConfirmData.pSDCheck[sdnum] |= ConfirmData.pSDYOfsCheck[sdnum];
	CurSD[sdnum].yoff = (SHORT)atoi (str);

	pSDSectorEdit[sdnum]->GetText (str, 6);
	ConfirmData.pSDSectorCheck[sdnum] = (CurSD[sdnum].sector != (SHORT)atoi (str));
	// May already be true (new SideDef), so use |=
	ConfirmData.pSDCheck[sdnum] |= ConfirmData.pSDSectorCheck[sdnum];
	CurSD[sdnum].sector = (SHORT)atoi (str);

	pSDAboveStatic[sdnum]->GetText (str, 9);
	memset(CurSD[sdnum].tex1, 0, 8);
	strncpy (CurSD[sdnum].tex1, str, 8);

	pSDBelowStatic[sdnum]->GetText (str, 9);
	memset(CurSD[sdnum].tex2, 0, 8);
	strncpy (CurSD[sdnum].tex2, str, 8);

	pSDNormalStatic[sdnum]->GetText (str, 9);
	memset(CurSD[sdnum].tex3, 0, 8);
	strncpy (CurSD[sdnum].tex3, str, 8);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::CopySDData (int sdnum, SHORT DestLD,
									 TConfirmLineDefDialogXfer &xfer)
{
	assert (sdnum == 0 || sdnum == 1);
	assert (DestLD >= 0 && DestLD < NumLineDefs);

	// Do not copy if SideDef CheckBox not checked
	if ( xfer.pSDCheck[sdnum] == FALSE )
		return;

	LineDef HUGE *pDestLineDef = &LineDefs[DestLD];

	// Retreive source and dest. SideDef numbers
	SHORT SrcSD, DestSD;
	if ( sdnum == 0 )
	{
		SrcSD = CurLineDef.sidedef1;
		DestSD = pDestLineDef->sidedef1;
	}
	else
	{
		SrcSD = CurLineDef.sidedef2;
		DestSD = pDestLineDef->sidedef2;
	}

	// Do not copy if no SideDef data for the source LineDef
	// (a warning message was displayed before)
	if ( SrcSD < 0 )
		return;

	// Check the destination SideDef
	if ( DestSD < 0 )
	{
		// By default, a new SideDef is created for the destination LineDef
		if ( Expert ||
			 Confirm ("The LineDef %d doesn't currently have a %s SideDef, so "
					  "it's impossible to copy the SideDef data from the "
					  "LineDef %d. \n\nDo you want to create a NEW %s SideDef "
					  "for the LineDef %d ?"
					  "\n(If not, this LineDef will be skipped.)",
					  DestLD, sdnum == 0 ? "first" : "second",
					  SelLineDefs->objnum,
					  sdnum == 0 ? "first" : "second", DestLD) == TRUE )
		{
			// Create new SideDef copied from the source SideDef
			InsertObject (OBJ_SIDEDEFS, SrcSD, 0, 0);

			// Link it to the destination LineDef
			DestSD = NumSideDefs - 1;
			if ( sdnum == 0 )		pDestLineDef->sidedef1 = DestSD;
			else               		pDestLineDef->sidedef2 = DestSD;
		}
		// Either we created a SideDef copied from the source SideDef,
		// or we skip the copy of theSideDef. So it's OK to quit
		return;
	}

	// Copy selected data members to the Dest. SideDef
	SideDef HUGE *pSrcSideDef = &SideDefs[SrcSD];
	SideDef HUGE *pDestSideDef = &SideDefs[DestSD];

	if ( xfer.pSDAboveCheck[sdnum] )
		strncpy (pDestSideDef->tex1, pSrcSideDef->tex1, 8);

	if ( xfer.pSDBelowCheck[sdnum] )
		strncpy (pDestSideDef->tex2, pSrcSideDef->tex2, 8);

	if ( xfer.pSDNormalCheck[sdnum] )
		strncpy (pDestSideDef->tex3, pSrcSideDef->tex3, 8);

	if ( xfer.pSDSectorCheck[sdnum] )
		pDestSideDef->sector = pSrcSideDef->sector;

	if ( xfer.pSDXOfsCheck[sdnum] )
		pDestSideDef->xoff = pSrcSideDef->xoff;

	if ( xfer.pSDYOfsCheck[sdnum] )
		pDestSideDef->yoff = pSrcSideDef->yoff;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  Retrieve LineDefSet from the first selected LineDef type.
//  If no set found, LineDefSet = -1
void TLineDefEditDialog::InitLineDefSet ()
{
	SHORT type;

	LineDefSet = -1;
	for (int i = 0 ; i < NB_LINEDEFSET  &&  LineDefSet < 0 ; i++)
	{
		for (int j = 0 ;
			 (type = GetLineDefTab(i)[j]) != -1;
			 j++)
		{
			if ( type == CurLineDef.type )
			{
				LineDefSet = i;
				break;
			}
		}
	}

	SendDlgItemMessage (IDC_DOORS, BM_SETCHECK,
						(WPARAM)LineDefSet == LD_DOORS, (LPARAM)0);
	SendDlgItemMessage (IDC_CEILINGS, BM_SETCHECK,
						(WPARAM)LineDefSet == LD_CEILINGS, (LPARAM)0);
	SendDlgItemMessage (IDC_RAISE_FLOORS, BM_SETCHECK,
						(WPARAM)LineDefSet == LD_RAISEFLOORS, (LPARAM)0);
	SendDlgItemMessage (IDC_LOWER_FLOORS, BM_SETCHECK,
						(WPARAM)LineDefSet == LD_LOWERFLOORS, (LPARAM)0);
	SendDlgItemMessage (IDC_MOVING_THINGS, BM_SETCHECK,
						(WPARAM)LineDefSet == LD_MOVINGTHINGS, (LPARAM)0);
	SendDlgItemMessage (IDC_SPECIAL, BM_SETCHECK,
						(WPARAM)LineDefSet == LD_SPECIAL, (LPARAM)0);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  Show the list of type for the LineDefSet set.
//  If LineDefSet == -1, UNKNOWN is displayed
void TLineDefEditDialog::SetLineDefList ()
{
	assert (pLineDefList->IsWindow());
	SHORT type;
	int TabStops[] = { 5*4, 5*4 + 5*4 };

	pLineDefList->ClearList();
	pLineDefList->SetTabStops (2, TabStops);

	if ( LineDefSet < 0  ||  LineDefSet >= NB_LINEDEFSET )
	{
		char str[50];

		wsprintf (str, "[%d]\tUnknown ???(DO NOT USE)", CurLineDef.type);
		pLineDefList->AddString (str);
		return;
	}

	for ( int i = 0 ;
		  (type = GetLineDefTab(LineDefSet)[i]) != -1;
		  i++)
	{
		char str[128];
		char ldname[128];

		strcpy (ldname, GetLineDefTypeLongName (type));

		// UGLY, but it works
		ldname[2] = '\t';
		wsprintf (str, "[%d]\t%s", type, ldname);
		pLineDefList->AddString (str);

		if ( CurLineDef.type == type )
			pLineDefList->SetSelString (str, -1);
	}
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SetTextureList ()
{
	assert (pTextureList->IsWindow());

	// Add texture1 and texture 2 names
	int TabStops[] = { 12 * 4, 12 * 4 + 4 * 4};
	pTextureList->SetTabStops (2, TabStops);

	// Add wall textures entries entries
	for (SHORT ti = 0 ; ti < NumWTexture ; ti++)
	{
		char str[30];

		wsprintf (str, "%s\t%3d\t%3d", WTexture[ti]->Name, WTexture[ti]->Width,
									   WTexture[ti]->Height);

		// Adds the texture name in the list
		pTextureList->AddString (str);
	}
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//	Setup Controls for the current LineDef (CurLineDef)
//  and copy the two SideDefs data to CurSD[0] and CurSD[1]
void TLineDefEditDialog::SetLineDef ()
{
	assert (CurLineDef.sidedef1 == -1  || (USHORT)CurLineDef.sidedef1 < NumSideDefs);
	assert (CurLineDef.sidedef2 == -1  || (USHORT)CurLineDef.sidedef2 < NumSideDefs);
	char str[10];

	// Init. current sidedef's
	if ( CurLineDef.sidedef1 >= 0 )
		CurSD[0] = SideDefs[CurLineDef.sidedef1];

	if ( CurLineDef.sidedef2 >= 0 )
		CurSD[1] = SideDefs[CurLineDef.sidedef2];

	// Show LineDef info
	wsprintf (str, "%d", CurLineDef.tag);
	pSectorTagEdit->SetText (str);

	wsprintf (str, "%d", CurLineDef.sidedef1);
	pSDEdit[0]->SetText (str);

	wsprintf (str, "%d", CurLineDef.sidedef2);
	pSDEdit[1]->SetText (str);

	wsprintf (str, "%d", CurLineDef.start);
	pVertex1Edit->SetText (str);

	wsprintf (str, "%d", CurLineDef.end);
	pVertex2Edit->SetText (str);

	// Setup flags checkboxes
	SHORT flags = CurLineDef.flags;
	pImpassableCheck->SetCheck   (flags & 0x01 ? BF_CHECKED : BF_UNCHECKED);
	pMonsterCrossCheck->SetCheck (flags & 0x02 ? BF_CHECKED : BF_UNCHECKED);
	pTwoSidedCheck->SetCheck     (flags & 0x04 ? BF_CHECKED : BF_UNCHECKED);
	pAboveUnpCheck->SetCheck     (flags & 0x08 ? BF_CHECKED : BF_UNCHECKED);
	pBelowUnpCheck->SetCheck     (flags & 0x10 ? BF_CHECKED : BF_UNCHECKED);
	pSecretCheck->SetCheck       (flags & 0x20 ? BF_CHECKED : BF_UNCHECKED);
	pBlockSoundCheck->SetCheck   (flags & 0x40 ? BF_CHECKED : BF_UNCHECKED);
	pNotShownCheck->SetCheck     (flags & 0x80 ? BF_CHECKED : BF_UNCHECKED);
	pShownCheck->SetCheck        (flags & 0x100 ? BF_CHECKED : BF_UNCHECKED);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//   Setup the the SideDef 'sdnum' controls. The temporary struct
//   CurSD[sdnum] must be initialized before calling this function.
void TLineDefEditDialog::SetSideDef (int sdnum)
{
	assert (sdnum == 0  ||  sdnum == 1);
	assert ( (sdnum == 0 && CurLineDef.sidedef1 < NumSideDefs) ||
			 (sdnum == 1 && CurLineDef.sidedef2 < NumSideDefs) );
	BOOL EnableSD = TRUE;	// Enable Control's ?

	if ( (sdnum == 0 && CurLineDef.sidedef1 < 0) ||
		 (sdnum == 1 && CurLineDef.sidedef2 < 0) )
		EnableSD = FALSE;

	// Disable controls if sidedef < 0, or Enable them
	pSDXPosEdit[sdnum]->EnableWindow (EnableSD);
	pSDYPosEdit[sdnum]->EnableWindow (EnableSD);
	pSDSectorEdit[sdnum]->EnableWindow (EnableSD);
	pSDAboveStatic[sdnum]->EnableWindow (EnableSD);
	pSDNormalStatic[sdnum]->EnableWindow (EnableSD);
	pSDBelowStatic[sdnum]->EnableWindow (EnableSD);

	pSDToAbove[sdnum]->EnableWindow (EnableSD);
	pSDToBelow[sdnum]->EnableWindow (EnableSD);
	pSDToNormal[sdnum]->EnableWindow (EnableSD);
	pSDClearAbove[sdnum]->EnableWindow (EnableSD);
	pSDClearBelow[sdnum]->EnableWindow (EnableSD);
	pSDClearNormal[sdnum]->EnableWindow (EnableSD);

	// Set empty values if sidedef < 0
	if ( (sdnum == 0 && CurLineDef.sidedef1 < 0) ||
		 (sdnum == 1 && CurLineDef.sidedef2 < 0) )
	{
		pSDXPosEdit[sdnum]->SetText ("0");
		pSDYPosEdit[sdnum]->SetText ("0");
		pSDSectorEdit[sdnum]->SetText ("-1");
		pSDAboveStatic[sdnum]->SetText ("-");
		pSDNormalStatic[sdnum]->SetText ("-");
		pSDBelowStatic[sdnum]->SetText ("-");
		return;
	}

	// Setup SideDef controls
	char str[9];
	wsprintf (str, "%d", CurSD[sdnum].xoff);
	pSDXPosEdit[sdnum]->SetText(str);

	wsprintf (str, "%d", CurSD[sdnum].yoff);
	pSDYPosEdit[sdnum]->SetText(str);

	wsprintf (str, "%d", CurSD[sdnum].sector);
	pSDSectorEdit[sdnum]->SetText(str);

	str[8] = '\0';
	strncpy (str, CurSD[sdnum].tex1, 8);
	pSDAboveStatic[sdnum]->SetText (str);

	strncpy (str, CurSD[sdnum].tex2, 8);
	pSDBelowStatic[sdnum]->SetText (str);

	strncpy (str, CurSD[sdnum].tex3, 8);
	pSDNormalStatic[sdnum]->SetText (str);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  Set LineDef set list to DOORS
void TLineDefEditDialog::DoorsClicked ()
{
	LineDefSet = LD_DOORS;
	SetLineDefList();
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  Set LineDef set list to ...
void TLineDefEditDialog::CeilingsClicked ()
{
	LineDefSet = LD_CEILINGS;
	SetLineDefList();
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  Set LineDef set list to ...
void TLineDefEditDialog::RaiseFloorsClicked ()
{
	LineDefSet = LD_RAISEFLOORS;
	SetLineDefList();
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  Set LineDef set list to ...
void TLineDefEditDialog::LowerFloorsClicked ()
{
	LineDefSet = LD_LOWERFLOORS;
	SetLineDefList();
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  Set LineDef set list to ...
void TLineDefEditDialog::MovingThingsClicked ()
{
	LineDefSet = LD_MOVINGTHINGS;
	SetLineDefList();
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  Set LineDef set list to ...
void TLineDefEditDialog::SpecialClicked ()
{
	LineDefSet = LD_SPECIAL;
	SetLineDefList();
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
// 	Set the LineDef type to the selected type
void TLineDefEditDialog::LineDefSelChange ()
{
	if ( LineDefSet < 0  ||  LineDefSet >= NB_LINEDEFSET )
		return;

	// Get selected item index
	int SelIndex = pLineDefList->GetSelIndex ();
	if ( SelIndex < 0 )
		return;

	CurLineDef.type = GetLineDefTab(LineDefSet)[SelIndex];
	ConfirmData.pTypeCheck = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD1ClearAboveClicked ()
{
	pSDAboveStatic[0]->SetText("-");
	ConfirmData.pSDCheck[0] = TRUE;
	ConfirmData.pSDAboveCheck[0] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD1ClearBelowClicked ()
{
	pSDBelowStatic[0]->SetText("-");
	ConfirmData.pSDCheck[0] = TRUE;
	ConfirmData.pSDBelowCheck[0] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD1ClearNormalClicked ()
{
	pSDNormalStatic[0]->SetText("-");
	ConfirmData.pSDCheck[0] = TRUE;
	ConfirmData.pSDNormalCheck[0] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD1ToAboveClicked ()
{
	if ( TextureName[0] == '\0' )
		return;

	pSDAboveStatic[0]->SetText (TextureName);
	ConfirmData.pSDCheck[0] = TRUE;
	ConfirmData.pSDAboveCheck[0] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD1ToBelowClicked ()
{
	if ( TextureName[0] == '\0' )
		return;

	pSDBelowStatic[0]->SetText (TextureName);
	ConfirmData.pSDCheck[0] = TRUE;
	ConfirmData.pSDBelowCheck[0] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD1ToNormalClicked ()
{
	if ( TextureName[0] == '\0' )
		return;

	pSDNormalStatic[0]->SetText (TextureName);
	ConfirmData.pSDCheck[0] = TRUE;
	ConfirmData.pSDNormalCheck[0] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD2ClearAboveClicked ()
{
	pSDAboveStatic[1]->SetText("-");
	ConfirmData.pSDCheck[1] = TRUE;
	ConfirmData.pSDAboveCheck[1] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD2ClearBelowClicked ()
{
	pSDBelowStatic[1]->SetText("-");
	ConfirmData.pSDCheck[1] = TRUE;
	ConfirmData.pSDBelowCheck[1] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD2ClearNormalClicked ()
{
	pSDNormalStatic[1]->SetText("-");
	ConfirmData.pSDCheck[1] = TRUE;
	ConfirmData.pSDNormalCheck[1] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD2ToAboveClicked ()
{
	if ( TextureName[0] == '\0' )
		return;

	pSDAboveStatic[1]->SetText (TextureName);
	ConfirmData.pSDCheck[1] = TRUE;
	ConfirmData.pSDAboveCheck[1] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD2ToBelowClicked ()
{
	if ( TextureName[0] == '\0' )
		return;

	pSDBelowStatic[1]->SetText (TextureName);
	ConfirmData.pSDCheck[1] = TRUE;
	ConfirmData.pSDBelowCheck[1] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD2ToNormalClicked ()
{
	if ( TextureName[0] == '\0' )
		return;

	pSDNormalStatic[1]->SetText (TextureName);
	ConfirmData.pSDCheck[1] = TRUE;
	ConfirmData.pSDNormalCheck[1] = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD1Killfocus ()
{
	SDKillFocus(0);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD2Killfocus ()
{
	SDKillFocus(1);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SDKillFocus (int sdnum)
{
	assert (sdnum == 0 || sdnum == 1);
	char num[8];
	SHORT sd;

#if 0
	// TODO: Why this doesn't work ?
	if ( ! pSDEdit[sdnum]->IsModified() )
		return;
#endif

	if ( ! pSDEdit[sdnum]->IsValid(FALSE) )
		return;

	pSDEdit[sdnum]->GetText (num, 8);
	sd = (SHORT)atoi (num);

	// Ignore if sidedef number was not changed
	if ( ( sdnum == 0 && sd == CurLineDef.sidedef1 ) ||
		 ( sdnum == 1 && sd == CurLineDef.sidedef2 ) )
	{
		return;
	}

	// Clear confirmation data, except sidedef number
	ConfirmData.pSDCheck[sdnum] = TRUE;
	ConfirmData.pSDAboveCheck[sdnum]  = FALSE;
	ConfirmData.pSDNormalCheck[sdnum] = FALSE;
	ConfirmData.pSDBelowCheck[sdnum]  = FALSE;
	ConfirmData.pSDSectorCheck[sdnum] = FALSE;
	ConfirmData.pSDXOfsCheck[sdnum]   = FALSE;
	ConfirmData.pSDYOfsCheck[sdnum]   = FALSE;

	// Check SideDef number is valid
	if ( sd != -1 )
	{
		// Check SideDef numbers are different
		if ( (sdnum == 0  &&  sd == CurLineDef.sidedef2) ||
			 (sdnum == 1  &&  sd == CurLineDef.sidedef1) )
		{
			MessageBox ("The two SideDef numbers MUST be different !",
						"Error",
						MB_OK | MB_ICONEXCLAMATION);
			sd = -1;
			pSDEdit[sdnum]->SetText ("-1");
		}
		else
			CurSD[sdnum] = SideDefs [sd];
	}

	// Copy SideDef number and setup controls
	if ( sdnum == 0 )		CurLineDef.sidedef1 = sd;
	else					CurLineDef.sidedef2 = sd;

	SetSideDef (sdnum);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD1NewClicked ()
{
	SDNew (0);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::SD2NewClicked ()
{
	SDNew (1);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  A new SideDef is created
void TLineDefEditDialog::SDNew (int sdnum)
{
	assert (sdnum == 0 || sdnum == 1);
	char str[10];
	SHORT sd;

	// Create new SideDef
	InsertObject (OBJ_SIDEDEFS, -1, 0, 0);

	// Fill confirmation data
	ConfirmData.pSDCheck[sdnum] = TRUE;
	ConfirmData.pSDAboveCheck[sdnum]  = FALSE;
	ConfirmData.pSDNormalCheck[sdnum] = FALSE;
	ConfirmData.pSDBelowCheck[sdnum]  = FALSE;
	ConfirmData.pSDSectorCheck[sdnum] = FALSE;
	ConfirmData.pSDXOfsCheck[sdnum]   = FALSE;
	ConfirmData.pSDYOfsCheck[sdnum]   = FALSE;

	// Copy SideDef number
	sd = NumSideDefs-1;
	if ( sdnum == 0 ) 	CurLineDef.sidedef1 = sd;
	else			  	CurLineDef.sidedef2 = sd;

	// Setup sector number
	SHORT OppSector = GetOppositeSector (SelLineDefs->objnum, sdnum == 0);

	// If no opposite sector, choose the last one
	if ( OppSector > 0 )
		OppSector = NumSectors-1;
	SideDefs[sd].sector = OppSector;

	// Setup SideDef number control
	wsprintf (str, "%d", sd);
	pSDEdit[sdnum]->SetText (str);

	// Copy SideDef to temporary SideDef struct.
	CurSD[sdnum] = SideDefs[sd];

	// Replace validators for SideDef number
	pSDEdit[0]->SetValidator (new TRangeValidator (-1, NumSideDefs-1));
	pSDEdit[1]->SetValidator (new TRangeValidator (-1, NumSideDefs-1));

	// Setup SideDef controls
	SetSideDef (sdnum);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//  A new sector tag displayed
void TLineDefEditDialog::SectorTagNewClicked ()
{
	char str[10];

	wsprintf (str, "%d", FindFreeTag());
	pSectorTagEdit->SetText (str);
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
// 	Set TextureName to the new selected texture.
void TLineDefEditDialog::TextureSelchange ()
{
	char texname[30];

	// Get selected texture name
	if ( pTextureList->GetSelString (texname, 29) <= 1 )
	{
		TextureName[0] = '\0';
		return;
	}

	// Don't go further than the '\t' char
	for (char *pstr = texname ; *pstr ; pstr++)
	{
		if ( *pstr == '\t' )
		{
			*pstr = '\0';
			break;
		}
	}

	// Don't do anything if no real change
	if ( strcmp (texname, TextureName) == 0 )
		return;

	strcpy (TextureName, texname);

	// If texture view dialog box opened, change selection
	if ( pWTextureDialog != NULL && pWTextureDialog->IsWindow() )
	{
		TextureListDBLClick();
	}
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::TextureListDBLClick ()
{
	// Don't select empty texture !
	if ( TextureName[0] == '\0' || strcmp (TextureName, "-") == 0 )
		return;

	// Create modeless dialog box
	if ( pWTextureDialog == NULL || pWTextureDialog->IsWindow() == FALSE )
	{
		delete pWTextureDialog;
		pWTextureDialog = new TDisplayWallTextureDialog (Parent);
		pWTextureDialog->Create();
	}

	if ( pWTextureDialog->IsWindow() )
	{
		if ( pWTextureDialog->SelectBitmap (TextureName) < 0 )
			Notify ("Error: Cannot select the texture name \"%s\" in the "
					"dialog box of Wall Texture view ! (BUG)", TextureName);
	}
	else
		Notify ("Error: Cannot create dialog box of Wall Texture view !");
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::EvLButtonDown (UINT modKeys, TPoint& point)
{
	TDialog::EvLButtonDown(modKeys, point);

	// Retreive object for handle
	TStatic *pStatic = GetPointedStatic (point);
	if ( pStatic == NULL )
		return;

	// If floor or ceiling static, get texture name
	char texname[9];
	pStatic->GetText (texname, 9);
	if ( texname[0] != '\0' && strcmp (texname, "-") != 0 )
	{
		pTextureList->SetSelString (texname, -1);
		TextureSelchange();  	// Be sure to change selection
	}
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
void TLineDefEditDialog::EvLButtonDblClk (UINT modKeys, TPoint& point)
{
	TDialog::EvLButtonDblClk(modKeys, point);

	// Retreive object for handle
	TStatic *pStatic = GetPointedStatic (point);
	if ( pStatic == NULL )
		return;

	// If floor or ceiling static, get texture name
	char texname[9];
	pStatic->GetText (texname, 9);
	if ( texname[0] != '\0' && strcmp (texname, "-") != 0 )
	{
		pTextureList->SetSelString (texname, -1);
		TextureSelchange();  		// Be sure to change selection
		TextureListDBLClick();		// Force to open Dialog Box
	}
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//
TStatic *TLineDefEditDialog::GetPointedStatic (TPoint& point)
{
	// Retreive object for handle
	TStatic *pStatic = NULL;

	if ( IsPointInDlgItem (IDC_SD1_ABOVE, point)  ||
		 IsPointInDlgItem (IDC_SD1_ABOVE_FRAME, point) )
	{
		pStatic = pSDAboveStatic[0];
	}

	else if ( IsPointInDlgItem (IDC_SD1_NORMAL, point)  ||
			  IsPointInDlgItem (IDC_SD1_NORMAL_FRAME, point) )
	{
		pStatic = pSDNormalStatic[0];
	}

	else if ( IsPointInDlgItem (IDC_SD1_BELOW, point)  ||
			  IsPointInDlgItem (IDC_SD1_BELOW_FRAME, point) )
	{
		pStatic = pSDBelowStatic[0];
	}

	else if ( IsPointInDlgItem (IDC_SD2_ABOVE, point)  ||
			  IsPointInDlgItem (IDC_SD2_ABOVE_FRAME, point) )
	{
		pStatic = pSDAboveStatic[1];
	}

	else if ( IsPointInDlgItem (IDC_SD2_NORMAL, point)  ||
			  IsPointInDlgItem (IDC_SD2_NORMAL_FRAME, point) )
	{
		pStatic = pSDNormalStatic[1];
	}

	else if ( IsPointInDlgItem (IDC_SD2_BELOW, point)  ||
			  IsPointInDlgItem (IDC_SD2_BELOW_FRAME, point) )
	{
		pStatic = pSDBelowStatic[1];
	}

	return pStatic;
}


//////////////////////////////////////////////////////////////////////
// TLineDefEditDialog
// ------------------
//   Look if the dialog control 'resid' window contains the point
//   'clientPoint', which is a dialog client coord. of the point
BOOL TLineDefEditDialog::IsPointInDlgItem (int itemId, TPoint &clientPoint)
{
	HWND hWnd = GetDlgItem (itemId);
	if ( hWnd == (HWND)NULL )
		return FALSE;

	TWindow wnd(hWnd);
	TRect wRect;
	wnd.GetWindowRect (wRect);
	TPoint TopLeft (wRect.left, wRect.top);
	TPoint BotRight(wRect.right, wRect.bottom);
	ScreenToClient (TopLeft);
	ScreenToClient (BotRight);
	TRect cRect (TopLeft, BotRight);

	return cRect.Contains (clientPoint);
}


void TLineDefEditDialog::AboveUnpClicked ()
{
	ConfirmData.pFlagsCheck = TRUE;
	ConfirmData.pAboveUnpCheck = TRUE;
}


void TLineDefEditDialog::BelowUnpClicked ()
{
	ConfirmData.pFlagsCheck = TRUE;
	ConfirmData.pBelowUnpCheck = TRUE;
}


void TLineDefEditDialog::BlockSoundClicked ()
{
	ConfirmData.pFlagsCheck = TRUE;
	ConfirmData.pBlockSoundCheck = TRUE;
}


void TLineDefEditDialog::ImpassableClicked ()
{
	ConfirmData.pFlagsCheck = TRUE;
	ConfirmData.pImpassableCheck = TRUE;
}


void TLineDefEditDialog::MonCrossClicked ()
{
	ConfirmData.pFlagsCheck = TRUE;
	ConfirmData.pMonCrossCheck = TRUE;
}


void TLineDefEditDialog::NotShownClicked ()
{
	ConfirmData.pFlagsCheck = TRUE;
	ConfirmData.pNotShownCheck = TRUE;
}


void TLineDefEditDialog::SecretClicked ()
{
	ConfirmData.pFlagsCheck = TRUE;
	ConfirmData.pSecretCheck = TRUE;
}


void TLineDefEditDialog::ShownClicked ()
{
	ConfirmData.pFlagsCheck = TRUE;
	ConfirmData.pShownCheck = TRUE;
}


void TLineDefEditDialog::TwoSidedClicked ()
{
	ConfirmData.pFlagsCheck = TRUE;
	ConfirmData.pTwoSidedCheck = TRUE;
}

