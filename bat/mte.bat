@ECHO OFF

IF "%~1"=="" (
	GOTO HELP
)

SET EXDIR=%~dp0

SET FNT=%1
SET ARGV=
SHIFT
:ARGVLOOP
IF "%1"=="" (
	GOTO ARGVENDLOOP
)
SET ARGV=%ARGV% %1
SHIFT
GOTO ARGVLOOP
:ARGVENDLOOP

IF "%FNT%"=="combine" (
	%EXDIR%\fnt\cbn.exe %ARGV%
	GOTO ENDEX
)

IF "%FNT%"=="separate" (
	%EXDIR%\fnt\spr.exe %ARGV%
	GOTO ENDEX
)

IF "%FNT%"=="split" (
	%EXDIR%\fnt\spl.exe %ARGV%
	GOTO ENDEX
)

IF "%FNT%"=="merge" (
	%EXDIR%\fnt\mge.exe %ARGV%
	GOTO ENDEX
)

IF "%FNT%"=="help" (
	:HELP
	ECHO combine   ^<OUTPUT FILE^> [FILE ^| DIRECTORY] .. [FILE ^| DIRECTORY]
	ECHO           Combine files and folders into a single file
	ECHO separate  ^<INPUT FILE^> [OUTPUT DIRECTORY]
	ECHO           Separates a merged file into the original files and folders.
	ECHO split     ^<INPUT FILE^> [OUTPUT DIRECTORY]
	ECHO           Split a file into multiple sub files
	ECHO merge     ^<INPUT FILE 000.bin^> [OUTPUT DIRECTORY]
	ECHO           Merge split files.
	
	GOTO ENDEX
)

:EXERROR

ECHO This command is wrong.
exit /b 1

:ENDEX
exit /b 0