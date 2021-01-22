#pragma once
#include "ChessMoves.h"

class MiniMax final
{
public:
	MiniMax();

	ChessMove CalculateNextMove(std::vector<ChessPiece> white, std::vector<ChessPiece> black);

private:
	float MiniMaxAlgo(ChessMove& newMove, std::vector<ChessPiece>& white, std::vector<ChessPiece>& black,int depth , bool isMaximizing);
	float RateBoard(std::vector<ChessPiece> white, std::vector<ChessPiece> black);
	float GetPieceRating(ChessPiece piece);
	int m_Depth;
};

