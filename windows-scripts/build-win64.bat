@echo off
set args=%
setlocal

if not exist "..\build\Win64" call generate-project-win64.bat

pushd ..\

pushd build
	pushd Win64
		cmake --build . --target tinygame
	popd
popd

:end
popd