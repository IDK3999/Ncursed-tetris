#include <ncurses.h>
#define line_limit 21
#define col_limit 12
#define col_offset 4
#define line_offset 2

int pieces[7][4][4] = {0};
int sizes[] = {3, 4, 4, 3, 3, 3, 3};

void initialize_board(int board[][col_limit]){
	for(int j = 0; j < col_limit; j++)
		board[line_limit - 1][j] = 8;
	for(int i = 0; i < line_limit; i++){
		board[i][col_limit - 1] = 8;
		board[i][0] = 8;
	}
}

void initialize_colours(){
	init_pair(1, COLOR_MAGENTA, COLOR_BLACK); // T
	init_pair(2, COLOR_YELLOW, COLOR_BLACK); // []
	init_pair(3, COLOR_CYAN, COLOR_BLACK); // |
	init_pair(4, COLOR_GREEN, COLOR_BLACK); // S
	init_pair(5, COLOR_RED, COLOR_BLACK); //Z
	init_pair(6, COLOR_BLUE, COLOR_BLACK); //J
	init_pair(7, 208, COLOR_BLACK); // L
	init_pair(8, COLOR_WHITE, COLOR_BLACK); // board
}

void initialize_pieces(){
	/// pieces[which][row][col]

	/// T
	pieces[0][0][1] = 1;
	for(int j = 0; j < 3; j++)
		pieces[0][1][j] = 1;

	/// █
	pieces[1][1][1] = 1;
	pieces[1][1][2] = 1;
	pieces[1][2][1] = 1;
	pieces[1][2][2] = 1;

	/// |
	for(int i = 0; i < 4; i++)
		pieces[2][i][1] = 1;

	/// S
	pieces[3][0][1] = 1;
	pieces[3][0][2] = 1;
	pieces[3][1][0] = 1;
	pieces[3][1][1] = 1;

	/// Z
	pieces[4][0][0] = 1;
	pieces[4][0][1] = 1;
	pieces[4][1][1] = 1;
	pieces[4][1][2] = 1;

	/// J
	pieces[5][0][0] = 1;
	for(int j = 0; j < 3; j++)
		pieces[5][1][j] = 1;

	/// L
	pieces[6][0][2] = 1;
	for(int j = 0; j < 3; j++)
		pieces[6][1][j] = 1;

}