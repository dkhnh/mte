@ECHO OFF
REM The directory containing this file.
SET EXDIR=%~dp0

REM Dynamic workspace dicrectory.
SET DWS=dyws

REM Text editor
SET TEDT=C:\Program Files\Notepad++\notepad++.exe

REM Create dynamic workspace dicrectory IF it doesn't exists.
IF NOT EXIST %EXDIR%%DWS%\NUL (
	mkdir %EXDIR%%DWS%
)

REM Check arguments.
IF "%1" == "" (
	GOTO HELP
)

REM For command: new
IF "%1" == "new" (
	IF "%2" == "" (
		GOTO CWRER
	)
	
	GOTO ARGVS
)

REM For command: del
IF "%1" == "del" (
	IF "%2" == "" (
		GOTO CWRER
	)
	
	REM For command: pws rm .
	REM Delete all cpp files.
	IF "%2" == "." (
		IF "%3" == "" (
			del %EXDIR%%DWS%\*.cpp
			GOTO ENDEX
		)
	)
	
	GOTO ARGVS
)

REM For command: op
IF "%1" == "op" (
	IF "%2" == "" (
		GOTO CWRER
	)
	
	REM For command: pws op .
	REM Delete all cpp files.
	IF "%2" == "." (
		IF "%3" == "" (
			"%TEDT%" %EXDIR%%DWS%\*.cpp
			GOTO ENDEX
		)
	)
	
	GOTO ARGVS
)

REM For command: run
IF "%1" == "run" (
	IF "%2" == "" (
		GOTO CWRER
	)
	
	GOTO ARGVS
)

REM For commands with a specified number of arguments
REM For command: pws ls
REM Show list cpp file.
IF "%1" == "ls" (
	IF NOT "%2" == "" (
		GOTO CWRER
	)
	
	FOR %%i IN (%EXDIR%%DWS%\*.cpp) DO (
		ECHO %%~ni
	)
	
	GOTO ENDEX
)

REM For command: pws ws
REM Open dynamic workspace dicrectory with 'explorer'.
IF "%1" == "ws" (
	IF NOT "%2" == "" (
		GOTO CWRER
	)
	
	explorer %EXDIR%%DWS%
	
	GOTO ENDEX
)

REM For command: pws help
REM Show all helps.
IF "%1" == "help" (
	IF NOT "%2" == "" (
		GOTO CWRER
	)
	:HELP
	ECHO new   ^<FILE NAME^> .. [FILE NAME]
	ECHO       Create new cpp files.
	ECHO del   ^<FILE NAME^> .. [FILE NAME]
	ECHO       Delete cpp files.
	ECHO op    ^<FILE NAME^> .. [FILE NAME]
	ECHO       Op cpp files with notepad++.
	ECHO run   ^<FILE NAME^> [ARGUMENTS]
	ECHO       Compile and run a cpp file.
	ECHO ls    Show cpp file list.
	ECHO ws    Open dynamic workspace dicrectory with nemo.
	ECHO help  Show all helps.
	ECHO.
	ECHO Shortcut commands:
	ECHO del   .
	ECHO       Delete all cpp files in dynamic workspace dicrectory.
	ECHO op    .
	ECHO       Open all cpp files in dynamic workspace dicrectory.
	ECHO ^<FILE NAME^>
	ECHO       Create a new cpp file and opening it with notepad++ if it doesn't not exists.
	ECHO ^<FILE NAME^> [ARGUMENTS]
	ECHO       Run and compile a cpp file if it exist.
	
	GOTO ENDEX
)

REM For shortcut command: dws <FILE NAME>
REM Create a new cpp file and opening it with notepad++ if it doesn't not exists.
if NOT EXIST %EXDIR%%DWS%\%1.cpp (
	if "%2" == "" (
		ECHO #include ^<iostream^>>> %EXDIR%%DWS%\%1.cpp
		ECHO.>> %EXDIR%%DWS%\%1.cpp
		ECHO #define show^(x^) std::cout ^<^< x ^<^< std::endl>> %EXDIR%%DWS%\%1.cpp
		ECHO.>> %EXDIR%%DWS%\%1.cpp
		ECHO int main^(int args, char **argv^) ^{>> %EXDIR%%DWS%\%1.cpp
		ECHO 	return 0;>> %EXDIR%%DWS%\%1.cpp
		ECHO ^}>> %EXDIR%%DWS%\%1.cpp
		
		"%TEDT%" %EXDIR%%DWS%\%1.cpp
		
		GOTO ENDEX
	) ELSE (
		GOTO CWRER
	)
)

