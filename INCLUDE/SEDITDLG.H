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

	FILE:         seditdlg.h

	OVERVIEW
	========
	Class definition for TSectorEditDialog (TDialog).
*/
#if !defined(__seditdlg_h)              // Sentry, use file only if it's not already included.
#define __seditdlg_h

#ifndef __common_h
	#include "common.h"
#endif


#ifndef __OWL_DIALOG_H
	#include <owl\dialog.h>
#endif

#ifndef __OWL_LISTBOX_H
	class _OWLCLASS TListBox;
#endif

#ifndef __OWL_STATIC_H
	class _OWLCLASS TStatic;
#endif

#ifndef __OWL_EDIT_H
	class _OWLCLASS TEdit;
#endif

#ifndef __wstructs_h
	#include "wstructs.h"	// Sector
#endif

#ifndef __objects_h
	#include "objects.h"	// SelPtr
#endif

#ifndef __viewbmp_h
	class TDisplayFloorTextureDialog;
#endif

#ifndef __cnfsedlg_h
	#include "cnfsedlg.h"
#endif

#include "seditdlg.rh"            // Definition of all resources.

//{{TDialog = TSectorEditDialog}}


class TSectorEditDialog : public TDialog
{
private:
	Sector CurSector;
	char TextureName[9];
	SelPtr SelSectors;
	TDisplayFloorTextureDialog *pFTextureDialog;
	TConfirmSectorDialogXfer ConfirmData;

protected:
	void SetTextureList();
	void SetSectorList();
	void SetSector();
	void GetSector();
	BOOL IsPointInDlgItem (int itemId, TPoint &clientPoint);
	TStatic *GetPointedStatic (TPoint& point);

public:
	TSectorEditDialog (TWindow* parent, SelPtr sel, TResId resId = IDD_SECTOR_EDIT, TModule* module = 0);
	virtual ~TSectorEditDialog ();

//{{TSectorEditDialogVIRTUAL_BEGIN}}
public:
	virtual void SetupWindow ();
//{{TSectorEditDialogVIRTUAL_END}}

//{{TSectorEditDialogRSP_TBL_BEGIN}}
protected:
	void CmOk ();
	void ToFloorClicked ();
	void FloorClearClicked ();
	void ToCeilingClicked ();
	void CeilingClearClicked ();
	void SectorSelChange ();
	void TextureSelChange ();
	void TextureDblclick ();
	void NewTagClicked ();
	void EvLButtonDown (UINT modKeys, TPoint& point);
	void EvLButtonDblClk (UINT modKeys, TPoint& point);
//{{TSectorEditDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSectorEditDialog);

protected:
	TEdit *pTagEdit;
	TListBox *pSectorList;
	TEdit *pLightEdit;
	TListBox *pTextureList;
	TStatic *pFloorStatic;
	TEdit *pFloorHeightEdit;
	TStatic *pCeilingStatic;
	TEdit *pCeilingHeightEdit;
};    //{{TSectorEditDialog}}


#endif                                      // __seditdlg_h sentry.

