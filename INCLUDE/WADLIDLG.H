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

	FILE:         wadlidlg.h

	OVERVIEW
	========
	Class definition for TWadlistDialog (TDialog).
*/
#if !defined(__wadlidlg_h)              // Sentry, use file only if it's not already included.
#define __wadlidlg_h

#ifndef __common_h
	#include "common.h"
#endif

#ifndef __windeu_h
	#include "windeu.h"
#endif

#ifndef __OWL_DIALOG_H
	#include <owl\dialog.h>
#endif

#ifndef __OWL_LISTBOX_H
	class _OWLCLASS TListBox;
#endif

#ifndef __OWL_CHECKBOX_H
	class _OWLCLASS TCheckBox;
#endif

#include "wadlidlg.rh"            // Definition of all resources.


//{{TDialog = TWadlistDialog}}
class TWadlistDialog : public TDialog {
private:
	TListBox *pWadlistBox;
	TListBox *pDirlistBox;
	TCheckBox *pOnlyEMCheckBox;

public:
	TWadlistDialog (TWindow* parent, TResId resId = IDD_WADLIST, TModule* module = 0);
	virtual ~TWadlistDialog ();
	void UpdateDirlistBox ();

//{{TWadlistDialogVIRTUAL_BEGIN}}
public:
	virtual void SetupWindow ();
//{{TWadlistDialogVIRTUAL_END}}

//{{TWadlistDialogRSP_TBL_BEGIN}}
protected:
	void LBNSelChange ();
	void OnlyEMClicked ();
//{{TWadlistDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TWadlistDialog);
};    //{{TWadlistDialog}}


#endif                                      // __wadlidlg_h sentry.

