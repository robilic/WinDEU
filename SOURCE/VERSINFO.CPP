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

	FILE:         versinfo.cpp

	OVERVIEW
	========
	Source file for implementation of TVersionInfo (TDialog).
*/

#include "common.h"
#pragma hdrstop

#include "versinfo.h"

#ifndef __OWL_STATIC_H
	#include <owl\static.h>
#endif

#if !defined(__FLAT__)
	#include <ver.h>
#endif

#ifndef __windeapp_h
	#include "windeapp.h"
#endif

#ifndef __buildtim_h
	#include "buildtim.h"
#endif

#define VERSION_STRING	"5.24"

// Use version info resource ?
// #define USE_VERSION_INFO
//
// I Will use the ProjectVersion when I will be able to understand the
// Win32 bug. It works under Win16, but not under Win32.
// This bug is in the ProgectRCVersion constructor.


#ifdef USE_VERSION_INFO
// Reading the VERSIONINFO resource.
class ProjectRCVersion
{
public:
	ProjectRCVersion (TModule *module);
	virtual ~ProjectRCVersion ();

	BOOL GetProductName (LPSTR &prodName);
	BOOL GetProductVersion (LPSTR &prodVersion);
	BOOL GetCopyright (LPSTR &copyright);
	BOOL GetDebug (LPSTR &debug);

protected:
	LPBYTE      TransBlock;
	void FAR    *FVData;

private:
	// Don't allow this object to be copied.
	ProjectRCVersion (const ProjectRCVersion &);
	ProjectRCVersion & operator =(const ProjectRCVersion &);
};
#endif

#ifdef USE_VERSION_INFO
/////////////////////////////////////////////////////////////////
// ProjectVersion
// --------------
//
ProjectRCVersion::ProjectRCVersion (TModule *module)
{
	static char    appFName[255];
	static char    subBlockName[255];
	DWORD   fvHandle;
	UINT    vSize;

	FVData = 0;

	module->GetModuleFileName(appFName, sizeof(appFName));
	DWORD dwSize = ::GetFileVersionInfoSize(appFName, &fvHandle);
	if (dwSize)
	{
		FVData  = (void FAR *)new char[(UINT)dwSize];
		if ( ::GetFileVersionInfo(appFName, fvHandle, dwSize, FVData) )
		{
			// Copy string to buffer so if the -dc compiler switch (Put constant
			// strings in code segments) is on VerQueryValue will work under
			// Win16.  This works around a problem in Microsoft's ver.dll
			// which writes to the string pointed to by subBlockName.
			lstrcpy(subBlockName, "\\VarFileInfo\\Translation");
			if (!VerQueryValue(FVData, subBlockName, (void FAR* FAR*)&TransBlock, &vSize))
			{
#ifdef __WIN32__
				DWORD LastError = ::GetLastError();
				char buf[256];

				::wsprintf (buf, "Last error code : %d, vsize = %d",
								 LastError, vSize);
				::MessageBox (0, buf, "DEBUG", MB_OK);
#endif
#if 0
				delete FVData;
				FVData = 0;
#endif
			}
#if 0
			else
#endif
				// Swap the words so wsprintf will print the lang-charset in the correct format.
				*(DWORD *)TransBlock = MAKELONG(HIWORD(*(DWORD *)TransBlock), LOWORD(*(DWORD *)TransBlock));
		}
	}
}


/////////////////////////////////////////////////////////////////
// ProjectVersion
// --------------
//
ProjectRCVersion::~ProjectRCVersion ()
{
	if (FVData)
		delete FVData;
}


/////////////////////////////////////////////////////////////////
// ProjectVersion
// --------------
//
BOOL ProjectRCVersion::GetProductName (LPSTR &prodName)
{
	UINT    vSize;
	char    subBlockName[255];

	wsprintf(subBlockName, "\\StringFileInfo\\%08lx\\%s", *(DWORD *)TransBlock, (LPSTR)"ProductName");
	return FVData ? VerQueryValue(FVData, subBlockName, (void FAR* FAR*)&prodName, &vSize) : FALSE;
}


/////////////////////////////////////////////////////////////////
// ProjectVersion
// --------------
//
BOOL ProjectRCVersion::GetProductVersion (LPSTR &prodVersion)
{
	UINT    vSize;
	char    subBlockName[255];

	// wsprintf(subBlockName, "\\StringFileInfo\\%08lx\\%s", *(DWORD *)TransBlock, (LPSTR)"ProductVersion");
	wsprintf(subBlockName, "\\StringFileInfo\\04E40409\\%s", (LPSTR)"ProductVersion");
	return FVData ? VerQueryValue(FVData, subBlockName, (void FAR* FAR*)&prodVersion, &vSize) : FALSE;
}


