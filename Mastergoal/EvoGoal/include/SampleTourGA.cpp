#include "stdafx.h"
#include "SampleTourGA.h"
#include "SampleTourData.h"
#include "SamplePopData.h"
#include "SampleTournament.h"

namespace AI
{
	int SampleTourGA::nTies = 0;
	SampleTourGA::SampleTourGA(const GAGenome& c) : GASimpleGA(c) 
	{
		this->sample = new GAPopulation(c, SAMP_SIZE);
		this->oldSample = this->sample->clone();

		for (int i = 0; i < this->pop->size(); i++)
		{
			this->pop->individual(i).evaluator(EvaluateInd);
			this->oldPop->individual(i).evaluator(EvaluateInd);
		}
		this->INITIAL_GENERATION = 1;
	}

	SampleTourGA::SampleTourGA(const GAPopulation& p) : GASimpleGA(p) 
	{
		this->sample = p.clone();
		this->sample->size(SAMP_SIZE);
		this->oldSample = this->sample->clone();		
		this->INITIAL_GENERATION = 1;
	}

	SampleTourGA::SampleTourGA(const SampleTourGA& ga) : GASimpleGA(ga) 
	{
		this->sample = ga.sample->clone();
		this->oldSample = ga.oldSample->clone();
		this->INITIAL_GENERATION = ga.INITIAL_GENERATION;
	}

	SampleTourGA::~SampleTourGA()
	{
		if (sample) { delete sample; sample = NULL; }
		if (oldSample) { delete oldSample; oldSample = NULL; }
	}

	void SampleTourGA::initialize(unsigned int seed)
	{
		InitTimeStats();
		GARandomSeed(seed);

		// Se crea el evalData que contendrá info sobre a quién vence cada host
		SampleTourData hostData(TOUR_DATA_MULT * SAMP_SIZE);
		for (int i = 0; i < pop->size(); i++)
		{
			pop->individual(i).evalData(hostData);
			oldPop->individual(i).evalData(hostData);
		}

		// Se crea el evalData que contendrá info sobre quiénes vencen a los parásitos
		SampleTourData parasiteData(TOUR_DATA_MULT * POP_SIZE);
		for (int i = 0; i < sample->size(); i++)
		{
			sample->individual(i).evalData(parasiteData);
			oldSample->individual(i).evalData(parasiteData);
		}

		// Se asocia el evaluador (que contiene el torneo Tournament) a las poblaciones
		pop->evaluator(EvaluatePop);
		oldPop->evaluator(EvaluatePop);

		// Se asocian las poblaciones de muestra (parásitos) con las poblaciones (huéspedes o hosts)
		SamplePopData popData(sample);
		pop->evalData(popData);
		SamplePopData oldPopData(oldSample);
		oldPop->evalData(oldPopData);
		
		sample->initialize();
		pop->initialize();

#ifdef RESTORE
		// Leer pop, samp, hall, history, nGen
		cout << "Cargando datos...\n";
		if (!Load())
		{
			cout << "No se pudo recuperar estado... Revisar RECOVER_FILE" << endl << "Presione ENTER para salir" << endl;
			getchar();
			exit(-1);
		}
#endif

		pop->evaluate(gaTrue);
		//pop->scale(gaTrue);

		stats.reset(*pop);
		
		cout << "Inicializado...\nGeneracion...\n";
		Save();
		startTime = clock();

		if(!scross) 
			GAErr(GA_LOC, className(), "initialize", gaErrNoSexualMating);
	}

	void SampleTourGA::step()
	{
		cout << stats.generation() + INITIAL_GENERATION  << "\t";

		GAPopulation *tmppop;		// Swap the old population with the new pop.
		tmppop = oldPop;		// When we finish the ++ we want the newly 
		oldPop = pop;			// generated population to be current (for
		pop = tmppop;			// references to it from member functions).

		tmppop = oldSample;
		oldSample = sample;
		sample = tmppop;

		SelectSample();

		GenerateNewPop();

		stats.numrep += pop->size();

		nTies = 0;

		pop->evaluate(gaTrue);	// get info about current pop for next time
		//pop->scale(gaTrue);
		hall.Insert(pop->best(0, GAPopulation::RAW), this->statistics().generation() + INITIAL_GENERATION);
		history.Add(pop->best(0, GAPopulation::RAW));

		endTime = clock();
		stepTimes[stats.generation()] = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		long hh, mm, ss;
		hh = (long)stepTimes[stats.generation()] / (long)3600;
		mm = (long)stepTimes[stats.generation()] / (long)60 - (long)60 * hh;
		ss = (long)stepTimes[stats.generation()] % (long)60;
		
		char tmpbuf[128];
		_strtime_s(tmpbuf, 128);
		cout << '\t' << tmpbuf << '\t';
		cout << hh << ":" << (mm < 10 ? "0" : "") << mm << ":" << (ss < 10 ? "0" : "") << ss;
		cout << "\tnWins: " << ((TOUR_DATA_MULT * POP_SIZE * SAMP_SIZE) - nTies) << "\tnDraws: " << nTies << endl;

		startTime = endTime;

		stats.update(*pop);		// update the statistics by one generation

		if (stats.generation() % BACK_FREQ == 0)
			Backup();
		
		if (stats.generation() % SAVE_FREQ == 0)
			Save();

		if (stats.generation() == this->ngen)
			EndTimeStats();
	}

