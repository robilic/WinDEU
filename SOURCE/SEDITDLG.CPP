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

	FILE:         seditdlg.cpp

	OVERVIEW
	========
	Source file for implementation of TSectorEditDialog (TDialog).
*/

#include "common.h"
#pragma hdrstop

#ifndef __seditdlg_h
	#include "seditdlg.h"
#endif

#ifndef __OWL_DIALOG_H
	#include <owl\dialog.h>
#endif

#ifndef __OWL_LISTBOX_H
	#include <owl\listbox.h>
#endif

#ifndef __OWL_EDIT_H
	#include <owl\edit.h>
#endif

#ifndef __OWL_STATIC_H
	#include <owl\static.h>
#endif

#ifndef __cnfsedlg_h
	#include "cnfsedlg.h"
#endif

#ifndef __levels_h
	#include "levels.h"
#endif

#ifndef __names_h
	#include "names.h"
#endif

#ifndef __newers_h
	#include "newers.h"
#endif

#ifndef __viewbmp_h
	#include "viewbmp.h"
#endif

// HELP Ids
#include "windeuhl.h"


const SHORT D_SectorTypeTab[] =
{
	0, 1, 2, 3, 5, 4, 16, 7,
	8, 9, 10, 11, 12, 13, 14,
	-1
};
#define NB_D_SECTOR	(sizeof(D_SectorTypeTab) / sizeof(SHORT))

const SHORT D2_SectorTypeTab[] =
{
	0, 1, 2, 3, 5, 4, 16, 7,
	8, 9, 10, 11, 12, 13, 14, 17,
	-1
};
#define NB_D2_SECTOR	(sizeof(D2_SectorTypeTab) / sizeof(SHORT))

const SHORT H_SectorTypeTab[] =
{
	0, 1, 2, 3, 5, 4, 16, 6, 7,
	8, 9, 10, 12, 13, 14,
	21, 22, 23, 26, 27, 28, 31,
	32, 33, 36, 37, 38, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, 50, 51,
	-1
};

#define NB_H_SECTOR	(sizeof(H_SectorTypeTab) / sizeof(SHORT))

