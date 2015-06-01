#include "stdafx.h"
#include "SampParTournament.h"
#include "Game.h"
using namespace GAParallel;

namespace AI
{
	SampParTournament::SampParTournament(GAPopulation *pop, GAPopulation *sample) : Tournament()
	{
		this->pop = pop;
		this->sample = sample;

		this->agentL = new Agent(GAME_LEVEL, LEFT, "agentL");
		this->agentR = new Agent(GAME_LEVEL, RIGHT, "agentR");

		for (int i = 0; i < MAX_PROCS; i++)
		{
			completedTasks[i] = true;
		}

		this->aGenomeSetter = RealAgentSetter;
	}

	SampParTournament::SampParTournament(const SampParTournament &tournament) : Tournament()
	{
		this->pop = tournament.pop;
		this->sample = tournament.sample;

		this->agentL = new Agent(GAME_LEVEL, LEFT, "agentL");
		this->agentR = new Agent(GAME_LEVEL, RIGHT, "agentR");

		this->aGenomeSetter = tournament.aGenomeSetter;

		for (int i = 0; i < MAX_PROCS; i++)
		{
			completedTasks[i] = tournament.completedTasks[i];
		}
	}

	SampParTournament::~SampParTournament()
	{
		if (agentL) { delete agentL; agentL = NULL; }
		if (agentR) { delete agentR; agentR = NULL; }
	}