/////////////////////////////////////////////////////////////////
// ProjectVersion
// --------------
//
BOOL ProjectRCVersion::GetCopyright (LPSTR &copyright)
{
	UINT    vSize;
	char    subBlockName[255];

	// wsprintf(subBlockName, "\\StringFileInfo\\%08lx\\%s", *(DWORD *)TransBlock, (LPSTR)"LegalCopyright");
	wsprintf(subBlockName, "\\StringFileInfo\\04e40409\\%s", (LPSTR)"LegalCopyright");
	return FVData ? VerQueryValue(FVData, subBlockName, (void FAR* FAR*)&copyright, &vSize) : FALSE;
}


/////////////////////////////////////////////////////////////////
// ProjectVersion
// --------------
//
BOOL ProjectRCVersion::GetDebug (LPSTR &debug)
{
	UINT    vSize;
	char    subBlockName[255];

	wsprintf(subBlockName, "\\StringFileInfo\\%08lx\\%s", *(DWORD *)TransBlock, (LPSTR)"SpecialBuild");
	return FVData ? VerQueryValue(FVData, subBlockName, (void FAR* FAR*)&debug, &vSize) : FALSE;
}
#endif


//{{TVersionInfo Implementation}}


//////////////////////////////////////////////////////////
// TVersionInfo
// ------------
//
TVersionInfo::TVersionInfo (TWindow* parent, TResId resId, TModule* module):
	TDialog(parent, resId, module)
{
}


//////////////////////////////////////////////////////////
// TVersionInfo
// ------------
//
TVersionInfo::~TVersionInfo ()
{
	Destroy();
}


//////////////////////////////////////////////////////////
// TVersionInfo
// ------------
//
void TVersionInfo::SetupWindow ()
{
#ifdef USE_VERSION_INFO
	LPSTR prodName = 0, prodVersion = 0, copyright = 0, debug = 0;
#endif
	char buffer[255];
	// Get the static text for the value based on VERSIONINFO.
	TStatic *versionCtrl = new TStatic(this, IDC_VERSION, 255);
	TStatic *winVersionCtrl = new TStatic(this, IDC_WINDEU_VERSION, 255);
	TStatic *buildTimeCtrl = new TStatic(this, IDC_BUILD_TIME, 255);
#ifdef USE_VERSION_INFO
	TStatic *copyrightCtrl = new TStatic(this, IDC_COPYRIGHT, 255);
	TStatic *debugCtrl = new TStatic(this, IDC_DEBUG, 255);
#endif

	TDialog::SetupWindow();
	::CenterWindow (this);

#ifdef USE_VERSION_INFO
	// Process the VERSIONINFO.
	ProjectRCVersion applVersion(GetModule());

	// Get the product name and product version strings.
	if (applVersion.GetProductName(prodName) && applVersion.GetProductVersion(prodVersion))
	{
		// IDC_VERSION is the product name and version number, the initial value
		// of IDC_VERSION is the word Version (in whatever language) product name
		// VERSION product version.
		char    versionName[128];

		buffer[0] = '\0';
		versionName[0] = '\0';

		versionCtrl->GetText(versionName, sizeof(versionName));
		wsprintf(buffer, "%s %s %s", prodName, versionName, prodVersion);

		versionCtrl->SetText(buffer);
	}

	//Get the legal copyright string.
	if (applVersion.GetCopyright(copyright))
		copyrightCtrl->SetText(copyright);

	// Only get the SpecialBuild text if the VERSIONINFO resource is there.
	if (applVersion.GetDebug(debug))
		debugCtrl->SetText(debug);
#else
	wsprintf (buffer, "Version %s", VERSION_STRING);
	versionCtrl->SetText (buffer);

	// Display build time and date from builtim.c
	wsprintf (buffer, "Build time: %s %s",
			  BuildDate, BuildTime);
	buildTimeCtrl->SetText (buffer);
#endif

	// Display 16-bit or 32-bit under the WinDEU icon 
#ifdef __WIN32__
	wsprintf (buffer, "%s", "32-bit");
#else
	wsprintf (buffer, "%s", "16-bit");
#endif
	winVersionCtrl->SetText(buffer);
}

