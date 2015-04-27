#ifndef ENCODER_H
#define ENCODER_H
#include "ComParams.h"
#include "ga/GAPopulation.h"
#include "ga/GARealGenome.h"

namespace GAParallel
{
	int CreateTask(int &iIni, int doubleMatch, GAPopulation &pop, GAPopulation &samp,  char *dest, int destSize);

	int EncodeWeights(float* w, int wSize, char *dest, int destSize);
}

#endif
