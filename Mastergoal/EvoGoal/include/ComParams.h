#ifndef COM_PARAMS_H
#define COM_PARAMS_H

#include "Definitions.h"

#define N_HOSTS_TO_SEND 40

// Cantidad de juegos que ser�n enviados a cada procesador
#define N_PARALLEL_GAMES (N_HOSTS_TO_SEND * SAMP_SIZE)

// N�mero m�ximo de procesadores disponibles
#define MAX_PROCS 50

// HEADER_SIZE + (POP_SIZE + SAMP_SIZE) * N_FEATURES * WEIGHT_LEN + '\0'
#define MAX_MSG (100 + (N_HOSTS_TO_SEND + SAMP_SIZE) * 24 * 12 + 1)

// RESULT_HEADER_SIZE + TOUR_DATA_MULT * (N_PARALLEL_GAMES + ';') + '\0'
#define RESULT_SIZE (110 + 2 * TOUR_DATA_MULT * N_PARALLEL_GAMES + 1)

#define WON_H 1

#define WON_P 2

#define TIED_H_P 0

#endif
