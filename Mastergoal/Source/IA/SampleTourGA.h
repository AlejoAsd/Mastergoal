#ifndef COEVOLUTIONAY_GA_H
#define COEVOLUTIONAY_GA_H

#include <ga/ga.h>
#include <ga/GASimpleGA.h>
#include "Definitions.h"
#include "HallOfFame.h"
#include "History.h"
#include <time.h>

namespace AI
{
	class SampleTourGA : public GASimpleGA
	{
	public:
		GADefineIdentity("SampleTourGA", GAID_SAMPLE_TOUR_GA);

		SampleTourGA(const GAGenome& c);

		SampleTourGA(const GAPopulation& p);

		SampleTourGA(const SampleTourGA& ga);

		virtual ~SampleTourGA();

		virtual void initialize(unsigned int seed = 0);		

		virtual void step();

		SampleTourGA & operator++() { step(); return *this; }		

		inline const HallOfFame& Hall() const { return hall; }

		inline const History& Historial() const { return history; }

		inline const GAPopulation& Sample() const { return *sample; }

		inline int SampleSize(int size) { this->oldSample->size(size); return this->sample->size(size); }

	protected:
		GAPopulation *sample; // Individuos seleccionados para el torneo
		GAPopulation *oldSample;

		HallOfFame hall; // Mejores individuos de toda la historia

		History history; // Historial de los mejores de cada generación

		int INITIAL_GENERATION; // Generación inicial (cuando se hace un recover)

		void SelectSample();

		// Se encarga de generar la nueva población usando elitismo, cruzamiento y mutación
		void GenerateNewPop();

		// Guarda el estado del AG por si se cierra repentinamente el programa
		void Save();

		// Carga el estado de un AG guardado (retorna true si se cargó con éxito)
		GABoolean Load();

		static void EvaluatePop(GAPopulation &pop);

		static float EvaluateInd(GAGenome& g);

		/** Auxiliares para guardar los tiempos **/
		clock_t startTime, endTime;

		// Tiempos de cada generación
		double stepTimes[N_GENERATIONS];

		void InitTimeStats();

		void EndTimeStats();

		void Backup();

		static int nTies;
	};		
}

#endif
