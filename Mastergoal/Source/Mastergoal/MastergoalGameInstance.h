// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "MastergoalGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MASTERGOAL_API UMastergoalGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMastergoalGameInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Juego)
	int32 Nivel;
};
