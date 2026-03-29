#include <iostream>
#include <limits>
#include <vector>

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

void undo(int r, int c) // FASTER
{
	board[r][c] = EMPTY;
}

int getNextFreeSpace(int c)
{
	for (int i = ROWS - 1; i >= 0; --i)
		if (board[i][c] == EMPTY)
			return i;
	return -1;
}

const bool isWinningMove(int p)
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

	// Checking the right diagonal
	for (int i = 0; i < ROWS - 3; ++i)
	{
		for (int j = 0; j < COLUMNS - 3; ++j)
		{
			if (board[i][j] == p &&
				board[i + 1][j + 1] == p &&
				board[i + 2][j + 2] == p &&
				board[i + 3][j + 3] == p)
				return true;
		}
	}

	// Checking the left diagonal
	for (int i = 3; i < ROWS; ++i)
	{
		for (int j = 0; j < COLUMNS - 3; ++j)
		{
			if (board[i][j] == p &&
				board[i - 1][j + 1] == p &&
				board[i - 2][j + 2] == p &&
				board[i - 3][j + 3] == p)
				return true;
		}
	}

	return false;
}

const bool isValid(int c)
{
	return board[0][c] == EMPTY;
}

vector<int> getValidMoves()
{
	vector<int> moves;
	for (int i = 0; i < COLUMNS; ++i)
		if (isValid(i))
			moves.push_back(i);
	return moves;
}

int evaluate() // TO UPGRADE LATER
{
	if (isWinningMove(PLAYER)) return -1000;
	if (isWinningMove(COMP)) return 1000;
	return 0;
}

int minimax(int depth, int alpha, int beta, bool isMax)
{
	int score = evaluate();
	if (depth == 0 || score == 1000 || score == -1000)
		return score;
	vector<int> moves = getValidMoves();

	if (isMax)
	{
		int maxScore = INT_MIN;
		for (int col : moves)
		{
			int row = getNextFreeSpace(col);
			playMove(row, col, COMP);
			int v = minimax(depth - 1, alpha, beta, false);
			undo(row, col);
			maxScore = max(maxScore, v);
			alpha = max(alpha, v);

			if (alpha >= beta) // beta cutoff
				break;
		}
		return maxScore;
	}
	else
	{
		int minScore = INT_MAX;
		for (int col : moves)
		{
			int row = getNextFreeSpace(col);
			playMove(row, col, PLAYER);
			int v = minimax(depth - 1, alpha, beta, true);
			undo(row, col);
			minScore = min(minScore, v);
			beta = min(beta, v);

			if (alpha >= beta) // beta cutoff
				break;
		}
		return minScore;
	}
}

int getBestMove(int depth)
{
	int maxScore = INT_MIN;
	int bestMove = 0;
	for (int col : getValidMoves())
	{
		int row = getNextFreeSpace(col);
		playMove(row, col, COMP);
		int score = minimax(depth - 1, INT_MIN, INT_MAX, false);
		undo(row, col);
		if (score > maxScore)
		{
			maxScore = score;
			bestMove = col;
		}
	}
	return bestMove;
}

int main()
{
	while (true)
	{
		printBoard();

		// PLAYER MOVE
		int col;
		do
		{
			cin >> col;
			printBoard();
		}
		while (col < 0 || col > 6);
		int row = getNextFreeSpace(col);
		playMove(row, col, PLAYER);
		if (isWinningMove(PLAYER))
		{
			printBoard();
			cout << "Victory" << endl;
			break;
		}

		// COMPUTER MOVE
		col = getBestMove(5);
		row = getNextFreeSpace(col);
		playMove(row, col, COMP);
		if (isWinningMove(COMP))
		{
			printBoard();
			cout << "You lost" << endl;
			break;
		}
	}
	return 0;
}