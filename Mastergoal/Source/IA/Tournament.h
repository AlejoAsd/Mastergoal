#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "Agent.h"
#include "Evaluators.h"
#include <ga/GAGenome.h>
#include <ga/GARealGenome.h>

namespace AI
{
	class Tournament
	{
	public:
		Tournament();

		Tournament(const Tournament& tournament);

		virtual ~Tournament();

		// Funci�n que setear� un agente a partir de un genoma espec�fico
		typedef void (*AGenomeSetter)(Agent*, GAGenome*);

		static void RealAgentSetter(Agent* agent, GAGenome* genome);

	public:
		virtual void Play() = 0;

		void AgentSetter(AGenomeSetter aGenSet) { aGenomeSetter = aGenSet; }

		inline void SetAgentGenome(Agent* agent, GAGenome* genome) { (*aGenomeSetter)(agent, genome); }

	protected:
		// Agentes que usar�n las funciones de evaluaci�n para competir
		Agent *agentL, *agentR;

		AGenomeSetter aGenomeSetter;
	};	
}

#endif
