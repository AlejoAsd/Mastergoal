// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CTestReplication.generated.h"

UCLASS()
class MASTERGOAL_API ACTestReplication : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTestReplication();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Componente base raíz
	class USceneComponent* Root;
	class UStaticMeshComponent* ComponenteMesh;
	class UMaterialInstance* Material;
	
};
