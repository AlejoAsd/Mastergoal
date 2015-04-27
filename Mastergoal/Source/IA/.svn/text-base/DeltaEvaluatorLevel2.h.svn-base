#ifndef DELTA_EVALUATOR_LEVEL_2_H
#define DELTA_EVALUATOR_LEVEL_2_H

#include "DeltaEvaluator.h"

namespace AI
{
	class DeltaEvaluatorLevel2 : public DeltaEvaluator
	{
	public:
		static const int N_FEATURES = 4;

		static const int MOVED_TOWARDS_BALL = 0; // Si acerco el jugador a la pelota
		static const int KICKED_FORWARD = 1; // Si adelanto la pelota
		static const int KICKED_AWAY_OP = 2; // Si alejo la pelota del oponente
		static const int MOVED_FORWARD = 3; // Si adelanto el jugador
		//static const int MOVED_FORWARD_1 = 3; // Si adelanto el jugador mas adelantado
		//static const int MOVED_FORWARD_2 = 4; // Si adelanto el jugador mas atrasado
		//static const int SHORTENED_DIST_BALL = 5; // Si disminuyo la minima distancia del equipo a la pelota

	public:
		DeltaEvaluatorLevel2();

		DeltaEvaluatorLevel2(float weights[]);

		// Función de evaluación, se asume que se evalua con respecto al turno de before
		float Evaluate(const State& before, const State& after, const Play& play);

		// Setea los pesos de la función de evalación
		void SetWeights(const float *weights);

		// Retorna -1 si hay error
		int ReadWeights(ifstream& file);

		// Retorna la cantidad de features
		int GetNFeatures() const { return N_FEATURES; }

	private:
		float weights[N_FEATURES];
		float features[N_FEATURES];

		void CalculateDeltas(const State& before, const State& after, const Play& play);

		int MinDistToBall(const Board& board, ORIENTATION team);
	};
}

#endif
