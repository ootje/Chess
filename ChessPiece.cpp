#include "pch.h"
#include "ChessPiece.h"

ChessPiece::ChessPiece(Color color, int position, Piece piece)
	:m_Color{color}
	, m_Position{position}
	, m_Piece{piece}
	, m_HasNotMoved{true}
{
};
ChessPiece::~ChessPiece() 
{
};

void ChessPiece::Draw(Texture* pTexture, Rectf dstRect) const
{
	Rectf srcRect;
	srcRect.bottom = int(m_Color) * pTexture->GetHeight()/2.f;
	srcRect.height = pTexture->GetHeight() / 2.f;
	srcRect.width = pTexture->GetWidth() / 6.f;
	srcRect.left = int(m_Piece) * srcRect.width;
	dstRect.bottom -= 5.f;
	pTexture->Draw(dstRect, srcRect);
}

int ChessPiece::GetPosition() const
{
	return m_Position;
}

void ChessPiece::SetPosition(int newPosition)
{
	m_HasNotMoved = false;
	m_Position = newPosition;
}

Piece ChessPiece::GetPiece() const
{
	return m_Piece;
}

bool ChessPiece::GetHasNotMoved() const
{
	return m_HasNotMoved;
}

Color ChessPiece::GetColor() const
{
	return m_Color;
}