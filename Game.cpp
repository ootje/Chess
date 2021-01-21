#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>
#include <cmath>

Game::Game( const Window& window ) 
	:m_Window{ window }
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	// INIT Squares
	m_SquareWidth = m_Window.width / 8.f;
	m_SquareHeight = m_Window.height / 8.f;
	m_pSquarePosition = new std::vector<Point2f*>;
	for (size_t x = 0; x < 8; x++)
	{
		for (size_t y = 0; y < 8; y++)
		{
			m_pSquarePosition->push_back(new Point2f(x * m_SquareWidth, y * m_SquareHeight));
		}
	}

	// INIT Pieces
	m_IsWhiteBottom = true;
	m_IsWhitesMove = true;
	m_pPiecesTexture = new Texture("Resources/ChessPieces.png");
	m_pWhitePieces = new std::vector<ChessPiece*>;
	for (size_t i = 8; i < 16; i++)
	{
		m_pWhitePieces->push_back(new ChessPiece(Color::white, i, Piece::pawn));
	}
	m_pWhitePieces->push_back(new ChessPiece(Color::white, 0, Piece::rook));
	m_pWhitePieces->push_back(new ChessPiece(Color::white, 7, Piece::rook));

	m_pWhitePieces->push_back(new ChessPiece(Color::white, 1, Piece::knight));
	m_pWhitePieces->push_back(new ChessPiece(Color::white, 6, Piece::knight));

	m_pWhitePieces->push_back(new ChessPiece(Color::white, 2, Piece::bishop));
	m_pWhitePieces->push_back(new ChessPiece(Color::white, 5, Piece::bishop));

	m_pWhitePieces->push_back(new ChessPiece(Color::white, 3, Piece::queen));
	m_pWhitePieces->push_back(new ChessPiece(Color::white, 4, Piece::king));

	m_pBlackPieces = new std::vector<ChessPiece*>;
	for (size_t i = 48; i < 56; i++)
	{
		m_pBlackPieces->push_back(new ChessPiece(Color::black, i, Piece::pawn));
	}
	m_pBlackPieces->push_back(new ChessPiece(Color::black, 56, Piece::rook));
	m_pBlackPieces->push_back(new ChessPiece(Color::black, 63, Piece::rook));

	m_pBlackPieces->push_back(new ChessPiece(Color::black, 57, Piece::knight));
	m_pBlackPieces->push_back(new ChessPiece(Color::black, 62, Piece::knight));

	m_pBlackPieces->push_back(new ChessPiece(Color::black, 58, Piece::bishop));
	m_pBlackPieces->push_back(new ChessPiece(Color::black, 61, Piece::bishop));

	m_pBlackPieces->push_back(new ChessPiece(Color::black, 59, Piece::queen));
	m_pBlackPieces->push_back(new ChessPiece(Color::black, 60, Piece::king));
}

