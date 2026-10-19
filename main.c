#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#include "init.c"

///#define limit 10 // 10 by 10 grid
#define line_limit 21
#define col_limit 12
#define col_offset 4
#define line_offset 2

void draw(int y, int x, int ch){
	mvaddch(y + line_offset, x * 2 + col_offset, ch);
	mvaddch(y + line_offset, 2 * x + 1 + col_offset, ch);
}

/*
void draw_cell(int y, int x, int ch){
	mvaddch(y, x * 2, ch);
}
*/

void make_board(int board[][col_limit]){
	for(int i = 0; i < line_limit; i++){
		for(int j = 0; j < col_limit; j++){
			if(board[i][j] != 0)
				draw(i, j, ' ' | A_REVERSE | COLOR_PAIR(board[i][j]));
		}
	}
}


void copy_tetrimino(int source[][4], int output[][4]){
	for(int i = 0; i < 4; i ++)
		for(int j = 0; j < 4; j++)
			output[i][j] = source[i][j];
}

void empty_tetrimino(int source[][4]){
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			source[i][j] = 0;
}

void rotation(int tetrimino[][4], int tmp[][4], int n){ //Code taken from https://pbinfo.ro
	empty_tetrimino(tmp);
	for (int j = 0; j < n; ++j)
	        for (int i = 0; i < n; ++i)
	            tmp[j][n - i - 1] = tetrimino[i][j];
}

void draw_piece(int tetrimino[][4], int y, int x, int current_piece){

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++)
			if(tetrimino[i][j] == 1)
				draw(y + i, x + j, ' ' | A_REVERSE | COLOR_PAIR(current_piece));
	}
}

int it_fits(int board[][col_limit], int current_piece[][4], int y_piece, int x_piece){
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++){
			if(current_piece[i][j] == 1){
				if(x_piece + j < 0 || x_piece + j >= col_limit)
					return false;
				if(y_piece + i >= line_limit || y_piece + i < 0)
					return false;
				if(board[y_piece + i][x_piece + j] != 0)
					return false;
			}

		}
	return true;
}

void write_to_board(int board[][col_limit], int tetrimino[][4], int x_piece, int y_piece, int current_piece){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(tetrimino[i][j]){
				board[y_piece + i][x_piece + j] = current_piece;
			}
		}
	}
}

long now_ms(void){
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

int all_played(int what_pieces[]){
	for(int i = 0; i < 7; i++)
		if(what_pieces[i] == 0)
			return false;

	return true;
}

void reset_played_pieces(int what_pieces[]){
	for(int i = 0; i < 7; i++)
		what_pieces[i] = 0;
}

int line_full(int line[]){
	for(int i = 1; i < col_limit - 1; i++){
		if(line[i] == 0){
			return false;
		}
	}
	return true;
}

void remove_line(int board[][col_limit], int row){
	for(int i = row; i > 0; i--){
		for(int j = 1; j < col_limit - 1; j++){
			board[i][j] = board[i - 1][j];
		}
	}
}

int check_remove_line(int board[][col_limit]){
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

int player_lost(int board[][col_limit]){
	for(int j = 1; j < col_limit - 1; j++)
		if(board[0][j])
			return true;
	return false;
}

int landing_row(int board[][col_limit], int y, int x, int tetrimino[][4]){
	while(it_fits(board, tetrimino, y + 1, x)){
		y ++;
	}
	return y;
}

void draw_shadow(int y, int x, const char *ch, int current_piece){
	attron(COLOR_PAIR(current_piece));
	mvaddstr(y + line_offset, x * 2 + col_offset, ch);
	attroff(COLOR_PAIR(current_piece));
}

void cast_shadow(int board[][col_limit], int x, int y, int tetrimino[][4], int current_piece){
	y = landing_row(board, y, x, tetrimino);
	
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++)
			if(tetrimino[i][j] == 1)
				draw_shadow(y + i, x + j, "░░", current_piece);
	}
}

