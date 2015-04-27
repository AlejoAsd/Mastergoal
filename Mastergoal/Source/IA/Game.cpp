#include "stdafx.h"
#include "Game.h"
#include "RulesManager.h"

using namespace Utils;

namespace Common
{
	// El usuario es encargado de hacer delete de los agents
	Game::Game(int level, ORIENTATION start, Agent* agentL, Agent* agentR)
	{
		this->currentState = new State(level, start);
		this->nPlays = 0;
		this->plays = new Play*[MAX_PLAYS];
		this->states = new State*[MAX_PLAYS];
		this->agentL = agentL;
		this->agentR = agentR;

		// Se inicializa las jugadas en plays a NULL
		for (int i = 0; i < MAX_PLAYS; i++)
		{
			this->plays[i] = NULL;
			this->states[i] = NULL;
		}
	}

	Game::~Game()
	{
		if (this->currentState)
		{
			delete this->currentState;
			this->currentState = NULL;
		}
		if (this->plays)
		{
			for (int i = 0; i < this->nPlays; i++)
			{
				delete this->plays[i];
				this->plays[i] = NULL;
			}
			delete[] this->plays;
			this->plays = NULL;
		}
		if (this->states)
		{
			for (int i = 0; i < this->nPlays; i++)
			{
				delete this->states[i];
				this->states[i] = NULL;
			}
			delete[] this->states;
			this->states = NULL;
		}
	}

