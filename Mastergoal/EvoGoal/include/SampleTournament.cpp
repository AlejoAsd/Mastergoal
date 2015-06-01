#include "stdafx.h"
#include "SampleTournament.h"
#include "Game.h"

namespace AI
{
	SampleTournament::SampleTournament(GAPopulation *pop, GAPopulation *sample) : Tournament()
	{
		this->pop = pop;
		this->sample = sample;

		this->agentL = new Agent(GAME_LEVEL, LEFT, "agentL");
		this->agentR = new Agent(GAME_LEVEL, RIGHT, "agentR");

		this->aGenomeSetter = RealAgentSetter;
	}

	SampleTournament::SampleTournament(const SampleTournament &tournament) : Tournament()
	{
		this->pop = tournament.pop;
		this->sample = tournament.sample;

		this->agentL = new Agent(GAME_LEVEL, LEFT, "agentL");
		this->agentR = new Agent(GAME_LEVEL, RIGHT, "agentR");

		this->aGenomeSetter = tournament.aGenomeSetter;
	}

	SampleTournament::~SampleTournament()
	{
		if (agentL) { delete agentL; agentL = NULL; }
		if (agentR) { delete agentR; agentR = NULL; }
	}

	void SampleTournament::Play()
	{
		// Se inicializa el score de la muestra
		// El score contendrá la cantidad de puntos ganados en los partidos (WIN_SCORE pts por cada victoria, DRAW_SCORE pts por cada empate)
		// j comienza en MINI_SAMP_SIZE porque los individuos ya compitieron contra los anteriores a MINI_SAMP_SIZE en BroodCrossover
		SampleTourData* parasiteData;
		for (int j = MINI_SAMP_SIZE; j < SAMP_SIZE; j++)
		{
			//TODO: borrar: sample->individual(j).score(0.0f);
			parasiteData = (SampleTourData*)sample->individual(j).evalData();
			parasiteData->Reset();
		}

		for (int i = 0; i < pop->size(); i++)
		{
			// Se inicializa el score del individuo
			GAGenome& host = pop->individual(i);
			SampleTourData* hostData = (SampleTourData*)host.evalData();
			// Se resetean los datos de a quiénes le gana el host
			hostData->Reset();
			//TODO: borrar: host.score(0.0f);

			SetAgentGenome(agentL, &host);

			for (int j = MINI_SAMP_SIZE; j < SAMP_SIZE; j++)
			{
				GAGenome& parasite = sample->individual(j);
				parasiteData = (SampleTourData*)parasite.evalData();
				SetAgentGenome(agentR, &parasite);

#ifdef DOUBLE_MATCH
				Common::ORIENTATION starts = LEFT;
#else
				// Se sortea quién comienza
				Common::ORIENTATION starts = GARandomBit() ? LEFT : RIGHT;
#endif

				// Se juega el primer partido 
				Game game1(GAME_LEVEL, starts, agentL, agentR);
				game1.DoGame();

				// Si ganó el host
				if (game1.GetWinner() == agentL)
				{
					//TODO: borrar: host.score(host.score() + WIN_SCORE);
					//TODO: borrar: parasite.score(parasite.score() + WIN_SCORE);
					// Se guarda que host vence a parasite
					hostData->Beats(&parasite);
					// Se guarda que parasite es vencido por host
					parasiteData->Loses(&host);
				}
				// Empate
				else if (game1.GetWinner() == NULL)
				{
					//TODO: borrar: host.score(host.score() + DRAW_SCORE);
					//TODO: borrar: parasite.score(parasite.score() + WIN_SCORE);
					// Se guarda que host empata con parasite
					hostData->Draws(&parasite);
					// Se guarda que parasite empata con host
					parasiteData->Draws(&host);
				}
				else
				{
					parasiteData->Beats(&host);
					hostData->Loses(&parasite);
				}

#ifdef DOUBLE_MATCH
				// Segundo juego, empieza RIGHT
				starts = RIGHT;
				Game game2(GAME_LEVEL, starts, agentL, agentR);
				game2.DoGame();

				// Si ganó el host
				if (game2.GetWinner() == agentL)
				{
					//TODO: borrar: host.score(host.score() + WIN_SCORE);
					//TODO: borrar: parasite.score(parasite.score() + WIN_SCORE);
					// Se guarda que host vence a parasite
					hostData->Beats(&parasite);
					// Se guarda que parasite es vencido por host
					parasiteData->Loses(&host);
				}
				// Empate
				else if (game2.GetWinner() == NULL)
				{
					//TODO: borrar: host.score(host.score() + DRAW_SCORE);
					//TODO: borrar: parasite.score(parasite.score() + WIN_SCORE);
					// Se guarda que host empata con parasite
					hostData->Draws(&parasite);
					// Se guarda que parasite empata con host
					parasiteData->Draws(&host);
				}
				else
				{
					parasiteData->Beats(&host);
					hostData->Loses(&parasite);
				}
#endif
			}
		}
	}	

