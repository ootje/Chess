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

	// Based on the proposed position from
	// https://medium.freecodecamp.org/simple-chess-ai-step-by-step-1d55a9266977
	float PositionRatingPawn(ChessPiece piece);
	float PositionRatingRook(ChessPiece piece);
	float PositionRatingBishop(ChessPiece piece);
	float PositionRatingKnight(ChessPiece piece);
	float PositionRatingKing(ChessPiece piece);
	float PositionRatingQueen(ChessPiece piece);

	int m_Depth;
};

