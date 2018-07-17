@echo off
pushd ..\
set args=%
setlocal

REM Check to see whether or not submodules need to be cloned
REM Right now, there are no thirdparty modules.  Here in case we add them.
:check_slikenet
REM if not exist "3rdParty\SLikeNet" goto clone
REM for /f %%i in ('dir /a /b 3rdParty\SLikeNet') do goto check_bullet
REM goto clone

:check_bullet
REM if not exist "3rdParty\bullet3" goto clone
REM for /f %%i in ('dir /a /b 3rdParty\bullet3') do goto skip
REM goto clone

goto skip

:clone
echo Cloning submodules...
git submodule update --init --recursive
goto make

:skip
REM echo Submodules already cloned

:make
if not exist "build\Win64" echo Creating x64 build directory...

if not exist "build" mkdir "build"
if not exist "build\Win64" mkdir "build\Win64"

echo Running cmake for 64 bit architecture...
pushd build
	pushd Win64
		cmake -G "Visual Studio 15 2017 Win64" ..\..\
	popd
popd

popd
echo Done!