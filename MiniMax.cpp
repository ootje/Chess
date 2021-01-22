#include "pch.h"
#include "MiniMax.h"
#include <algorithm>

MiniMax::MiniMax()
	:m_Depth{2}
{

}

ChessMove MiniMax::CalculateNextMove(std::vector<ChessPiece> white, std::vector<ChessPiece> black)
{
	ChessMove bestMove{ white.at(0).GetPosition(),white.at(0) };
	MiniMaxAlgo(bestMove, white, black, m_Depth, true);
	return bestMove;
}

float MiniMax::MiniMaxAlgo(ChessMove& newMove,std::vector<ChessPiece>& white, std::vector<ChessPiece>& black, int depth, bool isMaximizing)
{
	if (depth == 0) // or no valid move
	{
		return RateBoard(white, black);
	}

	float rating = 0.f;
	if (isMaximizing) //BOT
	{
		rating = -10000.f;
		for (auto& move : AllMovesForAllPieces(white,black,false)) //black
		{
			std::vector<ChessPiece> copyWhite = white;;
			std::vector<ChessPiece> copyBlack = black;

			MakeMove(move, copyWhite, copyBlack);
			ChessMove chessMove = newMove;
			float tempRating = MiniMaxAlgo(chessMove,copyWhite, copyBlack, depth - 1, false);
			if (rating < tempRating)
			{
				rating = tempRating;
				newMove = move;
			}
		}
	}
	else // PREDICT PLAYER
	{
		rating = 10000.f;
		for (auto& move : AllMovesForAllPieces(white, black, true)) //white
		{
			std::vector<ChessPiece> copyWhite = white;;
			std::vector<ChessPiece> copyBlack = black;

			MakeMove(move, copyWhite, copyBlack);
			ChessMove chessMove = newMove;
			float tempRating = MiniMaxAlgo(chessMove,copyWhite, copyBlack, depth - 1, true);
			if (rating > tempRating)
			{
				rating = tempRating;
				newMove = move;
			}
		}
	}

	return rating;
}

float MiniMax::RateBoard(std::vector<ChessPiece> white, std::vector<ChessPiece> black)
{
	float whiteRating = 0.f;
	for (auto& p : white)
	{
		whiteRating -= GetPieceRating(p);
	}
	float blackRating = 0.f;
	for (auto& p : black)
	{
		blackRating += GetPieceRating(p);
	}

	return (blackRating + whiteRating);
}

float MiniMax::GetPieceRating(ChessPiece piece)
{
	float value = 0.f;
	switch (piece.GetPiece())
	{
	case Piece::pawn:
		value += 10.f;
		break;
	case Piece::knight:
		value += 30.f;
		break;
	case Piece::bishop:
		value += 30.f;
		break;
	case Piece::rook:
		value += 50.f;
		break;
	case Piece::queen:
		value += 90.f;
		break;
	case Piece::king:
		value += 1000.f;
		break;
	}
	return value;
}
