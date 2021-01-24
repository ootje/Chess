# Chess
The goal of the project was to have a chess bot that plays like a player and makes it moves reasonably fast.

Implementation

The bot was implemented in cpp using the Prog2 framework.
This can be achieved by using the minimax algorithm. This algoritm does a depth first search into every possible board with a given depth. 
Where it will look for the best move for itself and then expect the best move for the player and then rates the board. This rating is 
first achieved by simply rating each piece that still stands, later this is improved by adding a piece square table which gives a slightly
modified score based on the position of each piece.
The minimax algorithm can be improved by using alha-beta pruning this algoritm significantly improves the speed of minimax by removing positions that 
result in a worse outcome compared to an already visited board position whitout removing the good board position.
The final improvment is the use of a transposition table, because in chess you can achieve the same position by doing different moves(for depths > 2) 
the algorithm can store these and upon reaching an already visited position it can stop searching any deeper. These positions are stored by using
zobrist hashing.

Result

The result is a chessBot that always plays black and has a set depth of 4 and takes ~15 seconds to find a move but this speeds up as the amount of pieces
on the board gets reduced. During the opening the bot makes decent moves but not always great which could be exploited. During the middle game 
the bot plays well and during the end game the bot will start to play way faster but not always the best move. Since there is no draw by repition 
or draw after 50 moves without taking pieces these situations can happen.

Conclusion

The bot works well and could be improved with ideas like: iterative deepening, quiescence search, ... for faster move generation and increased depth.
During the opening an opening book could be used so the opening is faster and better. Near the end game the bot is super fast and perhaps a way to 
increase the depth could improve it in this scenario.
