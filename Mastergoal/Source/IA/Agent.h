#ifndef AGENT_H
#define AGENT_H

#include "AbstractSearcher.h"
#include "Play.h"
#include "State.h"
#include "Definitions.h"

using namespace Common;

namespace AI
{
	class Agent
	{
	private:
		// Puntero al buscador (e.g. Negamax, Negascout, etc...)
		AbstractSearcher* searcher;

		// Orientación del agente)
		ORIENTATION orientation;

		// Nombre del agente
		string name;

	public:
		/** Constructores **/
		Agent(int level, ORIENTATION orientation, string name = "Agent");

		Agent(int level, ORIENTATION orientation, int searchDepth, string name = "Agent");

		Agent(AbstractSearcher* searcher, const ORIENTATION& orientation, const string name);

		~Agent();

		/** Getters y setters **/
		inline AbstractSearcher* GetSearcher() const { return searcher; }

		inline void SetSearcher(AbstractSearcher* value) { this->searcher = value; }

		inline ORIENTATION GetOrientation() const { return orientation; }

		inline void SetOrientation(const ORIENTATION& orientation) { this->orientation = orientation; }

		inline string GetName() const { return name; }

		inline void SetName(const string value) { this->name = value; }

		inline int GetMaxDepth() const { return searcher->GetMaxDepth(); }

		inline void SetMaxDepth(int value) { this->searcher->SetMaxDepth(value); }	

		// Retorna el Evaluator de la clase Searcher
		inline Evaluator* GetEvaluator() const { return this->searcher->GetEvaluator(); }

		/** Métodos **/
		// El usuario debe encargarse de hacer el delete luego
		inline Play* MakePlay(const State& state) { return searcher->FindPlay(state); }
	};
}

#endif
