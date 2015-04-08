// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "MastergoalPlayerController.generated.h"

/** PlayerController class used to enable cursor */
UCLASS()
class AMastergoalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMastergoalPlayerController(const FObjectInitializer& ObjectInitializer);
};


