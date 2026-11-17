
#ifdef _WIN32
	#include <curses.h>
#else
	#include <ncurses.h>
#endif

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#include "board_operations.c"


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

int highscore_write(int score, int highscore){
	if(score > highscore){
		FILE *f = fopen(".highscore.txt", "w");   /* "w" = write; creates the file, or empties it */
		if(f == NULL)
			return score;
		fprintf(f, "%d\n", score);                 /* like printf, into the file */
		fclose(f);
		return score;
	}
	return highscore;
}

void edit_speeds(int *delay, int *lock_delay){
	if(*(delay) >= 25)
		*(delay) -= *(delay) / 5;
	else
		if(*(lock_delay) > 45) // symbolical value; no further context shall be added
		*(lock_delay) -= *(lock_delay) / 5;
	
}

bool try_rotate(int board[][col_limit], int tetriminio[][4], int y_piece, int *x_piece, const int current_piece){
	int tmp[4][4];
	const int rotate_offsets[] = {0, 1, -1, 2, -2}; // array with possible offsets when rotating a piece
	rotation(tetriminio, tmp, sizes[current_piece]);

	for(int i = 0; i < 5; i++){
		if(it_fits(board, tmp, y_piece, *x_piece + rotate_offsets[i])){
			*x_piece += rotate_offsets[i];
			copy_tetrimino(tmp, tetriminio);
			return true;
		}
	}
	return false;
}

void draws_random_shit(int board[][col_limit], int tetriminio[][4], const int x_piece, const int y_piece, const int current_piece){
	make_board(board); // creates the board
	cast_shadow(board, x_piece, y_piece, tetriminio, current_piece + 1); // casts the shadow
	draw_piece(tetriminio, y_piece, x_piece, current_piece + 1); // draws the current piece
	
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

	int x_piece, y_piece; // position of the current piece
	int what_pieces[7] = {0};
	int copy_current_piece[4][4]; // another copy
	int current_piece = spawn_piece(copy_current_piece, what_pieces, &x_piece, &y_piece); // the number of the current piece (see init.c for more info)

	const int possible_scores[] = {0, 100, 300, 500, 800}; // array with possible scores
	long last = now_ms();

	while(ch != 'q'){
		erase();
		draws_random_shit(board, copy_current_piece, x_piece, y_piece, current_piece);
		draw_stats(total_lines, score, level, highscore);
		refresh();
		ch = getch();
		switch (ch) {
			case 'h': // moves the piece to the left if there is avalible room
			case KEY_LEFT:
				piece_moved = try_move(board, copy_current_piece, &y_piece, &x_piece, 0, -1);
				break;
			
			case 'j': // moves the piece down
			case KEY_DOWN:
				piece_moved = try_move(board, copy_current_piece, &y_piece, &x_piece, 1, 0);
				break;

			case 'k': // rotates the piece
			case KEY_UP:		
				piece_moved = try_rotate(board, copy_current_piece, y_piece, &x_piece, current_piece);
				break;
			
			case 'l':
			case KEY_RIGHT: // moves the piece to the right
				piece_moved = try_move(board, copy_current_piece, &y_piece, &x_piece, 0, 1);
				break;
			case ' ': // hard-drops the piece
				y_piece = landing_row(board, y_piece, x_piece, copy_current_piece);
				lock_start = 0; // locks piece
				break;
			case 'q':
				highscore = highscore_write(score, highscore);
				return false; // no break needed since it will never reach it
		}

		if(piece_moved && !it_fits(board, copy_current_piece, y_piece + 1, x_piece)){
			lock_start = now_ms();
			times_it_moved ++;
		}
		piece_moved = false;

		if(it_fits(board, copy_current_piece, y_piece + 1, x_piece)){ // lowers the piece or locks it
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

				score += (1 + level) * possible_scores[counter];

				if(total_lines / 10 > level)
					edit_speeds(&delay, &lock_delay);
				level = total_lines / 10;

				highscore = highscore_write(score, highscore);

				if(player_lost(board)) // ends the game
					return true;

				last = now_ms();

				current_piece = spawn_piece(copy_current_piece, what_pieces, &x_piece, &y_piece);
				
				if(!it_fits(board, copy_current_piece, y_piece, x_piece)) // checks if the new piece fits on the board
					return true;
			}
		}
	}
	return 0; //It shouldn't reach this...
}

void on_end(void){
	endwin();
	#ifndef _WIN32 // y'all don't get music
		system("pkill -f Korobeiniki-wansti-tobefree.mp3"); // stops the music
	#endif
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
	
	#ifndef _WIN32 // y'all don't get music
		system("mpv --no-terminal --no-video --loop Korobeiniki-wansti-tobefree.mp3 &"); // starts the music
	#endif

	int board[line_limit][col_limit] = {0}; // makes the board
	initialize_board(board);
	initialize_pieces();

	if (loop(board)){
		mvprintw(line_offset, col_offset, "YOU LOST!!!");
		mvprintw(line_offset + 1, col_offset, "Exiting...");
	}
	refresh();

	napms(500);   /* pause for 500 milliseconds */
	
	return 0;
}