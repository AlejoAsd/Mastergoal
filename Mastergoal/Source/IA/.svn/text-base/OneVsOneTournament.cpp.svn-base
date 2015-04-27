#include "stdafx.h"
#include "OneVsOneTournament.h"
#include "Game.h"
#include <iostream>
using namespace std;

namespace AI
{
	OneVsOneTournament::OneVsOneTournament(char* agent1Path, char* agent2Path, int nGames, int searchDepth) : Tournament()
	{
		this->agentL = new Agent(GAME_LEVEL, LEFT, searchDepth, agent1Path);
		this->agentR = new Agent(GAME_LEVEL, RIGHT, searchDepth, agent2Path);
		this->nGames = nGames;
		this->winsRight = this->winsLeft = 0;

		ifstream file;
		file.open(agent1Path, ios::in);
		this->agentL->GetEvaluator()->ReadWeights(file);
		file.close();
		file.open(agent2Path, ios::in);
		this->agentR->GetEvaluator()->ReadWeights(file);
		file.close();

		if (file.fail())
		{
			cout << "Error al abrir archivo en OneVsOneTournament" << endl;
			getchar();
		}

		this->aGenomeSetter = NULL;
	}

	OneVsOneTournament::OneVsOneTournament(const OneVsOneTournament& tournament)
	{
		cout << "ERROR: No constructor de copias!" << endl;
		getchar();
	}

	OneVsOneTournament::~OneVsOneTournament()
	{
		if (agentL) { delete agentL; agentL = NULL; }
		if (agentR) { delete agentR; agentR = NULL; }
	}

	void OneVsOneTournament::Play()
	{
		int nGame = 0;
		this->winsLeft = this->winsRight = 0;
		cout << "Empieza " << agentL->GetName().c_str() << endl;
		for (nGame = 0; nGame < nGames / 2; nGame++)
		{
			Game game1(GAME_LEVEL, LEFT, agentL, agentR);
			game1.DoGame();
			cout << nGame << ".";

			if (game1.GetWinner() == agentL)
			{
				cout << "\tGano " << agentL->GetName().c_str() << endl;
				winsLeft++;
			}
			else if (game1.GetWinner() == agentR)
			{
				cout << "\tGano " << agentR->GetName().c_str() << endl;
				winsRight++;
			}
			else
			{
				cout << "\tEmpate" << endl;
			}
		}
		cout << "Empieza " << agentR->GetName().c_str() << endl;
		for (; nGame < nGames; nGame++)
		{
			Game game1(GAME_LEVEL, RIGHT, agentL, agentR);
			game1.DoGame();
			cout << nGame << ".";

			if (game1.GetWinner() == agentL)
			{
				cout << "\tGano " << agentL->GetName().c_str() << endl;
				winsLeft++;
			}
			else if (game1.GetWinner() == agentR)
			{
				cout << "\tGano " << agentR->GetName().c_str() << endl;
				winsRight++;
			}
			else
			{
				cout << "\tEmpate" << endl;
			}
		}

		cout << "Resultado:\t" << winsLeft << " " << winsRight << " " << (nGames - winsLeft - winsRight) << endl;
	}
}
