#include "stdafx.h"
#include "Definitions.h"
#include "Evaluators.h"
using namespace AI;

void WriteConfig()
{
	ofstream file;
	file.open(CONFIG_FILE, ios::out);
#ifdef DOUBLE_MATCH
	file << "DOUBLE MATCH" << endl;
#else
	file << "SINGLE MATCH" << endl;
#endif
#ifdef EVAL_ACTIONS
	file << "EVALUATE ACTIONS" << endl;
#else
	file << "EVALUATE STATES" << endl;
#endif
	file << "POP_SIZE " << POP_SIZE << endl;		
	file << "SAMP_SIZE " << SAMP_SIZE << endl;
	file << "N_HALL_OF_FAME " << N_HALL_OF_FAME << endl;
	file << "HALL_SIZE " << HALL_SIZE << endl;
	file << "ELITE_SIZE " << ELITE_SIZE << endl;
	file << "N_GENERATIONS " << N_GENERATIONS << endl;
	file << "MINI_SAMP_SIZE " << MINI_SAMP_SIZE << endl;
	file << "N_OFFSPRINGS " << N_OFFSPRINGS << endl;
	file << "MAX_PLAYS " << MAX_PLAYS << endl;
	file << "P_MUT " << P_MUT << endl;
	file << "GAME_LEVEL " << GAME_LEVEL << endl;
	file << "WIN_SCORE " << WIN_SCORE << endl;
	file << "DRAW_SCORE " << DRAW_SCORE << endl;
	file << "MINIMAX_DEPTH " << MINIMAX_DEPTH << endl;
	file << "INFINITY " << INFINITY << endl;
	file << "MIN_WEIGHT " << MIN_WEIGHT << endl;
	file << "MAX_WEIGHT " << MAX_WEIGHT << endl;
	file.close();
}

int GetNFeatures()
{
	int nFeatures = 0;
#ifndef EVAL_ACTIONS
	switch (GAME_LEVEL)
	{
	case 1:
		nFeatures = EvaluatorLevel1::N_FEATURES;
		break;
	case 2:
		nFeatures = EvaluatorLevel2::N_FEATURES;
		break;
	case 3:
		nFeatures = EvaluatorLevel3::N_FEATURES;
		break;
	default: // nivel 4
		nFeatures = EvaluatorLevel4::N_FEATURES;
		break;
	}
#else
	switch (GAME_LEVEL)
	{
	case 1:
		nFeatures = DeltaEvaluatorLevel1::N_FEATURES;
		break;
	case 2:
		nFeatures = DeltaEvaluatorLevel2::N_FEATURES;
		break;
	case 3:
		nFeatures = DeltaEvaluatorLevel3::N_FEATURES;
		break;
	default: // nivel 4
		nFeatures = DeltaEvaluatorLevel4::N_FEATURES;
		break;
	}
#endif
	return nFeatures;
}