	void SampleTourGA::SelectSample()
	{
		SampleTourData* parasiteData = NULL;
		for (int i = 0; i < SAMP_SIZE; i++)
		{
			parasiteData = (SampleTourData*) oldSample->individual(i).evalData();
			parasiteData->Reset();
		}

		// Lista de individuos no agregados al sample
		GAList<GAGenome*> hosts;
		// Lista de individuos ya agregados
		GAGenome* inserted[SAMP_SIZE];
		for (int i = 0; i < SAMP_SIZE; i++)
		{
			inserted[i] = NULL;
		}

		for (int i = 0; i < POP_SIZE; i++)
		{
			hosts.insert(&oldPop->individual(i));
		}

		int c = 0;
		// Se seleccionan los individuos del salón de la fama
		hall.PrepareForSelection();
		while (c < N_HALL_OF_FAME)
		{
			GAGenome* g = hall.Select(); // Retorna NULL si no hay ninguno que seleccionar
			// Si ya no hay individuos para seleccionar en hall
			if (!g)
				break;
			//sample->individual(c).copy(*g);
			// g sera insertado en el sample
			inserted[c] = g;
			c++;
		}

		// Se selecciona el resto de los individuos de la población anterior
		while (c < SAMP_SIZE)
		{
			GAGenome *best = *hosts.head();
			float bestFit = 0;
			for (int i = 0; i < hosts.size(); i++)
			{
				//(*hosts.current())->score(0);
				float fit = 0.0f;
				SampleTourData* hostData = (SampleTourData*)(*hosts.current())->evalData();
				SampleTourData::DataIter winsIt = hostData->BeatsIter();
				SampleTourData::DataIter drawsIt = hostData->DrawsIter();

				while (winsIt.Next())
				{
					//(*hosts.current())->score((*hosts.current())->score() + (WIN_SCORE / (WIN_SCORE + winsIt.Current()->score())));
					//TODO: borrar: fit += WIN_SCORE / (WIN_SCORE + winsIt.Current()->score());
					parasiteData = (SampleTourData*)winsIt.Current()->evalData();
					fit += 1.0f / (1.0f + parasiteData->NLoses());
				}
				// TODO: ver si conviene o no considerar los empates para seleccionar el sample
				while (drawsIt.Next())
				{
					//(*hosts.current())->score((*hosts.current())->score() + (DRAW_SCORE / (WIN_SCORE + drawsIt.Current()->score())));
					//TODO: borrar: fit += DRAW_SCORE / (WIN_SCORE + drawsIt.Current()->score());
					parasiteData = (SampleTourData*)drawsIt.Current()->evalData();
					fit += DRAW_SCORE / (1.0f + WIN_SCORE * parasiteData->NLoses() + WIN_SCORE * parasiteData->NDraws());
				}

				if (fit > bestFit)
				{
					best = *hosts.current();
					bestFit = fit;
				}
				hosts.next();
			}
			//sample->individual(c).copy(*best);
			// Best sera insertado al sample si todavia no lo fue
			bool alreadyInserted = false;
			for (int j = c - 1; j >= 0; j--)
			{
				if (best->compare(*inserted[j]) == 0.0f)
				{
					alreadyInserted = true;
					break;
				}
			}
			// Si ya fue insertado, se lo remueve de la lista y pasa al siguiente
			if (alreadyInserted)
			{
				// Se remueve a best de la lista de no agregados
				do
				{
					if (best == *hosts.current())
					{
						hosts.remove();
						break;
					}
				} while (hosts.next());
				continue;
			}
			inserted[c] = best;

			// Se acualizan los scores de oldSample 
			// (cuántos en el sample nuevo vencen a cada individuo 
			// en oldSample)
			SampleTourData* bestData = (SampleTourData*)best->evalData();
			SampleTourData::DataIter winsIt = bestData->BeatsIter();
			SampleTourData::DataIter drawsIt = bestData->DrawsIter();
			while (winsIt.Next())
			{
				GAGenome* oldSInd = winsIt.Current();
				//TODO: borrar: oldSInd->score(oldSInd->score() + WIN_SCORE);
				parasiteData = (SampleTourData*)oldSInd->evalData();
				parasiteData->Loses(&sample->individual(c));
			}
			while (drawsIt.Next())
			{
				GAGenome* oldSInd = drawsIt.Current();
				//TODO: borrar: oldSInd->score(oldSInd->score() + WIN_SCORE);
				parasiteData = (SampleTourData*)oldSInd->evalData();
				parasiteData->Draws(&sample->individual(c));
			}

			// Se remueve a best de la lista de no agregados
			do
			{
				if (best == *hosts.current())
				{
					hosts.remove();
					break;
				}
			} while (hosts.next());
			c++;
		}

		for (int i = 0; i < SAMP_SIZE; i++)
		{
			sample->individual(i).copy(*inserted[i]);
		}
	}

