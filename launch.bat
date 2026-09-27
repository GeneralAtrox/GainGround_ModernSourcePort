@echo off
setlocal
cd /d "%~dp0"
if errorlevel 1 goto :failed

rem Use the same MSYS2 UCRT64 toolchain as scripts/Build-Source.ps1.
if not defined GAIN_GROUND_TOOLCHAIN_ROOT set "GAIN_GROUND_TOOLCHAIN_ROOT=C:\msys64\ucrt64"
set "PATH=%GAIN_GROUND_TOOLCHAIN_ROOT%\bin;%PATH%"
set "GAIN_GROUND_LAUNCH_BUILD=%~dp0build\launcher"

echo Building Gain Ground...
cmake -S "%~dp0native" -B "%GAIN_GROUND_LAUNCH_BUILD%" -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_CXX_COMPILER=%GAIN_GROUND_TOOLCHAIN_ROOT%/bin/g++.exe" "-DZLIB_ROOT=%GAIN_GROUND_TOOLCHAIN_ROOT%" -DGAIN_GROUND_RESEARCH_TESTS=OFF
if errorlevel 1 goto :failed
cmake --build "%GAIN_GROUND_LAUNCH_BUILD%" --target gain_ground_runtime --parallel 8
if errorlevel 1 goto :failed

echo Starting Gain Ground...
start "" "%GAIN_GROUND_LAUNCH_BUILD%\gain_ground_runtime.exe"
if errorlevel 1 goto :failed
exit /b 0

:failed
echo.
echo Gain Ground could not be built or launched. See the error above.
pause
exit /b 1