int loop(int board[][col_limit]){
	int ch = 0;
	int score = 0, total_lines = 0;
	srand(time(NULL));
	int x_piece = col_limit / 2, y_piece = 0;
	int current_piece = rand() % 7;
	int tmp[4][4];
	int what_pieces[7] = {0};
	int copy_current_piece[4][4];
	copy_tetrimino(pieces[current_piece], copy_current_piece);
	copy_tetrimino(pieces[current_piece], tmp);
	long last = now_ms();

	while(ch != 'q'){
		erase();
		make_board(board);
		cast_shadow(board, x_piece, y_piece, copy_current_piece, current_piece + 1);
		draw_piece(copy_current_piece, y_piece, x_piece, current_piece + 1);
		mvprintw(3, 40, "Lines: %d", total_lines);
		mvprintw(4, 40, "Score: %d", score);
		refresh();
		ch = getch();
		switch (ch) {
			case 'h':
			case KEY_LEFT:
				if(it_fits(board, copy_current_piece, y_piece, x_piece - 1) == true)
					x_piece --;
				break;
			
			case 'j':
			case KEY_DOWN:
				if(it_fits(board, copy_current_piece, y_piece + 1, x_piece) == true)
					y_piece ++;
				break;

			case 'k':
			case KEY_UP:
				rotation(copy_current_piece, tmp, sizes[current_piece]);
				switch (current_piece) {
					case 2:
						if(it_fits(board, tmp, y_piece, x_piece) == true){
							copy_tetrimino(tmp, copy_current_piece);
						}
						else if(it_fits(board, tmp, y_piece, x_piece + 1) == true){
							x_piece ++;
							copy_tetrimino(tmp, copy_current_piece);
						}
						else if(it_fits(board, tmp, y_piece, x_piece + 2) == true){
							x_piece += 2;
							copy_tetrimino(tmp, copy_current_piece);
						}
						else if(it_fits(board, tmp, y_piece, x_piece - 1) == true){
							x_piece --;
							copy_tetrimino(tmp, copy_current_piece);
						}
						else if(it_fits(board, tmp, y_piece, x_piece - 2) == true){
							x_piece -= 2;
							copy_tetrimino(tmp, copy_current_piece);
						}
						break;

					default:
						if(it_fits(board, tmp, y_piece, x_piece) == true){
							copy_tetrimino(tmp, copy_current_piece);
						}
						else if(it_fits(board, tmp, y_piece, x_piece + 1) == true){
							x_piece ++;
							copy_tetrimino(tmp, copy_current_piece);
						}
						else if(it_fits(board, tmp, y_piece, x_piece - 1) == true){
							x_piece --;
							copy_tetrimino(tmp, copy_current_piece);
						}
						break;
				}
				break;
			
			case 'l':
			case KEY_RIGHT:
				if(it_fits(board, copy_current_piece, y_piece, x_piece + 1) == true)
					x_piece ++;
				break;
			case ' ':
				y_piece = landing_row(board, y_piece, x_piece, copy_current_piece);
				last = 0; // locks piece
				break;
			case 'q':
				return false;
		}

		if(now_ms() - last >= 500){
			last = now_ms();
			if(it_fits(board, copy_current_piece, y_piece + 1, x_piece) == true){
				y_piece ++;
			}

			else{
				write_to_board(board, copy_current_piece, x_piece, y_piece, current_piece + 1);

				int counter = check_remove_line(board);
				total_lines += counter;

				switch (counter) {
					case 0:
						break;
					case 1:
						score += 100;
						break;
					case 2:
						score += 300;
						break;

					case 3:
						score += 500;
						break;
					
					case 4:
						score += 800;
						break;
				}
				counter = 0;

				if(player_lost(board))
					return true;

				what_pieces[current_piece] ++;
				if(all_played(what_pieces))
					reset_played_pieces(what_pieces);
				do{
					current_piece = rand() % 7;
				}while(what_pieces[current_piece] != 0);
				what_pieces[current_piece] ++;
				x_piece = col_limit / 2;
				y_piece = 0;
				copy_tetrimino(pieces[current_piece], copy_current_piece);
				copy_tetrimino(pieces[current_piece], tmp);
			}
		}
	}
	return ERR; //It shouldn't reach this...
}

int main()
{
	setlocale(LC_ALL, "");
	initscr();			/* Start curses mode 		*/
	start_color();
	initialize_colours();
	timeout(50);
	curs_set(0);
	cbreak();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */
	system("mpv --no-terminal --no-video --loop Korobeiniki-wansti-tobefree.mp3 &");

	int board[line_limit][col_limit] = {0};
	initialize_board(board);
	initialize_pieces();

	if (loop(board)){
		mvprintw(line_offset, col_offset, "YOU LOST!!!");
		mvprintw(line_offset + 1, col_offset, "Exiting...");
	}
	refresh();

	napms(500);   /* pause for 2000 milliseconds */

	erase();
	endwin();
	system("pkill -f Korobeiniki-wansti-tobefree.mp3");
	return 0;
}