	void SampleTourGA::EvaluatePop(GAPopulation &pop)
	{
		GAPopulation* sample = ((SamplePopData*)pop.evalData())->Sample();
		SampleTournament tour(&pop, sample);
		tour.Play(nTies);
		// Se asigna el fitness a cada individuo de acuerdo con los resultados del torneo
		for (int i = 0; i < pop.size(); i++)
		{
			pop.individual(i).evaluate(gaTrue);
		}		
	}

	float SampleTourGA::EvaluateInd(GAGenome &g)
	{
		GARealGenome& genome = (GARealGenome&)g;
		SampleTourData* hostData = (SampleTourData*)genome.evalData();
		SampleTourData* parasiteData = NULL;
		float score = 0.0f;

		// Se suman los scores de los partidos ganados
		SampleTourData::DataIter beatsIter = hostData->BeatsIter();		
		while (beatsIter.Next())
		{
			// parasiteData tiene info acerca de cuántos más lo vencen
			parasiteData = (SampleTourData*)beatsIter.Current()->evalData();

			// WIN_SCORE / (parasiteData->NLoses() * WIN_SCORE); // se simplifica WIN_SCORE
			score += 1.0f / parasiteData->NLoses();
			// TODO: probar score += WIN_SCORE / (WIN_SCORE * parasiteData->NLoses() + DRAW_SCORE * parasiteData->NDraws())
		}
			
		// Se suman los scores de los partidos empatados
		SampleTourData::DataIter drawsIter = hostData->DrawsIter();
		while (drawsIter.Next())
		{
			// parasiteData tiene info acerca de cuántos más lo vencen y empatan
			parasiteData = (SampleTourData*)drawsIter.Current()->evalData();
			score += DRAW_SCORE / (WIN_SCORE * parasiteData->NLoses() + WIN_SCORE * parasiteData->NDraws());
		}
		return score;
	}

	void SampleTourGA::GenerateNewPop()
	{
		int i;
		GAGenome *mom, *dad;          // tmp holders for selected genomes

		// Primero se pasan los mejores individuos sin cambios a la nueva población
		for(i = 0; i < ELITE_SIZE; i++)
		{
			pop->individual(i).copy(oldPop->individual(i));
		}

		for(i = ELITE_SIZE; i<pop->size()-1; i+=2)
		{	
			// Do selection
			mom = &(oldPop->select());  
			dad = &(oldPop->select());
			stats.numsel += 2;		// keep track of number of selections
			
			// Do crossover
			stats.numcro += (*scross)(*mom, *dad,
				&pop->individual(i), &pop->individual(i+1));

			// Do mutation
			stats.nummut += (pop->individual( i ).mutate(pMutation()));
			stats.nummut += (pop->individual(i+1).mutate(pMutation()));

			stats.numeval += 2;
		}
		// takes care of odd population
		if((pop->size() - ELITE_SIZE) % 2 != 0)
		{	// do the remaining population member
			mom = &(oldPop->select());  
			dad = &(oldPop->select());
			stats.numsel += 2;		// keep track of number of selections
			
			stats.numcro += (*scross)(*mom, *dad, &pop->individual(i), (GAGenome*)0);
			
			stats.nummut += (pop->individual( i ).mutate(pMutation()));

			stats.numeval += 1;
		}
	}

	void SampleTourGA::Save()
	{		
		ofstream file;
		file.open(STATS_FILE, ios::app);
		stats.write(file);
		file << "nWins: " << ((POP_SIZE * SAMP_SIZE) - nTies) << endl;
		file << "nDraws: " << nTies << endl;
		file << endl << endl;
		file.close();

		file.open(POP_FILE, ios::app);
		file << "nGen " << stats.generation() + INITIAL_GENERATION - 1 << endl;
		pop->write(file);
		file << endl;
		file.close();

		file.open(SAMP_FILE, ios::app);
		file << "nGen " << stats.generation() + INITIAL_GENERATION - 1  << endl;
		sample->write(file);
		file << endl;
		file.close();
		
		file.open(HOF_FILE, ios::out);
		//file << "nGen " << stats.generation() + INITIAL_GENERATION - 1  << endl;
		hall.Write(file);
		file << endl;
		file.close();

		file.open(HISTORY_FILE, ios::out);
		//file << "nGen " << stats.generation() + INITIAL_GENERATION - 1  << endl;
		history.Write(file);
		file << endl;
		file.close();
	}

