@ECHO OFF

REM The directory containing this file.
SET EXDIR=%~dp0

REM The file does not need compilation.
SET EXCEPT=ctr.cpp

REM Source directory.
SET SRCE=fnt_srce

REM Executable driectory.
SET EX=fnt

REM Check for existence of source directory.
IF NOT EXIST %EXDIR%%SRCE%\NUL (
	ECHO %EXDIR%%SRCE%\: No such directory.
	exit /b 3
)

REM Create executable direcotry if it doesn't exists.
IF NOT EXIST %EXDIR%%EX%\NUL (
	mkdir %EXDIR%%EX%
)

REM For command: cpl
REM Compile all files in source directory.
IF "%1" == "" (
	FOR %%i IN (%EXDIR%%SRCE%\*) DO (
		IF NOT "%%~nxi" == "%EXCEPT%" (
			ECHO %%~nxi
			g++ -o %EXDIR%%EX%\%%~ni %%i
		)
	)
	
	GOTO ENDEX
)

REM For command: cpl <FILE NAME> .. [FILE NAME]
REM Compile the required files in the source directory.
FOR %%i IN (%*) DO (
	IF EXIST %EXDIR%%SRCE%\%%i.cpp (
		IF NOT EXIST %EXDIR%%SRCE%\%%i.cpp\NUL (
			ECHO %%~i.cpp
			g++ -o %EXDIR%%EX%\%%i %EXDIR%%SRCE%\%%i.cpp
		) ELSE (
			ECHO %%i.cpp: No such file.
		)
	) ELSE (
		ECHO %%i.cpp: No such file.
	)
)

:ENDEX
exit /b 0