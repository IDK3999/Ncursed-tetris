#define line_limit 21
#define col_limit 12
#define col_offset 4
#define line_offset 2

int pieces[7][4][4] = {0};

void initialize_board(int board[][col_limit]){
	for(int j = 0; j < col_limit; j++)
		board[line_limit - 1][j] = 1;
	for(int i = 0; i < line_limit; i++){
		board[i][col_limit - 1] = 1;
		board[i][0] = 1;
	}
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