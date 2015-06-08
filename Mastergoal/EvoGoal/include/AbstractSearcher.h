#ifndef ABSTRACT_SEARCHER_H
#define ABSTRACT_SEARCHER_H

#include "State.h"
#include "Evaluator.h"
#include "SuccessorGenerator.h"
#include "Play.h"
#include "RulesManager.h"
#include <string.h>
#include <limits>

using namespace Common;
using namespace AI;

namespace AI
{
	struct RankedPlay
	{
		Play* play;
		float score;

		RankedPlay()
		{
			this->play = NULL;
			this->score = 0;
		}

		RankedPlay(const RankedPlay& rP)
		{
			this->score = rP.score;
			this->play = rP.play;// == NULL ? NULL : new Play(*rP.play);
		}
	};

	/** Define la clase de búsqueda base, los demás buscadores (e.g. Negamax) deben extender
		esta clase **/
	class AbstractSearcher
	{
	public:
		/** Constructores **/
		// Constructor para cuando no se va a usar libro de apertura
		AbstractSearcher(Evaluator* evaluator, int maxDepth = DEFAULT_MAX_DEPTH, int quiesDepth = DEFAULT_QUIES_DEPTH);		

		virtual ~AbstractSearcher();

		/** Getters y setters **/
		inline int GetMaxDepth() const { return this->maxDepth; }

		inline void SetMaxDepth(int value) { this->maxDepth = value; }

		inline int GetQuiescenceDepth() const { return this->quiescenceDepth; }

		inline void SetQuiescenceDepth(int value) { this->quiescenceDepth = value; }

		inline SuccessorGenerator* GetSuccessorGenerator() { return successorGen; }

		inline void SetSuccessorGenerator(SuccessorGenerator* value) { successorGen = value; }

		inline Evaluator* GetEvaluator() const { return this->evaluator; }
		
		/** Métodos **/
		// Se le llama para hacer una jugada
		// Primero busca la jugada en el libro de apertura si se lo usa, 
		// si no lo encuentra se realiza la búsqueda en el árbol llamando a FindPlayBySearch
		// @param state: el estado actual
		// @param depth: la profundidad de búsqueda
		inline Play* FindPlay(const State& state)
		{
			return FindPlayBySearch(state);
		}
		
	protected:
		static const int DEFAULT_MAX_DEPTH = MINIMAX_DEPTH;
		static const int DEFAULT_QUIES_DEPTH = 0;		

		/** Atributos **/
		// Profundidad máxima de búsqueda
		int maxDepth;

		// Profundidad máxima de búsqueda de quietud
		int quiescenceDepth;
		
		// Clase que contiene la función encargada de generar los sucesores de un estado en un orden específico 
		SuccessorGenerator* successorGen;
		
		// Clase que contiene la función de evaluación que se usará en la búsqueda 
		Evaluator* evaluator;

		/** Métodos **/
		// Retorna la mejor jugada de acuerdo con la búsqueda en el árbol generado
		virtual Play* FindPlayBySearch(const State& state) = 0;

		inline bool IsGoal(const Board* board)
		{
			return (board->GetBall()->GetRow() == 0 || board->GetBall()->GetRow() == 14);
		}

		inline bool IsGoal(const Play* play)
		{
			return (play->GetNMovements() > 1 &&
				(play->GetLastMovement()->GetSquareAfter()->GetRow() == 0 || play->GetLastMovement()->GetSquareAfter()->GetRow() == 14));
		}

		inline float EvaluateDrawnBall(const State& state)
		{
			//if (state.GetTurn() == LEFT)
			//{
			//	if (state.GetGoalsLeft() > state.GetGoalsRight())
			//		return EVOGOAL_INFINITY;
			//	else if (state.GetGoalsLeft() < state.GetGoalsRight())
			//		return -EVOGOAL_INFINITY;
			//	else
			//		return 0;
			//}
			////else state.GetTurn() == RIGHT
			//if (state.GetGoalsLeft() < state.GetGoalsRight())
			//	return EVOGOAL_INFINITY;
			//else if (state.GetGoalsLeft() > state.GetGoalsRight())
			//	return -EVOGOAL_INFINITY;

			return 0.0f;
		}

		inline bool SpecialSquare(const State& state)
		{
			return RulesManager::IsSpecialSquare(*state.GetBoard()->GetBall()->GetPosition(), state.GetTurn(), state.GetLevel());
		}
	};
}

#endif
