#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <fstream>
using namespace std;

// Cuando est� descomentado lee un archivo para recuperar el estado actual
//#define RESTORE

// Si se juegan dos partidos o solamente uno (eligiendo el que comienza al azar)
#define DOUBLE_MATCH

// Si se deben evaluar los estados o las acciones
//#define EVAL_ACTIONS

// Tama�o de la poblaci�n
#define POP_SIZE 300

// Tama�o de la poblaci�n de muestra (~=10% POP_SIZE) 
#define SAMP_SIZE 30

// Cantidad de individuos seleccionados del sal�n de la fama
#define N_HALL_OF_FAME 15

// Cantidad de individuos a ser considerados en el elitismo (~= 2,5% POP_SIZE)
#define ELITE_SIZE 3

// N�mero de generaciones a correr
#define N_GENERATIONS 300

// Tama�o del sal�n de la fama
#define HALL_SIZE 1002

// Tama�o de la muestra para el mini test de brood crossover
#define MINI_SAMP_SIZE 0

// Cantidad de hijos que tendr� cada pareja en el cruzammiento
#define N_OFFSPRINGS 2

// N�mero m�ximo de jugadas en un partido
#define MAX_PLAYS 40

// Nivel de juego por defecto
#define GAME_LEVEL 1

// Profundidad de b�squeda del algoritmo minimax de los agentes
#define MINIMAX_DEPTH 1

// Probabilidad de mutaci�n
#define P_MUT 0.2f

// Score por ganar un partido
#define WIN_SCORE 3.0f

// Score por empatar un partido
#define DRAW_SCORE 1.0f

// M�ximo valor posible para un estado (gol)
#define EVOGOAL_INFINITY 200000.0f

// M�nimo valor posible para un peso
#define MIN_WEIGHT -100.0f

// M�ximo valor posible para un peso
#define MAX_WEIGHT 100.0f

// Archivo donde se guardar� las caracter�sticas del programa (las constantes definidas en Definitions.h)
#define CONFIG_FILE "config.txt"

// Archivo donde se guardar�n las estad�sticas del algoritmo
#define STATS_FILE "stats.aux"

// Archivo donde se guardar� el estado del Hall of Fame
#define HOF_FILE "hof.aux"

// Archivo donde se guardar� el historial History
#define HISTORY_FILE "history.aux"

// Archivo donde se guardar� el estado de la poblaci�n actual
#define POP_FILE "pop.aux"

// Archivo donde se guardar� el estado de la poblaci�n de muestra (sample)
#define SAMP_FILE "samp.aux"

// Archivo de donde se recuperar� el proceso anterior interrumpido
#define RECOVER_FILE "recov.aux"

// Cada cu�ntas generaciones debe guardarse el estado del GA
#define SAVE_FREQ 1

// Cada cu�ntas generaciones hace un backup del estado
#define BACK_FREQ 1

#ifdef DOUBLE_MATCH
	// Si se juegan dos partidos es 2, si se juega 1 es 1
	#define TOUR_DATA_MULT 2
#else
	#define  TOUR_DATA_MULT 1
#endif

// GAIDs
#define GAID_SAMPLE_TOUR_GA 201
#define GAID_SAMPLE_SCALING 202

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(a) ((a) >= 0 ? (a) : (-(a)))
#endif

#ifndef SWAP
#define SWAP(Type, a, b) { Type aux = a; a = b; b = aux; }
#endif

void WriteConfig();

int GetNFeatures();

#endif
