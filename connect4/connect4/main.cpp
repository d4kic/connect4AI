#include <iostream>
#include <limits>

#define ROWS 6
#define COLUMNS 7

#define EMPTY 0
#define PLAYER 1
#define COMP 2

using namespace std;

int board[ROWS][COLUMNS]{};

void printBoard()
{
	system("cls");
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLUMNS; ++j)
			cout << board[i][j] << " ";
		cout << endl;
	}
}

void playMove(int r, int c, int p)
{
	board[r][c] = p;
}

int getNextFreeSpace(int c)
{
	for (int i = ROWS - 1; i >= 0; --i)
		if (board[i][c] == EMPTY)
			return i;
	return -1;
}

bool isWinningMove(int p)
{
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLUMNS - 3; ++j)
		{
			if (board[i][j] == p &&
				board[i][j + 1] == p &&
				board[i][j + 2] == p &&
				board[i][j + 3] == p)
				return true;
		}
	}

	for (int j = 0; j < COLUMNS; ++j)
	{
		for (int i = 0; i < ROWS - 3; ++i)
		{
			if (board[i][j] == p &&
				board[i + 1][j] == p &&
				board[i + 2][j] == p &&
				board[i + 3][j] == p)
				return true;
		}
	}

	// TO ADD: DIAGONAL CHECKING

	return false;
}

int main()
{
	while (true)
	{
		printBoard();
		int col;
		cin >> col;
		int row = getNextFreeSpace(col);
		playMove(row, col, PLAYER);
		if (isWinningMove(PLAYER))
		{
			printBoard();
			cout << "Victory" << endl;
			break;
		}

		// TO ADD: AI MOVES
	}
	return 0;
}