	/** Formato del archivo a leer **/
	// nGen x
	// individuos de Pop
	//
	// individuos de Samp
	//
	// variable first de hall
	// variable last de hall
	// individuos de hall:
	//		generation
	//		genes
	//		puntajes: G E P Score
	//
	// individuos de history:
	//		generation
	//		genes
	//		puntajes: G E P Score
	/** ~Formato del archivo a leer **/
	GABoolean SampleTourGA::Load()
	{
		ifstream file;
		char buf[15];
		file.open(RECOVER_FILE, ios::in);
		if (file.fail()) return gaFalse;
		file.getline(&buf[0], 15);
		// Se salta la parte "nGen " y se lee directamente el número
		int nGen = atoi(&buf[5]);
		// ININITAL_GENERATION debe ser la generación donde se quedó
		INITIAL_GENERATION = nGen + 1;
		// Sólamente se harán las iteraciones restantes
		this->ngen -= nGen;

		// Se lee la población
		for (int i = 0; i < POP_SIZE; i++)
		{
			if (pop->individual(i).read(file) != 0)
				return gaFalse;
		}

		// Se lee la muestra
		for (int i = 0; i < SAMP_SIZE; i++)
		{
			if (sample->individual(i).read(file) != 0)
				return gaFalse;
		}

		// Se lee hall
		hall.Insert(pop->individual(0), 0);
		if (hall.Read(file) != 0)
			return gaFalse;
		
		// Se lee history
		history.Add(pop->individual(0));
		if (history.Read(file) !=0)
			return gaFalse;

		file.close();

		return gaTrue;
	}

	void SampleTourGA::InitTimeStats()
	{
		ofstream file;
		file.open(STATS_FILE, ios::app);
		char tmpbuf[128];
		_strdate_s(tmpbuf, 128);
		cout << "SampleTourGA\nHora inicio:" << endl;
		file << "SampleTourGA\nHora inicio:" << endl;
		cout << "\t" << tmpbuf;
		file << "\t" << tmpbuf;
		_strtime_s(tmpbuf, 128);
		cout << "\t" << tmpbuf << endl << endl;
		file << "\t" << tmpbuf << endl << endl;
		file.close();
	}

	void SampleTourGA::EndTimeStats()
	{
		ofstream file;
		file.open(STATS_FILE, ios::app);
		char tmpbuf[128];
		_strdate_s(tmpbuf, 128);
		cout << "Hora fin:" << endl;
		file << "Hora fin:" << endl;
		cout << "\t" << tmpbuf;
		file << "\t" << tmpbuf;
		_strtime_s(tmpbuf, 128);
		cout << "\t" << tmpbuf << endl;
		file << "\t" << tmpbuf << endl;
		double avgTime = 0;
		for (unsigned int i = 0; i < ngen; i++)
		{
			avgTime += stepTimes[i];
		}
		double totTime = (double)endTime / CLOCKS_PER_SEC;

		long hh, mm, ss;
		hh = (long)totTime / (long)3600;
		mm = (long)totTime / (long)60 - (long)60 * hh;
		ss = (long)totTime % (long)60;
		cout << "Tiempo total:" << endl << "\t";
		file << "Tiempo total:" << endl << "\t";
		cout << hh << ":" << (mm < 10 ? "0" : "") << mm << ":" << (ss < 10 ? "0" : "") << ss << endl;
		file << hh << ":" << (mm < 10 ? "0" : "") << mm << ":" << (ss < 10 ? "0" : "") << ss << endl;

		avgTime = avgTime / ngen;
		hh = (long)avgTime / (long)3600;
		mm = (long)avgTime / (long)60 - (long)60 * hh;
		ss = (long)avgTime % (long)60;
		cout << "Tiempo promedio por generacion:" << endl << "\t";
		file << "Tiempo promedio por generacion:" << endl << "\t";
		cout << hh << ":" << (mm < 10 ? "0" : "") << mm << ":" << (ss < 10 ? "0" : "") << ss << endl;
		file << hh << ":" << (mm < 10 ? "0" : "") << mm << ":" << (ss < 10 ? "0" : "") << ss << endl;
		file.close();
	}

	void SampleTourGA::Backup()
	{
		ofstream file;
		file.open(RECOVER_FILE, ios::out);
		file << "nGen " << stats.generation() + INITIAL_GENERATION - 1 << endl;
		pop->write(file);
		sample->write(file);
		hall.Write(file);
		history.Write(file);
		file.close();
	}
}
