// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mastergoal.h"
#include "MastergoalGameMode.h"
#include "MastergoalPlayerController.h"

AMastergoalGameMode::AMastergoalGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// no pawn by default
	DefaultPawnClass = NULL;
	// use our own player controller class
	PlayerControllerClass = AMastergoalPlayerController::StaticClass();
	//PlayerStateClass = AMastergoalPlayerState::StaticClass();

	Nivel = 3;
}
