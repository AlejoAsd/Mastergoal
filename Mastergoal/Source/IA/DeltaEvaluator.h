#ifndef DELTA_EVALUATOR_H
#define DELTA_EVALUATOR_H

#include "State.h"
#include "Play.h"
#include <iostream>
using namespace std;
using namespace Common;

namespace AI
{
	/** Define la interfaz de la clase que contiene la función de evaluación **/
	class DeltaEvaluator
	{
	public:
		// Función de evaluación
		virtual float Evaluate(const State& before, const State& after, const Play& play) = 0;

		// Setea los pesos de la función de evalación
		virtual void SetWeights(const float *weights) = 0;

		// Devuelve la cantidad de features
		virtual int GetNFeatures() const = 0;

		virtual int ReadWeights(ifstream& file) = 0;

	protected:
		bool IsGoal(const Board& board, const ORIENTATION& o)
		{
			int j = board.GetBall()->GetRow();

			if ((j == 0 && o == LEFT) || (j == 14) && o == RIGHT)
				return true;
				
			return false;
		}
	};
}

#endif
