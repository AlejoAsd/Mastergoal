#ifndef SAMPLE_TOURNAMENT_H
#define SAMPLE_TOURNAMENT_H

#include <ga/GAPopulation.h>
#include "Tournament.h"
#include "SampleTourData.h"

namespace AI
{
	class SampleTournament : public Tournament
	{
	public:
		SampleTournament(GAPopulation *pop, GAPopulation *sample);

		SampleTournament(const SampleTournament& tournament);

		virtual ~SampleTournament();

		virtual void Play();

		virtual void Play(int &nTies);

	protected:
		GAPopulation *pop;
		GAPopulation *sample;		
	};
}

#endif