void Game::Cleanup( )
{
	for (auto& s : *m_pWhitePieces)
	{
		delete(s);
	}
	for (auto& s : *m_pBlackPieces)
	{
		delete(s);
	}
	for (auto& s : *m_pSquarePosition)
	{
		delete(s);
	}

	delete(m_pSquarePosition);
	delete(m_pPiecesTexture);
	delete(m_pWhitePieces);
	delete(m_pBlackPieces);
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw( ) const
{
	ClearBackground( );

	for (size_t x = 0; x < 8; x++)
	{
		for (size_t y = 0; y < 8; y++)
		{
			bool white = ((x + y) % 2);
			if (white)
			{
				glColor3f(238.f/255.f, 238.f/255.f, 210.f/255.f);
			}
			else
			{
				glColor3f(118.f / 255.f, 150.f / 255.f, 86.f / 255.f);
			}
			utils::FillRect(*m_pSquarePosition->at(x + y * 8), m_SquareWidth, m_SquareHeight);
		}
	}

	for (size_t i = 0; i < m_pWhitePieces->size(); i++)
	{
		Rectf dstRect;
		dstRect.bottom = int(float(m_pWhitePieces->at(i)->GetPosition()) / 8.f) * m_SquareHeight;
		dstRect.left = (m_pWhitePieces->at(i)->GetPosition() % 8) * m_SquareWidth;
		dstRect.width = m_SquareWidth;
		dstRect.height = m_SquareHeight;
		if (m_IsWhiteMoving)
		{
			if (m_WhiteMovingPiece == i)
			{
				dstRect.left = m_MousePosition.x - m_SquareWidth / 2.f;
				dstRect.bottom = m_MousePosition.y - m_SquareHeight / 2.f;
			}
		}
		m_pWhitePieces->at(i)->Draw(m_pPiecesTexture, dstRect);
	}

	for (size_t i = 0; i < m_pBlackPieces->size(); i++)
	{
		Rectf dstRect;
		dstRect.bottom = int(float(m_pBlackPieces->at(i)->GetPosition()) / 8.f) * m_SquareHeight;
		dstRect.left = (m_pBlackPieces->at(i)->GetPosition() % 8) * m_SquareWidth;
		dstRect.width = m_SquareWidth;
		dstRect.height = m_SquareHeight;
		if (m_IsBlackMoving)
		{
			if (m_BlackMovingPiece == i)
			{
				dstRect.left = m_MousePosition.x - m_SquareWidth / 2.f;
				dstRect.bottom = m_MousePosition.y - m_SquareHeight / 2.f;
			}
		}
		m_pBlackPieces->at(i)->Draw(m_pPiecesTexture, dstRect);
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
	m_MousePosition = Point2f(float(e.x), m_Window.height - float(e.y));
	//std::cout << (int)m_pWhitePieces->at(m_MovingPiece).GetPiece() << "\n";
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	int index = int(m_MousePosition.x / m_SquareWidth) + 8 * int(m_MousePosition.y / m_SquareHeight);
	for (int i = 0; i < (int)m_pWhitePieces->size();i++)
	{
		if (index == m_pWhitePieces->at(i)->GetPosition())
		{
			if (m_IsWhiteBottom)
			{
				m_WhiteMovingPiece = i;
				m_IsWhiteMoving = true;
			}
		}
	}
	for (int i = 0; i < (int)m_pBlackPieces->size(); i++)
	{
		if (index == m_pBlackPieces->at(i)->GetPosition())
		{
			if (m_IsWhiteBottom)
			{
				m_BlackMovingPiece = i;
				m_IsBlackMoving = true;
			}
		}
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	if (m_IsWhiteMoving)
	{
		int newIndex = int(m_MousePosition.x / m_SquareWidth) + 8 * int(m_MousePosition.y / m_SquareHeight);

		if (IsValidMove(newIndex, m_pWhitePieces->at(m_WhiteMovingPiece)))
		{
			m_pWhitePieces->at(m_WhiteMovingPiece)->SetPosition(newIndex);
		}

		m_IsWhiteMoving = false;
	}

	if (m_IsBlackMoving)
	{
		int newIndex = int(m_MousePosition.x / m_SquareWidth) + 8 * int(m_MousePosition.y / m_SquareHeight);

		if (IsValidMove(newIndex, m_pBlackPieces->at(m_BlackMovingPiece)))
		{
			m_pBlackPieces->at(m_BlackMovingPiece)->SetPosition(newIndex);
		}

		m_IsBlackMoving = false;
	}
}

bool Game::IsValidMove(int newPosition , ChessPiece* piece) const
{
	bool valid = IsValidMoveSwitch(newPosition,piece);
	if (!valid)
		return false;
	// dont jump on own pieces
	if (!bool(piece->GetColor()))
	{
		for (auto whitePiece : *m_pWhitePieces)
		{
			if (whitePiece->GetPosition() == newPosition)
			{
				return false;
			}
		}
	}
	else
	{
		for (auto blackPiece : *m_pBlackPieces)
		{
			if (blackPiece->GetPosition() == newPosition)
			{
				return false;
			}
		}
	}

	if (piece->GetPiece() == Piece::king) //cannot be in king loop would cause infinite loop
	{
		if (!bool(piece->GetColor()))
		{
			for (auto blackPiece : *m_pBlackPieces)
			{
				if (IsValidMoveSwitch(newPosition, blackPiece))
				{
					return false;
				}
			}
		}
		else
		{
			for (auto whitePiece : *m_pWhitePieces)
			{
				if (IsValidMoveSwitch(newPosition, whitePiece))
				{
					 return false;
				}
			}
		}
	}
	else
	{
		valid = KingUnderAttack(newPosition, piece);
	}
	// make sure king is not under attack

	return valid;
}

bool Game::IsValidMoveSwitch(int newPosition, ChessPiece* piece) const
{
	bool valid = false;
	switch (piece->GetPiece())
	{
	case Piece::pawn:
		valid = ValidPawnMove(newPosition, piece);
		break;
	case Piece::knight:
		valid = ValidKnightMove(newPosition, piece);
		break;
	case Piece::bishop:
		valid = ValidBishopMove(newPosition, piece);
		break;
	case Piece::rook:
		valid = ValidRookMove(newPosition, piece);
		break;
	case Piece::queen:
		valid = ValidQueenMove(newPosition, piece);
		break;
	case Piece::king:
		valid = ValidKingMove(newPosition, piece);
		break;
	}
	return valid;
}

bool Game::ValidPawnMove(int newPosition, ChessPiece* piece) const
{
	int direction = (int(piece->GetColor()) * 2 - 1);//should be 1 for going up and -1 for going down
	direction = -direction;
	bool valid = false;

	if (piece->GetHasNotMoved())
	{
		if (newPosition - piece->GetPosition() == direction*16)//big jump
		{
			valid = true;
		}
	}
	if (newPosition - piece->GetPosition() == direction*8)
	{
		valid = true;
	}


	return valid;
}

bool Game::ValidRookMove(int newPosition, ChessPiece* piece) const
{
	bool valid = false;
	if (int(newPosition/8.f) == int(piece->GetPosition()/8.f))
	{
		//horizontal
		bool sign = !std::signbit(float(newPosition - piece->GetPosition()));
		int position = piece->GetPosition() + (2 * int(sign) - 1);
		while (position != newPosition)
		{
			for (auto piece : *m_pBlackPieces)
			{
				if (piece->GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			for (auto piece : *m_pWhitePieces)
			{
				if (piece->GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			position += (2 * int(sign) - 1);
		}
		valid = true;
	}
	else if ((newPosition%8) == piece->GetPosition()%8)
	{
		//vertical
		bool sign = !std::signbit(float(newPosition - piece->GetPosition()));
		int position = piece->GetPosition() + (2 * int(sign) - 1) * 8;
		while (position != newPosition)
		{
			for (auto piece : *m_pBlackPieces)
			{
				if (piece->GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			for (auto piece : *m_pWhitePieces)
			{
				if (piece->GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			position += (2*int(sign)-1)*8;
		}
		valid = true;
	}

	return valid;
}

bool Game::ValidKnightMove(int newPosition, ChessPiece* piece) const
{
	bool valid = false;
	
	if (newPosition - 8 == piece->GetPosition()-2 || newPosition + 8 == piece->GetPosition() - 2)
	{
		valid = true;
	}
	else if (newPosition - 1 == piece->GetPosition() + 16 || newPosition + 1 == piece->GetPosition() + 16)
	{
		valid = true;
	}
	else if (newPosition - 8 == piece->GetPosition() + 2 || newPosition + 8 == piece->GetPosition() + 2)
	{
		valid = true;
	}
	else if (newPosition - 1 == piece->GetPosition() - 16 || newPosition + 1 == piece->GetPosition() - 16)
	{
		valid = true;
	}

	return valid;
}

bool Game::ValidBishopMove(int newPosition, ChessPiece* piece) const
{
	bool valid = false;
	int newY = newPosition / 8;
	int oldY = piece->GetPosition() / 8;
	int newX = newPosition % 8;
	int oldX = piece->GetPosition() % 8;
	if (abs(oldX-newX) == abs(oldY-newY))
	{
		bool up = !std::signbit(float(newY-oldY));
		bool right = !std::signbit(float(newX-oldX));
		int direction = (2 * int(up) - 1) * 8 + (2 * int(right) - 1);
		int position = piece->GetPosition() + direction;
		while (position != newPosition)
		{
			for (auto piece : *m_pBlackPieces)
			{
				if (piece->GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			for (auto piece : *m_pWhitePieces)
			{
				if (piece->GetPosition() == position)
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

bool Game::ValidQueenMove(int newPosition, ChessPiece* piece) const
{
	return ValidBishopMove(newPosition,piece) || ValidRookMove(newPosition,piece);
}

bool Game::ValidKingMove(int newPosition, ChessPiece* piece) const
{
	bool valid = false;
	if (newPosition + 1 == piece->GetPosition() || piece->GetPosition() == newPosition - 1)
	{
		valid = true;
	}
	else if (newPosition + 1 == piece->GetPosition()+8 || piece->GetPosition()+8 == newPosition - 1 || piece->GetPosition() + 8 == newPosition)
	{
		valid = true;
	}
	else if (newPosition + 1 == piece->GetPosition()-8 || piece->GetPosition()-8 == newPosition - 1 || piece->GetPosition() - 8 == newPosition)
	{
		valid = true;
	}

	if (piece->GetHasNotMoved() && !valid)
	{
		valid = CheckForCastling(newPosition,piece);
	}
	
	//not in danger walking

	return valid;
}

bool Game::KingUnderAttack(int newPosition, ChessPiece* piece) const
{
	bool valid = true;
	if (!bool(piece->GetColor()))
	{
		int kingPosition = -1;
		for (auto whitePiece : *m_pWhitePieces)
		{
			if (whitePiece->GetPiece() == Piece::king)
			{
				kingPosition = whitePiece->GetPosition();
				break;
			}
		}
		for (auto blackPiece : *m_pBlackPieces)
		{
			if (IsValidMoveSwitch(kingPosition, blackPiece))
			{
				valid = false;
				break;
			}
		}
	}
	else
	{
		int kingPosition = -1;
		for (auto blackPiece : *m_pBlackPieces)
		{
			if (blackPiece->GetPiece() == Piece::king)
			{
				kingPosition = blackPiece->GetPosition();
				break;
			}
		}
		for (auto whitePiece : *m_pWhitePieces)
		{
			if (IsValidMoveSwitch(kingPosition, whitePiece))
			{
				valid = false;
				break;
			}
		}
	}
	return valid;
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

bool Game::CheckForCastling(int newPosition, ChessPiece* piece)const
{
	if (!bool(piece->GetColor()))
	{
		if (newPosition == 6)
		{
			for (auto rook : *m_pWhitePieces)
			{
				if (rook->GetPosition() == 7 && rook->GetHasNotMoved())
				{
					//castle
					return true;
					break;
				}
			}
		}
		else if (newPosition == 2)
		{
			for (auto rook : *m_pWhitePieces)
			{
				if (rook->GetPosition() == 0 && rook->GetHasNotMoved())
				{
					//castle
					return true;
					break;
				}
			}
		}
	}
	else
	{
		if (newPosition == 62)
		{
			for (auto rook : *m_pBlackPieces)
			{
				if (rook->GetPosition() == 63 && rook->GetHasNotMoved())
				{
					//castle
					return true;
					break;
				}
			}
		}
		else if (newPosition == 58)
		{
			for (auto rook : *m_pWhitePieces)
			{
				if (rook->GetPosition() == 56 && rook->GetHasNotMoved())
				{
					//castle
					return true;
					break;
				}
			}
		}
	}

	return false;
}
