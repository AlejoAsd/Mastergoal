#include "stdafx.h"
#include "SampleScaling.h"
#include "SampleTourData.h"
#include <ga/GAGenome.h>

namespace AI
{
	void SampleScaling::evaluate(const GAPopulation &p)
	{
		for (int i = 0; i < p.size(); i++)
		{
			float f = 0;
			GAGenome &host = p.individual(i);
			SampleTourData::DataIter winsIt = ((SampleTourData*)host.evalData())->BeatsIter();
			SampleTourData::DataIter drawsIt = ((SampleTourData*)host.evalData())->DrawsIter();

			while (winsIt.Next())
			{
				f += WIN_SCORE / winsIt.Current()->score();
			}

			while (drawsIt.Next())
			{
				f += DRAW_SCORE / drawsIt.Current()->score();
			}
			host.fitness(f);
		}
	}
}
