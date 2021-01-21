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
	m_pSquarePosition = new std::vector<Point2f>;
	for (size_t x = 0; x < 8; x++)
	{
		for (size_t y = 0; y < 8; y++)
		{
			m_pSquarePosition->push_back(Point2f(x * m_SquareWidth, y * m_SquareHeight));
		}
	}

	// INIT Pieces
	m_IsWhiteBottom = true;
	m_IsWhitesMove = true;
	m_pPiecesTexture = new Texture("Resources/ChessPieces.png");
	m_pWhitePieces = new std::vector<ChessPiece>;
	for (size_t i = 8; i < 16; i++)
	{
		m_pWhitePieces->push_back(ChessPiece(Color::white, i, Piece::pawn));
	}
	m_pWhitePieces->push_back(ChessPiece(Color::white, 0, Piece::rook));
	m_pWhitePieces->push_back(ChessPiece(Color::white, 7, Piece::rook));

	m_pWhitePieces->push_back(ChessPiece(Color::white, 1, Piece::knight));
	m_pWhitePieces->push_back(ChessPiece(Color::white, 6, Piece::knight));

	m_pWhitePieces->push_back(ChessPiece(Color::white, 2, Piece::bishop));
	m_pWhitePieces->push_back(ChessPiece(Color::white, 5, Piece::bishop));

	m_pWhitePieces->push_back(ChessPiece(Color::white, 3, Piece::queen));
	m_pWhitePieces->push_back(ChessPiece(Color::white, 4, Piece::king));

	m_pBlackPieces = new std::vector<ChessPiece>;
	for (size_t i = 48; i < 56; i++)
	{
		m_pBlackPieces->push_back(ChessPiece(Color::black, i, Piece::pawn));
	}
	m_pBlackPieces->push_back(ChessPiece(Color::black, 56, Piece::rook));
	m_pBlackPieces->push_back(ChessPiece(Color::black, 63, Piece::rook));

	m_pBlackPieces->push_back(ChessPiece(Color::black, 57, Piece::knight));
	m_pBlackPieces->push_back(ChessPiece(Color::black, 62, Piece::knight));

	m_pBlackPieces->push_back(ChessPiece(Color::black, 58, Piece::bishop));
	m_pBlackPieces->push_back(ChessPiece(Color::black, 61, Piece::bishop));

	m_pBlackPieces->push_back(ChessPiece(Color::black, 59, Piece::queen));
	m_pBlackPieces->push_back(ChessPiece(Color::black, 60, Piece::king));
}

void Game::Cleanup( )
{
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
			utils::FillRect(m_pSquarePosition->at(x + y * 8), m_SquareWidth, m_SquareHeight);
		}
	}

	for (size_t i = 0; i < m_pWhitePieces->size(); i++)
	{
		Rectf dstRect;
		dstRect.bottom = int(float(m_pWhitePieces->at(i).GetPosition()) / 8.f) * m_SquareHeight;
		dstRect.left = (m_pWhitePieces->at(i).GetPosition() % 8) * m_SquareWidth;
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
		m_pWhitePieces->at(i).Draw(m_pPiecesTexture, dstRect);
	}

	for (size_t i = 0; i < m_pBlackPieces->size(); i++)
	{
		Rectf dstRect;
		dstRect.bottom = int(float(m_pBlackPieces->at(i).GetPosition()) / 8.f) * m_SquareHeight;
		dstRect.left = (m_pBlackPieces->at(i).GetPosition() % 8) * m_SquareWidth;
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
		m_pBlackPieces->at(i).Draw(m_pPiecesTexture, dstRect);
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
		if (index == m_pWhitePieces->at(i).GetPosition())
		{
			if (m_IsWhitesMove)
			{
				m_WhiteMovingPiece = i;
				m_IsWhiteMoving = true;
			}
		}
	}
	for (int i = 0; i < (int)m_pBlackPieces->size(); i++)
	{
		if (index == m_pBlackPieces->at(i).GetPosition())
		{
			if (!m_IsWhitesMove)
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

		MakeMove(newIndex, m_pWhitePieces->at(m_WhiteMovingPiece));

		m_IsWhiteMoving = false;
	}

	if (m_IsBlackMoving)
	{
		int newIndex = int(m_MousePosition.x / m_SquareWidth) + 8 * int(m_MousePosition.y / m_SquareHeight);

		MakeMove(newIndex, m_pBlackPieces->at(m_BlackMovingPiece));

		m_IsBlackMoving = false;
	}
}

