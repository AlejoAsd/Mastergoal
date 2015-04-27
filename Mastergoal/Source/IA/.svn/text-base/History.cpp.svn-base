#include "stdafx.h"
#include "History.h"
#include "SampleTourData.h"
#include <GA/GARealGenome.h>

namespace AI
{
	History::History()
	{
		for (int i = 0; i < N_GENERATIONS; i++)
		{
			bests[i] = NULL;
			nWins[i] = 0;
			nDraws[i] = 0;
			nLosses[i] = 0;
			scores[i] = 0.0f;
		}
		nGen = 0;
	}

	History::History(const History& history)
	{
		for (int i = 0; i < N_GENERATIONS; i++)
		{
			this->bests[i] = history.bests[i];
			this->nWins[i] = history.nWins[i];
			this->nDraws[i] = history.nDraws[i];
			this->nLosses[i] = history.nLosses[i];
			this->scores[i] = history.scores[i];
		}
		this->nGen = history.nGen;
	}

	History::~History()
	{
		for (int i = 0; i < N_GENERATIONS; i++)
		{
			if (bests[i])
				delete[] bests[i];
		}
	}

	void History::Add(const GAGenome &genome)
	{
		SampleTourData* data = (SampleTourData*)genome.evalData();
		if (bests[nGen]) delete[] bests[nGen];
		bests[nGen] = genome.clone();
		nWins[nGen] = data->NBeats();
		nDraws[nGen] = data->NDraws();
		nLosses[nGen] = data->NLoses();
		scores[nGen] = genome.score();
		nGen++;
	}

	void History::Write(std::ofstream &os)
	{
		os << nGen << endl;
		for (int i = 0; i < nGen; i++)
		{
			os << i + 1 << endl;
			bests[i]->write(os);
			os << endl << nWins[i] << " " << nDraws[i] << " " << nLosses[i] << " " << scores[i] << endl;
			os << endl;
		}
		os << endl;
	}

	void History::WriteStatsInCSV(std::ofstream &os)
	{
		for (int i = 0; i < nGen; i++)
		{
			os << nWins[i] << ";" << nDraws[i] << ";" << nLosses[i] << ";" << scores[i] << endl;
		}
		os << endl;
	}

	// Al igual que HallOfFame, History debe tener un individuo "auxiliar" previamente insertado
	int History::Read(std::ifstream &is)
	{
		// Lee nGen
		is >> nGen;
		if (is.fail()) return -1;

		GARealGenome* g = (GARealGenome*)bests[0]->clone();

		// Lee cada individuos
		for(int i = 0; i < nGen; i++)
		{
			int aux = 0;
			is >> aux;	// Lee la generación (sería i + 1)

			if(g->read(is) != 0)
				return -1;
			
			if (bests[i]) delete[] bests[i];
			bests[i] = g->clone();
			is >> nWins[i];
			if (is.fail()) return -1;
			is >> nDraws[i];
			if (is.fail()) return -1;
			is >> nLosses[i];
			if (is.fail()) return -1;
			is >> scores[i];
			if (is.fail()) return -1;
			g->score(scores[i]);
		}
		// Se debe cumplir la condición i == nGen al salir
		delete[] g; g = NULL;
		return 0;
	}
}
