@echo off
setlocal EnableExtensions

rem Freestanding Zydis static library for llvm-kmd (no CRT / no /GS / no SEH).
rem Requires VS 2022 x64 developer environment (cl.exe, lib.exe on PATH).

set ROOT=%~dp0zydis-4.1.0
set OUT=%~dp0..\Zyan
set OBJ=%ROOT%\build-kernel-obj

if not exist "%ROOT%\src\Decoder.c" (
  echo [!] Zydis sources missing at %ROOT%
  exit /b 1
)

where cl.exe >nul 2>&1
if errorlevel 1 (
  echo [!] cl.exe not found. Run from a VS x64 Native Tools / VsDevCmd prompt.
  exit /b 1
)

if exist "%OBJ%" rmdir /s /q "%OBJ%"
mkdir "%OBJ%"
mkdir "%OBJ%\zycore"
mkdir "%OBJ%\zycore\API"
mkdir "%OBJ%\zydis"

set INCLUDES=/I"%ROOT%\include" /I"%ROOT%\dependencies\zycore\include" /I"%ROOT%\src"
set DEFS=/DZYAN_NO_LIBC /DZYDIS_STATIC_BUILD /DZYCORE_STATIC_BUILD /DZYDIS_DISABLE_FORMATTER /DZYDIS_DISABLE_ENCODER /DWIN32 /DNDEBUG
rem /GS- no buffer security cookie/SEH handler, /Zl no defaultlib directives,
rem /Gy package functions for /OPT:REF, /Oi intrinsics, no exceptions
set CFLAGS=/nologo /c /O2 /GS- /Gy /Oi /Zl /W0 /TC %INCLUDES% %DEFS%

echo [*] Compiling Zycore...
for %%F in (
  Allocator.c ArgParse.c Bitset.c Format.c List.c String.c Vector.c Zycore.c
) do (
  cl %CFLAGS% "%ROOT%\dependencies\zycore\src\%%F" /Fo"%OBJ%\zycore\%%~nF.obj"
  if errorlevel 1 exit /b 1
)
for %%F in (Memory.c Process.c Synchronization.c Terminal.c Thread.c) do (
  cl %CFLAGS% "%ROOT%\dependencies\zycore\src\API\%%F" /Fo"%OBJ%\zycore\API\%%~nF.obj"
  if errorlevel 1 exit /b 1
)

echo [*] Compiling Zydis decoder...
for %%F in (
  Decoder.c DecoderData.c MetaInfo.c Mnemonic.c Register.c Segment.c SharedData.c String.c Utils.c Zydis.c
) do (
  cl %CFLAGS% "%ROOT%\src\%%F" /Fo"%OBJ%\zydis\%%~nF.obj"
  if errorlevel 1 exit /b 1
)

echo [*] Creating %OUT%\Zydis.lib ...
lib /nologo /OUT:"%OUT%\Zydis.lib" "%OBJ%\zycore\*.obj" "%OBJ%\zycore\API\*.obj" "%OBJ%\zydis\*.obj"
if errorlevel 1 exit /b 1

echo [*] Syncing public headers into Zyan\ ...
if exist "%OUT%\Zydis" rmdir /s /q "%OUT%\Zydis"
if exist "%OUT%\Zycore" rmdir /s /q "%OUT%\Zycore"
xcopy /E /I /Y /Q "%ROOT%\include\Zydis" "%OUT%\Zydis" >nul
xcopy /E /I /Y /Q "%ROOT%\dependencies\zycore\include\Zycore" "%OUT%\Zycore" >nul

echo [+] Built freestanding Zydis.lib
exit /b 0
