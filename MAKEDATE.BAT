@if not exist include\buildtim.h goto make_h
@goto make_c
@
:make_h
@REM Generate the BUILDTIM.H header to declare BuildTime vars.
@echo #ifndef __buildtim_h >include\buildtim.h
@echo #define __buildtim_h >>include\buildtim.h
@echo. >>include\buildtim.h
@echo extern char BuildDate[]; >>include\buildtim.h
@echo extern char BuildTime[]; >>include\buildtim.h
@echo. >>include\buildtim.h
@echo #endif /* __buildtim_h */ >>include\buildtim.h
@
:make_c
@REM Generate the BUILDTIM.C source to define BuildTime vars.
@echo #include "buildtim.h" >source\buildtim.c
@echo. >>source\buildtim.c
@echo char BuildDate[] = __DATE__; >>source\buildtim.c
@echo char BuildTime[] = __TIME__; >>source\buildtim.c
