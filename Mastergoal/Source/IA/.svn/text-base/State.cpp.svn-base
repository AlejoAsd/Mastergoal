#include "stdafx.h"
#include "State.h"
#include "Board.h"
#include "Player.h"
#include "Pieces.h"

namespace Common
{
	State::State(int level, ORIENTATION turn, int nKicks, int goalsRightTeam, int goalsLeftTeam)
	{
		this->level = level;
		this->board = new Board(level);
		this->turn = turn;
		this->playerWithBall = NULL;
		this->firstPly = true;
		this->goalsRightTeam = goalsRightTeam;
		this->goalsLeftTeam = goalsLeftTeam;
		this->playerWithBall = NULL;
		this->nKicks = nKicks;
	}

	State::State(const Board& board, ORIENTATION turn, int nKicks, bool firstPly, const Player* playerWithBall, int goalsRightTeam, int goalsLeftTeam)
	{
		this->level = 1;
		if (board.GetNPieces() == 2)
			this->level = 2;
		else if (board.GetNPieces() == 5)
			this->level = 3;
		else if (board.GetNPieces() == 3)
			this->level = 4;

		this->board = new Board(board);
		if (playerWithBall)
			this->playerWithBall = new Player(*playerWithBall);
		else
			this->playerWithBall = NULL;
		this->turn = turn;
		this->firstPly = firstPly;
		this->goalsRightTeam = goalsRightTeam;
		this->goalsLeftTeam = goalsLeftTeam;
		this->nKicks = nKicks;
	}

	State::State(const State& state)
	{
		this->level = state.level;
		this->board = new Board(*state.board);
		this->turn = state.turn;
		if (state.playerWithBall)
			this->playerWithBall = new Player(*state.playerWithBall);
		else
			this->playerWithBall = NULL;
		this->firstPly = state.firstPly;
		this->goalsRightTeam = state.goalsRightTeam;
		this->goalsLeftTeam = state.goalsLeftTeam;
		this->nKicks = state.nKicks;
	}

	State::~State()
	{
		if (board)
		{
			delete board;
			board = NULL;
		}
		if (playerWithBall)
		{
			delete playerWithBall;
			playerWithBall = NULL;
		}
	}

	void State::SetPlayerWithBall(const Player* player)
	{
		if (player!= NULL)
		{
			if (this->playerWithBall == NULL)
			{
				this->playerWithBall = new Player(*player);
			}
			else
			{
				this->playerWithBall->SetId(player->GetId());
				this->playerWithBall->SetOrientation(player->GetOrientation());
			}
		}
		else
		{
			if (this->playerWithBall)
			{
				delete this->playerWithBall;
				this->playerWithBall = NULL;
			}
			this->playerWithBall = NULL;
		}
	}

	void State::DoMove(const PlayerMovement* movement)
	{
		this->board->SetPlayerPosition(movement->GetPiece()->GetOrientation(), movement->GetPiece()->GetId(), movement->GetSquareAfter()->GetRow(), movement->GetSquareAfter()->GetCol());

		// Si el jugador llega a la pelota queda en posesión
		if (movement->GetSquareAfter()->Adjacent(*this->board->GetBall()->GetPosition()))
			SetPlayerWithBall(movement->GetPiece());
	}

	void State::UndoMove(const PlayerMovement* movement)
	{
		this->board->SetPlayerPosition(movement->GetPiece()->GetOrientation(), movement->GetPiece()->GetId(), movement->GetSquareBefore()->GetRow(), movement->GetSquareBefore()->GetCol());

		SetPlayerWithBall(NULL);
	}

	void State::DoMove(const BallMovement* movement, const Player* toWhom)
	{
		this->board->SetBallPosition(movement->GetSquareAfter()->GetRow(), movement->GetSquareAfter()->GetCol());

		// A quien pasa el balón (o NULL)
		SetPlayerWithBall(toWhom);
	}

	void State::UndoMove(const BallMovement* movement)
	{
		this->board->SetBallPosition(movement->GetSquareBefore()->GetRow(), movement->GetSquareBefore()->GetCol());		

		SetPlayerWithBall(movement->GetPlayerWhoMoves());
	}

	void State::DoPlay(const Common::Play& play)
	{
		MovementIterator it = play.GetMovementsIterator();
		
		DoMove(it.FirstMovement());

		const BallMovement* ballMovement = it.LastBallMovement();
		if (ballMovement)
		{
			DoMove(ballMovement, it.NextPlayerWithBall());
		}
	}

	void State::UndoPlay(const Common::Play &play)
	{
		MovementIterator it = play.GetMovementsIterator();

		const BallMovement* ballMovement = it.LastBallMovement();
		while (ballMovement)
		{
			UndoMove(ballMovement);
			ballMovement = it.PrevBallMovement();
		}
		UndoMove(it.FirstMovement());	
		// Se cambia el turno
		//this->turn = (this->turn == LEFT) ? RIGHT : LEFT;
	}
}