	void SampParTournament::Play()
	{
		// Se inicializa el score de la muestra
		// El score contendrá la cantidad de puntos ganados en los partidos (WIN_SCORE pts por cada victoria, DRAW_SCORE pts por cada empate)
		// j comienza en MINI_SAMP_SIZE porque los individuos ya compitieron contra los anteriores a MINI_SAMP_SIZE en BroodCrossover
		for (int j = MINI_SAMP_SIZE; j < SAMP_SIZE; j++)
		{
			SampleTourData* parasiteData = (SampleTourData*)sample->individual(j).evalData();
			parasiteData->Reset();
		}
		for (int i = 0; i < pop->size(); i++)
		{
			// Se inicializa el score del individuo
			SampleTourData* hostData = (SampleTourData*)pop->individual(i).evalData();
			// Se resetean los datos de a quiénes le gana el host
			hostData->Reset();
		}

		char task_msg[MAX_MSG];
		int nSend = 0, nRecv = 0;

		ifstream configFile;
		configFile.open("clientConfig.txt", ios::in);
		if (configFile.fail())
		{
			cout << "Error al leer el archivo \"clientConfig\"" << endl;
			getchar();
			exit(-1);
		}
		int nProcs = 0;
		configFile >> nProcs;
		if (configFile.fail())
		{
			cout << "Error al leer nProcs" << endl;
			getchar();
			exit(-1);
		}
		cout << "\t" << nProcs << " procesadores disponibles" << endl;

		u_short resultPort;
		configFile >> resultPort;
		if (configFile.fail())
		{
			cout << "Error al leer resultPort" << endl;
			getchar();
			exit(-1);
		}

		// Se hace listen ya para las respuestas
		SOCKET resultSocket;
		if (ListenOnPort(resultPort, resultSocket) == -1)
		{
			cout << "Error al escuchar puerto de resultados" << endl;
			getchar();
			exit(-1);
		}
		cout << "\tEscuchando respuestas en el puerto " << resultPort << "..." << endl;

		for (int i = 0; i < MAX_PROCS; i++)
		{	
			memset(hostIPs, 0x0, 50);
		}
		
		for (int i = 0; i < nProcs; i++)
		{
			char aux;
			int l = 0;
			configFile >> aux;
			while (aux != ':')
			{
				
				if (configFile.fail())
				{
					cout << "Error al leer ipAddress en configFile" << endl;
					getchar();
					exit(-1);
				}
				hostIPs[i][l++] = aux;
				configFile >> aux;
			}
			hostIPs[i][l] = '\0';
			
			configFile >> hostPorts[i];
			if (configFile.fail())
			{
				cout << "Error al leer port en configFile" << endl;
				getchar();
				exit(-1);
			}
		}
		configFile.close();

		int iIni = 0, jIni =0, nGames = N_PARALLEL_GAMES, doubleMatch = 0;
#ifdef DOUBLE_MATCH
			doubleMatch = 1;
#endif
		for (int i = 0; i < nProcs; i++)
		{
			// TODO: crearTask			
			CreateTask(iIni, doubleMatch,*pop, *sample, task_msg, MAX_MSG);
			if (SendTask(task_msg, hostPorts[i], hostIPs[i]) == -1)
			{
				cout << "Error al enviar tarea!" << endl;
				getchar();
				exit(-1);
			}
			nSend++;
			//cout << "\tTarea enviada a " << hostIPs[i] << ":" << hostPorts[i] << endl;
			//cout << "\tTamanho de tarea: " << (strlen(task_msg) + 1) << endl;
			completedTasks[i] = false;
		}
		
		bool done = false;

		SOCKADDR_IN serverAddress;
		int serverAddressLen = sizeof(serverAddress);
		SOCKET serverSocket = INVALID_SOCKET;
		char result_msg[RESULT_SIZE];
		while (!done)
		{
			// Recibir resultados
			serverSocket = accept(resultSocket, (struct sockaddr*)&serverAddress, &serverAddressLen);
			if (serverSocket == INVALID_SOCKET)
			{
				cout << "No se pudo crear clientSocket" << endl;
				closesocket(resultSocket);
				WSACleanup();
				getchar();
				exit(-1);
			}

			// Se reciben los resultados de los individuos
			memset(result_msg, 0x0, RESULT_SIZE);
			int resultLen = recv(serverSocket, result_msg, RESULT_SIZE, 0);
			if (resultLen <= 0)
			{
				cout << "Error al leer datos recibidos" << endl;
				closesocket(serverSocket);
				WSACleanup();
				getchar();
				exit(-1);
			}

			char auxBuf[MAX_MSG];
			int iResult;
			do {

				iResult = recv(serverSocket, auxBuf, MAX_MSG, 0);
				if ( iResult > 0 )
				{
					printf("Bytes received: %d\n", iResult);
					/*ofstream ff;
					ff.open("debugClient.txt", ios::app);
					ff << result_msg << endl << " --- " << endl;
					ff << auxBuf << endl;
					ff << "Total de bytes recibidos: " << (resultLen + iResult) << endl;
					ff << "RESULT_SIZE = " << RESULT_SIZE << endl;
					ff << "resultLen = " << resultLen << endl;
					ff << "iResult = " << iResult << endl;
					ff.close();*/
					strcat_s(result_msg, RESULT_SIZE, auxBuf);
					resultLen += iResult;
				}
				else if ( iResult == 0 )
				{
					//printf("Connection closed\n");
				}
				else
					printf("recv failed: %d\n", WSAGetLastError());

			} while( iResult > 0 );

			closesocket(serverSocket);
			nRecv++;
			int c = 0;
			u_short hostPort = 0;
			char hostIp[20];
			c = GetIpAndHost(result_msg, hostIp, hostPort);

			//cout << "\tRespuesta recibida de " << hostIp << ":" << hostPort << endl;		
			int procId = FindProcId(hostIp, hostPort, nProcs);
			//cout << procId << "-";
			if (procId == -1)
			{
				cout << "Error en FindProcId" << endl;
				getchar();
				exit(-1);
			}
			completedTasks[procId] = true;

			// Se actualiza quién vence a quién
			UpdateResults(result_msg, (int)strlen(result_msg), c);

			if (nSend == nRecv && iIni == pop->size())
				done = true;
			else if (iIni < pop->size())
			{
				CreateTask(iIni, doubleMatch, *pop, *sample, task_msg, MAX_MSG);

				if (SendTask(task_msg, hostPort, hostIp) == -1)
				{
					cout << "Error al enviar tarea!" << endl;
					getchar();
					exit(-1);
				}
				nSend++;
				//cout << "\tTarea enviada a " << hostIp << ":" << hostPort << endl;
				//cout << "\tTamanho de tarea: " << (strlen(task_msg) + 1) << endl;
				completedTasks[procId] = false;
			}
			else
			{
				cout << "\n\tRespuesta recibida de " << procId << endl;
			}
		}
		closesocket(resultSocket);
		cout << "\tGeneracion terminada!!!" << endl;
	}