void Game::ClearBackground() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::MakeMove(int newPosition, ChessPiece& piece)
{
	std::vector<ChessPiece>* pWhite = new std::vector<ChessPiece>;
	pWhite->insert(pWhite->end(), m_pWhitePieces->begin(), m_pWhitePieces->end());
	std::vector<ChessPiece>* pBlack = new std::vector<ChessPiece>;
	pBlack->insert(pBlack->end(), m_pBlackPieces->begin(), m_pBlackPieces->end());

	if (!IsValidMove(newPosition, piece, pWhite, pBlack))
	{
		delete(pWhite);
		delete(pBlack);
	}
	else // if valid
	{
		if (!bool(piece.GetColor()))
		{
			for (auto& p : *pBlack)
			{
				p.SetCanBeEp(false);
			}
		}
		else
		{
			for (auto& p : *pWhite)
			{
				p.SetCanBeEp(false);
			}

		}
		
		delete(m_pBlackPieces);
		delete(m_pWhitePieces);
		m_pWhitePieces = pWhite;
		m_pBlackPieces = pBlack;
		pWhite = nullptr;
		pBlack = nullptr;

		//swap turn
		m_IsWhitesMove = !m_IsWhitesMove;
	}
}

bool Game::IsValidMove(int newPosition, ChessPiece& piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack)
{
	if (newPosition == piece.GetPosition())
		return false;
	
	bool valid = IsValidMoveSwitch(newPosition,piece,pWhite,pBlack);
	if (!valid)
		return false;
	// dont jump on own pieces
	if (!bool(piece.GetColor()))
	{
		for (auto& whitePiece : *pWhite)
		{
			if (whitePiece.GetPosition() == newPosition)
			{
				return false;
			}
		}
	}
	else
	{
		for (auto& blackPiece : *pBlack)
		{
			if (blackPiece.GetPosition() == newPosition)
			{
				return false;
			}
		}
	}

	if (valid)
	{
		if (!bool(piece.GetColor()))
		{
			for (size_t i = 0; i < pBlack->size(); i++)
			{
				if (pBlack->at(i).GetPosition() == newPosition)
				{
					pBlack->erase(pBlack->begin() + i);
					break;
				}
				else if ((pBlack->at(i).GetPosition() == newPosition - 8) && pBlack->at(i).GetCanBeEP())//en passant
				{
					pBlack->erase(pBlack->begin() + i);
					break;
				}
			}
			for (size_t i = 0; i < pWhite->size(); i++)
			{
				if (pWhite->at(i).GetPosition() == piece.GetPosition())
				{
					pWhite->at(i).SetPosition(newPosition);
					break;
				}
			}
		}
		else
		{
			for (size_t i = 0; i < pWhite->size(); i++)
			{
				if (pWhite->at(i).GetPosition() == newPosition)
				{
					pWhite->erase(pWhite->begin() + i);
					break;
				}
				else if ((pWhite->at(i).GetPosition() == newPosition + 8) && pWhite->at(i).GetCanBeEP())//en passant
				{
					pWhite->erase(pWhite->begin() + i);
					break;
				}
			}
			for (size_t i = 0; i < pBlack->size(); i++)
			{
				if (pBlack->at(i).GetPosition() == piece.GetPosition())
				{
					pBlack->at(i).SetPosition(newPosition);
					break;
				}
			}
		}
	}

	if (piece.GetPiece() == Piece::king) //cannot be in king loop would cause infinite loop
	{
		if (!bool(piece.GetColor()))
		{
			for (auto& blackPiece : *pBlack)
			{
				if (IsValidMoveSwitch(newPosition, blackPiece,pWhite,pBlack))
				{
					return false;
				}
			}
		}
		else
		{
			for (auto& whitePiece : *pWhite)
			{
				if (IsValidMoveSwitch(newPosition, whitePiece,pWhite,pBlack))
				{
					 return false;
				}
			}
		}
	}
	else
	{
		valid = KingUnderAttack(newPosition, piece,pWhite,pBlack);
	}
	// make sure king is not under attack

	return valid;
}

bool Game::IsValidMoveSwitch(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack)
{
	bool valid = false;
	switch (piece.GetPiece())
	{
	case Piece::pawn:
		valid = ValidPawnMove(newPosition, piece,pWhite,pBlack);
		break;
	case Piece::knight:
		valid = ValidKnightMove(newPosition, piece,pWhite,pBlack);
		break;
	case Piece::bishop:
		valid = ValidBishopMove(newPosition, piece, pWhite, pBlack);
		break;
	case Piece::rook:
		valid = ValidRookMove(newPosition, piece, pWhite, pBlack);
		break;
	case Piece::queen:
		valid = ValidQueenMove(newPosition, piece, pWhite, pBlack);
		break;
	case Piece::king:
		valid = ValidKingMove(newPosition, piece, pWhite, pBlack);
		break;
	}
	return valid;
}

