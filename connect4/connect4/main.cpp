#include <iostream>
#include <limits>
#include <vector>

#define ROWS 6
#define COLUMNS 7

#define EMPTY 0
#define PLAYER 1
#define COMP 2

using namespace std;

const int ORDER[COLUMNS] = { 3, 2, 4, 1, 5, 0, 6 }; // Checks middle first

const int POSITION_WEIGHTS[ROWS][COLUMNS] = 
{
	{3, 4,  5,  7,  5, 4, 3},
	{4, 6,  8, 10,  8, 6, 4},
	{5, 8, 11, 13, 11, 8, 5},
	{5, 8, 11, 13, 11, 8, 5},
	{4, 6,  8, 10,  8, 6, 4},
	{3, 4,  5,  7,  5, 4, 3}
};

int board[ROWS][COLUMNS]{};

const void printBoard()
{
	system("cls");
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLUMNS; ++j)
			cout << board[i][j] << " ";
		cout << endl;
	}

	for (int i = 0; i < COLUMNS; ++i)
		cout << "- ";
	cout << endl;

	for (int i = 0; i < COLUMNS; ++i)
		cout << i << " ";
	cout << endl;
}

const int getNextFreeSpace(int c)
{
	for (int i = ROWS - 1; i >= 0; --i)
		if (board[i][c] == EMPTY)
			return i;
	return -1;
}

const bool isWinningMove(int p)
{
	// Checking horizontal
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLUMNS - 3; ++j)
			if (board[i][j] == p && board[i][j + 1] == p && board[i][j + 2] == p && board[i][j + 3] == p)
				return true;

	// Checking vertical
	for (int j = 0; j < COLUMNS; ++j)
		for (int i = 0; i < ROWS - 3; ++i)
			if (board[i][j] == p && board[i + 1][j] == p && board[i + 2][j] == p && board[i + 3][j] == p)
				return true;

	// Checking the right diagonal
	for (int i = 0; i < ROWS - 3; ++i)
		for (int j = 0; j < COLUMNS - 3; ++j)
			if (board[i][j] == p && board[i + 1][j + 1] == p && board[i + 2][j + 2] == p && board[i + 3][j + 3] == p)
				return true;

	// Checking the left diagonal
	for (int i = 3; i < ROWS; ++i)
		for (int j = 0; j < COLUMNS - 3; ++j)
			if (board[i][j] == p && board[i - 1][j + 1] == p && board[i - 2][j + 2] == p && board[i - 3][j + 3] == p)
				return true;

	return false;
}

const bool isValid(int c)
{
	return board[0][c] == EMPTY;
}

const vector<int> getValidMoves()
{
	vector<int> moves;
	for (int i = 0; i < COLUMNS; ++i)
	{
		int col = ORDER[i];
		if (isValid(col))
			moves.push_back(col);
	}
	return moves;
}

// EVALUATION
const int evaluateWindow(int c1, int c2, int c3, int c4)
{
	int comp = 0, player = 0, empty = 0;

	for (int v : { c1, c2, c3, c4 })
	{
		if (v == COMP) comp++;
		else if (v == PLAYER) player++;
		else empty++;
	}

	if (comp > 0 && player > 0) return 0;

	if (comp == 3 && empty == 1) return 900;
	if (comp == 2 && empty == 2) return 50;
	if (player == 3 && empty == 1) return -800;
	if (player == 2 && empty == 2) return -45;
	return 0;
}

const int evaluate()
{
	int score = 0;	

	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLUMNS; ++j)
		{
			if (board[i][j] == COMP) score += POSITION_WEIGHTS[i][j];
			else if (board[i][j] == PLAYER) score -= POSITION_WEIGHTS[i][j];
		}
	}
	
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLUMNS - 3; ++j)
			score += evaluateWindow(board[i][j], board[i][j + 1], board[i][j + 2], board[i][j + 3]);

	for (int j = 0; j < COLUMNS; ++j)
		for (int i = 0; i < ROWS - 3; ++i)
			score += evaluateWindow(board[i][j], board[i + 1][j], board[i + 2][j], board[i + 3][j]);

	for (int i = 0; i < ROWS - 3; ++i)
		for (int j = 0; j < COLUMNS - 3; ++j)
			score += evaluateWindow(board[i][j], board[i + 1][j + 1], board[i + 2][j + 2], board[i + 3][j + 3]);

	for (int i = 3; i < ROWS; ++i)
		for (int j = 0; j < COLUMNS - 3; ++j)
			score += evaluateWindow(board[i][j], board[i - 1][j + 1], board[i - 2][j + 2], board[i - 3][j + 3]);
	return score;
}
// ------------

const int minimax(int depth, int alpha, int beta, bool isMax)
{
	if (depth == 0) return evaluate();
		
	vector<int> moves = getValidMoves();
	if (moves.empty()) return 0; // DRAW

	if (isMax)
	{
		int maxScore = INT_MIN;
		for (int col : moves)
		{
			int row = getNextFreeSpace(col);
			board[row][col] = COMP;
			int v = isWinningMove(COMP) ? (INT_MAX - (10 - depth)) : minimax(depth - 1, alpha, beta, false);
			board[row][col] = EMPTY;

			maxScore = max(maxScore, v);
			alpha = max(alpha, v);

			if (alpha >= beta) // beta cut-off
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
			board[row][col] = PLAYER;
			int v = isWinningMove(PLAYER) ? (INT_MIN + (10 - depth)) : minimax(depth - 1, alpha, beta, true);
			board[row][col] = EMPTY;

			minScore = min(minScore, v);
			beta = min(beta, v);

			if (alpha >= beta) // beta cut-off
				break;
		}
		return minScore;
	}
}

const int getBestMove(int depth)
{
	int maxScore = INT_MIN;
	int bestMove = getValidMoves()[0];
	for (int col : getValidMoves())
	{
		int row = getNextFreeSpace(col);
		board[row][col] = COMP;
		int score = isWinningMove(COMP) ? INT_MAX : minimax(depth - 1, INT_MIN, INT_MAX, false);
		board[row][col] = EMPTY;
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
			cout << "Enter your move: ";
			cin >> col;
			if (col < 0 || col > 6 || !isValid(col))
			{
				printBoard();
				cout << "That move is invalid." << endl;
			}
		} while (col < 0 || col > 6 || !isValid(col));
		int row = getNextFreeSpace(col);
		board[row][col] = PLAYER;
		if (isWinningMove(PLAYER))
		{
			printBoard();
			cout << "Victory" << endl;
			break;
		}

		// COMPUTER MOVE
		col = getBestMove(7);
		row = getNextFreeSpace(col);
		board[row][col] = COMP;
		if (isWinningMove(COMP))
		{
			printBoard();
			cout << "You lost" << endl;
			break;
		}

		// CHECK IF DRAW
		bool draw = true;
		for (int i = 0; i < COLUMNS; ++i)
			if (isValid(i))
			{
				draw = false;
				break;
			}
		if (draw)
		{
			printBoard();
			cout << "Draw" << endl;
			break;
		}
	}
	return 0;
}