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

	FILE:         windeapp.cpp

	OVERVIEW
	========
	Source file for implementation of WinDEUApp (TApplication).
*/


#include "common.h"
#pragma hdrstop
#include <time.h>
#include <dir.h> // MAXPATH

#if 0
	#include <ctl3d.h>
#endif

#ifndef __windeapp_h
	#include "windeapp.h"		    // Definition of Application
#endif

#ifndef __windeu_h
	#include "windeu.h"
#endif

#ifndef __windeapp_rh
	#include "windeapp.rh"
#endif

#ifndef __wads_h
	#include "wads.h"
#endif

#ifndef __mainfram_h
	#include "mainfram.h"           // Definition of TMainFrame
#endif

#ifndef __aboutdlg_h
	#include "aboutdlg.h"
#endif


//
// Help constants
//
#include "windeuhl.h"

//
// Main window title
//
#ifdef __WIN32__
#define TITLE "WinDEU32: DEU for Windows 5.24"
#else
#define TITLE "WinDEU: DEU for Windows 5.24"
#endif

//
// Generated help file.
//
const char HelpName[] = "windeu.hlp";
static char HelpFileName[MAXPATH];

//
// Context sensitive help item ID
//
DWORD HelpContext = Contents;

//
// WinHelp opened?
//
BOOL HelpOpened = FALSE;

//
// Hook Handle returned by SetWindowsHookEx(WH_KEYBOARD, ...)
//
HHOOK hKeyboardHook = NULL;

//
// Hook Callback function called each time a key is pressed or released
// We only check for 'F1' key to do contextual help.
//
// int nCode;		// hook code
// WPARAM wParam;	// virtual-key code
// LPARAM lParam;	// keystroke-message information
//
// We must define it extern "C" because it's a Windows Callback Function
// Note: CALLBACK is defined as _far _pascal in WINDOWS.H (Win16)
//                is defined as __stdcall    in WINDEF.H  (Win32)
//       It means the Win32 hook function is case sensitive, so we
//       must change the .DEF file.
//
extern "C"
{
extern LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// Call next hook in chain and return if nCode less than 0
	if ( nCode >= 0 )
	{
		// Call WinHelp if F1 key is pressed
		if ( ( wParam                == VK_F1 ) && /* F1          */
			 ((lParam & 0x80000000L) == 0     )    /* Key pressed */ )
		{
			WinDEUApp *app = TYPESAFE_DOWNCAST(::Module, WinDEUApp);
			app->AppHelp(HELP_CONTEXT, HelpContext);

			// Return 1 to avoid passing message to the active window
			// (The message is not pushed in the queue)
			return 1;
		}
	}

	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

//
// Called at exit
//
void _USERENTRY CleanupApp()
{
	if ( hKeyboardHook != NULL )
		UnhookWindowsHookEx (hKeyboardHook);
	CleanupWindeu();
}


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(WinDEUApp, TApplication)
//{{WinDEUAppRSP_TBL_BEGIN}}
	EV_COMMAND(CM_HELP_ABOUT, CmHelpAbout),
	EV_COMMAND(CM_HELP_CONTENTS, CmHelpContents),
	EV_COMMAND(CM_HELP_SEARCH, CmHelpSearch),
	EV_COMMAND(CM_HELP_USING_HELP, CmHelpUsingHelp),
	EV_COMMAND(CM_HELP_TUTORIAL, CmHelpTutorial),
//{{WinDEUAppRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{WinDEUApp Implementation}}


//////////////////////////////////////////////////////////
// WinDEUApp
// ---------
//
WinDEUApp::WinDEUApp (int _argc, char **_argv):
	TApplication(TITLE)
{
	// Copy command options to local members
	argc = _argc;
	argv = _argv;

	atexit(CleanupApp);
}


//////////////////////////////////////////////////////////
// WinDEUApp
// ---------
//
WinDEUApp::~WinDEUApp ()
{
}


//////////////////////////////////////////////////////////
// WinDEUApp
// ---------
// MainWindow intialization.
void WinDEUApp::InitMainWindow ()
{
	MainWindow = new TMainFrame (GetName());
}


//////////////////////////////////////////////////////////
// WinDEUApp
// ---------
// Application instance intialisation. Read the INI file
// for default application options.
void WinDEUApp::InitInstance ()
{
	// Initialize and create MainWindow
	TApplication::InitInstance();

	// Retrieve module directory to construct help file full path
	if ( GetModuleFileName (HelpFileName, MAXPATH) )
	{
		for (int i = strlen(HelpFileName) - 1;
			 (i >= 0) && (HelpFileName[i] != '\\');
			 i--)
			;

		HelpFileName[i+1] = '\0';

		strcat (HelpFileName, HelpName);
	}

	// Keyboard hook for F1 help
#ifdef __WIN32__
	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD,
									 KeyboardProc,
									 *this,
									 0);
#else
	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD,
									 KeyboardProc,
									 *this,
									 ::GetCurrentTask());
