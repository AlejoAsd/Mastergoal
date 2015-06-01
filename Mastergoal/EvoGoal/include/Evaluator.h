#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "State.h"
#include <iostream>
using namespace std;
using namespace Common;

namespace AI
{

#ifndef SwapMacro
#define SwapMacro(Type, a, b) { Type aux = a; a = b; b = aux; }
#endif

	/** Define la interfaz de la clase que contiene la funci�n de evaluaci�n **/
	class Evaluator
	{
	public:
		// Funci�n de evaluaci�n
		virtual float Evaluate(const State& state) = 0;

		// Setea los pesos de la funci�n de evalaci�n
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
