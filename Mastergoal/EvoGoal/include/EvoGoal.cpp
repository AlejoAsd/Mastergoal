// EvoGoal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define INSTANTIATE_REAL_GENOME
#include <ga/GASelector.h>
#include <ga/GARealGenome.h>
#include <fstream>
#include "Definitions.h"
#include "SampleTourGA.h"
#include "Evaluators.h"
#include "FullCompTournament.h"
#include "OneVsOneTournament.h"
#include "Agent.h"
using namespace std;
using namespace AI;

float Objective(GAGenome& g);
void RankHall(int interval, int nFeatures);
void RankBests(int interval, int nFeatures, const History &history);
void BestD1D2Tournament();

int _tmain(int argc, _TCHAR* argv[])
{
	GARandomSeed();

	BestD1D2Tournament();
	getchar();
	return 0;
	// Se escribe la configuración con la que se corre el programa en CONFIG_FILE
	WriteConfig();

	GARealAlleleSet alleles(MIN_WEIGHT, MAX_WEIGHT);

	int nFeatures = GetNFeatures();

	RankHall(28, nFeatures);
	cout << "Ya!" << endl;
	getchar();
	return 0;

	GARealGenome genome(nFeatures, alleles);

	SampleTourGA ga(genome);
	ga.nGenerations(N_GENERATIONS);
	ga.pMutation(P_MUT);
	ga.populationSize(POP_SIZE);
	ga.scoreFrequency(SAVE_FREQ);
	ga.selectScores(GAStatistics::Maximum | GAStatistics::Minimum | GAStatistics::Mean);

	ga.evolve();
	cout << endl << "Ranking..." << endl;
	
	cout << "Ahora Rank Hall" << endl;
	RankHall(1, nFeatures);
	cout << "Done!" << endl << endl << "Presione ENTER para salir..." << endl;
	std::getchar();

	return 0;
}

float Objective(GAGenome& g)
{
	GARealGenome& genome = (GARealGenome&)g;
	SampleTourData* data = (SampleTourData*)genome.evalData();
	//float score = WIN_SCORE * data->NBeats() + DRAW_SCORE * data->NDraws();
	float score = 0.0f;

	// Se suman los scores de los partidos ganados
	SampleTourData::DataIter beatsIter = data->BeatsIter();		
	while (beatsIter.Next())
	{
		// losersData tiene info acerca de cuántos más lo vencen
		SampleTourData* losersData = (SampleTourData*)beatsIter.Current()->evalData();

		// WIN_SCORE / (losersData->NLoses() * WIN_SCORE); // se simplifica WIN_SCORE
		score += 1.0f / losersData->NLoses();
		// TODO: probar score += WIN_SCORE / (WIN_SCORE * losersData->NLoses() + DRAW_SCORE * losersData->NDraws())
	}

	// Se suman los scores de los partidos empatados
	SampleTourData::DataIter drawsIter = data->DrawsIter();
	while (drawsIter.Next())
	{
		// losersData tiene info acerca de cuántos más lo vencen y empatan
		SampleTourData* losersData = (SampleTourData*)drawsIter.Current()->evalData();
		score += DRAW_SCORE / (WIN_SCORE * losersData->NLoses() + WIN_SCORE * losersData->NDraws());
	}

	return score;
}