#endif
	// Parse command line and INI file.
	InitWindeu(argc, argv);

	// Enable CTL3D controls if needed
	Enable3DControls(Use3DControls);

	// Create and display credits for WinDEU
	CmHelpAbout();

	//RP moved from InitWinDEU
	if (Quieter == TRUE)
		Quiet = TRUE;
	if (Reminder == TRUE)
		FunnyMessage();

	/* load the wad files */
	OpenMainWad(MainWad);
	if (PatchWads)
		while (PatchWads[ 0])
		{
			OpenPatchWad( strupr( PatchWads[ 0]));
			PatchWads++;
		}

	/* sanity check */
	CloseUnusedWadFiles();
}


//////////////////////////////////////////////////////////
// WinDEUApp
// ---------
// Application instance intialisation. Read the INI file
// for default application options.
void WinDEUApp::Enable3DControls (BOOL enable)
{
/*
	//
	// If we are running Win95 or greater, then ctl3d is unnecessary
	//
	// Note: This line display 70005F03 got GetVerion() under the final
	//       beta release of Win95. So, the Win95 version is 395 (0x03 + 0x5F)
	// Notify ("GetVersion() : %08lx", ::GetVersion());
	if ( LOBYTE(LOWORD(::GetVersion())) * 100 +
		 HIBYTE(LOWORD(::GetVersion())) >= 395)
	   return;
*/

#if 0
	static enabled = FALSE;

	if ( enable == enabled )
		return;

	if ( enable )
	{
		Ctl3dRegister(*this);
		Ctl3dAutoSubclass(*this);
	}
	else
		Ctl3dUnregister(*this);
	enabled = enable;
#else
	if ( ! enable )
	{
		EnableCtl3d(FALSE);
		return;
	}

	// Don't try to define this message in the 'CATCH' bloc,
	// because we can't use var definition in it.
	// Maybe a Borland bug ?
	char *ErrorMsg =
			"Can't load 3D controls Dynamic Library.\n\n"
#ifdef __WIN32__
			"This version of WinDEU is the Win32 version. If you "
			"want 3D controls: you must have the 'CTL3D32.DLL' "
			"file if your path (preferably in the "
			"windows\\system' directory).\n\n"
			"If you are under Windows NT, the system directory is "
			"'%SystemRoot%\\<NT_Directory_name>\\system32',\n"
			"Example:\tc:\\winnt35\\system32\n\n"
#else
			"This version of WinDEU is the Win16 version. If you "
			"want 3D controls: you must have the 'CTL3DV2.DLL' "
			"file if your path (preferably in the "
			"'windows\\system' directory).\n\n"
			"If you are under Windows NT, the system directory is "
			"'%SystemRoot%\\<NT_Directory_name>\\system',\n"
			"Example:\tc:\\winnt35\\system\n\n"
#endif
			"If you don't want 3D controls, use the '3DControls' "
			"configuration option (see help file)";

	TRY
	{
		EnableCtl3d (TRUE);
	}
	CATCH ((...)
	{
//		MainWindow->MessageBox (ErrorMsg,
//								"WinDEU: 3D Controls library load error",
//								MB_OK | MB_ICONINFORMATION);
	})

	if ( ! Ctl3dEnabled() )
	{
		MainWindow->MessageBox (ErrorMsg,
								"WinDEU: 3D Controls library load error",
								MB_OK | MB_ICONINFORMATION);
	}
#endif
}


//////////////////////////////////////////////////////////
// WinDEUApp
// ---------
// Ask for confirmation before exiting application
BOOL WinDEUApp::CanClose ()
{
	BOOL result;

	// Default member function
	if ( TApplication::CanClose() == FALSE )
		return FALSE;

	TMainFrame *mainFrame = TYPESAFE_DOWNCAST(GetMainWindow(), TMainFrame);
	// Close level editor and return
	if ( mainFrame->inEditor )
	{
		mainFrame->StopEditLevel();
		return FALSE;
	}

	// Last chance message
	result = mainFrame->MessageBox (
				"Do you really want to quit WinDEU ?",
				"Quit WinDEU",
				MB_YESNO | MB_ICONQUESTION) == IDYES ? TRUE : FALSE ;

	// Close WinHelp
	if ( result && HelpOpened )
	{
		mainFrame->WinHelp(HelpFileName, HELP_QUIT, 0L);
		HelpOpened = FALSE;
	}

	return result;
}


