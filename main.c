#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#include "board_operations.c"

/*
void draw_cell(int y, int x, int ch){
	mvaddch(y, x * 2, ch);
}
*/

long now_ms(void){ // i don't even know atp
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

int highscore_get(){
	FILE *f = fopen(".highscore.txt", "r");   /* "r" = read */

	if(f == NULL)
		return 0;
	int highscore = 0;
	fscanf(f, "%d", &highscore);                   /* like scanf, from the file */
	fclose(f);

	return highscore;
}

void highscore_write(int score, int highscore){
	if(score > highscore){
		FILE *f = fopen(".highscore.txt", "w");   /* "w" = write; creates the file, or empties it */
		if(f == NULL)
			return;
		fprintf(f, "%d\n", score);                 /* like printf, into the file */
		fclose(f);

	}
}

void edit_speeds(int *delay, int *lock_delay){
	if(*(delay) >= 25)
		*(delay) -= *(delay) / 5;
	else
		if(*(lock_delay) > 45) // symbolical value; no further context shall be added
		*(lock_delay) -= *(lock_delay) / 5;
	
}

int loop(int board[][col_limit]){ // main game loop; return 1 if player lost or 0 if the player quits
	int ch = 0; // the pressed key
	int score = 0, total_lines = 0;
	int highscore = highscore_get();
	int delay = 1000; //the delay between drops is 1000; goes down by 20% each level
	int lock_delay = 1000; // the delay between the piece touching the ground and getting locked
	int times_it_moved = 0; // the amount of times a piece has moved while touching the ground
	long lock_start = 0;
	bool piece_moved = false;
	int level = 0; // level. Each 10 lines it goes up by 1. It's used to speed up the game and as a score multiplier
	srand(time(NULL));
	int x_piece = col_limit / 2, y_piece = 0; // position of the current piece
	int current_piece = rand() % 7; // the number of the current piece (see init.c for more info)
	int tmp[4][4]; // copy of the current tetriminio (see rotation() in board_operations.c)
	int what_pieces[7] = {0};
	int copy_current_piece[4][4]; // another copy
	copy_tetrimino(pieces[current_piece], copy_current_piece); // loads the piece into memory
	copy_tetrimino(pieces[current_piece], tmp); // copies the piece
	long last = now_ms();

	while(ch != 'q'){
		erase();
		make_board(board); // creates the board
		cast_shadow(board, x_piece, y_piece, copy_current_piece, current_piece + 1); // casts the shadow
		draw_piece(copy_current_piece, y_piece, x_piece, current_piece + 1); // draws the current piece
		mvprintw(3, 40, "Lines: %d", total_lines); // outputs the amount of total cleared lines
		mvprintw(4, 40, "Score: %d", score); // outputs the score
		mvprintw(5, 40, "Level: %d", level); // outputs the level
		mvprintw(6, 40, "Highscore: %d", highscore); // outputs the higscore
		refresh();
		ch = getch();
		switch (ch) {
			case 'h': // moves the piece to the left if there is avalible room
			case KEY_LEFT:
				if(it_fits(board, copy_current_piece, y_piece, x_piece - 1) == true){
					x_piece --;
					piece_moved = true;
				}
				break;
			
			case 'j': // moves the piece down
			case KEY_DOWN:
				if(it_fits(board, copy_current_piece, y_piece + 1, x_piece) == true){
					y_piece ++;
					piece_moved = true;
				}
				break;

			case 'k': // rotates the piece
			case KEY_UP:
				rotation(copy_current_piece, tmp, sizes[current_piece]);
				switch (current_piece) { // verifies where can the piece be rotated; don't even try to read it believe me it works
					case 2:
						if(it_fits(board, tmp, y_piece, x_piece) == true){
							copy_tetrimino(tmp, copy_current_piece);
							piece_moved = true;
						}
						else if(it_fits(board, tmp, y_piece, x_piece + 1) == true){
							x_piece ++;
							copy_tetrimino(tmp, copy_current_piece);
							piece_moved = true;
						}
						else if(it_fits(board, tmp, y_piece, x_piece + 2) == true){
							x_piece += 2;
							copy_tetrimino(tmp, copy_current_piece);
							piece_moved = true;
						}
						else if(it_fits(board, tmp, y_piece, x_piece - 1) == true){
							x_piece --;
							copy_tetrimino(tmp, copy_current_piece);
							piece_moved = true;
						}
						else if(it_fits(board, tmp, y_piece, x_piece - 2) == true){
							x_piece -= 2;
							copy_tetrimino(tmp, copy_current_piece);
							piece_moved = true;
						}
						break;

					default:
						if(it_fits(board, tmp, y_piece, x_piece) == true){
							copy_tetrimino(tmp, copy_current_piece);
							piece_moved = true;
						}
						else if(it_fits(board, tmp, y_piece, x_piece + 1) == true){
							x_piece ++;
							copy_tetrimino(tmp, copy_current_piece);
							piece_moved = true;
						}
						else if(it_fits(board, tmp, y_piece, x_piece - 1) == true){
							x_piece --;
							copy_tetrimino(tmp, copy_current_piece);
							piece_moved = true;
						}
						break;
				}
				break;
			
			case 'l':
			case KEY_RIGHT: // moves the piece to the right
				if(it_fits(board, copy_current_piece, y_piece, x_piece + 1) == true){
					x_piece ++;
					piece_moved = true;
				}
				break;
			case ' ': // hard-drops the piece
				y_piece = landing_row(board, y_piece, x_piece, copy_current_piece);
				lock_start = 0; // locks piece
				break;
			case 'q':
				highscore_write(score, highscore);
				if(score > highscore)
					highscore = score;
				return false; // no break needed since it will never reach it
		}

		if(piece_moved && !it_fits(board, copy_current_piece, y_piece + 1, x_piece) == true){
			lock_start = now_ms();
			times_it_moved ++;
		}
		piece_moved = false;

		if(it_fits(board, copy_current_piece, y_piece + 1, x_piece) == true){ // lowers the piece or locks it
			if(now_ms() - last >= delay){
				y_piece ++;
				last = now_ms();
				lock_start = now_ms();
			}
		}

		else{
			if(now_ms() - lock_start >= lock_delay || times_it_moved >= 15){
				write_to_board(board, copy_current_piece, x_piece, y_piece, current_piece + 1);
				
				times_it_moved = 0;

				int counter = check_remove_line(board);
				total_lines += counter;

				switch (counter) { // adds score based in lines
					case 0:
						break;
					case 1:
						score += (1 + level) * 100;
						break;
					case 2:
						score += (1 + level) * 300;
						break;

					case 3:
						score += (1 + level) * 500;
						break;
					
					case 4:
						score += (1 + level) * 800;
						break;
				}
				if(total_lines / 10 > level)
					edit_speeds(&delay, &lock_delay);
				level = total_lines / 10;
				counter = 0;

				highscore_write(score, highscore);
				if(score > highscore)
					highscore = score;

				if(player_lost(board)) // ends the game
					return true;

				what_pieces[current_piece] ++;
				if(all_played(what_pieces))
					reset_played_pieces(what_pieces);

				do{
					current_piece = rand() % 7; // picks a new piece by randomly taking an unused one from a bag
				}while(what_pieces[current_piece] != 0);

				what_pieces[current_piece] ++;
				x_piece = col_limit / 2; // resets it to the middle of the board
				y_piece = 0; // resets it to the top
				copy_tetrimino(pieces[current_piece], copy_current_piece); // loads the new piece into memory
				copy_tetrimino(pieces[current_piece], tmp); // copies the new piece
				if(!it_fits(board, tmp, y_piece, x_piece)) // checks if the new piece fits on the board
					return true;
			}
		}
	}
	return 0; //It shouldn't reach this...
}

void on_end(void){
	endwin();
	system("pkill -f Korobeiniki-wansti-tobefree.mp3"); // stops the music
}

void on_signal(int sig){ // note: this thing probably makes the program VERY unsafe
	(void) sig;
	exit(1);
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
	signal(SIGINT, on_signal); // note: this thing probably makes the program VERY unsafe
	signal(SIGTERM, on_signal); // note: this thing probably makes the program VERY unsafe
	atexit(on_end);
	system("mpv --no-terminal --no-video --loop Korobeiniki-wansti-tobefree.mp3 &"); // starts the music

	int board[line_limit][col_limit] = {0}; // makes the board
	initialize_board(board);
	initialize_pieces();

	if (loop(board)){
		mvprintw(line_offset, col_offset, "YOU LOST!!!");
		mvprintw(line_offset + 1, col_offset, "Exiting...");
	}
	refresh();

	napms(500);   /* pause for 500 milliseconds */

	erase();
	
	return 0;
}