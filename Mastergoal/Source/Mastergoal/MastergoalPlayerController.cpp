// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mastergoal.h"
#include "MastergoalPlayerController.h"

AMastergoalPlayerController::AMastergoalPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
