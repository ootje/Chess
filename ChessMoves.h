#pragma once
#include <vector>
#include <cmath>
#include "ChessPiece.h"
#include <iostream>


struct ChessMove
{
	ChessMove(int position, ChessPiece piece)
		:movingPiece{piece}
		, newPosition{position}
	{
	}
	int newPosition;
	ChessPiece movingPiece;

	/*ChessMove& operator=(const ChessMove& other)
	{
		std::swap(movingPiece, other.movingPiece);
		newPosition = other.newPosition;
		return *this;
	}*/
};


bool inline IsValidMove(ChessMove newMove, std::vector<ChessPiece>& white, std::vector<ChessPiece>& black);
bool inline IsValidMoveSwitch(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black);

bool inline ValidPawnMove(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black);
bool inline ValidRookMove(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black);
bool inline ValidKnightMove(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black);
bool inline ValidBishopMove(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black);
bool inline ValidQueenMove(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black);
bool inline ValidKingMove(ChessMove newMove, std::vector<ChessPiece>& white, std::vector<ChessPiece>& black);

bool inline KingUnderAttack(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black, int newKingPosition = -100);
bool inline CheckForCastling(ChessMove newMove, std::vector<ChessPiece>& white, std::vector<ChessPiece>& black);

bool inline MakeMove(ChessMove newMove, std::vector<ChessPiece>& white, std::vector<ChessPiece>& black, bool keepListConstant = false);

inline std::vector<ChessMove> AllMovesForOnePiece(ChessPiece piece, std::vector<ChessPiece> white, std::vector<ChessPiece> black);
inline std::vector<ChessMove> AllMovesForAllPieces(std::vector<ChessPiece> white, std::vector<ChessPiece> black, bool isWhitesMove);

bool IsValidMove(ChessMove newMove, std::vector<ChessPiece>& white, std::vector<ChessPiece>& black)
{
	if (newMove.newPosition == newMove.movingPiece.GetPosition())
		return false;

	bool valid = IsValidMoveSwitch(newMove, white, black);
	if (!valid)
		return false;
	// dont jump on own pieces
	if (!bool(newMove.movingPiece.GetColor()))
	{
		for (auto& whitePiece : white)
		{
			if (whitePiece.GetPosition() == newMove.newPosition)
			{
				return false;
			}
		}
	}
	else
	{
		for (auto& blackPiece : black)
		{
			if (blackPiece.GetPosition() == newMove.newPosition)
			{
				return false;
			}
		}
	}

	if (valid)
	{
		if (!bool(newMove.movingPiece.GetColor()))
		{
			for (size_t i = 0; i < black.size(); i++)
			{
				if (black.at(i).GetPosition() == newMove.newPosition)
				{
					black.erase(black.begin() + i);
					break;
				}
				else if ((black.at(i).GetPosition() == newMove.newPosition - 8) && black.at(i).GetCanBeEP())//en passant
				{
					black.erase(black.begin() + i);
					break;
				}
			}
			for (size_t i = 0; i < white.size(); i++)
			{
				if (white.at(i).GetPosition() == newMove.movingPiece.GetPosition())
				{
					white.at(i).SetPosition(newMove.newPosition);
					break;
				}
			}
		}
		else
		{
			for (size_t i = 0; i < white.size(); i++)
			{
				if (white.at(i).GetPosition() == newMove.newPosition)
				{
					white.erase(white.begin() + i);
					break;
				}
				else if ((white.at(i).GetPosition() == newMove.newPosition + 8) && white.at(i).GetCanBeEP())//en passant
				{
					white.erase(white.begin() + i);
					break;
				}
			}
			for (size_t i = 0; i < black.size(); i++)
			{
				if (black.at(i).GetPosition() == newMove.movingPiece.GetPosition())
				{
					black.at(i).SetPosition(newMove.newPosition);
					break;
				}
			}
		}
	}

	if (newMove.movingPiece.GetPiece() == Piece::king) //cannot be in king loop would cause infinite loop
	{
		if (!bool(newMove.movingPiece.GetColor()))
		{
			for (auto& blackPiece : black)
			{
				ChessMove move{ newMove.newPosition,blackPiece };
				if (IsValidMoveSwitch(move, white, black))
				{
					return false;
				}
			}
		}
		else
		{
			for (auto& whitePiece : white)
			{
				ChessMove move{ newMove.newPosition,whitePiece };
				if (IsValidMoveSwitch(move, white, black))
				{
					return false;
				}
			}
		}
	}
	else
	{
		valid = KingUnderAttack(newMove, white, black);
	}
	// make sure king is not under attack

	return valid;
}

