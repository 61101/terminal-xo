d=$(dirname "$0")
:; clear && g++ "$d/xo.cpp" -o /tmp/xo && /tmp/xo
:; exit

@echo off
g++ "%~dp0xo.cpp" -o "%TEMP%\xo.exe" && "%TEMP%\xo.exe"
