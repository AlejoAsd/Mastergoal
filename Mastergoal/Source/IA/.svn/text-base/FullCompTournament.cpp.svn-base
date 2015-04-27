#include "stdafx.h"
#include "FullCompTournament.h"
#include "Game.h"

namespace AI
{
	FullCompTournament::FullCompTournament(GAPopulation *pop) : Tournament()
	{
		this->pop = pop;

		this->agentL = new Agent(GAME_LEVEL, LEFT, "agentL");
		this->agentR = new Agent(GAME_LEVEL, RIGHT, "agentR");

		this->aGenomeSetter = RealAgentSetter;
	}

	FullCompTournament::FullCompTournament(const FullCompTournament &tournament) : Tournament()
	{
		this->pop = tournament.pop;

		this->agentL = new Agent(GAME_LEVEL, LEFT, "agentL");
		this->agentR = new Agent(GAME_LEVEL, RIGHT, "agentR");

		this->aGenomeSetter = tournament.aGenomeSetter;
	}

	FullCompTournament::~FullCompTournament()
	{
		if (agentL) { delete agentL; agentL = NULL; }
		if (agentR) { delete agentR; agentR = NULL; }
	}

	void FullCompTournament::Play()
	{
		SampleTourData* player2Data;
		for (int j = 0; j < pop->size(); j++)
		{
			player2Data = (SampleTourData*)pop->individual(j).evalData();
			player2Data->Reset();
		}

		cout << "Individuos rankeados: " << endl;
		for (int i = 0; i < pop->size() - 1; i++)
		{
			// Se inicializa el score del individuo
			GAGenome& player1 = pop->individual(i);
			SampleTourData* player1Data = (SampleTourData*)player1.evalData();

			SetAgentGenome(agentL, &player1);

			for (int j = i + 1; j < pop->size(); j++)
			{
				GAGenome& player2 = pop->individual(j);
				player2Data = (SampleTourData*)player2.evalData();
				SetAgentGenome(agentR, &player2);

#ifdef DOUBLE_MATCH
				Common::ORIENTATION starts = LEFT;
#else
				// Se sortea quién comienza
				Common::ORIENTATION starts = GARandomBit() ? LEFT : RIGHT;
#endif

				// Se juega el primer partido 
				Game game1(GAME_LEVEL, starts, agentL, agentR);
				game1.DoGame();

				// Si ganó el player1
				if (game1.GetWinner() == agentL)
				{
					// Se guarda que player1 vence a player2
					player1Data->Beats(&player2);
					player2Data->Loses(&player1);
				}
				// Si ganó el player2
				else if (game1.GetWinner() == agentR)
				{
					// Se guarda que player2 vence a player1
					player2Data->Beats(&player1);
					player1Data->Loses(&player2);
				}
				// Empate
				else
				{
					// Se guarda que player1 empata con player2
					player1Data->Draws(&player2);
					// Se guarda que player2 empata con player1
					player2Data->Draws(&player1);
				}

#ifdef DOUBLE_MATCH
				// Se juega el segundo partido, empieza RIGHT
				starts = RIGHT;
				Game game2(GAME_LEVEL, starts, agentL, agentR);
				game2.DoGame();

				// Si ganó el player1
				if (game2.GetWinner() == agentL)
				{
					// Se guarda que player1 vence a player2
					player1Data->Beats(&player2);
					player2Data->Loses(&player1);
				}
				// Si ganó el player2
				else if (game2.GetWinner() == agentR)
				{
					// Se guarda que player2 vence a player1
					player2Data->Beats(&player1);
					player1Data->Loses(&player2);
				}
				// Empate
				else
				{
					// Se guarda que player1 empata con player2
					player1Data->Draws(&player2);
					// Se guarda que player2 empata con player1
					player2Data->Draws(&player1);
				}
#endif
			}
			cout << i << " - ";
		}
		cout << endl;
	}
}