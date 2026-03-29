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

int getNextFreeSpace(int c)
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

vector<int> getValidMoves()
{
	vector<int> moves;
	for (int i = 0; i < COLUMNS; ++i)
	{
		int col = ORDER[i];
		if (isValid(i))
			moves.push_back(i);
	}
	return moves;
}

// EVALUATION
int evaluateWindow(vector<int> window)
{
	int score = 0, comp = 0, player = 0, empty = 0;

	for (int loc : window)
	{
		if (loc == COMP) comp++;
		else if (loc == PLAYER) player++;
		else empty++;
	}

	if (comp == 4) score += 1000000;
	else if (comp == 3 && empty == 1) score += 100;
	else if (comp == 2 && empty == 2) score += 10;

	if (player == 4) score -= 1000000;
	else if (player == 3 && empty == 1) score -= 70;
	else if (player == 2 && empty == 2) score -= 40;

	return score;
}

int evaluate()
{
	int score = 0;	

	int center = COLUMNS / 2;
	int count = 0;
	for (int i = 0; i < ROWS; ++i)
		if (board[i][center] == COMP)
			count++;
	score += 50 * count;
	
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLUMNS - 3; ++j)
			score += evaluateWindow({ board[i][j], board[i][j + 1], board[i][j + 2], board[i][j + 3] });

	for (int j = 0; j < COLUMNS; ++j)
		for (int i = 0; i < ROWS - 3; ++i)
			score += evaluateWindow({ board[i][j], board[i + 1][j], board[i + 2][j], board[i + 3][j] });

	for (int i = 0; i < ROWS - 3; ++i)
		for (int j = 0; j < COLUMNS - 3; ++j)
			score += evaluateWindow({ board[i][j], board[i + 1][j + 1], board[i + 2][j + 2], board[i + 3][j + 3] });

	for (int i = 3; i < ROWS; ++i)
		for (int j = 0; j < COLUMNS - 3; ++j)
			score += evaluateWindow({ board[i][j], board[i - 1][j + 1], board[i - 2][j + 2], board[i - 3][j + 3] });
	return score;
}
//

int minimax(int depth, int alpha, int beta, bool isMax)
{
	if (isWinningMove(COMP)) return INT_MAX;
	if (isWinningMove(PLAYER)) return INT_MIN;
	if (depth == 0) return evaluate();
		
	vector<int> moves = getValidMoves();

	if (isMax)
	{
		int maxScore = INT_MIN;
		for (int col : moves)
		{
			int row = getNextFreeSpace(col);
			board[row][col] = COMP;
			int v = isWinningMove(COMP) ? INT_MAX : minimax(depth - 1, alpha, beta, false);
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
			int v = isWinningMove(PLAYER) ? INT_MIN : minimax(depth - 1, alpha, beta, true);
			board[row][col] = EMPTY;

			minScore = min(minScore, v);
			beta = min(beta, v);

			if (alpha >= beta) // beta cut-off
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
		board[row][col] = COMP;
		int score = minimax(depth - 1, INT_MIN, INT_MAX, false);
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
		col = getBestMove(5);
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