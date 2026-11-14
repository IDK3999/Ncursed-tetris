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
		fprintf(f, "%d\n", score);                 /* like printf, into the file */
		fclose(f);

	}
}

int loop(int board[][col_limit]){ // main game loop; return 1 if player lost or 0 if the player quits
	int ch = 0; // the pressed key
	int score = 0, total_lines = 0;
	int highscore = highscore_get();
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
		mvprintw(5, 40, "Highscore: %d", highscore); // outputs the higscore
		refresh();
		ch = getch();
		switch (ch) {
			case 'h': // moves the piece to the left if there is avalible room
			case KEY_LEFT:
				if(it_fits(board, copy_current_piece, y_piece, x_piece - 1) == true)
					x_piece --;
				break;
			
			case 'j': // moves the piece down
			case KEY_DOWN:
				if(it_fits(board, copy_current_piece, y_piece + 1, x_piece) == true)
					y_piece ++;
				break;

			case 'k': // rotates the piece
			case KEY_UP:
				rotation(copy_current_piece, tmp, sizes[current_piece]);
				switch (current_piece) { // verifies where can the piece be rotated; don't even try to read it believe me it works
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
			case KEY_RIGHT: // moves the piece to the right
				if(it_fits(board, copy_current_piece, y_piece, x_piece + 1) == true)
					x_piece ++;
				break;
			case ' ': // hard-drops the piece
				y_piece = landing_row(board, y_piece, x_piece, copy_current_piece);
				last = 0; // locks piece
				break;
			case 'q':
				highscore_write(score, highscore);
				if(score > highscore)
					highscore = score;
				return false;
				break;
		}

		if(now_ms() - last >= 500){ // lowers the piece or locks it
			last = now_ms();
			if(it_fits(board, copy_current_piece, y_piece + 1, x_piece) == true){
				y_piece ++;
			}

			else{
				write_to_board(board, copy_current_piece, x_piece, y_piece, current_piece + 1);

				int counter = check_remove_line(board);
				total_lines += counter;

				switch (counter) { // adds score based in lines
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