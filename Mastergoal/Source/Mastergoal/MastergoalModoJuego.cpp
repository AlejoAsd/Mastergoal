// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "MastergoalModoJuego.h"

AMastergoalModoJuego::AMastergoalModoJuego(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// No usar un objeto por default
	DefaultPawnClass = NULL;

	// Usar el controlador de jugador definido
	//PlayerControllerClass = AMastergoalControladorJugador::StaticClass();
}



