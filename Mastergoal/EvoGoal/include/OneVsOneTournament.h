#ifndef ONE_VS_ONE_TOURNAMENT
#define ONE_VS_ONE_TOURNAMENT

#include "Tournament.h"

namespace AI
{
	class OneVsOneTournament : public Tournament
	{
	public:
		OneVsOneTournament(char* agent1Path, char* agent2Path, int nGames, int searchDepth);

		OneVsOneTournament(const OneVsOneTournament& tournament);

		virtual ~OneVsOneTournament();

		virtual void Play();

	private:
		int nGames;
		int winsLeft;
		int winsRight;
	};
}

#endif