bool IsValidMoveSwitch(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black)
{
	bool valid = false;
	switch (newMove.movingPiece.GetPiece())
	{
	case Piece::pawn:
		valid = ValidPawnMove(newMove, white, black);
		break;
	case Piece::knight:
		valid = ValidKnightMove(newMove, white, black);
		break;
	case Piece::bishop:
		valid = ValidBishopMove(newMove, white, black);
		break;
	case Piece::rook:
		valid = ValidRookMove(newMove, white, black);
		break;
	case Piece::queen:
		valid = ValidQueenMove(newMove, white, black);
		break;
	case Piece::king:
		valid = ValidKingMove(newMove, white, black);
		break;
	}
	return valid;
}

bool ValidPawnMove(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black)
{
	int direction = (int(newMove.movingPiece.GetColor()) * 2 - 1);//should be 1 for going up and -1 for going down
	direction = -direction;
	bool valid = false;

	if (newMove.movingPiece.GetHasNotMoved())
	{
		if (newMove.newPosition - newMove.movingPiece.GetPosition() == direction * 16)//big jump
		{
			valid = true;
			for (auto& p : white)
			{
				if (p.GetPosition() == newMove.newPosition || p.GetPosition() == newMove.newPosition - direction * 8)
				{
					return false;
				}
			}
			for (auto& p : black)
			{
				if (p.GetPosition() == newMove.newPosition || p.GetPosition() == newMove.newPosition - direction * 8)
				{
					return false;
				}
			}

			if (!bool(newMove.movingPiece.GetColor()))
			{
				for (size_t i = 0; i < white.size(); i++)
				{
					if (newMove.movingPiece.GetPosition() == white.at(i).GetPosition())
					{
						white.at(i).SetCanBeEp(true);
						break;
					}
				}
			}
			else
			{
				for (size_t i = 0; i < black.size(); i++)
				{
					if (newMove.movingPiece.GetPosition() == black.at(i).GetPosition())
					{
						black.at(i).SetCanBeEp(true);
						break;
					}
				}
			}
		}
	}
	if (newMove.newPosition - newMove.movingPiece.GetPosition() == direction * 8)
	{
		valid = true;
		for (auto& p : white)
		{
			if (p.GetPosition() == newMove.newPosition)
			{
				return false;
			}
		}
		for (auto& p : black)
		{
			if (p.GetPosition() == newMove.newPosition)
			{
				return false;
			}
		}
	}
	if (abs(newMove.newPosition - newMove.movingPiece.GetPosition() - direction * 8) == 1) //sideways
	{
		valid = false;
		if (!bool(newMove.movingPiece.GetColor()))
		{
			for (auto& p : black)
			{
				if (p.GetPosition() == newMove.newPosition || (p.GetPosition() + direction * 8 == newMove.newPosition && p.GetCanBeEP()))
				{
					valid = true;
				}
			}
		}
		else
		{
			for (auto& p : white)
			{
				if (p.GetPosition() == newMove.newPosition || (p.GetPosition() + direction * 8 == newMove.newPosition && p.GetCanBeEP()))
				{
					valid = true;
				}
			}
		}
	}



	return valid;
}

