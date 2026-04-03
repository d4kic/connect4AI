#include <iostream>
#include <limits>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

constexpr int ROWS = 6;
constexpr int COLUMNS = 7;
constexpr int EMPTY = 0;
constexpr int PLAYER = 1;
constexpr int COMP = 2;

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

int board[ROWS][COLUMNS] = { 0 };
int moveCount = 0;

int getNextFreeSpace(int c)
{
	for (int i = ROWS - 1; i >= 0; --i)
		if (board[i][c] == EMPTY)
			return i;
	return -1;
}

bool isWinningMove(int p)
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

bool isValid(int c)
{
	return board[0][c] == EMPTY;
}

vector<int> getValidMoves()
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
int evaluateWindow(int c1, int c2, int c3, int c4)
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

int evaluate()
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

int minimax(int depth, int alpha, int beta, bool isMax)
{
	if (depth == 0) return evaluate();
	
	if (moveCount == ROWS * COLUMNS) return 0; // DRAW
	vector<int> moves = getValidMoves();

	if (isMax)
	{
		int maxScore = INT_MIN;
		for (int col : moves)
		{
			int row = getNextFreeSpace(col);
			board[row][col] = COMP;
			moveCount++;
			int v = isWinningMove(COMP) ? (INT_MAX - (10 - depth)) : minimax(depth - 1, alpha, beta, false);
			board[row][col] = EMPTY;
			moveCount--;

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
			moveCount++;
			int v = isWinningMove(PLAYER) ? (INT_MIN + (10 - depth)) : minimax(depth - 1, alpha, beta, true);
			board[row][col] = EMPTY;
			moveCount--;

			minScore = min(minScore, v);
			beta = min(beta, v);

			if (alpha >= beta) // beta cut-off
				break;
		}
		return minScore;
	}
}

int getBestMove(int maxDepth)
{
	vector<int> moves = getValidMoves();

	for (int col : moves)
	{
		int row = getNextFreeSpace(col);
		board[row][col] = COMP;
		bool win = isWinningMove(COMP);
		board[row][col] = EMPTY;
		if (win) return col;
	}

	for (int col : moves)
	{
		int row = getNextFreeSpace(col);
		board[row][col] = PLAYER;
		bool loss = isWinningMove(PLAYER);
		board[row][col] = EMPTY;
		if (loss) return col;
	}

	int bestMove = moves[0];
	for (int depth = 1; depth <= maxDepth; ++depth)
	{
		int maxScore = INT_MIN;
		for (int col : moves)
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
		if (maxScore == INT_MAX) break;
	}
	return bestMove;
}

int main()
{
	bool gameOver = false;
	const int CELL = 100;
	sf::RenderWindow window(sf::VideoMode({ COLUMNS * CELL, ROWS * CELL }), "Connect 4");

	while (window.isOpen())
	{
		while (optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();

			if (!gameOver)
			{
				// Get the mouse click
				if (const sf::Event::MouseButtonPressed* mouse = event->getIf<sf::Event::MouseButtonPressed>())
				{
					int col = mouse->position.x / CELL;

					if (col >= 0 && col < COLUMNS && isValid(col))
					{
						// PLAYER MOVE
						int row = getNextFreeSpace(col);
						board[row][col] = PLAYER;
						moveCount++;

						if (isWinningMove(PLAYER))
						{
							cout << "Victory" << endl;
							gameOver = true;
						}
						else
						{
							// COMPUTER MOVE
							col = getBestMove(7);
							row = getNextFreeSpace(col);
							board[row][col] = COMP;
							moveCount++;

							if (isWinningMove(COMP))
							{
								cout << "You lost" << endl;
								gameOver = true;
							}
						}
					}
				}
			}
		}

		// Drawing board
		window.clear(sf::Color::Black);

		for (int i = 0; i < ROWS; ++i)
		{
			for (int j = 0; j < COLUMNS; ++j)
			{
				sf::CircleShape circle(40);
				circle.setPosition({ j * CELL + 5.f, i * CELL + 5.f });

				if (board[i][j] == PLAYER) circle.setFillColor(sf::Color::Magenta);
				else if (board[i][j] == COMP) circle.setFillColor(sf::Color::Yellow);
				else circle.setFillColor(sf::Color::White);

				window.draw(circle);
			}
		}

		window.display();
	}
	return 0;
}
