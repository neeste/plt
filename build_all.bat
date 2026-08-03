@echo off
pushd "%~dp0"

REM Locate Visual Studio vcvarsall.bat
set "VCVARS="
set "TOOLSET="
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
    set "TOOLSET=v143"
) else if exist "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARS=C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat"
    set "TOOLSET=v145"
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
    set "TOOLSET=v142"
)

echo ===================================================
echo   PLT Unified Build Script
echo ===================================================
echo.

if "%VCVARS%"=="" goto vcvars_missing

echo [1/4] Initializing x86 Build Environment...
call "%VCVARS%" x86 > nul

echo.
echo [2/4] Compiling Help File (wplt.chm)...
if not exist "C:\Program Files (x86)\HTML Help Workshop\hhc.exe" goto skip_chm
cd VS18
"C:\Program Files (x86)\HTML Help Workshop\hhc.exe" wplt.hhp
cd ..
goto build_sln

:skip_chm
echo [WARNING] HTML Help Workshop not found. Skipping .chm compilation.

:build_sln
echo.
echo [3/4] Building PLT Solution (Release)...
msbuild VS18\plt.sln /p:Configuration=Release /p:Platform=Win32 /p:PlatformToolset=%TOOLSET% /v:m
if errorlevel 1 goto msbuild_failed

echo.
echo [4/4] Compiling Inno Setup Installer...
set "ISCC_PATH="
if exist "C:\Program Files\Inno Setup 6\ISCC.exe" set "ISCC_PATH=C:\Program Files\Inno Setup 6\ISCC.exe"
if exist "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" set "ISCC_PATH=C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
if exist "C:\Program Files\Inno Setup 5\ISCC.exe" set "ISCC_PATH=C:\Program Files\Inno Setup 5\ISCC.exe"
if exist "C:\Program Files (x86)\Inno Setup 5\ISCC.exe" set "ISCC_PATH=C:\Program Files (x86)\Inno Setup 5\ISCC.exe"

if "%ISCC_PATH%"=="" goto iscc_missing

"%ISCC_PATH%" /O"Output" /F"PltSetup" plt.iss
if errorlevel 1 goto iscc_failed

echo.
echo ===================================================
echo   [SUCCESS] PLT Installer Built Successfully!
echo   Installer saved to Output\PltSetup.exe
echo ===================================================
goto end

:vcvars_missing
echo [ERROR] Visual Studio vcvarsall.bat not found! Please check your VS installation.
goto end_error

:msbuild_failed
echo [ERROR] MSBuild failed!
goto end_error

:iscc_missing
echo [ERROR] Inno Setup compiler (ISCC.exe) not found.
goto end_error

:iscc_failed
echo [ERROR] Inno Setup compilation failed!
goto end_error

:end_error
pause
popd
exit /b 1

:end
pause
popd
exit /b 0
