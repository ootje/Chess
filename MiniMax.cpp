#include "pch.h"
#include "MiniMax.h"
#include <algorithm>
#include <list>
#include <random>

MiniMax::MiniMax()
	:m_Depth{3}
{
	m_HashTable = std::vector<std::vector<Uint64>>{};
	m_VisitedBoards = std::unordered_set<Uint64>{};
	InitHashTable();
}

ChessMove MiniMax::CalculateNextMove(std::vector<ChessPiece> white, std::vector<ChessPiece> black)
{
	ChessMove bestMove{ white.at(0).GetPosition(),white.at(0) };
	m_VisitedBoards.clear();
	MiniMaxAlgo(bestMove, white, black, m_Depth, true,-10000.f,10000.f);
	return bestMove;
}

float MiniMax::MiniMaxAlgo(ChessMove& newMove,std::vector<ChessPiece>& white, std::vector<ChessPiece>& black, int depth, bool isMaximizing, float alpha, float beta)
{
	if (depth == 0) // or no valid move
	{
		return RateBoard(white, black);
	}

	Uint64 hash = GetHash(white,black);
	if (std::find(m_VisitedBoards.begin(),m_VisitedBoards.end(),hash) != m_VisitedBoards.end())
	{
		//break because board is already visited
		if (isMaximizing)
		{
			return 10000.f;
		}
		else
		{
			return -10000.f;
		}
	}
	else
	{
		m_VisitedBoards.insert(hash);
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
			float tempRating = MiniMaxAlgo(chessMove,copyWhite, copyBlack, depth - 1, false, alpha,beta);

			if (rating < tempRating)
			{
				rating = tempRating;
				newMove = move;
			}

			alpha = std::max(alpha, rating);
			if (alpha >= beta)
			{
				break;
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
			float tempRating = MiniMaxAlgo(chessMove,copyWhite, copyBlack, depth - 1, true,alpha, beta);
			
			if (rating > tempRating)
			{
				rating = tempRating;
				newMove = move;
			}

			beta = std::min(beta, rating);
			if (beta <= alpha)
			{
				break;
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
		value += PositionRatingPawn(piece);
		break;
	case Piece::knight:
		value += 30.f;
		value += PositionRatingKnight(piece);
		break;
	case Piece::bishop:
		value += 30.f;
		value += PositionRatingBishop(piece);
		break;
	case Piece::rook:
		value += 50.f;
		value += PositionRatingRook(piece);
		break;
	case Piece::queen:
		value += 90.f;
		value += PositionRatingQueen(piece);
		break;
	case Piece::king:
		value += 900.f;
		value += PositionRatingKing(piece);
		break;
	}
	return value;
}

float MiniMax::PositionRatingPawn(ChessPiece piece)
{
	int position = piece.GetPosition();
	if ((bool)piece.GetColor())
	{
		position = 63 - position;
	}
	std::list<int> half{ 8,16,32,33,15,23,39,38};
	std::list<int> one{9,10,13,14,40,41,46,47,34,37 };
	std::list<int> two{ 27,28 };
	std::list<int> twoHalf{ 35,36};
	std::list<int> three{ 43,44};
	std::list<int> five{ 48,49,50,51,52,53,54,55};
	std::list<int> minHalf{ 17,22};
	std::list<int> minOne{ 18,21};
	std::list<int> minTwo{ 11,12};

	if (std::find(half.begin(), half.end(), position) != half.end())
	{
		return 0.5f;
	}
	if (std::find(one.begin(), one.end(), position) != one.end())
	{
		return 1.f;
	}
	if (std::find(two.begin(), two.end(), position) != two.end())
	{
		return 2.f;
	}
	if (std::find(twoHalf.begin(), twoHalf.end(), position) != twoHalf.end())
	{
		return 2.5f;
	}
	if (std::find(three.begin(), three.end(), position) != three.end())
	{
		return 3.f;
	}
	if (std::find(five.begin(), five.end(), position) != five.end())
	{
		return 5.f;
	}
	if (std::find(minHalf.begin(), minHalf.end(), position) != minHalf.end())
	{
		return -0.5f;
	}
	if (std::find(minOne.begin(), minOne.end(), position) != minOne.end())
	{
		return -1.f;
	}
	if (std::find(minTwo.begin(), minTwo.end(), position) != minTwo.end())
	{
		return -2.f;
	}
	return 0.f;
}
float MiniMax::PositionRatingRook(ChessPiece piece)
{
	int position = piece.GetPosition();
	if ((bool)piece.GetColor())
	{
		position = 63 - position;
	}
	
	std::list<int> half{ 3,4,48,55 };
	std::list<int> one{ 49,50,51,52,53,54 };
	std::list<int> minHalf{8,16,24,32,40,15,23,31,39,47};

	if (std::find(half.begin(),half.end(),position) != half.end())
	{
		return 0.5f;
	}
	if (std::find(one.begin(),one.end(),position) != one.end())
	{
		return 1.f;
	}
	if (std::find(minHalf.begin(),minHalf.end(),position) != minHalf.end())
	{
		return -0.5f;
	}

	return 0.f;
}
float MiniMax::PositionRatingBishop(ChessPiece piece)
{
	int position = piece.GetPosition();
	if ((bool)piece.GetColor())
	{
		position = 63 - position;
	}
	std::list<int> half{ 9,14,33,34,42,45,37,38};
	std::list<int> one{ 17,18,19,20,21,22,26,27,28,29,35,36,43,44};
	std::list<int> minOne{1,2,3,4,5,6,8,16,24,32,40,48,15,23,31,39,47,55,57,58,59,60,61,62};
	std::list<int> minTwo{0,7,56,63};

	if (std::find(half.begin(), half.end(), position) != half.end())
	{
		return 0.5f;
	}
	if (std::find(one.begin(), one.end(), position) != one.end())
	{
		return 1.f;
	}
	if (std::find(minOne.begin(), minOne.end(), position) != minOne.end())
	{
		return -1.f;
	}
	if (std::find(minTwo.begin(), minTwo.end(), position) != minTwo.end())
	{
		return -2.f;
	}
	return 0.f;
}
float MiniMax::PositionRatingKnight(ChessPiece piece)
{
	int position = piece.GetPosition();
	if ((bool)piece.GetColor())
	{
		position = 63 - position;
	}
	std::list<int> half{ 17,33,11,12,22,38};
	std::list<int> one{ 18,42,21,45};
	std::list<int> oneHalf{19,2,26,29,34,37,43,44};
	std::list<int> two{27,28,35,36};
	std::list<int> minTwo{9,49,14,54};
	std::list<int> minThree{ 2,3,4,5,16,24,32,40,23,31,39,47,58,59,60,61};
	std::list<int> minFour{ 1,6,8,15,48,55,57,62};
	std::list<int> minFive{0,7,56,63};

	if (std::find(half.begin(), half.end(), position) != half.end())
	{
		return 0.5f;
	}
	if (std::find(one.begin(), one.end(), position) != one.end())
	{
		return 1.f;
	}
	if (std::find(oneHalf.begin(), oneHalf.end(), position) != oneHalf.end())
	{
		return 1.5f;
	}
	if (std::find(two.begin(), two.end(), position) != two.end())
	{
		return 2.f;
	}
	if (std::find(minTwo.begin(), minTwo.end(), position) != minTwo.end())
	{
		return -2.f;
	}
	if (std::find(minThree.begin(), minThree.end(), position) != minThree.end())
	{
		return -3.f;
	}
	if (std::find(minFour.begin(), minFour.end(), position) != minFour.end())
	{
		return -4.f;
	}
	if (std::find(minFive.begin(), minFive.end(), position) != minFive.end())
	{
		return -5.f;
	}
	return 0.f;
}
float MiniMax::PositionRatingKing(ChessPiece piece)
{
	int position = piece.GetPosition();
	if ((bool)piece.GetColor())
	{
		position = 63 - position;
	}

	std::list<int> one{2,5};
	std::list<int> two{0,8,9,7,14,15};
	std::list<int> three{1,6};
	std::list<int> minOne{16,23};
	std::list<int> minTwo{24,17,18,19,20,21,22,31};
	std::list<int> minThree{32,40,48,56,25,26,29,30,39,47,55,63};
	std::list<int> minFour{33,41,49,57,34,42,50,58,37,45,53,61,38,46,54,62};
	std::list<int> minFive{35,36,43,44,21,52,59,60};

	if (std::find(one.begin(), one.end(), position) != one.end())
	{
		return 1.f;
	}
	if (std::find(two.begin(), two.end(), position) != two.end())
	{
		return 2.f;
	}
	if (std::find(three.begin(), three.end(), position) != three.end())
	{
		return 3.f;
	}
	if (std::find(minOne.begin(), minOne.end(), position) != minOne.end())
	{
		return -1.f;
	}
	if (std::find(minTwo.begin(), minTwo.end(), position) != minTwo.end())
	{
		return -2.f;
	}
	if (std::find(minThree.begin(), minThree.end(), position) != minThree.end())
	{
		return -3.f;
	}
	if (std::find(minFour.begin(), minFour.end(), position) != minFour.end())
	{
		return -4.f;
	}
	if (std::find(minFive.begin(), minFive.end(), position) != minFive.end())
	{
		return -5.f;
	}
	return 0.f;
}
float MiniMax::PositionRatingQueen(ChessPiece piece)
{
	int position = piece.GetPosition();
	if ((bool)piece.GetColor())
	{
		position = 63 - position;
	}

	std::list<int> half{10,17,18,19,20,21,26,2,28,29,34,35,36,37,42,43,44,45};
	std::list<int> minHalf{3,4,31,32,39,59,60};
	std::list<int> minOne{1,2,5,6,8,15,16,23,40,47,48,55,57,58,61,62};
	std::list<int> minTwo{0,7,56,63};

	if (std::find(half.begin(),half.end(),position) != half.end())
	{
		return 0.5f;
	}
	if (std::find(minHalf.begin(), minHalf.end(), position) != minHalf.end())
	{
		return -0.5f;
	}
	if (std::find(minOne.begin(), minOne.end(), position) != minOne.end())
	{
		return -1.f;
	}
	if (std::find(minTwo.begin(), minTwo.end(), position) != minTwo.end())
	{
		return -2.f;
	}
	return 0.f;
}

Uint64 MiniMax::GetHash(std::vector<ChessPiece> white, std::vector<ChessPiece> black)
{
	Uint64 hash = 0;
	for (size_t i = 0; i < 64; i++)
	{
		for (auto& p : white)
		{
			if (p.GetPosition() == i)
			{
				size_t j = int(p.GetPiece());
				hash = hash ^ m_HashTable.at(i).at(j);
			}
		}
		for (auto& p : black)
		{
			if (p.GetPosition() == i)
			{
				size_t j = int(p.GetPiece()) + 6;//+6 because black
				hash = hash ^ m_HashTable.at(i).at(j);
			}
		}
	}
	return hash;
}

void MiniMax::InitHashTable()
{
	for (size_t i = 0; i < 64; i++)
	{
		std::vector<Uint64> vector{};
		for (size_t j = 0; j < 12; j++)
		{
			vector.push_back(0);
		}
		m_HashTable.push_back(vector);
	}
	for (size_t i = 0; i < 64; i++)
	{
		for (size_t j = 0; j < 12; j++)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<Uint64> distribution(std::numeric_limits<std::uint64_t>::min(), std::numeric_limits<std::uint64_t>::max());
			m_HashTable.at(i).at(j) = distribution(gen);
		}
	}
}