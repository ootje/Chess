#pragma once
#include "ChessMoves.h"
#include <unordered_set>

class MiniMax final
{
public:
	MiniMax();

	ChessMove CalculateNextMove(std::vector<ChessPiece> white, std::vector<ChessPiece> black);

private:
	float MiniMaxAlgo(ChessMove& newMove, std::vector<ChessPiece>& white, std::vector<ChessPiece>& black,int depth , bool isMaximizing , float alpha, float beta);
	float RateBoard(std::vector<ChessPiece> white, std::vector<ChessPiece> black);
	float GetPieceRating(ChessPiece piece, std::vector<ChessPiece> white, std::vector<ChessPiece> black);

	// Based on the proposed position from
	// https://medium.freecodecamp.org/simple-chess-ai-step-by-step-1d55a9266977
	float PositionRatingPawn(ChessPiece piece);
	float PositionRatingRook(ChessPiece piece);
	float PositionRatingBishop(ChessPiece piece);
	float PositionRatingKnight(ChessPiece piece);
	float PositionRatingKing(ChessPiece piece);
	float PositionRatingQueen(ChessPiece piece);

	int m_Depth;
	std::unordered_set<Uint64> m_VisitedBoards;
	std::vector<std::vector<Uint64>> m_HashTable;
	Uint64 GetHash(std::vector<ChessPiece> white, std::vector<ChessPiece> black);
	void InitHashTable();
};

