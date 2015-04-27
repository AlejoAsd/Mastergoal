#ifndef HISTORY_H
#define HISTORY_H

#include <ga/ga.h>
#include <ga/GAGenome.h>
#include "Definitions.h"
#include <fstream>

using namespace std;

namespace AI
{
	class History
	{
	public:
		class HistoryIter
		{
		public:
			HistoryIter(GAGenome *const*members, int size) { this->members = members; this->size = size; this->current = -1; }

			HistoryIter(const HistoryIter& it) { this->members = it.members; this->size = it.size; this->current = it.current; }

			HistoryIter& operator=(const HistoryIter& it) { this->members = it.members; this->size = it.size; this->current = it.current; return *this; }

			~HistoryIter() {}

			inline GAGenome* Next() 
			{
				if (current == size - 1)
					return NULL;

				return members[++current];
			}

			inline GAGenome* Current() { if (current != -1) return members[current]; return NULL; }

			inline GAGenome* Prev()
			{
				if (current == 0)
					return NULL;

				if (current == -1) // Si todavía no se inicializó, se iterará en reversa
				{
					current = size - 1; // Se inicializa current
					if (current == -1) // size == 0, History está vacío
						return NULL;
				}
				else
					current--;

				return members[current];
			}

			inline void Reset() { current = -1; }

			// Retorna la generación del individuo actual
			inline int CurrentGen() const { return current + 1; }

		protected:			
			int size;
			int current;
			GAGenome *const*members;
		};

	public:
		History();

		History(const History& hisotry);

		~History();

		void Add(const GAGenome& genome);

		void Write(std::ofstream &os);

		void WriteStatsInCSV(std::ofstream &os);

		int Read(std::ifstream &is);

		inline int Size() const { return nGen; }

		inline HistoryIter Iterator() const { return HistoryIter(&bests[0], nGen); }

	protected:
		GAGenome* bests[N_GENERATIONS];	// Mejores individuos de cada generación
		int nWins[N_GENERATIONS];		// Cantidad de partidas ganadas
		int nDraws[N_GENERATIONS];		// Cantidad de partidas empatadas
		int nLosses[N_GENERATIONS];		// Cantidad de partidas perdidas
		float scores[N_GENERATIONS];
		int nGen;						// Última generación insertada
	};
}

#endif