	void SampParTournament::Play(int &nTies)
	{
		// Se inicializa el score de la muestra
		// El score contendrá la cantidad de puntos ganados en los partidos (WIN_SCORE pts por cada victoria, DRAW_SCORE pts por cada empate)
		// j comienza en MINI_SAMP_SIZE porque los individuos ya compitieron contra los anteriores a MINI_SAMP_SIZE en BroodCrossover
		for (int j = MINI_SAMP_SIZE; j < SAMP_SIZE; j++)
		{
			SampleTourData* parasiteData = (SampleTourData*)sample->individual(j).evalData();
			parasiteData->Reset();
		}
		for (int i = 0; i < pop->size(); i++)
		{
			// Se inicializa el score del individuo
			SampleTourData* hostData = (SampleTourData*)pop->individual(i).evalData();
			// Se resetean los datos de a quiénes le gana el host
			hostData->Reset();
		}

		char task_msg[MAX_MSG];
		int nSend = 0, nRecv = 0;

		ifstream configFile;
		configFile.open("clientConfig.txt", ios::in);
		if (configFile.fail())
		{
			cout << "Error al leer el archivo \"clientConfig\"" << endl;
			getchar();
			exit(-1);
		}
		int nProcs = 0;
		configFile >> nProcs;
		if (configFile.fail())
		{
			cout << "Error al leer nProcs" << endl;
			getchar();
			exit(-1);
		}
		cout << "\n\t" << nProcs << " procesos disponibles" << endl;

		u_short resultPort;
		configFile >> resultPort;
		if (configFile.fail())
		{
			cout << "Error al leer resultPort" << endl;
			getchar();
			exit(-1);
		}

		// Se hace listen ya para las respuestas
		SOCKET resultSocket;
		if (ListenOnPort(resultPort, resultSocket) == -1)
		{
			cout << "Error al escuchar puerto de resultados" << endl;
			getchar();
			exit(-1);
		}
		//cout << "\tEscuchando respuestas en el puerto " << resultPort << "..." << endl;

		for (int i = 0; i < MAX_PROCS; i++)
		{	
			memset(hostIPs, 0x0, 50);
		}
		
		for (int i = 0; i < nProcs; i++)
		{
			char aux;
			int l = 0;
			configFile >> aux;
			while (aux != ':')
			{
				
				if (configFile.fail())
				{
					cout << "Error al leer ipAddress en configFile" << endl;
					getchar();
					exit(-1);
				}
				hostIPs[i][l++] = aux;
				configFile >> aux;
			}
			hostIPs[i][l] = '\0';
			
			configFile >> hostPorts[i];
			if (configFile.fail())
			{
				cout << "Error al leer port en configFile" << endl;
				getchar();
				exit(-1);
			}
		}
		configFile.close();

		int iIni = 0, jIni =0, nGames = N_PARALLEL_GAMES, doubleMatch = 0;
#ifdef DOUBLE_MATCH
			doubleMatch = 1;
#endif
		for (int i = 0; i < nProcs; i++)
		{
			// TODO: crearTask
			CreateTask(iIni, doubleMatch,*pop, *sample, task_msg, MAX_MSG);
			if (SendTask(task_msg, hostPorts[i], hostIPs[i]) == -1)
			{
				cout << "Error al enviar tarea!" << endl;
				getchar();
				exit(-1);
			}
			nSend++;
			//cout << "\tTarea enviada a " << hostIPs[i] << ":" << hostPorts[i] << endl;
			//cout << "\tTamanho de tarea: " << (strlen(task_msg) + 1) << endl;
			completedTasks[i] = false;
		}
		
		bool done = false;

		SOCKADDR_IN serverAddress;
		int serverAddressLen = sizeof(serverAddress);
		SOCKET serverSocket = INVALID_SOCKET;
		char result_msg[RESULT_SIZE];
		while (!done)
		{
			// Recibir resultados
			serverSocket = accept(resultSocket, (struct sockaddr*)&serverAddress, &serverAddressLen);
			if (serverSocket == INVALID_SOCKET)
			{
				cout << "No se pudo crear clientSocket" << endl;
				closesocket(resultSocket);
				WSACleanup();
				getchar();
				exit(-1);
			}

			// Se reciben los resultados de los individuos
			memset(result_msg, 0x0, RESULT_SIZE);
			int resultLen = recv(serverSocket, result_msg, RESULT_SIZE, 0);
			if (resultLen <= 0)
			{
				cout << "Error al leer datos recibidos" << endl;
				closesocket(serverSocket);
				WSACleanup();
				getchar();
				exit(-1);
			}

			char auxBuf[MAX_MSG];
			int iResult;
			do {

				iResult = recv(serverSocket, auxBuf, MAX_MSG, 0);
				if ( iResult > 0 )
				{
					printf("Bytes received: %d\n", iResult);
					/*ofstream ff;
					ff.open("debugClient.txt", ios::app);
					ff << result_msg << endl << " --- " << endl;
					ff << auxBuf << endl;
					ff << "Total de bytes recibidos: " << (resultLen + iResult) << endl;
					ff << "RESULT_SIZE = " << RESULT_SIZE << endl;
					ff << "resultLen = " << resultLen << endl;
					ff << "iResult = " << iResult << endl;
					ff.close();*/
					strcat_s(result_msg, RESULT_SIZE, auxBuf);
					cout << "RSIZE: " << RESULT_SIZE << " result_msg: " << strlen(result_msg) << " auxBuf: " << strlen(auxBuf);
					resultLen += iResult;
				}
				else if ( iResult == 0 )
				{
					//printf("Connection closed\n");
				}
				else
					printf("recv failed: %d\n", WSAGetLastError());

			} while( iResult > 0 );

			closesocket(serverSocket);
			nRecv++;
			int c = 0;
			u_short hostPort = 0;
			char hostIp[20];
			c = GetIpAndHost(result_msg, hostIp, hostPort);

			//cout << "\tRespuesta recibida de " << hostIp << ":" << hostPort << endl;
			int procId = FindProcId(hostIp, hostPort, nProcs);
			cout << procId << "-";
			if (procId == -1)
			{
				cout << "Error en FindProcId" << endl;
				getchar();
				exit(-1);
			}
			completedTasks[procId] = true;

			// Se actualiza quién vence a quién
			if (UpdateResults(result_msg, (int)strlen(result_msg), c, nTies) == -1)
			{
				cout << "Error al UpdateResults" << endl;
				getchar();
				exit(-1);
			}

			if (nSend == nRecv && iIni == pop->size())
				done = true;
			else if (iIni < pop->size())
			{
				if (hostPort != 1604)
				{
				CreateTask(iIni, doubleMatch, *pop, *sample, task_msg, MAX_MSG);

				if (SendTask(task_msg, hostPort, hostIp) == -1)
				{
					cout << "Error al enviar tarea a " << inet_addr(hostIp) << ":" << hostPort << endl;
					continue;
				}
				nSend++;
				//cout << "\tTarea enviada a " << hostIp << ":" << hostPort << endl;
				//cout << "\tTamanho de tarea: " << (strlen(task_msg) + 1) << endl;
				completedTasks[procId] = false;
				}
			}
			/*else
			{
				cout << "\n\tRespuesta recibida de " << hostIp << ":" << hostPort << endl;
			}*/
		}
		closesocket(resultSocket);
		cout << "\tGeneracion terminada!!!" << endl;
	}	

