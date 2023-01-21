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

	FILE:         thingdlg.cpp

	OVERVIEW
	========
	Source file for implementation of TThingEditDialog (TViewBitmapDialog).
*/

#include "common.h"
#pragma hdrstop

#include "windeu.h"

#include "thingdlg.h"

#ifndef __OWL_LISTBOX_H
	#include <owl\listbox.h>
#endif

#ifndef __OWL_CHECKBOX_H
	#include <owl\checkbox.h>
#endif

#ifndef __OWL_RADIOBUT_H
	#include <owl\radiobut.h>
#endif

#ifndef __viewbmp_h
	#include "viewbmp.h"
#endif

#ifndef __levels_h
	#include "levels.h"
#endif

#ifndef __things_h
	#include "things.h"
#endif

#ifndef __newers_h
	#include "newers.h"
#endif

#ifndef __cnfthdlg_h
	#include "cnfthdlg.h"
#endif

// HELP IDs
#include "windeuhl.h"

//
// Flags appearence bits
//
#define LEVEL12_FLAG	0x0001
#define LEVEL3_FLAG		0x0002
#define LEVEL45_FLAG	0x0004
#define DEAF_FLAG		0x0008
#define MULTI_FLAG		0x0010


//
// Type of things
//
#define TH_PLAYER			0
#define TH_ENEMY			1
#define TH_WEAPON			2
#define TH_BONUS			3
#define TH_KEYS				4
#define TH_DECORATION		5
#define TH_DEADBODIES		6
#define TH_HANGINGBODIES	7
#define TH_LIGHTSOURCES		8


//
// Array of things for each set
//

/////////////////////////////////////////////////////////////////////
//
// DOOM 1  -  DOOM2  things
//
/////////////////////////////////////////////////////////////////////

#define DOOM2_ONLY	0x8000

//
// DOOM 1 - DOOM 2
//
const SHORT D_PlayerTab[] =
{
	THING_PLAYER1,
	THING_PLAYER2,
	THING_PLAYER3,
	THING_PLAYER4,
	THING_DEATHMATCH,
	THING_TELEPORT,
	-1
};


//
// DOOM 1 - DOOM 2
//
const SHORT D_EnemyTab[] =
{
	THING_TROOPER,
	THING_SARGEANT,
	THING_IMP,
	THING_DEMON,
	THING_SPECTOR,
	THING_BARON,
	THING_LOSTSOUL,
	THING_CACODEMON,
	THING_CYBERDEMON,
	THING_SPIDERBOSS,
	THING_ARCHVILE		 	| DOOM2_ONLY,
	THING_CHAINGUNNER	 	| DOOM2_ONLY,
	THING_REVENANT		 	| DOOM2_ONLY,
	THING_MANCUBUS		 	| DOOM2_ONLY,
	THING_ARACHNOTRON	 	| DOOM2_ONLY,
	THING_HELLKNIGHT	 	| DOOM2_ONLY,
	THING_PAINELEMENTAL	 	| DOOM2_ONLY,
	THING_WOLFENSTEIN_SS 	| DOOM2_ONLY,
	THING_SPAWNSPOT		 	| DOOM2_ONLY,
	THING_BOSSBRAIN		 	| DOOM2_ONLY,
	THING_BOSSSHOOTER	 	| DOOM2_ONLY,
	-1
};

const SHORT D_WeaponTab[] =
{
	THING_SHOTGUN,
	THING_SUPERSHOTGUN 		| DOOM2_ONLY,
	THING_CHAINGUN,
	THING_LAUNCHER,
	THING_PLASMAGUN,
	THING_CHAINSAW,
	THING_SHELLS,
	THING_AMMOCLIP,
	THING_ROCKET,
	THING_ENERGYCELL,
	THING_BFG9000,
	// Ammunition
	THING_SHELLBOX,
	THING_AMMOBOX,
	THING_ROCKETBOX,
	THING_ENERGYPACK,
	THING_BACKPACK,
	-1
};

