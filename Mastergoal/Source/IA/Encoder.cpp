#include "stdafx.h"
#include "Encoder.h"
#include <string.h>
#include <stdlib.h>

namespace GAParallel
{
	int EncodeWeights(float* w, int wSize, char *dest, int destSize)
	{
		char aux[20];
		dest[0] = '\0';
		aux[0] = '\0';
		int i;
		for (i = 0; i < wSize - 1; i++)
		{
			sprintf_s(aux, 20, "%f", w[i]);
			strcat_s(dest, destSize, aux);
			strcat_s(dest, destSize, ",");
		}
		sprintf_s(aux, 20, "%f", w[i]);
		strcat_s(dest, destSize, aux);
		strcat_s(dest, destSize, ";");

		return 0;
	}

	int CreateTask(int &iIni, int doubleMatch, GAPopulation &pop, GAPopulation &samp,  char *dest, int destSize)
	{
		int nInd = 0;
		dest[0] = '\0';
		char aux[300];
		aux[0] = '\0';
		char tempDest[MAX_MSG];
		tempDest[0] = '\0';

		float weights[30];
		int nWeights = GetNFeatures();

		for (int i = iIni; i < pop.size(); i++)
		{
			// Si ya se agregaron los N_HOSTS_TO_SEND individuos
			if (nInd >= N_HOSTS_TO_SEND) 
				break;

			for (int k = 0; k < nWeights; k++)
			{
				weights[k] = ((GARealGenome&)(pop.individual(i))).gene(k);
			}
			// Se codifica el huésped y agrega al mensaje
			EncodeWeights(weights, nWeights, aux, 300);
			strcat_s(tempDest, MAX_MSG, aux);
			nInd++;
		}

		for (int j = MINI_SAMP_SIZE; j < samp.size(); j++)
		{
			for (int k = 0; k < nWeights; k++)
			{
				weights[k] = ((GARealGenome&)(samp.individual(j))).gene(k);
			}
			// Se codifica el parásito y agrega al mensaje
			EncodeWeights(weights, nWeights, aux, 300);
			strcat_s(tempDest, MAX_MSG, aux);
		}

		// Ahora se agrega el header
		_itoa_s(iIni, aux, 300, 10);
		strcat_s(aux, 300, ";");
		strcat_s(dest, destSize, aux);

		_itoa_s(nInd, aux, 300, 10);
		strcat_s(aux, 300, ";");
		strcat_s(dest, destSize, aux);

		_itoa_s(samp.size(), aux, 300, 10);
		strcat_s(aux, 300, ";");
		strcat_s(dest, destSize, aux);

		_itoa_s(doubleMatch, aux, 300, 10);
		strcat_s(aux, 300, ";");
		strcat_s(dest, destSize, aux);

		strcat_s(dest, destSize, tempDest);

		iIni += nInd;

		return 0;
	}
}