	void SampleTournament::Play(int &nTies)
	{
		nTies = 0;
		// Se inicializa el score de la muestra
		// El score contendrá la cantidad de puntos ganados en los partidos (WIN_SCORE pts por cada victoria, DRAW_SCORE pts por cada empate)
		// j comienza en MINI_SAMP_SIZE porque los individuos ya compitieron contra los anteriores a MINI_SAMP_SIZE en BroodCrossover
		SampleTourData* parasiteData;
		for (int j = MINI_SAMP_SIZE; j < SAMP_SIZE; j++)
		{
			//TODO: borrar: sample->individual(j).score(0.0f);
			parasiteData = (SampleTourData*)sample->individual(j).evalData();
			parasiteData->Reset();
		}

		for (int i = 0; i < pop->size(); i++)
		{
			// Se inicializa el score del individuo
			GAGenome& host = pop->individual(i);
			SampleTourData* hostData = (SampleTourData*)host.evalData();
			// Se resetean los datos de a quiénes le gana el host
			hostData->Reset();
			//TODO: borrar: host.score(0.0f);

			SetAgentGenome(agentL, &host);

			for (int j = MINI_SAMP_SIZE; j < SAMP_SIZE; j++)
			{
				GAGenome& parasite = sample->individual(j);
				parasiteData = (SampleTourData*)parasite.evalData();
				SetAgentGenome(agentR, &parasite);

#ifdef DOUBLE_MATCH
				Common::ORIENTATION starts = LEFT;
#else
				// Se sortea quién comienza
				Common::ORIENTATION starts = GARandomBit() ? LEFT : RIGHT;
#endif

				// Se juega el primer partido 
				Game game1(GAME_LEVEL, starts, agentL, agentR);
				game1.DoGame();

				// Si ganó el host
				if (game1.GetWinner() == agentL)
				{
					//TODO: borrar: host.score(host.score() + WIN_SCORE);
					//TODO: borrar: parasite.score(parasite.score() + WIN_SCORE);
					// Se guarda que host vence a parasite
					hostData->Beats(&parasite);
					// Se guarda que parasite es vencido por host
					parasiteData->Loses(&host);
				}
				// Empate
				else if (game1.GetWinner() == NULL)
				{
					//TODO: borrar: host.score(host.score() + DRAW_SCORE);
					//TODO: borrar: parasite.score(parasite.score() + WIN_SCORE);
					// Se guarda que host empata con parasite
					hostData->Draws(&parasite);
					// Se guarda que parasite empata con host
					parasiteData->Draws(&host);
					nTies++;
				}
				else
				{
					parasiteData->Beats(&host);
					hostData->Loses(&parasite);
				}

#ifdef DOUBLE_MATCH
				// Segundo juego, empieza RIGHT
				starts = RIGHT;
				Game game2(GAME_LEVEL, starts, agentL, agentR);
				game2.DoGame();

				// Si ganó el host
				if (game2.GetWinner() == agentL)
				{
					//TODO: borrar: host.score(host.score() + WIN_SCORE);
					//TODO: borrar: parasite.score(parasite.score() + WIN_SCORE);
					// Se guarda que host vence a parasite
					hostData->Beats(&parasite);
					// Se guarda que parasite es vencido por host
					parasiteData->Loses(&host);
				}
				// Empate
				else if (game2.GetWinner() == NULL)
				{
					//TODO: borrar: host.score(host.score() + DRAW_SCORE);
					//TODO: borrar: parasite.score(parasite.score() + WIN_SCORE);
					// Se guarda que host empata con parasite
					hostData->Draws(&parasite);
					// Se guarda que parasite empata con host
					parasiteData->Draws(&host);
					nTies++;
				}
				else
				{
					parasiteData->Beats(&host);
					hostData->Loses(&parasite);
				}
#endif
			}
		}
	}	
}
