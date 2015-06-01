#ifndef EVALUATOR_LEVEL_4_H
#define EVALUATOR_LEVEL_4_H

#include "Evaluator.h"
#include "Definitions.h"

namespace AI
{
	class EvaluatorLevel4 : public Evaluator
	{
	public:
		static const int N_FEATURES = 24;

		static const int BALL_ADVANCE = 0;
		static const int PL_BALL_DIST_1 = 1;
		static const int PL_BALL_DIST_2 = 2;
		static const int PL_BALL_DIST_3 = 3;
		static const int PL_ADVANCE_1 = 4;
		static const int PL_ADVANCE_2 = 5;
		static const int PL_ADVANCE_3 = 6;
		static const int PL_TOO_CLOSE_BORDER = 7;		
		static const int PL_KEEPER_IN_AREA = 8;
		static const int OP_BALL_DIST_1 = 9;
		static const int OP_BALL_DIST_2 = 10;
		static const int OP_BALL_DIST_3 = 11;
		static const int OP_ADVANCE_1 = 12;
		static const int OP_ADVANCE_2 = 13;
		static const int OP_ADVANCE_3 = 14;
		static const int OP_TOO_CLOSE_BORDER = 15;
		static const int OP_KEEPER_IN_AREA = 16;
		static const int CLOSER_TO_BALL = 17;
		static const int NEUTRAL_SQR = 18;
		static const int SPECIAL_SQR = 19;
		static const int PLS_TOO_CLOSE = 20;
		static const int PLS_TOO_FAR = 21;
		static const int OPS_TOO_CLOSE = 22;
		static const int OPS_TOO_FAR = 23;

	public:
		EvaluatorLevel4();

		EvaluatorLevel4(float weights[]);

		// Función de evaluación
		float Evaluate(const State& state);

		// Setea los pesos de la función de evalación
		void SetWeights(const float *weights);

		// Retorna la cantidad de features
		int GetNFeatures() const { return N_FEATURES; }

		int ReadWeights(ifstream &file);

	private:
		float weights[N_FEATURES];
		float features[N_FEATURES];

		void CalculateFeatures(const State& state);
	};
}

#endif
