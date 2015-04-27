#ifndef HALL_OF_FAME_H
#define HALL_OF_FAME_H

#include <ga/ga.h>
#include <ga/GAGenome.h>
#include "Definitions.h"
#include <fstream>
using namespace std;

namespace AI
{
	class HallOfFame
	{
	public:
		class HallIter
		{
		public:
			HallIter(GAGenome **members, int* membersGen, int first, int last) { this->members = members; this->membersGen = membersGen; this->first = first; this->last = last; this->current = -1; }

			HallIter(const HallIter& it) { this->members = it.members; this->membersGen = it.membersGen; this->first = it.first; this->last = it.last; this->current = it.current; }

			HallIter& operator=(const HallIter& it) { this->members = it.members; this->membersGen = it.membersGen; this->first = it.first; this->last = it.last; this->current = it.current; return *this; }

			~HallIter() {}

			inline GAGenome* Next() 
			{
				int lastIndex = last - 1;
				if (lastIndex == -1) lastIndex = HALL_SIZE;

				if (current == lastIndex)
					return NULL;

				if (current == -1)
					current = first;
				else
					current = (current + 1) % (HALL_SIZE + 1);

				return members[current];
			}

			inline GAGenome* Current() { if (current != -1) return members[current]; return NULL; }

			inline GAGenome* Prev() 
			{ 
				if (current == first || current == -1) 
					return NULL;

				current--;
				if (current == -1) current = HALL_SIZE;

				return members[current];
			}

			inline int CurrentGen() const { if (current != -1) return membersGen[current]; return -1; }

			inline void Reset() { current = -1; }

		protected:			
			int first;
			int last;
			int current;
			GAGenome **members;
			int* membersGen;
		};

		typedef int (*HallReader)(ifstream&, HallOfFame&);

		static int RealGenomeHallReader(ifstream& is, HallOfFame& hall);
	public:
		HallOfFame();

		HallOfFame(const HallOfFame& hall);

		~HallOfFame();

		inline int Size() const { return (last >= first) ? (last - first) : ((HALL_SIZE + 1) - (first - last)); }

		virtual void Insert(const GAGenome& genome, int nGen);

		// Se debe llamar a este método antes de llamar a Select() por primera vez
		void PrepareForSelection();

		// Selecciona al azar un individuo
		virtual GAGenome* Select();

		// Retorna true si el genoma está en el salón de la fama (genome.comparator() no debe ser NULL)
		virtual GABoolean Contains(const GAGenome& genome);

		virtual void Write(ofstream& os);

		virtual int Read(ifstream& is) { return (*reader)(is, *this); }

		// Setea la función que se usará para leer de un archivo
		inline void Reader(HallReader reader) { this->reader = reader; }

		inline HallIter Iterator() const { return HallIter(members, membersGen, first, last); }

	protected:
		GAGenome** members;	// Individuos del salón de la fama
		int* membersGen; // Indica la generación en la que surgió el individuo como mejor
		int first;  // Apunta al primer miembro, es decir, al más antiguo
		int last;	// Apunta uno después del último miembro

		/** Variables auxiliares para seleccionar individuos para la población sample **/
		GAGenome** notSelected; // Individuos no seleccionados aún
		int nSelFirst, nSelLast;
		int nSelSize; // Cantidad de individuos no seleccionados aún

		// Puntero a la función que se llamará en Read
		HallReader reader;
	};
}

#endif
