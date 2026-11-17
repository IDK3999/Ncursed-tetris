# Cross-compiles a 32-bit Windows tetris.exe on Linux.
# Needs: mingw-w64-gcc and mingw-w64-pdcurses (AUR).
# -DPDC_FORCE_UTF8 must match how PDCurses was built (UTF-8 + wide characters).
# -static puts everything into the .exe, so no extra DLLs are needed on Windows.
i686-w64-mingw32-gcc -std=gnu99 -Wall -Wextra -O2 -DPDC_FORCE_UTF8 \
	-I/usr/i686-w64-mingw32/include/pdcurses \
	main.c -o tetris.exe \
	-static -lpdcurses -lgdi32 -lcomdlg32 -lwinmm
