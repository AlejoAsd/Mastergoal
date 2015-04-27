#include "stdafx.h"
#include "HallOfFame.h"
#include <ga/GARealGenome.h>
#include "SampleTourData.h"

namespace AI
{
	HallOfFame::HallOfFame()
	{
		members = new GAGenome*[HALL_SIZE + 1];
		membersGen = new int[HALL_SIZE + 1];
		first = last = 0;

		notSelected = new GAGenome*[HALL_SIZE + 1];
		nSelFirst = nSelLast = nSelSize = 0;		

		for (int i = 0; i < HALL_SIZE + 1; i++)
		{
			members[i] = NULL;
			membersGen[i] = -1;
		}
		reader = RealGenomeHallReader;
	}

	HallOfFame::HallOfFame(const HallOfFame& hall)
	{
		this->members = new GAGenome*[HALL_SIZE + 1];
		this->membersGen = new int[HALL_SIZE + 1];
		this->first = hall.first;
		this->last = hall.last;
		this->nSelFirst = hall.nSelFirst;
		this->nSelLast = hall.nSelLast;
		this->nSelSize = hall.nSelSize;
		this->notSelected = new GAGenome*[HALL_SIZE + 1];

		for (int i = 0; i < HALL_SIZE + 1; i++)
		{
			this->members[i] = hall.members[i];
			this->membersGen[i] = hall.membersGen[i];
			if (hall.notSelected[i] != NULL)
				this->notSelected[i] = this->members[i];
			else
				this->notSelected[i] = NULL;
		}
		this->reader = hall.reader;
	}

	HallOfFame::~HallOfFame()
	{
		if (members)
		{
			for (int i = 0; i < HALL_SIZE + 1; i++)
			{
				if (members[i])
				{
					delete members[i];
					members[i] = NULL;
				}
			}
			delete[] members;
			members = NULL;
		}
		if (notSelected)
		{
			delete[] notSelected;
			notSelected = NULL;
		}
		if (membersGen)
		{
			delete[] membersGen;
			membersGen = NULL;
		}
	}

	void HallOfFame::Insert(const GAGenome& genome, int nGen)
	{
		// No se inserta más de una vez un mismo individuo
		if (Contains(genome))
			return;

		// Se inserta
		if (!members[last])
		{
			members[last] = genome.clone();
			membersGen[last] = nGen;
		}
		else
		{
			members[last]->copy(genome);
			membersGen[last] = nGen;
		}

		last = (last + 1) % (HALL_SIZE + 1);
		// Si la lista ya estaba llena
		if (last == first)
			first = (first + 1) % (HALL_SIZE + 1);
	}

	void HallOfFame::PrepareForSelection()
	{
		nSelFirst = first;
		nSelLast = last;
		for (int i = first; i != last; i = (i + 1) % (HALL_SIZE + 1))
		{
			notSelected[i] = members[i];
		}
		// La cantidad de miembros no seleccionados es igual a la cantidad total de miembros
		nSelSize = Size();
	}

	GAGenome* HallOfFame::Select()
	{
		// Si no hay ningún elemento posible a seleccionar (ya fueron todos seleccionados o el salón está vació)
		if (nSelSize < 1)
			return NULL;

		// Hay nSelSize individuos que no fueron seleccionados aún
		// El s-ésimo elemento es el seleccionado
		int s = GARandomInt(1, nSelSize);

		/** Se debe buscar el s-ésimo elemento distinto de NULL en notSelected **/
		int c = 0; // contador de elementos no nulos (elementos encontrados)
		int i; // Índice de los elementos en notSelected
		for (i = nSelFirst; i != nSelLast; i = (i + 1) % (HALL_SIZE + 1))
		{
			if (notSelected[i] != NULL)
			{
				c++; // Se encontró el c-ésimo (luego de hacer el ++) elemento distinto de NULL
				if (c == s)
					break; // ahora i es el índice  del s-ésimo elemento (el que debe ser retornado)
			}
		}
		GAGenome *genome = notSelected[i];
		// Se quita al genoma de la lista de no seleccionados
		notSelected[i] = NULL;
		// La cantidad de elementos aún no seleccionados disminuye en uno
		nSelSize--;
		return genome;
	}

	GABoolean HallOfFame::Contains(const GAGenome& genome)
	{
		for (int i = first; i != last; i = (i + 1) % (HALL_SIZE + 1))
		{
			if (members[i]->compare(genome) == 0.0f)
				return gaTrue;
		}
		return gaFalse;
	}

	void HallOfFame::Write(std::ofstream &os)
	{
		os << first << endl << last << endl;
		for (int i = first; i != last; i = (i + 1) % (HALL_SIZE + 1))
		{
			os << membersGen[i] << endl;
			members[i]->write(os);
			// TODO: cuidado si cambio de torneo
			SampleTourData* data = (SampleTourData*)members[i]->evalData();			
			os << endl << data->NBeats() << " " << data->NDraws() << " " << data->NLoses() << " " << members[i]->score() << endl;
			os << endl;
		}
		os << endl;
	}

	int HallOfFame::RealGenomeHallReader(std::ifstream &is, HallOfFame& hall)
	{
		int f = 0;
		int l = 0;

		if (is.fail()) return -1;
		// Lee el índice inferior
		is >> f;
		if (is.fail()) return -1;
		// Lee el índice superior
		is >> l;
		if (is.fail()) return -1;
		
		// TODO: ????
		// El que se insertó afuera
		// Se supone que se insertó un elemento afuera
		int lastEl = hall.last - 1;
		if (lastEl < 0) lastEl = HALL_SIZE;
		GARealGenome* g = (GARealGenome*)hall.members[lastEl]->clone();

		hall.first = f;
		hall.last = hall.first;
		// TODO: preguntar
		//GARealAlleleSet alleles(MIN_WEIGHT, MAX_WEIGHT);
		//GARealGenome g(N_FEATURES, alleles);
		
		for (int i = f; i != l; i = (i + 1) % (HALL_SIZE + 1))
		{
			int generation;
			// Lee membersGeneration
			is >> generation;
			if (is.fail()) return -1;

			if (g->read(is) != 0)
				return -1;
			float val;
			// Lee nBeats
			is >> val;
			if (is.fail()) return -1;
			// Lee nDraws
			is >> val;
			if (is.fail()) return -1;
			// Lee nLoses
			is >> val;
			if (is.fail()) return -1;
			// Lee el score
			is >> val;
			if (is.fail()) return -1;
			g->score(val);
			hall.Insert(*g, generation);
		}		
		delete g;
		return 0;
	}
}