// TODO: tener en cuenta la generación
void RankHall(int interval, int nFeatures)
{
	HallOfFame hall;
	GARealAlleleSet alleles(MIN_WEIGHT, MAX_WEIGHT);
	GARealGenome g(nFeatures, alleles, Objective);
	hall.Insert(g, 0);
	
	ifstream input;
	input.open(HOF_FILE, ios::in);
	if (input.fail()) return;
	if (hall.Read(input) == -1)
	{
		input.close();
		cout << "Error al leer Hall" << endl;
		getchar();
		exit(-1);
	}
	input.close();

	// TODO: revisar
	int tournamentSize = hall.Size() / interval + (hall.Size() % interval == 0 ? 0 : 1);

	SampleTourData data(TOUR_DATA_MULT * tournamentSize);
	g.evalData(data);
	GAPopulation pop(g, tournamentSize);	

	HallOfFame::HallIter it = hall.Iterator();
	int count = 0;
	int index = 0;
	while (it.Next())
	{
		if (index % interval == 0)
		{
			pop.individual(count).copy(*it.Current());
			count++;
		}
		index++;
	}
	// Al salir count == tournamentSize

	//int inc = hall.Size() / tournamentSize;
	//// Se carga la población
	//int i, j;
	//for (i = 0, j = 0; i < tournamentSize; i++, j += inc)
	//{
	//	pop.individual(i).copy(hall.
	//}

	FullCompTournament tournament(&pop);
	tournament.Play();

	// Se asigna el fitness a cada individuo de acuerdo con los resultados del torneo
	for (int i = 0; i < tournamentSize; i++)
	{
		pop.individual(i).evaluate(gaTrue);
	}

	it = hall.Iterator();
	HallOfFame rankedHall;
	for (int c = 0; c < pop.size(); c++)
	{
		it.Reset();
		it.Next();
		if (pop.individual(c).compare(*it.Current()) == 0.0f)
		{
			rankedHall.Insert(pop.individual(c), it.CurrentGen());
		}
		else
		{
			while (it.Next())
			{
				if (pop.individual(c).compare(*it.Current()) == 0.0f)
				{
					rankedHall.Insert(pop.individual(c), it.CurrentGen());
					break;
				}
			}
		}
	}
	//while (it.Next())
	//{
	//	//cout << *it.Current() << endl;
	//	if (pop.individual(c).compare(*it.Current()) == 0.0f)
	//	{
	//		rankedHall.Insert(pop.individual(c), it.CurrentGen());
	//	}
	//	else
	//	{
	//		for (int i = (c + 1) % tournamentSize; i != c; i = (i + 1) % tournamentSize)
	//		{
	//			if (pop.individual(i).compare(*it.Current()) == 0.0f)
	//			{
	//				rankedHall.Insert(pop.individual(i), it.CurrentGen());
	//				break;
	//			}
	//		}
	//	}
	//	c++;
	//}

	ofstream output, outcsv;
	output.open("rankedHall.txt", ios::out);
	outcsv.open("rankedHall.csv", ios::out);
	outcsv << "gen;wins;draws;losts;raw score;shared score" << endl;
	/*for (int i = 0; i < tournamentSize; i++)
	{
		SampleTourData* dataPtr = (SampleTourData*)pop.individual(i).evalData();
		pop.individual(i).write(output);
		output << dataPtr->NBeats() << " " << dataPtr->NDraws() << " " << dataPtr->NLoses() << " " << pop.individual(i).score() << endl;
		outcsv << dataPtr->NBeats() << ";" << dataPtr->NDraws() << ";" << dataPtr->NLoses() << ";" << 3 * dataPtr->NBeats() + dataPtr->NDraws() << ";" << pop.individual(i).score() << endl;
	}*/
	it = rankedHall.Iterator();
	while (it.Next())
	{
		SampleTourData* dataPtr = (SampleTourData*)it.Current()->evalData();
		it.Current()->write(output);
		output << dataPtr->NBeats() << " " << dataPtr->NDraws() << " " << dataPtr->NLoses() << " " << it.Current()->score() << endl;
		outcsv << it.CurrentGen() << ";" << dataPtr->NBeats() << ";" << dataPtr->NDraws() << ";" << dataPtr->NLoses() << ";" << 3 * dataPtr->NBeats() + dataPtr->NDraws() << ";" << it.Current()->score() << endl;
	}
	output.close();
	outcsv.close();
}

void RankBests(int interval, int nFeatures, const History &history)
{
	History bests;
	int bestsGens[N_GENERATIONS]; // Va a guardar las generaciones de los mejores
	int nBests = 0; // Índice de bestsGens
	// Se insertan los individuos de history a best, sin repetir
	History::HistoryIter histIter = history.Iterator();
	while (histIter.Next())
	{
		bool contains = false;
		History::HistoryIter bestsIter = bests.Iterator();
		// Compara los que ya están insertados en bests con el current de history (que se quiere insertar ahora)
		while (bestsIter.Prev())
		{
			if (bestsIter.Current()->compare(*histIter.Current()) == 0.0f)
			{
				contains = true;
				break;
			}
		}
		if (!contains)
		{
			bestsGens[nBests++] = histIter.CurrentGen();
			bests.Add(*histIter.Current());
		}
	}
	// TODO: revisar
	int tournamentSize = bests.Size() / interval + (bests.Size() % interval == 0 ? 0 : 1);
	int *bestsSelected = new int[tournamentSize]; // Contiene las generaciones de los individuos del torneo
	SampleTourData data(TOUR_DATA_MULT * tournamentSize);
	
	GAPopulation pop(*histIter.Current(), tournamentSize);	

	History::HistoryIter it = bests.Iterator();
	int count = 0;
	int index = 0;
	while (it.Next())
	{
		if (index % interval == 0)
		{
			pop.individual(count).copy(*it.Current());
			pop.individual(count).evalData(data);
			bestsSelected[count] = bestsGens[index];
			count++;
		}
		index++;
	}
	// Al salir count == tournamentSize

	FullCompTournament tournament(&pop);
	tournament.Play();

	// Se asigna el fitness a cada individuo de acuerdo con los resultados del torneo
	for (int i = 0; i < tournamentSize; i++)
	{
		pop.individual(i).evaluate(gaTrue);
	}

	ofstream output, outcsv;
	output.open("rankedHall.txt", ios::out);
	outcsv.open("rankedHall.csv", ios::out);
	outcsv << "NGen;Wins;Draws;Losts;Score;Shared Score" << endl;
	for (int i = 0; i < tournamentSize; i++)
	{
		SampleTourData* dataPtr = (SampleTourData*)pop.individual(i).evalData();
		output << bestsSelected[i] << endl;
		pop.individual(i).write(output);
		output << endl << dataPtr->NBeats() << " " << dataPtr->NDraws() << " " << dataPtr->NLoses() << " " << 3 * dataPtr->NBeats() + dataPtr->NDraws() << " " << pop.individual(i).score() << endl;
		outcsv << bestsSelected[i] << ";" << dataPtr->NBeats() << ";" << dataPtr->NDraws() << ";" << dataPtr->NLoses() << ";" << 3 * dataPtr->NBeats() + dataPtr->NDraws() << ";" << pop.individual(i).score() << endl;
	}
	output.close();
	outcsv.close();
	delete[] bestsSelected;
}

