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

	FILE:         thingdlg.h

	OVERVIEW
	========
	Class definition for TThingEditDialog (TViewBitmapDialog).
*/
#if !defined(__thingdlg_h)              // Sentry, use file only if it's not already included.
#define __thingdlg_h

#ifndef __common_h
	#include "common.h"
#endif

#ifndef __viewbmp_h
	#include "viewbmp.h"
#endif

#ifndef __OWL_LISTBOX_H
	class _OWLCLASS TListBox;
#endif

#ifndef __OWL_EDIT_H
	class _OWLCLASS TEdit;
#endif

#ifndef __OWL_CHECKBOX_H
	class _OWLCLASS TCheckBox;
#endif

#ifndef __OWL_RADIOBUT_H
	class _OWLCLASS TRadioButton;
#endif

#ifndef __objects_h
	#include "objects.h"	// SelPtr, Thing
#endif

#ifndef __levels_h
	#include "levels.h"
#endif

#ifndef __things_h
	#include "things.h"
#endif

#ifndef __cnfthdlg_h
	#include "cnfthdlg.h"
#endif


#include "thingdlg.rh"            // Definition of all resources.


//{{TDialog = TThingEditDialog}}

class TThingEditDialog : public TViewBitmapDialog
{
protected:
	SelPtr SelThings;
	Thing  CurThing;
	int    ThingSet;
	char   PictureName[9];
	TConfirmThingDialogXfer ConfirmData;

protected:
	virtual void InitBitmapControl ();
	void SetupListBox ();
	void InitThingSet ();
	void GetAngle();
	void SetAngle();
	void GetAppearance();
	void SetAppearance();

public:
	TThingEditDialog (TWindow* parent, SelPtr SelectedThinges, TResId resId = IDD_THING_EDIT, TModule* module = 0);
	virtual ~TThingEditDialog ();

//{{TThingEditDialogVIRTUAL_BEGIN}}
public:
	virtual void SetupWindow ();
//{{TThingEditDialogVIRTUAL_END}}

protected:
	TEdit        *pXPosEdit;
	TEdit        *pYPosEdit;

	TRadioButton *pPlayerRadio;
	TRadioButton *pEnemyRadio;
	TRadioButton *pWeaponRadio;
	TRadioButton *pBonusRadio;
	TRadioButton *pKeysRadio;
	TRadioButton *pDecorationRadio;
	TRadioButton *pDeadBodiesRadio;
	TRadioButton *pHangingBodiesRadio;
	TRadioButton *pLightSourcesRadio;

	TRadioButton *pAngleERadio;
	TRadioButton *pAngleNRadio;
	TRadioButton *pAngleNERadio;
	TRadioButton *pAngleNWRadio;
	TRadioButton *pAngleSRadio;
	TRadioButton *pAngleSERadio;
	TRadioButton *pAngleSWRadio;
	TRadioButton *pAngleWRadio;

	TCheckBox *pDeafCheck;
	TCheckBox *pLevel12Check;
	TCheckBox *pLevel3Check;
	TCheckBox *pLevel45Check;
	TCheckBox *pMultiPlayerCheck;

	TListBox *pThingList;

//{{TThingEditDialogRSP_TBL_BEGIN}}
protected:
	void CmOk ();
	void PlayerClicked ();
	void EnemyClicked ();
	void WeaponClicked ();
	void BonusClicked ();
	void KeysClicked ();
	void DeadBodiesClicked ();
	void HangingBodiesClicked ();
	void DecorationClicked ();
	void LightSourcesClicked ();
	void ThingSelchange ();
	void ThingListDblclk ();
	void AngleClicked ();
	void Level12Clicked ();
	void Level3Clicked ();
	void Level45Clicked ();
	void DeafClicked ();
	void MultiClicked ();
	void XPosChange ();
    void YPosChange ();
//{{TThingEditDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TThingEditDialog);
};    //{{TThingEditDialogDialog}}


#endif                                      // __thingdlg_h sentry.