bool ValidRookMove(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black)
{
	bool valid = false;
	if (int(newMove.newPosition / 8.f) == int(newMove.movingPiece.GetPosition() / 8.f))
	{
		//horizontal
		bool sign = !std::signbit(float(newMove.newPosition - newMove.movingPiece.GetPosition()));
		int position = newMove.movingPiece.GetPosition() + (2 * int(sign) - 1);
		while (position != newMove.newPosition)
		{
			for (auto& piece : black)
			{
				if (piece.GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			for (auto& piece : white)
			{
				if (piece.GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			position += (2 * int(sign) - 1);
		}
		valid = true;
	}
	else if ((newMove.newPosition % 8) == newMove.movingPiece.GetPosition() % 8)
	{
		//vertical
		bool sign = !std::signbit(float(newMove.newPosition - newMove.movingPiece.GetPosition()));
		int position = newMove.movingPiece.GetPosition() + (2 * int(sign) - 1) * 8;
		while (position != newMove.newPosition)
		{
			for (auto& piece : black)
			{
				if (piece.GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			for (auto& piece : white)
			{
				if (piece.GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			position += (2 * int(sign) - 1) * 8;
		}
		valid = true;
	}

	return valid;
}

bool ValidKnightMove(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black)
{
	bool valid = false;
	if ((newMove.newPosition - 8) == (newMove.movingPiece.GetPosition() - 2) || (newMove.newPosition + 8) == (newMove.movingPiece.GetPosition() - 2))// left up-down
	{
		if ((newMove.newPosition - 8) % 8 == (newMove.movingPiece.GetPosition() - 2) % 8 || (newMove.newPosition + 8) % 8 == (newMove.movingPiece.GetPosition() - 2) % 8)
		{
			if ((newMove.newPosition - 8) / 8 == (newMove.movingPiece.GetPosition() - 2) / 8 || (newMove.newPosition + 8) / 8 == (newMove.movingPiece.GetPosition() - 2) / 8)
			{
				return true;
			}
		}
	}
	if ((newMove.newPosition - 1) == (newMove.movingPiece.GetPosition() + 16) || (newMove.newPosition + 1) == (newMove.movingPiece.GetPosition() + 16))//up right-left
	{
		if ((newMove.newPosition - 1) % 8 == (newMove.movingPiece.GetPosition() + 16) % 8 || (newMove.newPosition + 1) % 8 == (newMove.movingPiece.GetPosition() + 16) % 8)
		{
			if ((newMove.newPosition - 1) / 8 == (newMove.movingPiece.GetPosition() + 16) / 8 || (newMove.newPosition + 1) / 8 == (newMove.movingPiece.GetPosition() + 16) / 8)
			{
				return true;
			}
		}
	}
	if ((newMove.newPosition - 8) == (newMove.movingPiece.GetPosition() + 2) || (newMove.newPosition + 8) == (newMove.movingPiece.GetPosition() + 2))// right
	{
		if ((newMove.newPosition - 8) % 8 == (newMove.movingPiece.GetPosition() + 2) % 8 || (newMove.newPosition + 8) % 8 == (newMove.movingPiece.GetPosition() + 2) % 8)
		{
			if ((newMove.newPosition - 8) / 8 == (newMove.movingPiece.GetPosition() + 2) / 8 || (newMove.newPosition + 8) / 8 == (newMove.movingPiece.GetPosition() + 2) / 8)
			{
				return true;
			}
		}
	}
	if ((newMove.newPosition - 1) == (newMove.movingPiece.GetPosition() - 16) || (newMove.newPosition + 1) == (newMove.movingPiece.GetPosition() - 16) )// down right-left
	{
		if ((newMove.newPosition - 1) % 8 == (newMove.movingPiece.GetPosition() - 16) % 8 || (newMove.newPosition + 1) % 8 == (newMove.movingPiece.GetPosition() - 16) % 8)
		{
			if ((newMove.newPosition - 1) / 8 == (newMove.movingPiece.GetPosition() - 16) / 8 || (newMove.newPosition + 1) / 8 == (newMove.movingPiece.GetPosition() - 16) / 8)
			{
				return true;
			}
		}
	}

	return false;
}

bool ValidBishopMove(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black)
{
	bool valid = false;
	int newY = newMove.newPosition / 8;
	int oldY = newMove.movingPiece.GetPosition() / 8;
	int newX = newMove.newPosition % 8;
	int oldX = newMove.movingPiece.GetPosition() % 8;
	if (abs(oldX - newX) == abs(oldY - newY))
	{
		bool up = !std::signbit(float(newY - oldY));
		bool right = !std::signbit(float(newX - oldX));
		int direction = (2 * int(up) - 1) * 8 + (2 * int(right) - 1);
		int position = newMove.movingPiece.GetPosition() + direction;
		while (position != newMove.newPosition)
		{
			for (auto& piece : black)
			{
				if (piece.GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			for (auto& piece : white)
			{
				if (piece.GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			position += direction;
		}
		valid = true;
	}
	return valid;
}

bool ValidQueenMove(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black)
{
	return ValidBishopMove(newMove, white, black) || ValidRookMove(newMove, white, black);
}

bool ValidKingMove(ChessMove newMove, std::vector<ChessPiece>& white, std::vector<ChessPiece>& black)
{
	bool valid = false;
	if (newMove.newPosition + 1 == newMove.movingPiece.GetPosition() || newMove.movingPiece.GetPosition() == newMove.newPosition - 1)
	{
		valid = true;
	}
	else if (newMove.newPosition + 1 == newMove.movingPiece.GetPosition() + 8 || newMove.movingPiece.GetPosition() + 8 == newMove.newPosition - 1 || newMove.movingPiece.GetPosition() + 8 == newMove.newPosition)
	{
		valid = true;
	}
	else if (newMove.newPosition + 1 == newMove.movingPiece.GetPosition() - 8 || newMove.movingPiece.GetPosition() - 8 == newMove.newPosition - 1 || newMove.movingPiece.GetPosition() - 8 == newMove.newPosition)
	{
		valid = true;
	}

	if (newMove.movingPiece.GetHasNotMoved() && !valid)
	{
		valid = CheckForCastling(newMove, white, black);
	}

	//not in danger walking

	return valid;
}

bool KingUnderAttack(ChessMove newMove, std::vector<ChessPiece> white, std::vector<ChessPiece> black, int newKingPosition)
{
	bool valid = true;
	if (!bool(newMove.movingPiece.GetColor()))
	{
		int kingPosition = newKingPosition;
		if (kingPosition == -100)
		{
			for (auto& whitePiece : white)
			{
				if (whitePiece.GetPiece() == Piece::king)
				{
					kingPosition = whitePiece.GetPosition();
					break;
				}
			}
		}
		for (auto& blackPiece : black)
		{
			ChessMove move{ kingPosition,blackPiece };
			if (IsValidMoveSwitch(move, white, black))
			{
				valid = false;
				break;
			}
		}
	}
	else
	{
		int kingPosition = newKingPosition;
		if (kingPosition == -100)
		{
			for (auto& blackPiece : black)
			{
				if (blackPiece.GetPiece() == Piece::king)
				{
					kingPosition = blackPiece.GetPosition();
					break;
				}
			}
		}
		for (auto& whitePiece : white)
		{
			ChessMove move{ kingPosition,whitePiece };
			if (IsValidMoveSwitch(move, white, black))
			{
				valid = false;
				break;
			}
		}
	}
	return valid;
}

bool CheckForCastling(ChessMove newMove, std::vector<ChessPiece>& white, std::vector<ChessPiece>& black)
{
	if (!bool(newMove.movingPiece.GetColor()))
	{
		if (newMove.newPosition == 6)
		{
			for (auto& rook : white)
			{
				if (rook.GetPosition() == 7 && rook.GetHasNotMoved())
				{
					ChessMove rookMove{ 5,rook };
					ChessMove kingMove{ newMove.newPosition - 1 , newMove.movingPiece };
					if (IsValidMove(rookMove, white, black) && KingUnderAttack(kingMove, white, black, newMove.newPosition - 1))
					{
						//castle
						rook.SetPosition(5);
						return true;
					}
				}
			}
		}
		else if (newMove.newPosition == 2)
		{
			for (auto& rook : white)
			{
				if (rook.GetPosition() == 0 && rook.GetHasNotMoved())
				{
					ChessMove rookMove{ 3,rook };
					ChessMove kingMove{ newMove.newPosition + 1 , newMove.movingPiece };
					if (IsValidMove(rookMove, white, black) && KingUnderAttack(kingMove, white, black, newMove.newPosition + 1))
					{
						//castle
						rook.SetPosition(3);
						return true;
					}
				}
			}
		}
	}
	else
	{
		if (newMove.newPosition == 62)
		{
			for (auto& rook : black)
			{
				if (rook.GetPosition() == 63 && rook.GetHasNotMoved())
				{
					ChessMove rookMove{ 61,rook };
					ChessMove kingMove{ newMove.newPosition - 1 , newMove.movingPiece };
					if (IsValidMove(rookMove, white, black) && KingUnderAttack(kingMove, white, black, newMove.newPosition - 1))
					{
						//castle
						rook.SetPosition(61);
						return true;
					}
				}
			}
		}
		else if (newMove.newPosition == 58)
		{
			for (auto& rook : black)
			{
				if (rook.GetPosition() == 56 && rook.GetHasNotMoved())
				{
					ChessMove rookMove{ 59,rook };
					ChessMove kingMove{ newMove.newPosition - 1 , newMove.movingPiece };
					if (IsValidMove(rookMove, white, black) && KingUnderAttack(kingMove, white, black, newMove.newPosition + 1))
					{
						//castle
						rook.SetPosition(59);
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool MakeMove(ChessMove newMove, std::vector<ChessPiece>& white, std::vector<ChessPiece>& black, bool keepListConstant)
{
	std::vector<ChessPiece> copyWhite = white;;
	std::vector<ChessPiece> copyBlack = black;

	if (!IsValidMove(newMove, copyWhite, copyBlack))
	{
		return false;
	}
	else // if valid
	{
		if (!bool(newMove.movingPiece.GetColor()))
		{
			for (auto& p : black)
			{
				p.SetCanBeEp(false);
			}
		}
		else
		{
			for (auto& p : white)
			{
				p.SetCanBeEp(false);
			}

		}

		if (!keepListConstant)
		{
			white = copyWhite;
			black = copyBlack;
		}
		return true;
	}
}

std::vector<ChessMove> AllMovesForOnePiece(ChessPiece piece, std::vector<ChessPiece> white, std::vector<ChessPiece> black)
{
	std::vector<ChessMove> vector{};
	for (size_t x = 0; x < 8; x++)
	{
		for (size_t y = 0; y < 8; y++)
		{
			int position = y * 8 + x;
			ChessMove move{ position,piece };
			if (MakeMove(move,white,black,true))
			{
				vector.push_back(ChessMove(y * 8 + x, piece));
			}
		}
	}
	return vector;
}

std::vector<ChessMove> AllMovesForAllPieces(std::vector<ChessPiece> white, std::vector<ChessPiece> black, bool isWhitesMove)
{
	std::vector<ChessMove> vector{};
	if (isWhitesMove)
	{
		for (auto p : white)
		{
			std::vector<ChessMove> tempVector = AllMovesForOnePiece(p, white, black);
			vector.insert(vector.end(), tempVector.begin(), tempVector.end());
		}
	}
	else
	{
		for (auto p : black)
		{
			std::vector<ChessMove> tempVector = AllMovesForOnePiece(p, white, black);
			vector.insert(vector.end(), tempVector.begin(), tempVector.end());
		}
	}
	return vector;
}