const SHORT D_BonusTab[] =
{
	THING_ARMBONUS1,
	THING_GREENARMOR,
	THING_BLUEARMOR,
	THING_HLTBONUS1,
	THING_STIMPACK,
	THING_MEDKIT,
	THING_SOULSPHERE,
	THING_BLURSPHERE,
	THING_MAP,
	THING_RADSUIT,
	THING_LITEAMP,
	THING_BESERK,
	THING_INVULN,
	THING_MEGASPHERE		| DOOM2_ONLY,
	-1
};

const SHORT D_KeysTab[] =
{
	THING_REDCARD,
	THING_YELLOWCARD,
	THING_BLUECARD,
	THING_REDSKULLKEY,
	THING_YELLOWSKULLKEY,
	THING_BLUESKULLKEY,
	-1
};

const SHORT D_DecorationTab[] =
{
	THING_BARREL,
	THING_TECHCOLUMN,
	THING_TGREENPILLAR,
	THING_TREDPILLAR,
	THING_SGREENPILLAR,
	THING_SREDPILLAR,
	THING_PILLARHEART,
	THING_PILLARSKULL,
	THING_EYEINSYMBOL,
	THING_BROWNSTUB,
	THING_GREYTREE,
	THING_BROWNTREE,
	THING_FUELCAN,
	THING_STALAGMITE,
	-1
};

const SHORT D_DeadBodiesTab[] =
{
	THING_DEADPLAYER,
	THING_DEADTROOPER,
	THING_DEADSARGEANT,
	THING_DEADIMP,
	THING_DEADDEMON,
	THING_DEADCACODEMON,
	THING_DEADLOSTSOUL,
	THING_BONES,
	THING_BONES2,
	THING_POOLOFBLOOD,
	THING_SKULLTOPPOLE,
	THING_HEADSKEWER,
	THING_PILEOFSKULLS,
	THING_IMPALEDBODY,
	THING_IMPALEDBODY2,
	THING_SKULLSINFLAMES,
	-1
};


const SHORT D_HangingBodiesTab[] =
{
	THING_HANGINGSWAYING,
	THING_HANGINGARMSOUT,
	THING_HANGINGONELEG,
	THING_HANGINGTORSO,
	THING_HANGINGLEG,
	THING_HANGINGSWAYING2,
	THING_HANGINGARMSOUT2,
	THING_HANGINGONELEG2,
	THING_HANGINGTORSO2,
	THING_HANGINGLEG2,
	// DOOM 2
	THING_KEEN 				| DOOM2_ONLY,
	-1
};


const SHORT D_LightSourcesTab[] =
{
	THING_CANDLE,
	THING_LAMP,
	THING_CANDELABRA,
	THING_TBLUETORCH,
	THING_TGREENTORCH,
	THING_TREDTORCH,
	THING_SBLUETORCH,
	THING_SGREENTORCH,
	THING_SREDTORCH,
	-1
};


//
// DOOM 1 - DOOM 2
//
const SHORT *D_ThingSetTab[] =
{
	D_PlayerTab,
	D_EnemyTab,
	D_WeaponTab,
	D_BonusTab,
	D_KeysTab,
	D_DecorationTab,
	D_DeadBodiesTab,
	D_HangingBodiesTab,
	D_LightSourcesTab,
};

const NB_D_THINGSET = sizeof (D_ThingSetTab) / sizeof (SHORT *);


/////////////////////////////////////////////////////////////////////
//
// HERETIC  things
//
/////////////////////////////////////////////////////////////////////

const SHORT H_PlayerTab[] =
{
	1, 2, 3, 4,
	11, 14, 52, 74,
	-1
};


const SHORT H_EnemyTab[] =
{
	5, 6, 7, 9, 15, 45, 46, 56, 64,
	65, 66, 68, 69, 70, 90, 92,
	-1
};

