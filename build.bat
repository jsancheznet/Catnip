@echo off

pushd build

set SDL_INCLUDE_DIR="..\libraries\SDL2-2.26.3\include"
set SDL_LIB_DIR="..\libraries\SDL2-2.26.3\lib\x64"

set IncludeDirectories=-I%SDL_INCLUDE_DIR%
set LibDirectories=-LIBPATH:%SDL_LIB_DIR%

set CompilerFlags= /nologo /W4 /Ot /FS %IncludeDirectories% /Zi /EHsc /MD /D "_WINDOWS"
set LinkerFlags=/nologo /DEBUG %LibDirectories%

cl /std:c17 /Tc ..\main.c %CompilerFlags% /link %LinkerFlags% -SUBSYSTEM:WINDOWS SDL2.lib SDL2main.lib shell32.lib

popd
