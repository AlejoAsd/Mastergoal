// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MastergoalFicha.h"
#include "../../EvoGoal/include/Agent.h"
#include "../../EvoGoal/include/State.h"
#include "../../EvoGoal/include/Board.h"

typedef struct Accion
{
	int32 Fila = -1;
	int32 Columna = -1;
	AMastergoalFicha* Ficha = NULL;
} Accion;

/**
 * 
 */
class MASTERGOAL_API MastergoalAI
{
public:
	// Propiedades
	int32 Nivel;
	int32 Alto;
	int32 Ancho;
	class AMastergoalFicha* Pelota;

	class Agent* AgentePC;
	class State* Estado;
	class Board* Tablero;
	Accion* Acciones;
	int32 AccionActual;
	int32 CantidadAcciones;

	// Constructor/Destructor
	MastergoalAI(int Nivel = 3, int32 Alto = 15, int32 Ancho = 11);
	~MastergoalAI();

	Accion Jugar(class AMastergoalCasilla* Casillas[15][11], TArray<class AMastergoalFicha*> Fichas);
	Accion ProximaJugada();

	inline const bool ExistenJugadas() { return AccionActual < CantidadAcciones; }

private:
	void ActualizarTablero(Common::Board* Tablero, TArray<class AMastergoalFicha*> Fichas);
	void ActualizarEstado(Common::Board* Tablero);
	void Actualizar(Common::Board* Tablero, TArray<class AMastergoalFicha*> Fichas);
	Accion ConvertirMovimientoEnAccion(const Movement* Movimiento, class AMastergoalCasilla* Casillas[15][11], AMastergoalFicha* Ficha);
};