const SHORT H_WeaponTab[] =
{
	53, 2001, 2002, 2003, 2004, 2005,
	// Ammunition
	8, 10, 12, 13, 16, 18, 19, 20, 21,
	22, 23, 54, 55,
	-1
};

const SHORT H_BonusTab[] =
{
	30, 31, 32, 33, 34, 35, 36,
	75, 81, 82, 83, 84, 85, 86,
	-1
};

const SHORT H_KeysTab[] =
{
	73, 79, 80,
	-1
};

const SHORT H_DecorationTab[] =
{
	29, 43, 47, 48, 49,
	94, 95, 96, 2035,
	-1
};

const SHORT H_SoundsTab[] =
{
	41, 42,
	1200, 1201, 1202, 1203, 1204, 1205,
	1206, 1207, 1208, 1209,
	-1
};


const SHORT H_HangingBodiesTab[] =
{
	17, 24, 25, 26, 37, 38, 39, 40,
	51,
	-1
};


const SHORT H_LightSourcesTab[] =
{
	27, 28, 44, 50, 76, 87,
	-1
};


const SHORT *H_ThingSetTab[] =
{
	H_PlayerTab,
	H_EnemyTab,
	H_WeaponTab,
	H_BonusTab,
	H_KeysTab,
	H_DecorationTab,
	H_SoundsTab,
	H_HangingBodiesTab,
	H_LightSourcesTab,
};

const NB_H_THINGSET = sizeof (H_ThingSetTab) / sizeof (SHORT *);


//
// Returns the numbers of thing sets
//
inline int GetNbThingSet(void)
{
	return DoomVersion == 16 ? NB_H_THINGSET : NB_D_THINGSET;
}


//
// Return the thing set number 'i'
//
const SHORT *GetThingSet(int i)
{
	assert (i >= 0  &&  i < GetNbThingSet());
	if ( DoomVersion == 16 )		return H_ThingSetTab[i];
	else							return D_ThingSetTab[i];
}


//
// Is the thing type 'i' a valid thing from set 'tset'
// (Used to ignore DOOM2 things from thing sets)
//
BOOL IsThingType(int tset, int i)
{
	assert (tset >= 0  &&  tset < GetNbThingSet());
	if ( (::DoomVersion == 1) && (GetThingSet(tset)[i] & DOOM2_ONLY) )
	{
		return FALSE;
	}

	return TRUE;
}


//
// Find a thing set from a thing number
// (-1 if thing is not in a set)
//
int FindThingSet(SHORT tnum)
{
	int tset = -1;

	for (int i = 0; (i < GetNbThingSet())  &&  (tset < 0); i++)
	{
		SHORT type;
		for (int j = 0; (type = GetThingSet(i)[j]) != -1; j++)
		{
			// Skip DOOM2_only entries if DOOM1 version
			if ( IsThingType(i, j) )
			{
				type &= ~DOOM2_ONLY;
				if ( type == tnum )
				{
					tset = i;
					break;
				}
			}
		}
	}

	return tset;
}



