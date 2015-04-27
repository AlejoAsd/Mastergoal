#ifndef ELIMINATORY_TOURNAMENT_H
#define ELIMINATORY_TOURNAMENT_H

#include <GA/GAPopulation.h>
#include <GA/GAList.h>
#include <GA/GAGenome.h>
#include "Tournament.h"

namespace AI
{
	class EliminatoryTournament : public Tournament
	{
	public:
		EliminatoryTournament(GAPopulation* population);

		virtual ~EliminatoryTournament();

		void Play();

	private:
		// Participantes de la rueda de ganadores
		GAList<GAGenome*> winners;

		// Participantes de la rueda de perdedores
		GAList<GAGenome*> losers;
	
		int WinnersRound();

		int LosersRound();

		void Final();
	};
}

#endif
