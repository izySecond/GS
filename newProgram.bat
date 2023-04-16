@echo off
REM Batch procedure to create a basic project for Keil uVision
REM Copies the necessary file for the TI-Board at HAW-hamburg
REM
REM           Franz Korf 
REM           HAW Hamburg 
REM           July 2020
REM           
REM           Based on the script newProject of Alfed Lohmann
REM

REM Restrict searchpath
set PATH=C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\system32\WBEM;C:\WINDOWS\system32\nls;

ECHO Anlegen eines neues Keil C / Assember Programms 
ECHO.

REM Abfragen des Programmnames via script 
set PROGRAMM=" "
for /f "tokens=*" %%i in ('cscript //nologo  programname.vbs') do set PROGRAMM=%%i

REM Fehlerbehandlung 
if %PROGRAMM% ==" " goto noname 
cd Programs
if EXIST %PROGRAMM% goto error1

REM Gueltiger Programmname gewaehlt
ECHO Programmname: %PROGRAMM%
md %PROGRAMM%
if ERRORLEVEL 1 goto error2
xcopy ..\ITS_Keil_prj_base\HelloWorld .\%PROGRAMM%\ /S >NUL
cd .\%PROGRAMM%
if ERRORLEVEL 1 goto error2
del /q README.md > NUL
echo . > README.md

rename HelloWorld.uvoptx %PROGRAMM%.uvoptx
rename HelloWorld.uvprojx %PROGRAMM%.uvprojx
del /q HelloWorld.uvguix.* 2> NUL

del /q /s ITSboard > NUL 2> NUL
del /q /s RTE > NUL 2> NUL
del /q /s DebugConfig > NUL 2> NUL
goto ende

REM Fehlerbehandlung
:noname
echo Es wurde keine Programmname angegeben.
pause
exit
:error1
cd ..
echo Datei/Verzeichnis %PROGRAMM% existiert schon.
pause
exit
:error2
cd ..
echo Verzeichnis %PROGRAMM% wurde nicht erzeugt.
pause 
exit

REM We did our job
:ende
cd ..
cd ..
ECHO ...done
pause