	// Hace que el agente al que le toca jugar juegue. Retorna 0 cuando termina el partido.
	int Game::DoPlay(int nivel, ficha ball, ficha* left, ficha* right)
	{

		/**********Agrego las posiciones de las fichas al tablero**********/
		//pelota
		this->currentState->board->ball->SetPosition(ball.row, ball.col);

		//ficha 1 (player y pc)
		this->currentState->board->leftPieces[0]->SetPosition(left[0].row, left[0].col);
		this->currentState->board->rightPieces[0]->SetPosition(right[0].row, right[0].col);
		
		//ficha 2 (player y pc)
		if (nivel == 2 || nivel == 3){
			this->currentState->board->leftPieces[1]->SetPosition(left[1].row, left[1].col);
			this->currentState->board->rightPieces[1]->SetPosition(right[1].row, right[1].col);
		}

		//ficha 3, 4 y 5 (player y pc)
		if (nivel == 3){
			this->currentState->board->leftPieces[2]->SetPosition(left[2].row, left[2].col);
			this->currentState->board->rightPieces[2]->SetPosition(right[2].row, right[2].col);

			this->currentState->board->leftPieces[3]->SetPosition(left[3].row, left[3].col);
			this->currentState->board->rightPieces[3]->SetPosition(right[3].row, right[3].col);

			this->currentState->board->leftPieces[4]->SetPosition(left[4].row, left[4].col);
			this->currentState->board->rightPieces[4]->SetPosition(right[4].row, right[4].col);
		}

		/**********Imprimo posiciones antes de realizar la jugada**********/
		printf("Antes del movimiento\n");
		printf("ball: %d, %d\n", this->currentState->GetBoard()->GetBall()->GetRow(), this->currentState->GetBoard()->GetBall()->GetCol());
		printf("leftPiece: %d, %d\n", this->currentState->GetBoard()->GetLeftPieces()[0]->GetRow(), this->currentState->GetBoard()->GetLeftPieces()[0]->GetCol());
		printf("rightPiece: %d, %d\n", this->currentState->GetBoard()->GetRightPieces()[0]->GetRow(), this->currentState->GetBoard()->GetRightPieces()[0]->GetCol());

		if (nivel == 2 || nivel == 3){
			printf("leftPiece1: %d, %d\n", this->currentState->GetBoard()->GetLeftPieces()[1]->GetRow(), this->currentState->GetBoard()->GetLeftPieces()[1]->GetCol());
			printf("rightPiece1: %d, %d\n", this->currentState->GetBoard()->GetRightPieces()[1]->GetRow(), this->currentState->GetBoard()->GetRightPieces()[1]->GetCol());
		}

		if (nivel == 3){
			printf("leftPiece2: %d, %d\n", this->currentState->GetBoard()->GetLeftPieces()[2]->GetRow(), this->currentState->GetBoard()->GetLeftPieces()[2]->GetCol());
			printf("rightPiece2: %d, %d\n", this->currentState->GetBoard()->GetRightPieces()[2]->GetRow(), this->currentState->GetBoard()->GetRightPieces()[2]->GetCol());

			printf("leftPiece3: %d, %d\n", this->currentState->GetBoard()->GetLeftPieces()[3]->GetRow(), this->currentState->GetBoard()->GetLeftPieces()[3]->GetCol());
			printf("rightPiece3: %d, %d\n", this->currentState->GetBoard()->GetRightPieces()[3]->GetRow(), this->currentState->GetBoard()->GetRightPieces()[3]->GetCol());

			printf("leftPiece4: %d, %d\n", this->currentState->GetBoard()->GetLeftPieces()[4]->GetRow(), this->currentState->GetBoard()->GetLeftPieces()[4]->GetCol());
			printf("rightPiece4: %d, %d\n", this->currentState->GetBoard()->GetRightPieces()[4]->GetRow(), this->currentState->GetBoard()->GetRightPieces()[4]->GetCol());
		}
		
		// Si juega el agenteL
		if (this->currentState->GetTurn() == LEFT)
		{
			this->plays[this->nPlays] = this->agentL->MakePlay(*currentState);
		}
		else // si juega el agenteR
		{
			this->plays[this->nPlays] = this->agentR->MakePlay(*currentState);
		}

		// Se agrega (antes de hacer la jugada) el estado a la lista de estados
		this->states[this->nPlays] = new State(*currentState);

		// Si es pelota ahogada, termina el partido
		if (this->plays[this->nPlays] == NULL)
			return 0;

		// Se realiza la jugada
		this->currentState->DoPlay(*this->plays[this->nPlays]);

		// Si hay un ciclo, termina el partido
		if (DetectCycle())
			return 0;

		// Se verifica si debe cambiarse de turno
		if (!RulesManager::IsSpecialSquare(*currentState->GetBoard()->GetBall()->GetPosition(), currentState->GetTurn(), currentState->GetLevel())
			// Si es special square pero nMovements del play es 1, quiere decir que no se
			// chutó la pelota en esa jugada, por lo que no se repite el turno
			|| plays[nPlays]->GetNMovements() == 1)
			currentState->SwitchTurn();

		this->nPlays++;

		// Si se metió gol, termina el juego
		if (this->currentState->GetBoard()->GetBall()->GetRow() == 0)
		{
			this->currentState->SetGoalsLeft(1);
			return 0;
		}
		if (this->currentState->GetBoard()->GetBall()->GetRow() == 14)
		{
			this->currentState->SetGoalsRight(1);
			return 0;
		}

		// Si ya se jugaron MAX_PLAYS jugadas, termina el partido
		//if (this->nPlays >= MAX_PLAYS)
		//	return 0;

		/**********Obtengo las nuevas posiciones de las fichas del nuevo tablero**********/
		//pelota
		this->ball.row = this->currentState->GetBoard()->GetBall()->GetRow();
		this->ball.col = this->currentState->GetBoard()->GetBall()->GetCol();
		
		//ficha 1 (player y pc)
		this->left[0].row = this->currentState->GetBoard()->GetLeftPieces()[0]->GetRow();
		this->left[0].col = this->currentState->GetBoard()->GetLeftPieces()[0]->GetCol();
		this->right[0].row = this->currentState->GetBoard()->GetRightPieces()[0]->GetRow();
		this->right[0].col = this->currentState->GetBoard()->GetRightPieces()[0]->GetCol();
		
		//ficha 2 (player y pc)
		if (nivel == 2 || nivel == 3){
			this->left[1].row = this->currentState->GetBoard()->GetLeftPieces()[1]->GetRow();
			this->left[1].col = this->currentState->GetBoard()->GetLeftPieces()[1]->GetCol();
			this->right[1].row = this->currentState->GetBoard()->GetRightPieces()[1]->GetRow();
			this->right[1].col = this->currentState->GetBoard()->GetRightPieces()[1]->GetCol();
		}

		//fichas 3, 4 y 5 (player y pc)
		if (nivel == 3){
			this->left[2].row = this->currentState->GetBoard()->GetLeftPieces()[2]->GetRow();
			this->left[2].col = this->currentState->GetBoard()->GetLeftPieces()[2]->GetCol();
			this->right[2].row = this->currentState->GetBoard()->GetRightPieces()[2]->GetRow();
			this->right[2].col = this->currentState->GetBoard()->GetRightPieces()[2]->GetCol();

			this->left[3].row = this->currentState->GetBoard()->GetLeftPieces()[3]->GetRow();
			this->left[3].col = this->currentState->GetBoard()->GetLeftPieces()[3]->GetCol();
			this->right[3].row = this->currentState->GetBoard()->GetRightPieces()[3]->GetRow();
			this->right[3].col = this->currentState->GetBoard()->GetRightPieces()[3]->GetCol();

			this->left[4].row = this->currentState->GetBoard()->GetLeftPieces()[4]->GetRow();
			this->left[4].col = this->currentState->GetBoard()->GetLeftPieces()[4]->GetCol();
			this->right[4].row = this->currentState->GetBoard()->GetRightPieces()[4]->GetRow();
			this->right[4].col = this->currentState->GetBoard()->GetRightPieces()[4]->GetCol();
		}

		// Continúa el partido
		return 1;
	}

	void Game::DoGame()
	{
		this->currentState->SetFirstPly(false);
		//DoPlay();
		//while(DoPlay());
	}

	Agent* Game::GetWinner()
	{
		if (this->currentState->GetGoalsLeft() == 1)
		{
			return agentL;
		}
		else if (this->currentState->GetGoalsRight() == 1)
		{
			return agentR;
		}
		return NULL;
	}

	bool Game::DetectCycle()
	{
		int foundPos;
		for (foundPos = nPlays - 4; foundPos >= nPlays / 2; foundPos--)
		{
			if (states[nPlays]->GetTurn() == states[foundPos]->GetTurn() &&
				states[nPlays]->GetBoard()->Equals(*states[foundPos]->GetBoard()))
			{
				break;
			}
		}

		if (foundPos < nPlays / 2) return false;

		for (int i = nPlays - 1, j = foundPos - 1; i > foundPos; i--, j--)
		{
			if (!(states[i]->GetTurn() == states[j]->GetTurn() &&
				states[i]->GetBoard()->Equals(*states[j]->GetBoard())))
				return false;
		}

		return true;
	}
}
