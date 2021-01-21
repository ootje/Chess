#pragma once
#include <vector>
#include "ChessPiece.h"

class Game
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	std::vector<ChessPiece>* m_pWhitePieces;
	std::vector<ChessPiece>* m_pBlackPieces;
	Texture* m_pPiecesTexture;

	std::vector<Point2f>* m_pSquarePosition;
	float m_SquareWidth;
	float m_SquareHeight;


	bool m_IsWhiteBottom;

	Point2f m_MousePosition;

	bool m_IsWhiteMoving;
	bool m_IsBlackMoving;

	int m_WhiteMovingPiece;
	int m_BlackMovingPiece;

	bool m_IsWhitesMove;

	bool IsValidMove(int newPosition, ChessPiece& piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack) ;
	bool IsValidMoveSwitch(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack) ;

	bool ValidPawnMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack);
	bool ValidRookMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack);
	bool ValidKnightMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack);
	bool ValidBishopMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack);
	bool ValidQueenMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack);
	bool ValidKingMove(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack);

	bool KingUnderAttack(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack, int newKingPosition = -100);
	bool CheckForCastling(int newPosition, ChessPiece piece, std::vector<ChessPiece>* pWhite, std::vector<ChessPiece>* pBlack);

	void MakeMove(int newPosition, ChessPiece& piece);
};