bool Game::ValidPawnMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack)
{
	int direction = (int(piece.GetColor()) * 2 - 1);//should be 1 for going up and -1 for going down
	direction = -direction;
	bool valid = false;

	if (piece.GetHasNotMoved())
	{
		if (newPosition - piece.GetPosition() == direction*16)//big jump
		{
			valid = true;
			for (auto& p : *pWhite)
			{
				if (p.GetPosition() == newPosition || p.GetPosition() == newPosition - direction*8)
				{
					return false;
				}
			}
			for (auto& p : *pBlack)
			{
				if (p.GetPosition() == newPosition || p.GetPosition() == newPosition - direction * 8)
				{
					return false;
				}
			}

			if (!bool(piece.GetColor()))
			{
				for (size_t i = 0; i < pWhite->size(); i++)
				{
					if (piece.GetPosition() == pWhite->at(i).GetPosition())
					{
						pWhite->at(i).SetCanBeEp(true);
						break;
					}
				}
			}
			else
			{
				for (size_t i = 0; i < pBlack->size(); i++)
				{
					if (piece.GetPosition() == pBlack->at(i).GetPosition())
					{
						pBlack->at(i).SetCanBeEp(true);
						break;
					}
				}
			}
		}
	}
	if (newPosition - piece.GetPosition() == direction*8)
	{
		valid = true;
		for (auto& p : *pWhite)
		{
			if (p.GetPosition() == newPosition)
			{
				return false;
			}
		}
		for (auto& p : *pBlack)
		{
			if (p.GetPosition() == newPosition)
			{
				return false;
			}
		}
	}
	if (abs(newPosition-piece.GetPosition()-direction*8) == 1) //sideways
	{
		valid = false;
		if (!bool(piece.GetColor()))
		{
			for (auto& p : *pBlack)
			{
				if (p.GetPosition() == newPosition || (p.GetPosition() + direction*8 == newPosition && p.GetCanBeEP()))
				{
					valid = true;
				}
			}
		}
		else
		{
			for (auto& p : *pWhite)
			{
				if (p.GetPosition() == newPosition || (p.GetPosition() + direction*8 == newPosition && p.GetCanBeEP()))
				{
					valid = true;
				}
			}
		}
	}



	return valid;
}

