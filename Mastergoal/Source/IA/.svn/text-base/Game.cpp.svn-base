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
	int Game::DoPlay()
	{
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
		if (this->nPlays >= MAX_PLAYS)
			return 0;

		// Continúa el partido
		return 1;
	}

	void Game::DoGame()
	{
		this->currentState->SetFirstPly(false);
		while(DoPlay());
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
