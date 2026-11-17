@echo off
rem Compiles a 32-bit tetris.exe on Windows. UNTESTED - see notes below.
rem Needs:
rem  - a 32-bit MinGW gcc on the PATH (for example WinLibs, the i686 version)
rem  - PDCurses, built inside its wincon folder with:  mingw32-make WIDE=Y UTF8=Y
rem Change PDCURSES to the folder where you unpacked PDCurses.

set PDCURSES=C:\PDCurses

gcc -std=gnu99 -Wall -Wextra -O2 -DPDC_WIDE -DPDC_FORCE_UTF8 -I"%PDCURSES%" main.c -o tetris.exe -static "%PDCURSES%\wincon\pdcurses.a" -lgdi32 -lcomdlg32 -lwinmm

rem keep the window open if compiling failed, so the errors can be read
if errorlevel 1 pause