bool Game::ValidRookMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack)
{
	bool valid = false;
	if (int(newPosition/8.f) == int(piece.GetPosition()/8.f))
	{
		//horizontal
		bool sign = !std::signbit(float(newPosition - piece.GetPosition()));
		int position = piece.GetPosition() + (2 * int(sign) - 1);
		while (position != newPosition)
		{
			for (auto& piece : *pBlack)
			{
				if (piece.GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			for (auto& piece : *pWhite)
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
	else if ((newPosition%8) == piece.GetPosition()%8)
	{
		//vertical
		bool sign = !std::signbit(float(newPosition - piece.GetPosition()));
		int position = piece.GetPosition() + (2 * int(sign) - 1) * 8;
		while (position != newPosition)
		{
			for (auto& piece : *pBlack)
			{
				if (piece.GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			for (auto& piece : *pWhite)
			{
				if (piece.GetPosition() == position)
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

bool Game::ValidKnightMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack)
{
	bool valid = false;
	
	if ((newPosition - 8)%8 == (piece.GetPosition()-2)%8 || (newPosition + 8)%8 == (piece.GetPosition() - 2)%8)
	{
		if ((newPosition - 8)/8 == (piece.GetPosition() - 2)/8 || (newPosition + 8)/8 == (piece.GetPosition() - 2)/8)
		{
			return true;
		}
	}
	if ((newPosition - 1)%8 == (piece.GetPosition() + 16)%8 || (newPosition + 1)%8 == (piece.GetPosition() + 16)%8)
	{
		if ((newPosition - 1)/8 == (piece.GetPosition() + 16)/8 || (newPosition + 1)/8 == (piece.GetPosition() + 16)/8)
		{
			return true;
		}
	}
	if ((newPosition - 8)%8 == (piece.GetPosition() + 2)%8 || (newPosition + 8)%8 == (piece.GetPosition() + 2)%8)
	{
		if ((newPosition - 8)/8 == (piece.GetPosition() + 2)/8 || (newPosition + 8)/8 == (piece.GetPosition() + 2)/8)
		{
			return true;
		}
	}
	if ((newPosition - 1)%8 == (piece.GetPosition() - 16)%8 || (newPosition + 1)%8 == (piece.GetPosition() - 16)%8)
	{
		if ((newPosition - 1)/8 == (piece.GetPosition() - 16)/8 || (newPosition + 1)/8 == (piece.GetPosition() - 16)/8)
		{
			return true;
		}
	}

	return false;
}

bool Game::ValidBishopMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack)
{
	bool valid = false;
	int newY = newPosition / 8;
	int oldY = piece.GetPosition() / 8;
	int newX = newPosition % 8;
	int oldX = piece.GetPosition() % 8;
	if (abs(oldX-newX) == abs(oldY-newY))
	{
		bool up = !std::signbit(float(newY-oldY));
		bool right = !std::signbit(float(newX-oldX));
		int direction = (2 * int(up) - 1) * 8 + (2 * int(right) - 1);
		int position = piece.GetPosition() + direction;
		while (position != newPosition)
		{
			for (auto& piece : *pBlack)
			{
				if (piece.GetPosition() == position)
				{
					//blocked
					return false;
				}
			}
			for (auto& piece : *pWhite)
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

bool Game::ValidQueenMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack)
{
	return ValidBishopMove(newPosition,piece,pWhite,pBlack) || ValidRookMove(newPosition,piece, pWhite, pBlack);
}

bool Game::ValidKingMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack)
{
	bool valid = false;
	if (newPosition + 1 == piece.GetPosition() || piece.GetPosition() == newPosition - 1)
	{
		valid = true;
	}
	else if (newPosition + 1 == piece.GetPosition()+8 || piece.GetPosition()+8 == newPosition - 1 || piece.GetPosition() + 8 == newPosition)
	{
		valid = true;
	}
	else if (newPosition + 1 == piece.GetPosition()-8 || piece.GetPosition()-8 == newPosition - 1 || piece.GetPosition() - 8 == newPosition)
	{
		valid = true;
	}

	if (piece.GetHasNotMoved() && !valid)
	{
		valid = CheckForCastling(newPosition,piece,pWhite,pBlack);
	}
	
	//not in danger walking

	return valid;
}

bool Game::KingUnderAttack(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack, int newKingPosition)
{
	bool valid = true;
	if (!bool(piece.GetColor()))
	{
		int kingPosition = newKingPosition;
		if (kingPosition == -100)
		{
			for (auto& whitePiece : *pWhite)
			{
				if (whitePiece.GetPiece() == Piece::king)
				{
					kingPosition = whitePiece.GetPosition();
					break;
				}
			}
		}
		for (auto& blackPiece : *pBlack)
		{
			if (IsValidMoveSwitch(kingPosition, blackPiece,pWhite,pBlack))
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
			for (auto& blackPiece : *pBlack)
			{
				if (blackPiece.GetPiece() == Piece::king)
				{
					kingPosition = blackPiece.GetPosition();
					break;
				}
			}
		}
		for (auto& whitePiece : *pWhite)
		{
			if (IsValidMoveSwitch(kingPosition, whitePiece,pWhite,pBlack))
			{
				valid = false;
				break;
			}
		}
	}
	return valid;
}

bool Game::CheckForCastling(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack)
{
	if (!bool(piece.GetColor()))
	{
		if (newPosition == 6)
		{
			for (auto& rook : *pWhite)
			{
				if (rook.GetPosition() == 7 && rook.GetHasNotMoved())
				{
					if (IsValidMove(5,rook,pWhite,pBlack) && KingUnderAttack(newPosition - 1,piece,pWhite,pBlack, newPosition - 1))
					{
						//castle
						rook.SetPosition(5);
						return true;
					}
				}
			}
		}
		else if (newPosition == 2)
		{
			for (auto& rook : *pWhite)
			{
				if (rook.GetPosition() == 0 && rook.GetHasNotMoved())
				{
					if (IsValidMove(3, rook, pWhite, pBlack) && KingUnderAttack(newPosition + 1, piece, pWhite, pBlack, newPosition + 1))
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
		if (newPosition == 62)
		{
			for (auto& rook : *pBlack)
			{
				if (rook.GetPosition() == 63 && rook.GetHasNotMoved())
				{
					if (IsValidMove(61, rook, pWhite, pBlack) && KingUnderAttack(newPosition - 1, piece, pWhite, pBlack, newPosition - 1))
					{
						//castle
						rook.SetPosition(61);
						return true;
					}
				}
			}
		}
		else if (newPosition == 58)
		{
			for (auto& rook : *pBlack)
			{
				if (rook.GetPosition() == 56 && rook.GetHasNotMoved())
				{
					if (IsValidMove(59, rook, pWhite, pBlack) && KingUnderAttack(newPosition + 1, piece, pWhite, pBlack, newPosition + 1))
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