/////////////////////////////////////////////////////////////////
// WinDEUApp
// ---------
//
BOOL WinDEUApp::ProcessAppMsg (MSG& msg)
{
#if 0

	BOOL result;

	if (msg.message == WM_COMMAND)
	{
		if (ContextHelp || (GetKeyState(VK_F1) < 0))
		{
			ContextHelp = FALSE;
			GetMainWindow()->WinHelp(HelpFileName, HELP_CONTEXT, msg.wParam);
			return TRUE;
		}
	}
	else
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			if (msg.wParam == VK_F1)
			{
				// If the Shift/F1 then set the help cursor and turn on the modal help state.
				if (::GetKeyState(VK_SHIFT) < 0)
				{
					ContextHelp = TRUE;
					HelpCursor = ::LoadCursor(GetMainWindow()->GetModule()->GetInstance(), MAKEINTRESOURCE(IDC_HELPCURSOR));
					::SetCursor(HelpCursor);
					return TRUE;        // Gobble up the message.
				}
				else
				{
					// If F1 w/o the Shift key then bring up help's main index.
					// GetMainWindow()->WinHelp(HelpFileName, HELP_INDEX, 0L);
					GetMainWindow()->WinHelp(HelpFileName, HelpContext, 0L);
					return TRUE;        // Gobble up the message.
				}
			}
			else
			{
				if (ContextHelp && (msg.wParam == VK_ESCAPE))
				{
					if (HelpCursor)
						::DestroyCursor(HelpCursor);
					ContextHelp = FALSE;
					HelpCursor = 0;
					GetMainWindow()->SetCursor(0, IDC_ARROW);
					return TRUE;    // Gobble up the message.
				}
			}
			break;

		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			if (ContextHelp)
			{
				::SetCursor(HelpCursor);
				return TRUE;        // Gobble up the message.
			}
			break;

		case WM_INITMENU:
			if (ContextHelp)
			{
				::SetCursor(HelpCursor);
				return TRUE;        // Gobble up the message.
			}
			break;

		case WM_ENTERIDLE:
			if (msg.wParam == MSGF_MENU)
			{
				if (GetKeyState(VK_F1) < 0)
				{
					ContextHelp = TRUE;
					GetMainWindow()->PostMessage(WM_KEYDOWN, VK_RETURN, 0L);
					return TRUE;       // Gobble up the message.
				}
			}
			break;

		default:
			;
		}  // End of switch
	}

#endif
	return TApplication::ProcessAppMsg(msg);
}


/////////////////////////////////////////////////////////////////
// WinDEUApp
// ---------
//
void WinDEUApp::AppHelp(UINT command, DWORD data)
{
	HelpOpened = GetMainWindow()->WinHelp(HelpFileName, command, data);
}


/////////////////////////////////////////////////////////////////
// WinDEUApp
// ---------
//
void WinDEUApp::CmHelpAbout ()
{
	//
	// Show the modal dialog.
	//
	SET_HELP_CONTEXT(About_WinDEU);
	WindeuAboutDlg(GetMainWindow()).Execute();
	RESTORE_HELP_CONTEXT();
}


/////////////////////////////////////////////////////////////////
// WinDEUApp
// ---------
//
void WinDEUApp::CmHelpContents ()
{
	AppHelp(HELP_CONTENTS, 0L);
}


/////////////////////////////////////////////////////////////////
// WinDEUApp
// ---------
//
void WinDEUApp::CmHelpSearch ()
{
	AppHelp(HELP_PARTIALKEY, (DWORD)"");
}




/////////////////////////////////////////////////////////////////
// WinDEUApp
// ---------
//
void WinDEUApp::CmHelpTutorial ()
{
	AppHelp (HELP_CONTEXT, Tutorial);
}


/////////////////////////////////////////////////////////////////
// WinDEUApp
// ---------
//
void WinDEUApp::CmHelpUsingHelp ()
{
	AppHelp(HELP_HELPONHELP, 0L);
}


//////////////////////////////////////////////////////////
// OwlMain
// -------
//
int OwlMain (int argc, char* argv[])
{
	WinDEUApp App(argc, (char **)argv);

	return App.Run();
}