void RankHistory(int interval, int nFeatures)
{
	History history;
	ifstream histFile;
	histFile.open(HISTORY_FILE, ios::in);
	if (history.Read(histFile) == -1)
	{
		cout << "Error en RankHistory al leer archivo" << endl;
		getchar();
		exit(-1);
	}
	histFile.close();

	History bests;
	int bestsGens[N_GENERATIONS]; // Va a guardar las generaciones de los mejores
	int nBests = 0; // Índice de bestsGens
	// Se insertan los individuos de history a best, sin repetir
	History::HistoryIter histIter = history.Iterator();
	while (histIter.Next())
	{
		bool contains = false;
		History::HistoryIter bestsIter = bests.Iterator();
		// Compara los que ya están insertados en bests con el current de history (que se quiere insertar ahora)
		while (bestsIter.Prev())
		{
			if (bestsIter.Current()->compare(*histIter.Current()) == 0.0f)
			{
				contains = true;
				break;
			}
		}
		if (!contains)
		{
			bestsGens[nBests++] = histIter.CurrentGen();
			bests.Add(*histIter.Current());
		}
	}
	// TODO: revisar
	int tournamentSize = bests.Size() / interval + (bests.Size() % interval == 0 ? 0 : 1);
	int *bestsSelected = new int[tournamentSize]; // Contiene las generaciones de los individuos del torneo
	SampleTourData data(TOUR_DATA_MULT * tournamentSize);
	
	GAPopulation pop(*histIter.Current(), tournamentSize);	

	History::HistoryIter it = bests.Iterator();
	int count = 0;
	int index = 0;
	while (it.Next())
	{
		if (index % interval == 0)
		{
			pop.individual(count).copy(*it.Current());
			pop.individual(count).evalData(data);
			pop.individual(count).evaluator(Objective);
			bestsSelected[count] = bestsGens[index];
			count++;
		}
		index++;
	}
	// Al salir count == tournamentSize

	FullCompTournament tournament(&pop);
	tournament.Play();

	// Se asigna el fitness a cada individuo de acuerdo con los resultados del torneo
	for (int i = 0; i < tournamentSize; i++)
	{
		pop.individual(i).evaluate(gaTrue);
	}

	ofstream output, outcsv;
	output.open("rankedHall.txt", ios::out);
	outcsv.open("rankedHall.csv", ios::out);
	outcsv << "NGen;Wins;Draws;Losts;Score;Shared Score" << endl;
	for (int i = 0; i < tournamentSize; i++)
	{
		SampleTourData* dataPtr = (SampleTourData*)pop.individual(i).evalData();
		output << bestsSelected[i] << endl;
		pop.individual(i).write(output);
		output << endl << dataPtr->NBeats() << " " << dataPtr->NDraws() << " " << dataPtr->NLoses() << " " << 3 * dataPtr->NBeats() + dataPtr->NDraws() << " " << pop.individual(i).score() << endl;
		outcsv << bestsSelected[i] << ";" << dataPtr->NBeats() << ";" << dataPtr->NDraws() << ";" << dataPtr->NLoses() << ";" << 3 * dataPtr->NBeats() + dataPtr->NDraws() << ";" << pop.individual(i).score() << endl;
	}
	output.close();
	outcsv.close();
	delete[] bestsSelected;
}

void BestD1D2Tournament()
{
	cout << "Profundidad 1" << endl;
	OneVsOneTournament t1("agentL1_1", "agentL1_2", 10, 1);
	t1.Play();
	cout << endl << "Profundidad 2" << endl;
	OneVsOneTournament t2("agentL1_1", "agentL1_2", 10, 2);
	t2.Play();
	cout << endl << "Profundidad 3" << endl;
	OneVsOneTournament t3("agentL1_1", "agentL1_2", 10, 3);
	t3.Play();
	cout << endl << "Profundidad 4" << endl;
	OneVsOneTournament t4("agentL1_1", "agentL1_2", 10, 4);
	t4.Play();
}
