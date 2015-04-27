#ifndef SAMPLE_PARALLEL_TOURNAMENT_H
#define SAMPLE_PARALLEL_TOURNAMENT_H

#include <ga/GAPopulation.h>
#include "Tournament.h"
#include "SampleTourData.h"
#include "Encoder.h"
#include <winsock2.h>

namespace AI
{
	class SampParTournament : public Tournament
	{
	public:
		SampParTournament(GAPopulation *pop, GAPopulation *sample);

		SampParTournament(const SampParTournament& tournament);

		virtual ~SampParTournament();

		virtual void Play();

		virtual void Play(int &nTies);

	protected:
		GAPopulation *pop;
		GAPopulation *sample;
		char hostIPs[MAX_PROCS][50];
		u_int hostPorts[MAX_PROCS];
		bool completedTasks[MAX_PROCS];

		int SendTask(char* task, u_short portNo, char* ipAddress);
		int ListenOnPort(u_short listenPort, SOCKET &s);
		int GetIpAndHost(char* hdr, char* ip, u_short &port);
		int UpdateResults(char *resultsMsg, int msgLen, int &c);
		int UpdateResults(char *resultsMsg, int msgLen, int &c, int &nTies);
		int GetHeader(int &iI, int &nInd, char *resultsMsg, int msgLen, int &c);
		int GetNextResult(char *resultsMsg, int msgLen, int &c);
		int FindProcId(char* ip, u_short nProc, int nProcesors);
	};
}

#endif
