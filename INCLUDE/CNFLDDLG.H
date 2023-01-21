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

	FILE:         cnflddlg.h

	OVERVIEW
	========
	Class definition for TConfirmLineDefDialog (TDialog).
*/
#if !defined(__cnflddlg_h)              // Sentry, use file only if it's not already included.
#define __cnflddlg_h

#ifndef __common_h
	#include "common.h"
#endif

#ifndef __windeu_h
	#include "windeu.h"
#endif

#ifndef __OWL_DIALOG_H
	#include <owl\dialog.h>
#endif

#ifndef __OWL_CHECKBOX_H
	class _OWLCLASS TCheckBox;
#endif

#include "cnflddlg.rh"


//{{TDialog = TConfirmLineDefDialog}}
struct TConfirmLineDefDialogXfer {
	BOOL    pTypeCheck;
	BOOL    pFlagsCheck;
	BOOL    pSectorCheck;
	BOOL    pVertex1Check;
	BOOL    pVertex2Check;
	BOOL    pSDCheck[2];
	BOOL    pImpassableCheck;
	BOOL    pAboveUnpCheck;
	BOOL    pBelowUnpCheck;
	BOOL    pBlockSoundCheck;
	BOOL    pMonCrossCheck;
	BOOL    pNotShownCheck;
	BOOL    pSecretCheck;
	BOOL    pShownCheck;
	BOOL    pTwoSidedCheck;
	BOOL    pSDAboveCheck[2];
	BOOL    pSDBelowCheck[2];
	BOOL    pSDNormalCheck[2];
	BOOL    pSDSectorCheck[2];
	BOOL    pSDXOfsCheck[2];
	BOOL    pSDYOfsCheck[2];
};


class TConfirmLineDefDialog : public TDialog
{
public:
	TConfirmLineDefDialog (TWindow* parent, TConfirmLineDefDialogXfer *pXFer, TResId resId = IDD_CONFIRM_LINEDEF, TModule* module = 0);
	virtual ~TConfirmLineDefDialog ();

protected:
	void EnableFlags ();
	void EnableSD (int sdnum);

protected:
	TCheckBox *pTypeCheck;
	TCheckBox *pFlagsCheck;
	TCheckBox *pSectorCheck;
	TCheckBox *pVertex1Check;
	TCheckBox *pVertex2Check;
	TCheckBox *pSDCheck[2];
	TCheckBox *pImpassableCheck;
	TCheckBox *pAboveUnpCheck;
	TCheckBox *pBelowUnpCheck;
	TCheckBox *pBlockSoundCheck;
	TCheckBox *pMonCrossCheck;
	TCheckBox *pNotShownCheck;
	TCheckBox *pSecretCheck;
	TCheckBox *pShownCheck;
	TCheckBox *pTwoSidedCheck;
	TCheckBox *pSDAboveCheck[2];
	TCheckBox *pSDBelowCheck[2];
	TCheckBox *pSDNormalCheck[2];
	TCheckBox *pSDSectorCheck[2];
	TCheckBox *pSDXOfsCheck[2];
	TCheckBox *pSDYOfsCheck[2];

//{{TConfirmLineDefDialogVIRTUAL_BEGIN}}
public:
	virtual void SetupWindow ();
//{{TConfirmLineDefDialogVIRTUAL_END}}

//{{TConfirmLineDefDialogRSP_TBL_BEGIN}}
protected:
	void CmOk ();
	void FlagsClicked ();
	void SD1CheckClicked ();
	void SD2CheckClicked ();
//{{TConfirmLineDefDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TConfirmLineDefDialog);
};    //{{TConfirmLineDefDialog}}

#endif                                      // __cnflddlg_h sentry.

