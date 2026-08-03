@echo off
setlocal
echo ==============================================
echo       PLT Windows Installer Build Script
echo ==============================================
echo.

REM Optional: Update version numbers if Perl is available
where perl >nul 2>&1
if %errorlevel% equ 0 (
    echo [*] Updating version information using Perl...
    perl version.pl
) else (
    echo [*] Perl not found in PATH. Skipping version.pl update.
)
echo.

echo [*] Compiling Inno Setup Installer...
if exist "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" (
    "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" /O"Output" /F"PltSetup" plt.iss
    if errorlevel 1 (
        echo.
        echo       Error: Inno Setup Compilation failed.
        pause
        exit /b 1
    )
) else (
    echo.
    echo       Error: Inno Setup 6 (ISCC.exe) not found at default location.
    pause
    exit /b 1
)

echo.
echo ==============================================
echo BUILD SUCCESSFUL! 
echo Installer generated at: Output\PltSetup.exe
echo ==============================================
pause
