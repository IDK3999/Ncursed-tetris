#pragma once

#include <ncurses.h>
#include "checks.c"

void draw(int y, int x, int ch){ // draws 2 characters (ch) to the screen
	mvaddch(y + line_offset, x * 2 + col_offset, ch);
	mvaddch(y + line_offset, 2 * x + 1 + col_offset, ch);
}

void draw_piece(int tetrimino[][4], int y, int x, int current_piece){ // draws a piece to the screen

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++)
			if(tetrimino[i][j] == 1)
				draw(y + i, x + j, ' ' | A_REVERSE | COLOR_PAIR(current_piece));
	}
}

void draw_shadow(int y, int x, const char *ch, int current_piece){ // draws the shadow of a piece to the screen
	attron(COLOR_PAIR(current_piece));
	mvaddstr(y + line_offset, x * 2 + col_offset, ch);
	attroff(COLOR_PAIR(current_piece));
}

void cast_shadow(int board[][col_limit], int x, int y, int tetrimino[][4], int current_piece){ // decides where a shadow must be drawn,
	y = landing_row(board, y, x, tetrimino);												   // then draws it
	
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++)
			if(tetrimino[i][j] == 1)
				draw_shadow(y + i, x + j, "░░", current_piece);
	}
}