//
// Find a thing type from a thing set and an index in the set
// (This function must skip DOOM2 entries if DOOM1 version)
//
SHORT FindThingType(int tset, int tindex)
{
	assert (tset >= 0  &&  tset < GetNbThingSet());
	SHORT tnum = -1;
	SHORT curindex = 0;
	SHORT type;

	for (int j = 0; (type = GetThingSet(tset)[j]) != -1; j++)
	{
		// Skip DOOM2_only entries if DOOM1 version
		if ( IsThingType(tset, j) )
		{
			// Compare current index and needed index
			type &= ~DOOM2_ONLY;
			if ( tindex == curindex )
			{
				tnum = type;
				break;
			}
			curindex++;
		}
	}

	return tnum;
}


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(TThingEditDialog, TViewBitmapDialog)
//{{TThingEditDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_BONUS, BonusClicked),
	EV_BN_CLICKED(IDC_DEAD_BODIES, DeadBodiesClicked),
	EV_BN_CLICKED(IDC_DECORATION, DecorationClicked),
	EV_BN_CLICKED(IDC_ENEMY, EnemyClicked),
	EV_BN_CLICKED(IDC_LIGHT_SOURCES, LightSourcesClicked),
	EV_BN_CLICKED(IDC_WEAPON, WeaponClicked),
	EV_BN_CLICKED(IDC_PLAYER, PlayerClicked),
	EV_BN_CLICKED(IDC_KEYS, KeysClicked),
	EV_BN_CLICKED(IDC_HANGINGBODIES, HangingBodiesClicked),
	EV_LBN_SELCHANGE(IDC_THING_LIST, ThingSelchange),
	EV_LBN_DBLCLK(IDC_THING_LIST, ThingListDblclk),
	EV_BN_CLICKED(IDOK, CmOk),
	EV_BN_CLICKED(IDC_ANGLE_E, AngleClicked),
	EV_BN_CLICKED(IDC_ANGLE_N, AngleClicked),
	EV_BN_CLICKED(IDC_ANGLE_NE, AngleClicked),
	EV_BN_CLICKED(IDC_ANGLE_NW, AngleClicked),
	EV_BN_CLICKED(IDC_ANGLE_S, AngleClicked),
	EV_BN_CLICKED(IDC_ANGLE_SE, AngleClicked),
	EV_BN_CLICKED(IDC_ANGLE_SW, AngleClicked),
	EV_BN_CLICKED(IDC_ANGLE_W, AngleClicked),
	EV_BN_CLICKED(IDC_LEVEL12, Level12Clicked),
	EV_BN_CLICKED(IDC_LEVEL3, Level3Clicked),
	EV_BN_CLICKED(IDC_LEVEL45, Level45Clicked),
	EV_BN_CLICKED(IDC_DEAF, DeafClicked),
	EV_BN_CLICKED(IDC_MULTIPLAYER, MultiClicked),
