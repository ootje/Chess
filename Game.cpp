#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>

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
	if (!m_IsWhitesMove)
	{
		MiniMax newMiniMax;

		std::vector<ChessPiece>* pWhite = new std::vector<ChessPiece>;
		pWhite->insert(pWhite->end(), m_pWhitePieces->begin(), m_pWhitePieces->end());
		std::vector<ChessPiece>* pBlack = new std::vector<ChessPiece>;
		pBlack->insert(pBlack->end(), m_pBlackPieces->begin(), m_pBlackPieces->end());

		ChessMove move = newMiniMax.CalculateNextMove(*pWhite, *pBlack);
		if (MakeMove(move,*m_pWhitePieces, *m_pBlackPieces))
		{
			m_IsWhitesMove = true;
		}

		if (pWhite != nullptr)
			delete(pWhite);
		if (pBlack != nullptr)
			delete(pBlack);
	}
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
		/*if (m_IsBlackMoving)
		{
			if (m_BlackMovingPiece == i)
			{
				dstRect.left = m_MousePosition.x - m_SquareWidth / 2.f;
				dstRect.bottom = m_MousePosition.y - m_SquareHeight / 2.f;
			}
		}*/
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
		ChessMove move{ newIndex,m_pWhitePieces->at(m_WhiteMovingPiece) };
		if (MakeMove(move, *m_pWhitePieces, *m_pBlackPieces))
		{
			m_IsWhitesMove = false;
			std::cout << newIndex << '\n';
		}

		m_IsWhiteMoving = false;
	}
	/*else if (m_IsBlackMoving)
	{
		int newIndex = int(m_MousePosition.x / m_SquareWidth) + 8 * int(m_MousePosition.y / m_SquareHeight);

		MakeMove(newIndex, m_pBlackPieces->at(m_BlackMovingPiece),m_pBlackPieces,m_pBlackPieces);

		m_IsBlackMoving = false;
	}*/
}

void Game::ClearBackground() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
