// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mastergoal.h"
#include "MastergoalCasilla.h"
#include "MastergoalTablero.h"

AMastergoalCasilla::AMastergoalCasilla(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Crear el componente base y definirlo como la ra�z
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ComponenteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// Valores por defecto
	Fila = -1;
	Columna = -1;
}

void AMastergoalCasilla::Inicializar(class AMastergoalTablero* Tablero, int32 Fila, int32 Columna, UStaticMesh* Mesh, UMaterialInstance* Material)
{
	this->Tablero = Tablero;
	this->Fila = Fila;
	this->Columna = Columna;

	this->Mesh = Mesh;
	this->Material = Material;

	ActualizarComponenteMesh();
	ComponenteMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	ComponenteMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	ComponenteMesh->AttachTo(Root);
	ComponenteMesh->OnClicked.AddDynamic(this, &AMastergoalCasilla::OnClick);
	ComponenteMesh->OnInputTouchBegin.AddDynamic(this, &AMastergoalCasilla::OnTouch);
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

void AMastergoalCasilla::ActualizarComponenteMesh()
{
	// Actualizar el modelo y el material de la casilla
	ComponenteMesh->SetStaticMesh(Mesh);
	ComponenteMesh->SetMaterial(0, Material);
}

/// Handlers
void AMastergoalCasilla::OnClick(UPrimitiveComponent* ClickedComp)
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked cell"));
	if (Tablero->FichaSeleccionada != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Moving selected piece to %d,%d"), Fila, Columna);
		bool result = Tablero->MoverFicha(Tablero->FichaSeleccionada, Fila, Columna);
		if (result)
		{
			UE_LOG(LogTemp, Warning, TEXT("Move successful"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid move"));
		}
	}
}

// Handler del evento touch para una casilla
void AMastergoalCasilla::OnTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnClick(TouchedComponent);
}