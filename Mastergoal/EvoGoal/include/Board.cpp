#include "stdafx.h"
#include "Board.h"
#include "Player.h"
#include "Movements.h"
#include "Play.h"

namespace Common
{
	/*Board::Board(int level, BallPiece* ball, PlayerPiece** leftPieces, PlayerPiece** rightPieces)
	{
		this->level = level;
		this->ball = ball;
		this->leftPieces = leftPieces;
		this->rightPieces = rightPieces;
	}*/

	Board::Board(int level)
	{
		this->nPieces = 1;
		if (level == 2)
			this->nPieces = 2;
		else if (level == 3)
			this->nPieces = 5;
		else if (level == 4)
			this->nPieces = 3;
		Initialize();
	}

	Board::Board(const Common::Board& board)
	{
		this->nPieces = board.nPieces;
		this->ball = new BallPiece(*board.GetBall());

		leftPieces = new PlayerPiece*[this->nPieces];
		rightPieces = new PlayerPiece*[this->nPieces];

		for (int i = 0; i < this->nPieces; i++)
		{
			leftPieces[i] = new PlayerPiece(*board.leftPieces[i]);
			rightPieces[i] = new PlayerPiece(*board.rightPieces[i]);
		}
	}

	Board::~Board()
	{
		if (ball)
		{
			delete ball;
			ball = NULL;
		}

		if (leftPieces)
		{
			for (int i = 0; i < this->nPieces; i++)
			{
				if (leftPieces[i])
				{
					delete leftPieces[i];
					leftPieces[i] = NULL;
				}
			}
			delete leftPieces;
			leftPieces = NULL;
		}

		if (rightPieces)
		{
			for (int i = 0; i < this->nPieces; i++)
			{
				if (rightPieces[i])
				{
					delete rightPieces[i];
					rightPieces[i] = NULL;
				}
			}
			delete rightPieces;
			rightPieces = NULL;
		}
	}

	void Board::Initialize()
	{
		ball = new BallPiece(7, 5);
		if (this->nPieces == 1)
		{
			leftPieces = new PlayerPiece*[1];
			rightPieces = new PlayerPiece*[1];

			leftPieces[0] = new PlayerPiece(LEFT, ID_N, 10, 5);
			rightPieces[0] = new PlayerPiece(RIGHT, ID_N, 4, 5);
		}
		else if (this->nPieces == 2)
		{
			leftPieces = new PlayerPiece*[2];
			rightPieces = new PlayerPiece*[2];

			leftPieces[0] = new PlayerPiece(LEFT, ID_N, 10, 5);
			rightPieces[0] = new PlayerPiece(RIGHT, ID_N, 4, 5);

			leftPieces[1] = new PlayerPiece(LEFT, ID_P, 12, 5);
			rightPieces[1] = new PlayerPiece(RIGHT, ID_P, 2, 5);
		}
		else if (this->nPieces == 5)
		{
			leftPieces = new PlayerPiece*[5];
			rightPieces = new PlayerPiece*[5];

			leftPieces[0] = new PlayerPiece(LEFT, ID_N, 8, 2);
			rightPieces[0] = new PlayerPiece(RIGHT, ID_N, 6, 2);

			leftPieces[1] = new PlayerPiece(LEFT, ID_P, 8, 8);
			rightPieces[1] = new PlayerPiece(RIGHT, ID_P, 6, 8);

			leftPieces[2] = new PlayerPiece(LEFT, ID_L, 10, 3);
			rightPieces[2] = new PlayerPiece(RIGHT, ID_L, 4, 3);

			leftPieces[3] = new PlayerPiece(LEFT, ID_C, 10, 7);
			rightPieces[3] = new PlayerPiece(RIGHT, ID_C, 4, 7);

			leftPieces[4] = new PlayerPiece(LEFT, ID_G, 11, 5);
			rightPieces[4] = new PlayerPiece(RIGHT, ID_G, 3, 5);
		}
		else  // nPieces == 3
		{
			leftPieces = new PlayerPiece*[3];
			rightPieces = new PlayerPiece*[3];

			leftPieces[0] = new PlayerPiece(LEFT, ID_N, 9, 2);
			rightPieces[0] = new PlayerPiece(RIGHT, ID_N, 5, 2);

			leftPieces[1] = new PlayerPiece(LEFT, ID_P, 9, 8);
			rightPieces[1] = new PlayerPiece(RIGHT, ID_P, 5, 8);

			leftPieces[2] = new PlayerPiece(LEFT, ID_G, 11, 5);
			rightPieces[2] = new PlayerPiece(RIGHT, ID_G, 3, 5);
		}
	}

