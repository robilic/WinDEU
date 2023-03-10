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

	FILE:         aboutdlg.cpp

	OVERVIEW
	========
	Source file for implementation of WindeuAboutDlg (TDialog).
*/


#include "common.h"
#pragma hdrstop

#include "aboutdlg.h"

#ifndef __windeapp_h
	#include "windeapp.h"
#endif

#ifndef __versinfo_h
	#include "versinfo.h"
#endif


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(WindeuAboutDlg, TDialog)
//{{WindeuAboutDlgRSP_TBL_BEGIN}}
	EV_WM_ERASEBKGND,
	EV_BN_CLICKED(IDC_VERSION_INFO, VersionInfoClicked),
//{{WindeuAboutDlgRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{WindeuAboutDlg Implementation}}


//////////////////////////////////////////////////////////
// WindeuAboutDlg
// --------------
// Construction/Destruction handling.
//
WindeuAboutDlg::WindeuAboutDlg (TWindow *parent, TResId resId, TModule *module)
	: TDialog(parent, resId, module),
	  LogoDib(*GetApplication(), IDB_LOGO)
{
}


//////////////////////////////////////////////////////////
// WindeuAboutDlg
// --------------
//
WindeuAboutDlg::~WindeuAboutDlg ()
{
	Destroy();
}


//////////////////////////////////////////////////////////
// WindeuAboutDlg
// --------------
//
char far* WindeuAboutDlg::GetClassName ()
{
	return "WinDEUAboutDlg";
}


//////////////////////////////////////////////////////////
// WindeuAboutDlg
// --------------
//
void WindeuAboutDlg::GetWindowClass (WNDCLASS& wndClass)
{
    TDialog::GetWindowClass(wndClass);

    wndClass.hbrBackground = NULL;
}


//////////////////////////////////////////////////////////
// WindeuAboutDlg
// --------------
//
void WindeuAboutDlg::SetupWindow ()
{
	TDialog::SetupWindow();
	::CenterWindow (this);

    // Setup size of client area as size of logo
    TRect cRect;
    TRect wRect;

    GetClientRect(cRect);
    GetWindowRect(wRect);

	int newWidth  = wRect.Width()  - cRect.Width()  + LogoDib.Width();
	int newHeight = wRect.Height() - cRect.Height() + LogoDib.Height();

	wRect.right = wRect.left + newWidth;
	wRect.bottom = wRect.top + newHeight;

	MoveWindow (wRect);
}


//////////////////////////////////////////////////////////
// WindeuAboutDlg
// --------------
//  Draw the logo in the background
//
BOOL WindeuAboutDlg::EvEraseBkgnd (HDC dc)
{
	TDC bdc(dc);
	TRect DibRect(TPoint(0,0), TSize(LogoDib.Width(), LogoDib.Height()));
	TPalette pal(LogoDib);

	TRect cRect = GetClientRect();
	cRect.right = cRect.left + LogoDib.Width();
	cRect.bottom = cRect.top + LogoDib.Height();

	bdc.SelectObject(pal);
	bdc.RealizePalette();
	bdc.StretchDIBits (cRect,
					   DibRect,
					   LogoDib);

	return TRUE;
}


//////////////////////////////////////////////////////////
// WindeuAboutDlg
// --------------
//
void WindeuAboutDlg::VersionInfoClicked ()
{
	TVersionInfo(this).Execute();
}

