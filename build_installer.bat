@echo off
setlocal
cd /d "%~dp0"
echo ==============================================
echo       PLT Windows Installer Build Script
echo ==============================================
echo.

where perl >nul 2>&1
if errorlevel 1 goto skip_perl

echo [*] Updating version information using Perl...
perl version.pl
goto build_installer

:skip_perl
echo [*] Perl not found in PATH. Skipping version.pl update.

:build_installer
echo.
echo [*] Compiling Inno Setup Installer...
if not exist "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" goto iscc_not_found

"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" /O"Output" /F"PltSetup" plt.iss
if errorlevel 1 goto iscc_failed

echo.
echo ==============================================
echo BUILD SUCCESSFUL! 
echo Installer generated at: Output\PltSetup.exe
echo ==============================================
pause
exit /b 0

:iscc_not_found
echo.
echo Error: Inno Setup 6 (ISCC.exe) not found at default location.
pause
exit /b 1

:iscc_failed
echo.
echo Error: Inno Setup Compilation failed.
pause
exit /b 1
