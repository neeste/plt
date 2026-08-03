@echo off
setlocal
echo ==============================================
echo       PLT Windows Installer Build Script
echo ==============================================
echo.

REM Optional: Update version numbers if Perl is available
where perl >nul 2>&1
if %errorlevel% equ 0 (
    echo [1/4] Updating version information using Perl...
    perl version.pl
) else (
    echo [1/4] Perl not found in PATH. Skipping version.pl update.
)
echo.

REM Step 1: Compile Help File
echo [2/4] Compiling wplt.chm help file...
if exist "C:\Program Files (x86)\HTML Help Workshop\hhc.exe" (
    cd VS18
    "C:\Program Files (x86)\HTML Help Workshop\hhc.exe" wplt.hhp
    cd ..
) else (
    echo       HTML Help Workshop not found. Skipping help file compilation.
)
echo.

REM Step 2: Compile the Visual Studio Solution
echo [3/4] Building VS18\plt.sln with MSBuild...
where msbuild >nul 2>&1
if %errorlevel% equ 0 (
    cd VS18
    msbuild plt.sln /p:Configuration=Release /restore
    if errorlevel 1 (
        echo       Error: MSBuild failed.
        cd ..
        exit /b 1
    )
    cd ..
) else (
    echo       Error: MSBuild not found in PATH!
    echo       Please run this script from a "Developer Command Prompt for VS" or "x86 Native Tools Command Prompt".
    exit /b 1
)
echo.

REM Step 3: Build the Inno Setup Installer
echo [4/4] Compiling Inno Setup Installer...
if exist "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" (
    "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" /O"Output" /F"PltSetup" plt.iss
    if errorlevel 1 (
        echo       Error: Inno Setup Compilation failed.
        exit /b 1
    )
) else (
    echo       Error: Inno Setup 6 (ISCC.exe) not found at default location.
    exit /b 1
)
echo.

echo ==============================================
echo BUILD SUCCESSFUL! 
echo Installer generated at: Output\PltSetup.exe
echo ==============================================
pause
