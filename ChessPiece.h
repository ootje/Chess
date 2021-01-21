#pragma once
#include "Texture.h"

enum class Color
{
	white = 0,
	black = 1,
};
enum class Piece
{
	rook,
	bishop,
	queen,
	king,
	knight,
	pawn
};



class ChessPiece final
{
public:
	ChessPiece(Color color,int position,Piece piece);
	~ChessPiece();

	void Draw(Texture* pTexture, Rectf dstRect) const;

	int GetPosition() const;
	void SetPosition(int newPosition);
	Piece GetPiece() const;
	bool GetHasNotMoved() const;
	Color GetColor() const;

	// no pointers for storage so...
	ChessPiece(const ChessPiece& other) = default;
	ChessPiece& operator=(const ChessPiece& other) = default;
	ChessPiece(ChessPiece&& other) = default;
	ChessPiece& operator=(ChessPiece&& other) = default;

private:
	int m_Position;
	Color m_Color;
	Piece m_Piece;
	bool m_HasNotMoved;
};

