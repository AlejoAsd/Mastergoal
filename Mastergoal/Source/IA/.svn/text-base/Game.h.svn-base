#ifndef GAME_H
#define GAME_H

#include "State.h"
#include "Agent.h"
#include "Definitions.h"

using namespace AI;

namespace Common
{
	class Game
	{
	public:
		Game(int level = GAME_LEVEL, ORIENTATION start = LEFT, Agent* agentL = NULL, Agent* agentR = NULL);

		~Game();

		inline Agent* GetAgentL() const { return this->agentL; }

		inline Agent* GetAgentR() const { return this->agentR; }

		inline void SetAgent1(Agent* agent) { this->agentL = agent; }

		inline void SetAgent2(Agent* agent) { this->agentR = agent; }

		inline const State* GetState() const { return this->currentState; }

		// Hace que el agente al que le toca jugar juegue. Retorna 0 cuando termina el partido.
		int DoPlay();

		// Desarrolla el juego completo, hasta llegar a MAX_PLAYS o que uno de los agentes meta gol
		void DoGame();	

		// Retorna un puntero al vencedor, NULL si hay empate
		Agent* GetWinner();

	private:
		// Estado actual de juego
		State* currentState;
		
		// Vector con las jugadas del partido
		Play** plays;

		// Vector con los estados del partido
		State** states;

		// Contador de la cantidad de jugadas hechas
		int nPlays;

		// Agente blanco. El usuario es encargado de hacer delete de los agents.
		Agent* agentL;

		// Agente rojo. El usuario es encargado de hacer delete de los agents.
		Agent* agentR;

		bool DetectCycle();
	};
}

#endif
