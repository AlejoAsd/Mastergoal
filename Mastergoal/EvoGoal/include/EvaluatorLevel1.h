#ifndef EVALUATOR_LEVEL_1_H
#define EVALUATOR_LEVEL_1_H

#include "Evaluator.h"

namespace AI
{
	class EvaluatorLevel1 : public Evaluator
	{
	public:
		static const int N_FEATURES = 8;

		static const int BALL_ADVANCE = 0;
		static const int PL_BALL_DIST_1 = 1;
		static const int PL_ADVANCE_1 = 2;
		static const int PL_BALL_COL_DIST = 3;
		static const int OP_BALL_DIST_1 = 4;
		static const int OP_ADVANCE_1 = 5;
		static const int OP_BALL_COL_DIST = 6;
		static const int CLOSER_TO_BALL = 7;

	public:
		EvaluatorLevel1();

		EvaluatorLevel1(float weights[]);

		// Función de evaluación
		float Evaluate(const State& state);

		// Setea los pesos de la función de evalación
		void SetWeights(const float *weights);

		// Retorna -1 si hay error
		int ReadWeights(ifstream& file);

		// Retorna la cantidad de features
		int GetNFeatures() const { return N_FEATURES; }

	private:
		float weights[N_FEATURES];
		float features[N_FEATURES];

		void CalculateFeatures(const State& state);
	};
}

#endif
