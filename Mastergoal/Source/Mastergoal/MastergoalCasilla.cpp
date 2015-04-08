// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mastergoal.h"
#include "MastergoalCasilla.h"
#include "MastergoalTablero.h"

AMastergoalCasilla::AMastergoalCasilla(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Crear el componente base y definirlo como la raíz
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ComponenteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
}

void AMastergoalCasilla::Inicializar(class AMastergoalTablero* Tablero, UStaticMesh* Mesh, UMaterialInstance* Material)
{
	this->Tablero = Tablero;

	this->Mesh = Mesh;
	this->Material = Material;

	ActualizarComponenteMesh();
	ComponenteMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
	ComponenteMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	ComponenteMesh->AttachTo(Root);
	ComponenteMesh->OnClicked.AddDynamic(this, &AMastergoalCasilla::OnClick);
	ComponenteMesh->OnInputTouchBegin.AddDynamic(this, &AMastergoalCasilla::OnTouch);
}

void AMastergoalCasilla::ActualizarComponenteMesh()
{
	// Actualizar el modelo y el material de la casilla
	ComponenteMesh->SetStaticMesh(Mesh);
	ComponenteMesh->SetMaterial(0, Material);
}

/// Helpers
FVector AMastergoalCasilla::GetSize()
{
	FVector min;
	FVector max;
	UStaticMeshComponent* mesh = this->GetComponenteMesh();

	mesh->GetLocalBounds(min, max);

	return max - min;
}

/// Handlers
void AMastergoalCasilla::OnClick(UPrimitiveComponent* ClickedComp)
{
	
}

// Handler del evento touch para una casilla
void AMastergoalCasilla::OnTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnClick(TouchedComponent);
}
