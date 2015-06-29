// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "MastergoalAI.h"
#include "MastergoalTablero.h"
#include "MastergoalFicha.h"
#include "MastergoalCasilla.h"
#include "../../EvoGoal/include/Agent.h"
#include "../../EvoGoal/include/State.h"
#include "../../EvoGoal/include/Board.h"
#include "../../EvoGoal/include/Square.h"
#include "../../EvoGoal/include/Player.h"
#include "../../EvoGoal/include/Play.h"
#include "../../EvoGoal/include/Movement.h"
#include "../../EvoGoal/include/PlayerMovement.h"
#include "../../EvoGoal/include/BallMovement.h"
#include "../../EvoGoal/include/MovementIterator.h"

MastergoalAI::MastergoalAI(int32 Nivel, int32 Alto, int32 Ancho)
{
	this->Nivel = Nivel;

	this->Alto = Alto - 1;
	this->Ancho = Ancho - 1;

	this->Acciones = nullptr;
	this->AccionActual = 0;
	this->CantidadAcciones = 0;

	this->AgentePC = new Agent(Nivel, RIGHT, 2, "PC");
	this->Tablero = new Board(Nivel);
	this->Estado = new State(*(this->Tablero), RIGHT);
}

MastergoalAI::~MastergoalAI()
{
	if (this->AgentePC)
	{
		delete this->AgentePC;
		this->AgentePC = nullptr;
	}
	if (this->Tablero)
	{
		delete this->Tablero;
		this->Tablero = nullptr;
	}
	if (this->Estado)
	{
		delete this->Estado;
		this->Estado = nullptr;
	}
	if (this->Acciones)
	{
		delete[] this->Acciones;
		this->Acciones = nullptr;
	}
}

void MastergoalAI::ActualizarTablero(Common::Board* Tablero, TArray<class AMastergoalFicha*> Fichas)
{
	ORIENTATION Orientacion;
	PLAYER_ID Contadores[2];
	int ContadorObjetivo;

	// Escribir la posición de las fichas
	for (auto Iter(Fichas.CreateIterator()); Iter; Iter++)
	{
		if (!(*Iter)->IsValidLowLevel()) continue;

		Orientacion = (*Iter)->Equipo == BLANCO ? LEFT : RIGHT;
		ContadorObjetivo = (*Iter)->Equipo == BLANCO ? 0 : 1;
		
		// Definir la posición de la ficha dependiendo del tipo
		if ((*Iter)->Tipo == PELOTA)
		{
			this->Tablero->SetBallPosition(Alto - (*Iter)->Fila, Ancho - (*Iter)->Columna);
			this->Pelota = (*Iter);
		}
		else if ((*Iter)->Arquero)
		{
			this->Tablero->SetPlayerPosition(Orientacion, ID_G, Alto - (*Iter)->Fila, Ancho - (*Iter)->Columna);
		}
		else
		{
			this->Tablero->SetPlayerPosition(Orientacion, Contadores[ContadorObjetivo], Alto - (*Iter)->Fila, Ancho - (*Iter)->Columna);
		}

		// Incrementar el contador de jugadores para el equipo
		Contadores[ContadorObjetivo] = static_cast<PLAYER_ID>(Contadores[ContadorObjetivo] + 1);
	}
}

void MastergoalAI::ActualizarEstado(Common::Board* Tablero)
{
	if (this->Estado)
	{
		delete this->Estado;
		this->Estado = nullptr;
	}
	this->Estado = new State(static_cast<const Board>(*Tablero), RIGHT, 0);
}

void MastergoalAI::Actualizar(Common::Board* Tablero, TArray<class AMastergoalFicha*> Fichas)
{
	ActualizarTablero(Tablero, Fichas);
	ActualizarEstado(Tablero);
}

Accion MastergoalAI::Jugar(class AMastergoalCasilla* Casillas[15][11], TArray<class AMastergoalFicha*> Fichas)
{
	Actualizar(this->Tablero, Fichas);

	// Obtener la jugada
	Play* Jugada = AgentePC->MakePlay(*(this->Estado));

	UE_LOG(LogTemp, Warning, TEXT("Play found. N:%d"), Jugada->GetNMovements());

	// Reiniciar la lista de acciones
	if (this->Acciones != nullptr)
	{
		delete[] this->Acciones;
	}
	CantidadAcciones = Jugada->GetNMovements();
	this->Acciones = new Accion[CantidadAcciones];
	
	// Poblar la lista de acciones
	MovementIterator Iter = Jugada->GetMovementsIterator();
	Iter.Next(); 
	Iter.Next();

	const Movement* Movimiento;
	AccionActual = 0;
	while ((Movimiento = Iter.NextBallMovement()) != nullptr)
	{
		this->Acciones[AccionActual++] = ConvertirMovimientoEnAccion(Movimiento, Casillas, Pelota);
	}
	AccionActual = 0;

	// Retornar la primera acción
	return ConvertirMovimientoEnAccion(Jugada->GetFirstMovement(), Casillas, nullptr);
}

Accion MastergoalAI::ProximaJugada()
{
	return Acciones[AccionActual++];
}

Accion MastergoalAI::ConvertirMovimientoEnAccion(const Movement* Movimiento, class AMastergoalCasilla* Casillas[15][11], AMastergoalFicha* Ficha)
{
	int32 FilaAux;
	int32 ColumnaAux;

	Accion MovimientoFicha;
	MovimientoFicha.Fila = -1;
	MovimientoFicha.Columna = -1;
	MovimientoFicha.Ficha = nullptr;

	const Square* CasillaAnterior = Movimiento->GetSquareBefore();
	const Square* CasillaObjetivo = Movimiento->GetSquareAfter();

	// Definir las posiciones de la acción
	MovimientoFicha.Fila = Alto - CasillaObjetivo->GetRow();
	MovimientoFicha.Columna = Ancho - CasillaObjetivo->GetCol();

	// Obtener la ficha que realizó el movimiento
	FilaAux = Alto - CasillaAnterior->GetRow();
	ColumnaAux = Ancho - CasillaAnterior->GetCol();
	if (Ficha == nullptr && Casillas[FilaAux][ColumnaAux]->Ficha != nullptr)
	{
		MovimientoFicha.Ficha = Casillas[FilaAux][ColumnaAux]->Ficha;
	}
	else if (Ficha != nullptr)
	{
		MovimientoFicha.Ficha = Ficha;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AI attempted to select a Piece that doesn't exist! R:%d C:%d"), FilaAux, ColumnaAux);
	}

	return MovimientoFicha;
}