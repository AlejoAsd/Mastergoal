// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "CTestReplication.h"


// Sets default values
ACTestReplication::ACTestReplication()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;

	// Crear el componente base y definirlo como la raíz
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ComponenteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ComponenteMesh->SetRelativeLocation(FVector(0, 0, 0));

	UStaticMesh* MeshToUse = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Mastergoal/Modelos/Ficha.Ficha")));
	if (MeshToUse && ComponenteMesh)
	{
		ComponenteMesh->SetStaticMesh(MeshToUse);
	}

	UMaterialInstance* MaterialToUse = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), NULL, TEXT("/Game/Mastergoal/Materiales/M_Basic_Wall_Red.M_Basic_Wall_Red")));
	if (MaterialToUse && ComponenteMesh)
	{
		ComponenteMesh->SetMaterial(0, MaterialToUse);
	}
}

// Called when the game starts or when spawned
void ACTestReplication::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACTestReplication::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (HasAuthority())
	{
		this->AddActorWorldOffset(FVector(0, 1, 0));
	}
}