	int SampParTournament::SendTask(char* task, u_short portNo, char* ipAddress)
	{
		WSADATA w;
		// Initialize Winsock
		if (WSAStartup(MAKEWORD(2,2), &w) != 0) {
			cout << "WSAStartup failed" << endl;
			return -1;
		}

		SOCKADDR_IN target; //Socket address information
		target.sin_family = AF_INET; // address family Internet
		target.sin_port = htons (portNo); //Port to connect on
		target.sin_addr.s_addr = inet_addr (ipAddress); //Target IP

		SOCKET s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
		if (s == INVALID_SOCKET)
		{
			cout << "Error al crear socket" << endl;
			WSACleanup();
			return -1; //Couldn't create the socket
		}

		if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
		{
			cout << "Error al conectar con " << ipAddress << ":" << portNo << endl;
			closesocket(s);
			WSACleanup();
			return -1; //Couldn't create the socket
		}
		//sendto(s, task, (int)strlen(task) + 1, 0, (SOCKADDR *)&target, sizeof(target));
		// Send buffer
		if (send(s, task, (int)strlen(task) + 1, 0) == SOCKET_ERROR) 
		{
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(s);
			WSACleanup();
			return -1;
		}

		//cout << "\tDatos enviados a " << ipAddress << ":" << portNo << endl;
		
		// shutdown the connection since no more data will be sent
		if (shutdown(s, SD_SEND) == SOCKET_ERROR) 
		{
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(s);
			WSACleanup();
			return -1;
		}

		// Receive until the peer closes the connection
		int iResult;
		char auxBuf[MAX_MSG];
		do {

			iResult = recv(s, auxBuf, MAX_MSG, 0);
			if ( iResult > 0 )
				printf("Bytes received: %d\n", iResult);
			else if ( iResult == 0 )
			{
				//printf("Connection closed\n");
			}
			else
				printf("recv failed: %d\n", WSAGetLastError());

		} while( iResult > 0 );

		// cleanup
		closesocket(s);
		WSACleanup();

		return 0;
	}

