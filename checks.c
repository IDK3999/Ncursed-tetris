#pragma once

#ifdef _WIN32
	#include <curses.h>
#else
	#include <ncurses.h>
#endif

#include "init.c"


int it_fits(int board[][col_limit], int current_piece[][4], int y_piece, int x_piece){ // checks if a piece fits
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++){
			if(current_piece[i][j] == 1){
				if(x_piece + j < 0 || x_piece + j >= col_limit)
					return false; // doesn't fit
				if(y_piece + i >= line_limit || y_piece + i < 0)
					return false; // doesn't fit
				if(board[y_piece + i][x_piece + j] != 0)
					return false; // doesn't fit
			}

		}
	return true; // fits
}

int all_played(int what_pieces[]){ //checks if all 7 pieces have been played
	for(int i = 0; i < 7; i++)
		if(what_pieces[i] == 0)
			return false; // there are still pieces to be played

	return true; // all pieces have been played
}

int line_full(int line[]){ // checks if the line is full
	for(int i = 1; i < col_limit - 1; i++){
		if(line[i] == 0){
			return false; // there still is an empty cell
		}
	}
	return true; // the line is fully occupied and will be cleared
}

int player_lost(int board[][col_limit]){ // checks if the player lost the game
	for(int j = 1; j < col_limit - 1; j++)
		if(board[0][j])
			return true; // the player has lost (skill issue lol)
	return false; // the player is still alive
}

int landing_row(int board[][col_limit], int y, int x, int tetrimino[][4]){ // self-explainatory
	while(it_fits(board, tetrimino, y + 1, x)){ // while there still is room to go down
		y ++;
	}
	return y;
}

bool try_move(int board[][col_limit], int tetriminio[][4], int *y_piece, int *x_piece, int dy, int dx){
	if(it_fits(board, tetriminio, *y_piece + dy, *x_piece + dx)){
		*x_piece += dx;
		*y_piece += dy;
		return true;
	}
	return false;
}