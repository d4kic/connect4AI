# Connect 4 AI
Implementation of Connect 4 game written in C++, with an AI opponent implemented using Minimax algorithm with Alpha-beta pruning. 

To play run ./connect4/x64/Debug/connect4.exe
## Features
- GUI made using SFML 3.0 library
- Minimax algorithm to check possible moves
- Alpha-beta pruning for cutting off branches that are unnecessary for checking
- Iterative deepening to progressively search deeper moves
- Heuristic evaluation function and positional weights to evaluate current state of the board
- Move ordering for efficiency
## Possible future improvements
- Better evaluation function
- Adding transposition table
- Parallelisation
- GUI Enchancements
- Bitboard representation of the board
