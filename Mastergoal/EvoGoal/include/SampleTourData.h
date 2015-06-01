#ifndef SAMPLE_TOUR_DATA_H
#define SAMPLE_TOUR_DATA_H

#include <ga/GAEvalData.h>
#include <ga/GAGenome.h>
#include "Definitions.h"

namespace AI
{
	class SampleTourData : public GAEvalData
	{
	public:
		class DataIter
		{
		public:
			DataIter(GAGenome **data, int size) { this->data = data; this->size = size; this->index = -1; }

			DataIter(const DataIter& it) { this->data = it.data; this->size = it.size; this->index = it.index; }

			DataIter& operator=(const DataIter& it) { this->data = it.data; this->size = it.size; this->index = it.index; return *this; }

			~DataIter() {}

			inline GAGenome* Next() { if (index < size - 1) return data[++index]; return NULL; }

			inline GAGenome* Current() { if (index > -1) return data[index]; return NULL; }

			inline GAGenome* Prev() { if (index > 0) return data[--index]; return NULL; }

			inline void Reset() { index = -1; }

		protected:
			int index;
			int size;
			GAGenome **data;
		};

		SampleTourData(int size);

		SampleTourData(const SampleTourData& data);

		virtual ~SampleTourData();

		SampleTourData& operator=(const SampleTourData& data);

		virtual SampleTourData* clone() const;

		virtual void copy(const GAEvalData& data);

		DataIter BeatsIter() const { return DataIter(beats, nBeats); }

		DataIter DrawsIter() const { return DataIter(draws, nDraws); }

		DataIter LosesIter() const { return DataIter(loses, nLoses); }

		inline void Reset() 
		{ 
			for (int i = 0; i < size; i++)
			{
				beats[i] = NULL;
				draws[i] = NULL;
				loses[i] = NULL;
			}
			nBeats = nDraws = nLoses = 0; 
		}

		inline GAGenome* Beats(GAGenome* individual) { return beats[nBeats++] = individual; }

		inline GAGenome* Draws(GAGenome* individual) { return draws[nDraws++] = individual; }

		inline GAGenome* Loses(GAGenome* individual) { return loses[nLoses++] = individual; }

		inline int NBeats() const { return nBeats; }

		inline int NDraws() const { return nDraws; }

		inline int NLoses() const { return nLoses; }

	protected:
		// Punteros a los parásitos que vence o empata un huésped
		// Tendrán una dimensión de TOUR_DATA_MULT * SAMP_SIZE (ver constructor) pues cada huésped jugará dos partidos
		// contra cada parásito (uno comenzando él y el otro comenzando el parásito)
		GAGenome **beats;
		GAGenome **draws;
		GAGenome **loses;
		int size;

		int nBeats;
		int nDraws;
		int nLoses;
	};	
}

#endif