//{{TThingEditDialogRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TThingEditDialog Implementation}}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
TThingEditDialog::TThingEditDialog (TWindow* parent, SelPtr SelectedThings, TResId resId, TModule* module):
	TViewBitmapDialog(parent, resId, module)
{
	SelThings = SelectedThings;
	CurThing  = Things[SelectedThings->objnum];
	strcpy(PictureName, "");
	memset (&ConfirmData, 0, sizeof(ConfirmData));

	pXPosEdit = newTEdit (this, IDC_THING_XPOS, 7);
	pXPosEdit->SetValidator(new TRangeValidator(MapMinX, MapMaxX));
	pYPosEdit = newTEdit (this, IDC_THING_YPOS, 7);
	pYPosEdit->SetValidator(new TRangeValidator(MapMinY, MapMaxY));

	pPlayerRadio = newTRadioButton (this, IDC_PLAYER);
	pEnemyRadio = newTRadioButton (this, IDC_ENEMY);
	pWeaponRadio = newTRadioButton (this, IDC_WEAPON);
	pBonusRadio = newTRadioButton (this, IDC_BONUS);
	pKeysRadio = newTRadioButton (this, IDC_KEYS);
	pDecorationRadio = newTRadioButton (this, IDC_DECORATION);
	pDeadBodiesRadio = newTRadioButton (this, IDC_DEAD_BODIES);
	pHangingBodiesRadio = newTRadioButton (this, IDC_HANGINGBODIES);
	pLightSourcesRadio = newTRadioButton (this, IDC_LIGHT_SOURCES);

	pAngleERadio = newTRadioButton(this, IDC_ANGLE_E, 0);
	pAngleNRadio = newTRadioButton(this, IDC_ANGLE_N, 0);
	pAngleNERadio = newTRadioButton(this, IDC_ANGLE_NE, 0);
	pAngleNWRadio = newTRadioButton(this, IDC_ANGLE_NW, 0);
	pAngleSRadio = newTRadioButton(this, IDC_ANGLE_S, 0);
	pAngleSERadio = newTRadioButton(this, IDC_ANGLE_SE, 0);
	pAngleSWRadio = newTRadioButton(this, IDC_ANGLE_SW, 0);
	pAngleWRadio = newTRadioButton(this, IDC_ANGLE_W, 0);

	pDeafCheck = newTCheckBox(this, IDC_DEAF, 0);
	pLevel12Check = newTCheckBox(this, IDC_LEVEL12, 0);
	pLevel3Check = newTCheckBox(this, IDC_LEVEL3, 0);
	pLevel45Check = newTCheckBox(this, IDC_LEVEL45, 0);
	pMultiPlayerCheck = newTCheckBox(this, IDC_MULTIPLAYER, 0);

	pThingList = newTListBox (this, IDC_THING_LIST);
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
TThingEditDialog::~TThingEditDialog ()
{
	Destroy();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//  Retrieve Thing set from the first selected thing type.
//  and update the thing set check boxes.
void
TThingEditDialog::InitThingSet ()
{
	ThingSet = FindThingSet(CurThing.type);

	pPlayerRadio->SetCheck (ThingSet == TH_PLAYER ? BF_CHECKED : BF_UNCHECKED);
	pEnemyRadio->SetCheck (ThingSet == TH_ENEMY ? BF_CHECKED : BF_UNCHECKED);
	pWeaponRadio->SetCheck (ThingSet == TH_WEAPON ? BF_CHECKED : BF_UNCHECKED);
	pBonusRadio->SetCheck (ThingSet == TH_BONUS ? BF_CHECKED : BF_UNCHECKED);
	pKeysRadio->SetCheck (ThingSet == TH_KEYS ? BF_CHECKED : BF_UNCHECKED);
	pDecorationRadio->SetCheck (ThingSet == TH_DECORATION ? BF_CHECKED : BF_UNCHECKED);
	pDeadBodiesRadio->SetCheck (ThingSet == TH_DEADBODIES ? BF_CHECKED : BF_UNCHECKED);
	pHangingBodiesRadio->SetCheck (ThingSet == TH_HANGINGBODIES ? BF_CHECKED : BF_UNCHECKED);
	pLightSourcesRadio->SetCheck (ThingSet == TH_LIGHTSOURCES ? BF_CHECKED : BF_UNCHECKED);
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
void
TThingEditDialog::SetupWindow ()
{
	TViewBitmapDialog::SetupWindow();
	::CenterWindow (this);

	// No dead bodies, but sounds in HERETIC
	if ( ::DoomVersion == 16 )
	{
		pDeadBodiesRadio->SetCaption("Sounds");
	}

	// Retreive thing set and setup check boxes
	InitThingSet ();

	// Add thing types of thing set in list box
	SetupListBox();

	// Set angle and appearence boxes
	SetAngle();
	SetAppearance();

	// Insert thing position in edit boxes
	char tmp[20];
	sprintf (tmp, "%d", CurThing.xpos);
	pXPosEdit->SetText(tmp);

	sprintf (tmp, "%d", CurThing.ypos);
	pYPosEdit->SetText(tmp);

	// Show thing picture (Things appears immediately)
	ThingListDblclk();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------------
// Fills the thing type list box with the thing set types.
//
void
TThingEditDialog::SetupListBox ()
{
	SHORT type;

	pThingList->ClearList();

	// If no available thing set, display only one name
	if ( ThingSet < 0  ||  ThingSet >= GetNbThingSet() )
	{
		char thname[128];
		strcpy (thname, GetThingName (CurThing.type));
		pThingList->AddString (thname);
		return;
	}

	// Display all names of the thing set
	for ( int i = 0 ;
		  (type = GetThingSet(ThingSet)[i]) != -1;
		  i++)
	{
		// Skip DOOM2_Only if DOOM1 version
		if ( IsThingType(ThingSet, i) )
		{
			char thname[128];
			type &= ~DOOM2_ONLY;
			strcpy (thname, GetThingName(type));
			pThingList->AddString (thname);

			if ( CurThing.type == type )
				pThingList->SetSelString (thname, -1);
		}
	}
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
void
TThingEditDialog::InitBitmapControl ()
{
	pBitmapControl = new TSprite256Control (this, IDC_BITMAP_CONTROL, "",
											BitmapControlX, BitmapControlY,
											BitmapControlW, BitmapControlH);
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
void TThingEditDialog::PlayerClicked ()
{
	ThingSet = TH_PLAYER;
	SetupListBox ();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
void TThingEditDialog::WeaponClicked ()
{
	ThingSet = TH_WEAPON;
	SetupListBox ();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
void TThingEditDialog::EnemyClicked ()
{
	ThingSet = TH_ENEMY;
	SetupListBox ();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
void TThingEditDialog::BonusClicked ()
{
	ThingSet = TH_BONUS;
	SetupListBox ();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
void TThingEditDialog::KeysClicked ()
{
	ThingSet = TH_KEYS;
	SetupListBox ();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
void TThingEditDialog::DeadBodiesClicked ()
{
	ThingSet = TH_DEADBODIES;
	SetupListBox ();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
void TThingEditDialog::HangingBodiesClicked ()
{
	ThingSet = TH_HANGINGBODIES;
	SetupListBox ();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------------
//
void TThingEditDialog::DecorationClicked ()
{
	ThingSet = TH_DECORATION;
	SetupListBox ();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------
//
void TThingEditDialog::LightSourcesClicked ()
{
	ThingSet = TH_LIGHTSOURCES;
	SetupListBox ();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------------
//
void TThingEditDialog::CmOk ()
{
	// Cannot close if edit controls not valid
	if ( !CanClose() )
		return;

	// Get thing position
	char tmp[20];
	pXPosEdit->GetText(tmp, 9);
	if ( CurThing.xpos != atoi(tmp) )	ConfirmData.pXPosCheck = TRUE;
	CurThing.xpos = atoi(tmp);

	pYPosEdit->GetText(tmp, 9);
	if ( CurThing.ypos != atoi(tmp) )	ConfirmData.pYPosCheck = TRUE;
	CurThing.ypos = atoi(tmp);

	// Get angle and appearence
	GetAngle();
	GetAppearance();

	// Update confirm data for X and Y pos
	// DEBUG: This doesn't work !!! (always FALSE)
	// ConfirmData.pXPosCheck = pXPosEdit->IsModified();
	// ConfirmData.pYPosCheck = pYPosEdit->IsModified();

	// Do we made changes ?
	TPtr pThing = &Things[SelThings->objnum];

	if ( memcmp (pThing, &CurThing, sizeof(Thing)) != 0 )
	{
		MadeChanges = TRUE;
	}

	// Update first thing in list
	*pThing = CurThing;

	// If more than one thing, copy selected attributes to them
	if ( SelThings->next != NULL )
	{
		SET_HELP_CONTEXT(Confirming_copy_of_Thing_attributes);

		if ( TConfirmThingDialog (this, ConfirmData).Execute() == IDOK )
		{
			// Copy the selected attributes of CurThing to the selection list
			for (SelPtr cur = SelThings->next ; cur != NULL ; cur = cur->next )
			{
				Thing HUGE *pThing = &Things[cur->objnum];
				Thing ThingBefore = *pThing;	// Copy before changes

				if ( ConfirmData.pXPosCheck )	pThing->xpos  = CurThing.xpos;
				if ( ConfirmData.pYPosCheck )	pThing->ypos  = CurThing.ypos;
				if ( ConfirmData.pAngleCheck ) 	pThing->angle = CurThing.angle;
				if ( ConfirmData.pTypeCheck )  	pThing->type  = CurThing.type;

				// Copy appearence flag
				if ( ConfirmData.pLevel12Check )
					if ( CurThing.when & LEVEL12_FLAG )
						pThing->when  |= LEVEL12_FLAG;
					else
						pThing->when  &= ~LEVEL12_FLAG;

				if ( ConfirmData.pLevel3Check )
					if ( CurThing.when & LEVEL3_FLAG )
						pThing->when  |= LEVEL3_FLAG;
					else
						pThing->when  &= ~LEVEL3_FLAG;

				if ( ConfirmData.pLevel45Check )
					if ( CurThing.when & LEVEL45_FLAG )
						pThing->when  |= LEVEL45_FLAG;
					else
						pThing->when  &= ~LEVEL45_FLAG;

				if ( ConfirmData.pDeafCheck )
					if ( CurThing.when & DEAF_FLAG )
						pThing->when  |= DEAF_FLAG;
					else
						pThing->when  &= ~DEAF_FLAG;

				if ( ConfirmData.pMultiCheck )
					if ( CurThing.when & MULTI_FLAG )
						pThing->when  |= MULTI_FLAG;
					else
						pThing->when  &= ~MULTI_FLAG;

				// Did we made changes?
				if ( memcmp(pThing, &ThingBefore, sizeof(Thing)) != 0 )
					MadeChanges;
			}
		}
		RESTORE_HELP_CONTEXT();
	}

	// Close dialog box
	TViewBitmapDialog::CmOk();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------------
//
void TThingEditDialog::GetAngle ()
{
	if ( pAngleERadio->GetCheck() == BF_CHECKED )
		CurThing.angle = 0;

	else if ( pAngleNERadio->GetCheck() == BF_CHECKED )
		CurThing.angle = 45;

	else if ( pAngleNRadio->GetCheck() == BF_CHECKED )
		CurThing.angle = 90;

	else if ( pAngleNWRadio->GetCheck() == BF_CHECKED )
		CurThing.angle = 135;

	else if ( pAngleWRadio->GetCheck() == BF_CHECKED )
		CurThing.angle = 180;

	else if ( pAngleSWRadio->GetCheck() == BF_CHECKED )
		CurThing.angle = 225;

	else if ( pAngleSRadio->GetCheck() == BF_CHECKED )
		CurThing.angle = 270;

	else if ( pAngleSERadio->GetCheck() == BF_CHECKED )
		CurThing.angle = 315;
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------------
//
void TThingEditDialog::SetAngle ()
{
	if ( CurThing.angle == 0 )
		pAngleERadio->SetCheck(BF_CHECKED);

	else if ( CurThing.angle == 45 )
		pAngleNERadio->SetCheck(BF_CHECKED);

	else if ( CurThing.angle == 90 )
		pAngleNRadio->SetCheck(BF_CHECKED);

	else if ( CurThing.angle == 135 )
		pAngleNWRadio->SetCheck(BF_CHECKED);

	else if ( CurThing.angle == 180 )
		pAngleWRadio->SetCheck(BF_CHECKED);

	else if ( CurThing.angle == 225 )
		pAngleSWRadio->SetCheck(BF_CHECKED);

	else if ( CurThing.angle == 270 )
		pAngleSRadio->SetCheck(BF_CHECKED);

	else if ( CurThing.angle == 315 )
		pAngleSERadio->SetCheck(BF_CHECKED);
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------------
//
void TThingEditDialog::GetAppearance ()
{
	if ( pLevel12Check->GetCheck() == BF_CHECKED )
		CurThing.when |= LEVEL12_FLAG;
	else
		CurThing.when &= ~LEVEL12_FLAG;

	if ( pLevel3Check->GetCheck() == BF_CHECKED )
		CurThing.when |= LEVEL3_FLAG;
	else
		CurThing.when &= ~LEVEL3_FLAG;

	if ( pLevel45Check->GetCheck() == BF_CHECKED )
		CurThing.when |= LEVEL45_FLAG;
	else
		CurThing.when &= ~LEVEL45_FLAG;

	if ( pDeafCheck->GetCheck() == BF_CHECKED )
		CurThing.when |= DEAF_FLAG;
	else
		CurThing.when &= ~DEAF_FLAG;

	if ( pMultiPlayerCheck->GetCheck() == BF_CHECKED )
		CurThing.when |= MULTI_FLAG;
	else
		CurThing.when &= ~MULTI_FLAG;
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------------
//
void TThingEditDialog::SetAppearance ()
{
	pLevel12Check->SetCheck ( (CurThing.when & LEVEL12_FLAG) ? BF_CHECKED : BF_UNCHECKED);
	pLevel3Check->SetCheck ( (CurThing.when & LEVEL3_FLAG) ? BF_CHECKED : BF_UNCHECKED);
	pLevel45Check->SetCheck ( (CurThing.when & LEVEL45_FLAG) ? BF_CHECKED : BF_UNCHECKED);
	pDeafCheck->SetCheck ( (CurThing.when & DEAF_FLAG) ? BF_CHECKED : BF_UNCHECKED);
	pMultiPlayerCheck->SetCheck ( (CurThing.when & MULTI_FLAG) ? BF_CHECKED : BF_UNCHECKED);
}



//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------------
//  The user changed the selected thing type in the list box,
//  we update the CurThing struct
void TThingEditDialog::ThingSelchange ()
{
	if ( ThingSet < 0  ||  ThingSet >= GetNbThingSet() )
		return;

	// Get selected string
	int SelIndex = pThingList->GetSelIndex ();
	if ( SelIndex < 0)
		return;

	CurThing.type = FindThingType(ThingSet, SelIndex);
	ConfirmData.pTypeCheck = TRUE;	// Remember in confirm data

	// Display thing picture
	ThingListDblclk();
}


//////////////////////////////////////////////////////////////////////
// TThingEditDialog
// ----------------
//   The user double clicked on the thing type.
//   Display the thing picture in the preview group.
//   NOTE: This function is now called when any selection
//         is done, because bitmap display is now much faster
//         than before.
void TThingEditDialog::ThingListDblclk ()
{
	char picname[9];

	// CurThing.Type is always meaninggul at this time
	// since we received a 'LBN_SETSEL' message early
	strcpy (picname, GetThingPicName(CurThing.type));

	// If same picture, nothing to do
	if ( stricmp(picname, PictureName)    == 0  &&
		 strnicmp(PictureName, "PLAY", 4) != 0 )
		return;
	strcpy (PictureName, picname);


	// Choose doom palette is player start position
	SHORT remap;

	switch (CurThing.type)
	{
		case THING_PLAYER1:
			remap =  0x00;
			break;
		case THING_PLAYER2:
			remap = -0x10;
			break;
		case THING_PLAYER3:
			remap = -0x30;
			break;
		case THING_PLAYER4:
			remap =  0x40;
			break;
		default:
			remap =  0x00;
			break;
	}

	pBitmapControl->SelectBitmap(picname, remap);
}


/////////////////////////////////////////////////////////////////////
//
// Functions to setup the ConfirmData fields when the user changes
// something in the dialog box.
//
/////////////////////////////////////////////////////////////////////

void TThingEditDialog::AngleClicked ()
{
	ConfirmData.pAngleCheck = TRUE;
}

void TThingEditDialog::Level12Clicked ()
{
	ConfirmData.pLevel12Check = TRUE;
}


void TThingEditDialog::Level3Clicked ()
{
	ConfirmData.pLevel3Check = TRUE;
}


void TThingEditDialog::Level45Clicked ()
{
	ConfirmData.pLevel45Check = TRUE;
}


void TThingEditDialog::DeafClicked ()
{
	ConfirmData.pDeafCheck = TRUE;
}


void TThingEditDialog::MultiClicked ()
{
	ConfirmData.pMultiCheck = TRUE;
}

