// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "MastergoalGameMode.generated.h"

/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class AMastergoalGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMastergoalGameMode(const FObjectInitializer& ObjectInitializer);
};



