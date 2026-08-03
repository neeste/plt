@echo off
pushd "%~dp0"

REM Locate Visual Studio vcvarsall.bat
set "VCVARS="
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARS=C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat"
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
)

echo ===================================================
echo   PLT Unified Build Script
echo ===================================================
echo.

if "%VCVARS%"=="" (
    echo [ERROR] Visual Studio vcvarsall.bat not found! Please check your VS installation.
    pause
    exit /b 1
)

echo [1/4] Initializing x86 Build Environment...
call "%VCVARS%" x86 > nul

echo.
echo [2/4] Compiling Help File (wplt.chm)...
if exist "C:\Program Files (x86)\HTML Help Workshop\hhc.exe" (
    cd VS18
    "C:\Program Files (x86)\HTML Help Workshop\hhc.exe" wplt.hhp
    cd ..
) else (
    echo [WARNING] HTML Help Workshop not found. Skipping .chm compilation.
)

echo.
echo [3/4] Building PLT Solution (Release)...
msbuild VS18\plt.sln /p:Configuration=Release /v:m
if errorlevel 1 (
    echo [ERROR] MSBuild failed!
    pause
    exit /b 1
)

echo.
echo [4/4] Compiling Inno Setup Installer...
set "ISCC_PATH="
if exist "C:\Program Files\Inno Setup 6\ISCC.exe" set "ISCC_PATH=C:\Program Files\Inno Setup 6\ISCC.exe"
if exist "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" set "ISCC_PATH=C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
if exist "C:\Program Files\Inno Setup 5\ISCC.exe" set "ISCC_PATH=C:\Program Files\Inno Setup 5\ISCC.exe"
if exist "C:\Program Files (x86)\Inno Setup 5\ISCC.exe" set "ISCC_PATH=C:\Program Files (x86)\Inno Setup 5\ISCC.exe"

if "%ISCC_PATH%"=="" (
    echo [ERROR] Inno Setup compiler (ISCC.exe) not found.
    pause
    exit /b 1
)

"%ISCC_PATH%" /O"Output" /F"PltSetup" plt.iss
if errorlevel 1 (
    echo [ERROR] Inno Setup compilation failed!
    pause
    exit /b 1
)

echo.
echo ===================================================
echo   [SUCCESS] PLT Installer Built Successfully!
echo   Installer saved to Output\PltSetup.exe
echo ===================================================
pause
popd
