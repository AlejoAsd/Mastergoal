#ifndef EVALUATOR_LEVEL_2_H
#define EVALUATOR_LEVEL_2_H

#include "Evaluator.h"

namespace AI
{
	class EvaluatorLevel2 : public Evaluator
	{
	public:
		static const int N_FEATURES = 17;

		static const int BALL_ADVANCE = 0;
		static const int PL_BALL_DIST_1 = 1;
		static const int PL_BALL_DIST_2 = 2;
		static const int PL_ADVANCE_1 = 3;
		static const int PL_ADVANCE_2 = 4;
		static const int PL_TOO_CLOSE_BORDER = 5;
		static const int OP_BALL_DIST_1 = 6;
		static const int OP_BALL_DIST_2 = 7;
		static const int OP_ADVANCE_1 = 8;
		static const int OP_ADVANCE_2 = 9;
		static const int OP_TOO_CLOSE_BORDER = 10;
		static const int CLOSER_TO_BALL = 11;
		static const int NEUTRAL_SQR = 12;
		static const int PLS_TOO_CLOSE = 13;
		static const int PLS_TOO_FAR = 14;
		static const int OPS_TOO_CLOSE = 15;
		static const int OPS_TOO_FAR = 16;

	public:
		EvaluatorLevel2();

		EvaluatorLevel2(float weights[]);

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
