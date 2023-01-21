@echo off
@Echo Backing up WINDEU program components
@c:\utils\pkware\pkzip -u c:\backup\w16deu52.zip windeu.exe windeu.ini windeu.hlp readme.txt whatsnew.txt
@c:\utils\pkware\pkzip -uPr c:\backup\w16deu52.zip tutor\*.wad license ctl3dll\ctl3dv2.dll
@REM
@Echo Backing up WINDEU32 program components
@c:\utils\pkware\pkzip -u c:\backup\w32deu52 windeu32.exe windeu.ini windeu.hlp readme.txt whatsnew.txt
@c:\utils\pkware\pkzip -uPr c:\backup\w32deu52.zip tutor\*.wad license ctl3dll\ctl3d32.dll
@REM
@Echo Backing up WINDEU sources and IDE components
@c:\utils\pkware\pkzip -upr c:\backup\wscdeu52 *.* -x*.csm -x*.exe -x*.obj -x*.rws -x*.~* -x*.res -x*.bak -x*.hlp