SHORT GetSectorType(int i)
{
	if ( DoomVersion == 16 )
	{
		assert (i >= 0 && i < NB_H_SECTOR);
		return H_SectorTypeTab[i];
	}
	else if ( DoomVersion == 2 )
	{
		assert (i >= 0 && i < NB_D2_SECTOR);
		return D2_SectorTypeTab[i];
	}
	else
	{
		assert (i >= 0 && i < NB_D_SECTOR);
		return D_SectorTypeTab[i];
	}
}


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TSectorEditDialog, TDialog)
//{{TSectorEditDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_TO_FLOOR, ToFloorClicked),
	EV_BN_CLICKED(IDC_TO_CEILING, ToCeilingClicked),
	EV_BN_CLICKED(IDC_FLOOR_CLEAR, FloorClearClicked),
	EV_BN_CLICKED(IDC_CEILING_CLEAR, CeilingClearClicked),
	EV_LBN_SELCHANGE(IDC_SECTOR_LIST, SectorSelChange),
	EV_LBN_SELCHANGE(IDC_FTEXTURE_LIST, TextureSelChange),
	EV_LBN_DBLCLK(IDC_FTEXTURE_LIST, TextureDblclick),
	EV_BN_CLICKED(IDC_NEW_TAG, NewTagClicked),
	EV_BN_CLICKED(IDOK, CmOk),
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONDBLCLK,
//{{TSectorEditDialogRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TSectorEditDialog Implementation}}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
TSectorEditDialog::TSectorEditDialog (TWindow* parent, SelPtr sel,
									  TResId resId, TModule* module):
	TDialog(parent, resId, module)
{
	assert (sel != NULL);
	assert (sel->objnum >= 0 && sel->objnum < NumSectors);

	SelSectors = sel;
	CurSector =Sectors[sel->objnum];
	TextureName[0] = '\0';
	memset(&ConfirmData, 0, sizeof(ConfirmData));

	// Crete object for texture view, but do not create dialog box.
	pFTextureDialog = NULL;

	// Create objects for controls
	pTagEdit           = newTEdit(this, IDC_TAG_EDIT, 6);
	pSectorList        = newTListBox(this, IDC_SECTOR_LIST);
	pLightEdit         = newTEdit(this, IDC_LIGHT_EDIT, 4);
	pTextureList       = newTListBox(this, IDC_FTEXTURE_LIST);
	pFloorStatic       = newTStatic(this, IDC_FLOOR_TEXT, 9);
	pFloorHeightEdit   = newTEdit(this, IDC_FLOOR_HEIGHT, 6);
	pCeilingStatic     = newTStatic(this, IDC_CEILING_TEXT, 9);
	pCeilingHeightEdit = newTEdit(this, IDC_CEILING_HEIGHT, 6);
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
TSectorEditDialog::~TSectorEditDialog ()
{
	Destroy();
	delete pFTextureDialog;
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::SetupWindow ()
{
	TDialog::SetupWindow();
	::CenterWindow (this);

	SetTextureList();
	SetSectorList();
	SetSector();

	// Setup validators
	pLightEdit->SetValidator (new TRangeValidator (0, 255));
	pTagEdit->SetValidator (new TRangeValidator (0, SHRT_MAX));
	pFloorHeightEdit->SetValidator (new TRangeValidator (-16384, 16383));
	pCeilingHeightEdit->SetValidator (new TRangeValidator (-16384, 16383));
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::SetSectorList ()
{
	assert (pSectorList->IsWindow());
	int Special;
	int TabStops[] = { 5*4 };

	pSectorList->ClearList();
	pSectorList->SetTabStops (1, TabStops);

	for ( int i = 0 ;
		  (Special = GetSectorType(i)) != -1;
		  i++)
	{
		char str[128];
		char sname[128];

		strcpy (sname, GetSectorTypeLongName (Special));
		wsprintf (str, "[%d]\t%s", Special, sname);
		pSectorList->AddString (str);

		if ( CurSector.special == Special )
			pSectorList->SetSelString (str, -1);
	}
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::SetTextureList ()
{
	assert (pTextureList->IsWindow());
	assert (FTexture != NULL);

	pTextureList->ClearList();

	// Add texture1 and texture 2 names
	for (SHORT i = 0 ; i < NumFTexture ; i++)
	{
		assert (FTexture[i] != NULL);
		pTextureList->AddString (FTexture[i]->Name);
	}
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::SetSector()
{
	char str[10];

	// Show Sector info
	wsprintf (str, "%d", CurSector.tag);
	pTagEdit->SetText (str);

	wsprintf (str, "%d", CurSector.light);
	pLightEdit->SetText (str);

	wsprintf (str, "%d", CurSector.floorh);
	pFloorHeightEdit->SetText (str);

	wsprintf (str, "%d", CurSector.ceilh);
	pCeilingHeightEdit->SetText (str);

	str[8] = '\0';
	memcpy (str, CurSector.floort, 8);
	pFloorStatic->SetText (str);

	memcpy (str, CurSector.ceilt, 8);
	pCeilingStatic->SetText (str);
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::GetSector()
{
	char str[10];

	// Get Sector info
	pTagEdit->GetText (str, 6);
	if ( CurSector.tag != (SHORT)atoi(str) )	ConfirmData.pTagCheck = TRUE;
	CurSector.tag = (SHORT)atoi (str);

	pLightEdit->GetText (str, 4);
	if ( CurSector.light != (SHORT)atoi(str) )	ConfirmData.pLightCheck = TRUE;
	CurSector.light = (SHORT)atoi (str);

	pFloorHeightEdit->GetText (str, 6);
	if ( CurSector.floorh != (SHORT)atoi(str) )	ConfirmData.pFloorHeightCheck = TRUE;
	CurSector.floorh = (SHORT)atoi (str);

	pCeilingHeightEdit->GetText (str, 6);
	if ( CurSector.ceilh != (SHORT)atoi(str) )	ConfirmData.pCeilingHeightCheck = TRUE;
	CurSector.ceilh = (SHORT)atoi (str);

	pFloorStatic->GetText (str, 9);
	memset (CurSector.floort, 0, 8);
	strncpy (CurSector.floort, str, 8);

	pCeilingStatic->GetText (str, 9);
	memset (CurSector.ceilt, 0, 8);
	strncpy (CurSector.ceilt, str, 8);
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::CmOk ()
{
	// Cannot close if edit controls not valid
	if ( ! CanClose() )
		return;

	// Save first selected sector info
	GetSector();

	SPtr pSector = &Sectors[SelSectors->objnum];

	// Did the user made changes?
	if ( memcmp(pSector, &CurSector, sizeof(Sector)) != 0 )
	{
		MadeChanges = TRUE;
	}
	*pSector = CurSector;

	// Copy to the others selected sectors
	if ( SelSectors->next != NULL )
	{
		SET_HELP_CONTEXT(Confirming_copy_of_Sector_attributes);
		if ( TConfirmSectorDialog (this, ConfirmData).Execute() == IDOK )
		{
			for (SelPtr cur = SelSectors->next ; cur != NULL ; cur = cur->next)
			{
				Sector HUGE *pSector = &Sectors[cur->objnum];
				Sector SectorBefore = *pSector;	// Copy before changing

				if ( ConfirmData.pSpecialCheck )
					pSector->special = CurSector.special;

				if ( ConfirmData.pTagCheck )
					pSector->tag = CurSector.tag;

				if ( ConfirmData.pLightCheck )
					pSector->light = CurSector.light;

				if ( ConfirmData.pFloorTextureCheck )
					strncpy (pSector->floort, CurSector.floort, 8);

				if ( ConfirmData.pFloorHeightCheck )
					pSector->floorh = CurSector.floorh;

				if ( ConfirmData.pCeilingTextureCheck )
					strncpy (pSector->ceilt, CurSector.ceilt, 8);

				if ( ConfirmData.pCeilingHeightCheck )
					pSector->ceilh = CurSector.ceilh;

				// Did we made changes ?
				if ( memcmp(pSector, &SectorBefore, sizeof(Sector)) != 0 )
					MadeChanges = TRUE;
			}
		}
		RESTORE_HELP_CONTEXT();
	}

	// Close Dialog box
	TDialog::CmOk();
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::ToFloorClicked ()
{
	if ( TextureName[0] != '\0' )
	{
		pFloorStatic->SetText (TextureName);
		ConfirmData.pFloorTextureCheck = TRUE;
	}
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::ToCeilingClicked ()
{
	if ( TextureName[0] != '\0' )
	{
		pCeilingStatic->SetText (TextureName);
		ConfirmData.pCeilingTextureCheck = TRUE;
	}
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::FloorClearClicked ()
{
	pFloorStatic->SetText ("-");
	ConfirmData.pFloorTextureCheck = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::CeilingClearClicked ()
{
	pCeilingStatic->SetText ("-");
	ConfirmData.pCeilingTextureCheck = TRUE;
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::SectorSelChange ()
{
	// Get selected item index
	int SelIndex = pSectorList->GetSelIndex ();
	if ( SelIndex < 0 )
		return;

	CurSector.special = GetSectorType(SelIndex);
	ConfirmData.pSpecialCheck = TRUE;
}



//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::NewTagClicked ()
{
	char str[10];

	wsprintf (str, "%d", FindFreeTag());
	pTagEdit->SetText (str);
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
TStatic *TSectorEditDialog::GetPointedStatic (TPoint& point)
{
	// Retreive object for handle
	TStatic *pStatic = NULL;

	if ( IsPointInDlgItem (IDC_CEILING_TEXT, point)  ||
		 IsPointInDlgItem (IDC_CEILING_FRAME, point) )
	{
		pStatic = pCeilingStatic;
	}

	else if ( IsPointInDlgItem (IDC_FLOOR_TEXT, point)  ||
			  IsPointInDlgItem (IDC_FLOOR_FRAME, point) )
	{
		pStatic = pFloorStatic;
	}

	return pStatic;
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//   Look if the dialog control 'resid' window contains the point
//   'clientPoint', which is a dialog client coord. of the point
BOOL TSectorEditDialog::IsPointInDlgItem (int itemId, TPoint &clientPoint)
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


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::TextureSelChange ()
{
	char texname[9];
	// Get selected texture name
	if ( pTextureList->GetSelString (texname, 8) <= 1 )
	{
		TextureName[0] = '\0';
		return;
	}
	// Don't do anything if no real change
	if ( strcmp (texname, TextureName) == 0 )
		return;

	strcpy (TextureName, texname);

	// If texture view dialog box opened, display texture
	if ( pFTextureDialog != NULL && pFTextureDialog->IsWindow() )
	{
		TextureDblclick();
	}
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::TextureDblclick ()
{
	// Don't select empty texture !
	if ( TextureName[0] == '\0' || strcmp (TextureName, "-") == 0 )
		return;

	// Create modeless dialog box
	if ( pFTextureDialog == NULL || pFTextureDialog->IsWindow() == FALSE )
	{
		delete pFTextureDialog;
		pFTextureDialog = new TDisplayFloorTextureDialog (Parent);
		pFTextureDialog->Create();
	}

	if ( pFTextureDialog->IsWindow() )
	{
		if ( pFTextureDialog->SelectBitmap (TextureName) < 0 )
			Notify ("Error: Cannot select the texture name \"%s\" in the "
					"dialog box of Floor/Ceiling Texture view ! (BUG)",
					TextureName);
	}
	else
		Notify ("Error: Cannot create dialog box of Floor/Ceiling "
				"Texture view !");
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::EvLButtonDown (UINT modKeys, TPoint& point)
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
		TextureSelChange();  	// Be sure to change selection
	}
}


//////////////////////////////////////////////////////////////////////
// TSectorEditDialog
// -----------------
//
void TSectorEditDialog::EvLButtonDblClk (UINT modKeys, TPoint& point)
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
		TextureSelChange();  	// Be sure to change selection
		TextureDblclick();		// Force to open Dialog Box
	}
}

