#pragma once

#ifdef _WIN32
	#include <curses.h>
#else
	#include <ncurses.h>
#endif

#include <stdlib.h>
#include "draw_operations.c"


void make_board(int board[][col_limit]){ // sets up each individual cell with required value
	for(int i = 0; i < line_limit; i++){
		for(int j = 0; j < col_limit; j++){
			if(board[i][j] != 0)
				draw(i, j, ' ' | A_REVERSE | COLOR_PAIR(board[i][j]));
		}
	}
}

void write_to_board(int board[][col_limit], int tetrimino[][4], int x_piece, int y_piece, int current_piece){ // locks the current piece to the board
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(tetrimino[i][j]){
				board[y_piece + i][x_piece + j] = current_piece;
			}
		}
	}
}

void reset_played_pieces(int what_pieces[]){ // self-explainatory
	for(int i = 0; i < 7; i++)
		what_pieces[i] = 0;
}


void remove_line(int board[][col_limit], int row){ // lowers all lines above the filled line by one (1) line
	for(int i = row; i > 0; i--){
		for(int j = 1; j < col_limit - 1; j++){
			board[i][j] = board[i - 1][j];
		}
	}
}

int check_remove_line(int board[][col_limit]){ // checks how many full lines were removed
	int counter = 0;
	for(int i = line_limit - 2; i > 0; i--){
		if(line_full(board[i])){
			remove_line(board, i);
			i ++;
			counter ++;
		}
	}
	return counter;
}

void copy_tetrimino(int source[][4], int output[][4]){ // copies the source tetrimino into the output tetrimino
	for(int i = 0; i < 4; i ++)
		for(int j = 0; j < 4; j++)
			output[i][j] = source[i][j];
}

void empty_tetrimino(int source[][4]){ // sets all cells in a tetriminio to 0
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			source[i][j] = 0;
}

void rotation(int tetrimino[][4], int tmp[][4], int n){ // Code taken from https://pbinfo.ro
	empty_tetrimino(tmp);						// self-explainatory
	for (int j = 0; j < n; ++j)
	        for (int i = 0; i < n; ++i)
	            tmp[j][n - i - 1] = tetrimino[i][j];
}

int spawn_piece(int copy_current_piece[][4], int what_pieces[], int *x_piece, int *y_piece){
	if(all_played(what_pieces)){
		reset_played_pieces(what_pieces);
	}

	int current_piece;
	do{
		current_piece = rand() % 7; // picks a new, unpicked piece
	}while(what_pieces[current_piece] != 0);
	what_pieces[current_piece] ++;

	*(x_piece) = col_limit / 2; // resets to top-middle
	*(y_piece) = 0; // resets to top row

	copy_tetrimino(pieces[current_piece], copy_current_piece);

	return current_piece;
}