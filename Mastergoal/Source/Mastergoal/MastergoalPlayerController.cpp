// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mastergoal.h"
#include "MastergoalPlayerController.h"
#include "MastergoalTablero.h"

AMastergoalPlayerController::AMastergoalPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}


void AMastergoalPlayerController::MoverFicha(int32 Fila, int32 Columna)
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT EXECUTION"));
	ServerMoverFicha(Tablero, Fila, Columna);
}

bool AMastergoalPlayerController::ServerMoverFicha_Validate(AMastergoalTablero* Board, int32 Fila, int32 Columna)
{
	return true;
}

void AMastergoalPlayerController::ServerMoverFicha_Implementation(AMastergoalTablero* Board, int32 Fila, int32 Columna)
{
	UE_LOG(LogTemp, Warning, TEXT("SERVER EXECUTION"));
	if (Board != nullptr)
		Board->MoverFicha(Fila, Columna);
	else
		UE_LOG(LogTemp, Error, TEXT("Attempted to access undefined Board"));
}


void AMastergoalPlayerController::Seleccionar(int32 Fila, int32 Columna)
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT EXECUTION"));
	ServerSeleccionar(Tablero, Fila, Columna);
}

bool AMastergoalPlayerController::ServerSeleccionar_Validate(AMastergoalTablero* Board, int32 Fila, int32 Columna)
{
	return true;
}

void AMastergoalPlayerController::ServerSeleccionar_Implementation(AMastergoalTablero* Board, int32 Fila, int32 Columna)
{
	UE_LOG(LogTemp, Warning, TEXT("SERVER EXECUTION"));
	if (Board != nullptr)
		Board->Seleccionar(Fila, Columna);
	else
		UE_LOG(LogTemp, Error, TEXT("Attempted to access undefined Board"));
}