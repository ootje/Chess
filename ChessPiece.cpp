#include "pch.h"
#include "ChessPiece.h"

ChessPiece::ChessPiece(Color color, int position, Piece piece)
	:m_Color{color}
	, m_Position{position}
	, m_Piece{piece}
	, m_HasNotMoved{true}
	, m_CanBeEnPassant{ false }
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
	if (m_Color == Color::white)
	{
		if (m_Piece == Piece::pawn)
		{
			if (int(newPosition/8) == 7)
			{
				m_Piece = Piece::queen;
			}
		}
	}
	else
	{
		if (m_Piece == Piece::pawn)
		{
			if (int(newPosition / 8) == 0)
			{
				m_Piece = Piece::queen;
			}
		}
	}
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

bool ChessPiece::GetCanBeEP() const
{
	return m_CanBeEnPassant;
}
void ChessPiece::SetCanBeEp(bool canBe)
{
	m_CanBeEnPassant = canBe;
}

ChessPiece::ChessPiece(const ChessPiece& other)
{
	m_Color = other.m_Color;
	m_Position = other.m_Position;
	m_Piece = other.m_Piece;
	m_HasNotMoved = other.m_HasNotMoved;
	m_CanBeEnPassant = other.m_CanBeEnPassant;
}

ChessPiece& ChessPiece::operator=(const ChessPiece& other)
{
	m_Color = other.m_Color;
	m_Position = other.m_Position;
	m_Piece = other.m_Piece;
	m_HasNotMoved = other.m_HasNotMoved;
	m_CanBeEnPassant = other.m_CanBeEnPassant;
	return *this;
}