	const PlayerPiece* Board::GetPlayer(const ORIENTATION& orientation, PLAYER_ID id) const
	{
		if (orientation == LEFT)
		{
			for (int i = 0; i < this->nPieces; i++)
			{
				if (leftPieces[i]->GetOrientation() == orientation
					&& leftPieces[i]->GetId() == id)
					return leftPieces[i];
			}
		}
		if (orientation == RIGHT)
		{
			for (int i = 0; i < this->nPieces; i++)
			{
				if (rightPieces[i]->GetOrientation() == orientation
					&& rightPieces[i]->GetId() == id)
					return rightPieces[i];
			}
		}
		return NULL;
	}

	void Board::SetBallPosition(int row, int col)
	{
		ball->SetPosition(row, col);
	}

	void Board::SetPlayerPosition(const ORIENTATION& orientation, PLAYER_ID id, int row, int col)
	{
		if (orientation == LEFT)
		{
			for (int i = 0; i < this->nPieces; i++)
			{
				if (leftPieces[i]->GetOrientation() == orientation
					&& leftPieces[i]->GetId() == id)
				{
					leftPieces[i]->SetPosition(row, col);
					return;
				}
			}
		}
		if (orientation == RIGHT)
		{
			for (int i = 0; i < this->nPieces; i++)
			{
				if (rightPieces[i]->GetOrientation() == orientation
					&& rightPieces[i]->GetId() == id)
				{
					rightPieces[i]->SetPosition(row, col);
					return;
				}
			}
		}
	}

	void Board::SetBallPosition(const Square& position)
	{
		ball->SetPosition(position);
	}

	void Board::SetPlayerPosition(const ORIENTATION& orientation, PLAYER_ID id, const Square& position)
	{
		if (orientation == LEFT)
		{
			for (int i = 0; i < this->nPieces; i++)
			{
				if (leftPieces[i]->GetOrientation() == orientation
					&& leftPieces[i]->GetId() == id)
				{
					leftPieces[i]->SetPosition(position);
					return;
				}
			}
		}
		if (orientation == RIGHT)
		{
			for (int i = 0; i < this->nPieces; i++)
			{
				if (rightPieces[i]->GetOrientation() == orientation
					&& rightPieces[i]->GetId() == id)
				{
					rightPieces[i]->SetPosition(position);
					return;
				}
			}
		}
	}

	void Board::SetPiecePosition(const Piece* piece, int row, int col)
	{
		if (piece->GetType() == TYPE_BALL)
			SetBallPosition(row, col);
		else if (piece->GetType() == TYPE_PLAYER)
		{
			const PlayerPiece* player = (PlayerPiece*)piece;
			ORIENTATION orientation = player->GetOrientation();
			PLAYER_ID id = player->GetId();
			SetPlayerPosition(orientation, id, row, col);
		}
	}

	Board* Board::Clone()
	{
		return new Board(*this);
	}

	// TODO: modificar para que no tenga en cuenta el ID de los jugadores, excepto el arquero
	bool Board::Equals(const Board& board) const
	{
		if (*this->ball->GetPosition() != *board.ball->GetPosition())
			return false;

		for (int i = 0; i < this->nPieces; i++)
		{
			if (*this->leftPieces[i]->GetPosition() != *board.leftPieces[i]->GetPosition())
				return false;
			if (*this->rightPieces[i]->GetPosition() != *board.rightPieces[i]->GetPosition())
				return false;
		}
		return true;
	}
}