	int SampParTournament::ListenOnPort(u_short listenPort, SOCKET &s)
	{
		// Initialize Winsock
		WSADATA w;
		if (WSAStartup(MAKEWORD(2,2), &w) != 0) {
			cout << "WSAStartup failed" << endl;
			getchar();
			return -1;
		}

		s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s == INVALID_SOCKET)
		{
			cout << "No se pudo crear listenSocket" << endl;
			WSACleanup();
			getchar();
			return -1;
		}

		SOCKADDR_IN hostAddress;
		hostAddress.sin_family = AF_INET;
		hostAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		hostAddress.sin_port = htons(listenPort);

		if (bind(s, (LPSOCKADDR)&hostAddress, sizeof(hostAddress)) == SOCKET_ERROR)
		{
			cout << "Error en bind de listenSocket" << endl;
			closesocket(s);
			WSACleanup();
			getchar();
			return -1;
		}

		listen(s, SOMAXCONN);
		return 0;
	}

	int SampParTournament::GetIpAndHost(char* hdr, char* ip, u_short &port)
	{
		char aux[50];
		int c = 0;
		while (hdr[c] != ':')
		{
			ip[c] = hdr[c];
			c++;
		}
		ip[c++] = '\0';

		int i = 0;
		while (hdr[c] != ';')
		{
			aux[i] = hdr[c];
			i++;
			c++;
		}
		aux[i] = '\0';
		port = (u_short) atoi(aux);
		return ++c;
	}

	int SampParTournament::UpdateResults(char *resultsMsg, int msgLen, int &c)
	{
		int iI, nInd;
		if (GetHeader(iI, nInd, resultsMsg, msgLen, c) == -1) return -1;

		for (int i = iI; i < iI + nInd; i++)
		{
			GAGenome& host = pop->individual(i);
			SampleTourData* hostData = (SampleTourData*)host.evalData();			

			for (int j = MINI_SAMP_SIZE; j < SAMP_SIZE; j++)
			{
				GAGenome& parasite = sample->individual(j);
				SampleTourData* parasiteData = (SampleTourData*)parasite.evalData();

				int result = GetNextResult(resultsMsg, msgLen, c);
				if (result == WON_H)
				{
					// Se guarda que host vence a parasite
					hostData->Beats(&parasite);
					// Se guarda que parasite es vencido por host
					parasiteData->Loses(&host);
				}
				else if (result == WON_P)
				{
					// Se guarda que parasite vence a host
					parasiteData->Beats(&host);
					// Se guarda que host es vencido por parasite
					hostData->Loses(&parasite);
				}
				else
				{
					// Se guarda que host empata con parasite
					hostData->Draws(&parasite);
					// Se guarda que parasite empata con host
					parasiteData->Draws(&host);
				}

#ifdef DOUBLE_MATCH
				result = GetNextResult(resultsMsg, msgLen, c);

				if (result == WON_H)
				{
					// Se guarda que host vence a parasite
					hostData->Beats(&parasite);
					// Se guarda que parasite es vencido por host
					parasiteData->Loses(&host);
				}
				else if (result == WON_P)
				{
					// Se guarda que parasite vence a host
					parasiteData->Beats(&host);
					// Se guarda que host es vencido por parasite
					hostData->Loses(&parasite);
				}
				else
				{
					// Se guarda que host empata con parasite
					hostData->Draws(&parasite);
					// Se guarda que parasite empata con host
					parasiteData->Draws(&host);
				}
#endif
			}
		}

		if (c != msgLen)
			return -1;
		return 0;
	}

	int SampParTournament::UpdateResults(char *resultsMsg, int msgLen, int &c, int &nTies)
	{
		int iI, nInd;
		if (GetHeader(iI, nInd, resultsMsg, msgLen, c) == -1) return -1;

		for (int i = iI; i < iI + nInd; i++)
		{
			GAGenome& host = pop->individual(i);
			SampleTourData* hostData = (SampleTourData*)host.evalData();			

			for (int j = MINI_SAMP_SIZE; j < SAMP_SIZE; j++)
			{
				GAGenome& parasite = sample->individual(j);
				SampleTourData* parasiteData = (SampleTourData*)parasite.evalData();

				int result = GetNextResult(resultsMsg, msgLen, c);
				if (result == WON_H)
				{
					// Se guarda que host vence a parasite
					hostData->Beats(&parasite);
					// Se guarda que parasite es vencido por host
					parasiteData->Loses(&host);
				}
				else if (result == WON_P)
				{
					// Se guarda que parasite vence a host
					parasiteData->Beats(&host);
					// Se guarda que host es vencido por parasite
					hostData->Loses(&parasite);
				}
				else
				{
					nTies++;
					// Se guarda que host empata con parasite
					hostData->Draws(&parasite);
					// Se guarda que parasite empata con host
					parasiteData->Draws(&host);
				}

#ifdef DOUBLE_MATCH
				result = GetNextResult(resultsMsg, msgLen, c);

				if (result == WON_H)
				{
					// Se guarda que host vence a parasite
					hostData->Beats(&parasite);
					// Se guarda que parasite es vencido por host
					parasiteData->Loses(&host);
				}
				else if (result == WON_P)
				{
					// Se guarda que parasite vence a host
					parasiteData->Beats(&host);
					// Se guarda que host es vencido por parasite
					hostData->Loses(&parasite);
				}
				else
				{
					nTies++;
					// Se guarda que host empata con parasite
					hostData->Draws(&parasite);
					// Se guarda que parasite empata con host
					parasiteData->Draws(&host);
				}
#endif
			}
		}

		if (c != msgLen)
			return -1;
		return 0;
	}

	int SampParTournament::GetHeader(int &iI, int &nInd, char *resultsMsg, int msgLen, int &c)
	{
		char aux[20];
		int l = 0;
		while (resultsMsg[c] != ';')
		{
			aux[l] = resultsMsg[c];
			l++;
			c++;
		}
		aux[l] = '\0';
		iI = atoi(aux);

		c++;
		l = 0;
		while (resultsMsg[c] != ';')
		{
			aux[l] = resultsMsg[c];
			l++;
			c++;
		}
		aux[l] = '\0';
		nInd = atoi(aux);

		c++;

		if (c > msgLen)
			return -1;

		return 0;
	}

	int SampParTournament::GetNextResult(char *resultsMsg, int msgLen, int &c)
	{
		char aux[2];
		int l = 0;

		while (resultsMsg[c] != ';')
		{
			aux[l] = resultsMsg[c];
			l++;
			c++;
			if (c > msgLen) return -1;
		}
		c++;
		aux[1] = '\0';
		return atoi(aux);
	}

	int SampParTournament::FindProcId(char* ip, u_short nProc, int nProcesors)
	{
		for (int i = 0; i < nProcesors; i++)
		{
			if (nProc == hostPorts[i] && (strcmp(ip, hostIPs[i]) == 0))
			{
				return i;
			}
		}
		return -1;
	}
}