REM For commands with an unknown number of arguments
:ARGVS

SET FARGV=%1
SET ARGV=
:ARGVLOOP
SHIFT
IF "%1"=="" (
	GOTO EARGVLOOP
)
SET ARGV=%ARGV% %1
GOTO ARGVLOOP
:EARGVLOOP

REM For command: pws new <FILE NAME> .. [FILE NAME]
REM Create new cpp files.
IF "%FARGV%" == "new" (
	FOR %%i IN (%ARGV%) DO (
		IF EXIST %EXDIR%%DWS%\%%i.cpp (
			ECHO %%i: File already exists.
		) ELSE (
			ECHO #include ^<iostream^>>> %EXDIR%%DWS%\%%i.cpp
			ECHO.>> %EXDIR%%DWS%\%%i.cpp
			ECHO #define show^(x^) std::cout ^<^< x ^<^< std::endl>> %EXDIR%%DWS%\%%i.cpp
			ECHO.>> %EXDIR%%DWS%\%%i.cpp
			ECHO int main^(int args, char **argv^) ^{>> %EXDIR%%DWS%\%%i.cpp
			ECHO 	return 0;>> %EXDIR%%DWS%\%%i.cpp
			ECHO ^}>> %EXDIR%%DWS%\%%i.cpp
		)
	)
	GOTO ENDEX
)

REM For command: dws del <FILE NAME> .. [FILE NAME]
REM Delete cpp files.
IF "%FARGV%" == "del" (
	FOR %%i IN (%ARGV%) DO (
		IF NOT EXIST %EXDIR%%DWS%\%%i.cpp (
			ECHO %%i.cpp: No such file.
		) ELSE (
			IF EXIST %EXDIR%%DWS%\%%i.cpp\NUL (
				ECHO %%i.cpp: No such file.
			) ELSE (
				del %EXDIR%%DWS%\%%i.cpp
			)
		)
	)
	GOTO ENDEX
)

REM For command: dws op <FILE NAME> .. [FILE NAME]
REM Open cpp files.
IF "%FARGV%" == "op" (
	FOR %%i IN (%ARGV%) DO (
		IF NOT EXIST %EXDIR%%DWS%\%%i.cpp (
			ECHO %%i.cpp: No such file.
		) ELSE (
			IF EXIST %EXDIR%%DWS%\%%i.cpp\NUL (
				ECHO %%i.cpp: No such file.
			) ELSE (
				"%TEDT%" %EXDIR%%DWS%\%%i.cpp
			)
		)
	)
	GOTO ENDEX
)

REM For command: pws run <FILE NAME> [ARGUMENTS]
REM Compile and run a cpp file.
IF "%FARGV%" == "run" (
	IF NOT EXIST %EXDIR%%DWS%\%2.cpp (
		ECHO %2.cpp: No such file.
		exit /b 3
	) ELSE (
		IF EXIST %EXDIR%%DWS%\%2.cpp\NUL (
			ECHO %2.cpp: No such file.
			exit /b 3
		) ELSE (
			g++ -o %EXDIR%%DWS%\%2.exe %EXDIR%%DWS%\%2.cpp
			%EXDIR%%DWS%\%2.exe %ARGV%
			del %EXDIR%%DWS%\%2.exe
		)
	)
	
	GOTO ENDEX
)

REM For shortcut command: dws <FILE NAME> [ARGUMENTS]
REM Run and compile a cpp file if it exist.
if EXIST %EXDIR%%DWS%\%FARGV%.cpp (
	g++ -o %EXDIR%%DWS%\%FARGV%.exe %EXDIR%%DWS%\%FARGV%.cpp
	%EXDIR%%DWS%\%FARGV%.exe %ARGV%
	del %EXDIR%%DWS%\%FARGV%.exe
	
	GOTO ENDEX
)

:CWRER
ECHO This command is wrong.
exit /b 1

:ENDEX
exit /b 0