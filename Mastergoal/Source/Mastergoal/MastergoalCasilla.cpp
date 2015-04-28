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

	ComponenteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// Valores por defecto
	InfluenciaRojo = 0;
	InfluenciaBlanco = 0;
	Arquero = 0;

	Fila = -1;
	Columna = -1;

	Ficha = nullptr;
}

void AMastergoalCasilla::Inicializar(class AMastergoalTablero* Tablero, int32 Equipo, bool Area, bool Corner, bool Arco, 
									 bool Especial, int32 Fila, int32 Columna, UStaticMesh* Mesh, UMaterialInstance* Material)
{
	this->Tablero = Tablero;
	this->Equipo = Equipo;

	this->Area = Area;
	this->Corner = Corner;
	this->Arco = Arco;
	this->Especial = Especial;

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

/*
 * Indica si la influencia de los equipos se neutraliza en la casilla.
 * Una casilla sin influencia también es considerada neutral.
 * En caso de querer estar seguro que no hay ningún tipo de influencia en la casilla usar InfluenciaCero().
 */
bool AMastergoalCasilla::InfluenciaNeutra()
{
	return (InfluenciaRojo - InfluenciaBlanco) == 0;
}

/*
 * Indica si la casilla no está bajo influencia alguna
 */
bool AMastergoalCasilla::InfluenciaCero()
{
	return InfluenciaRojo == 0 && InfluenciaBlanco == 0;
}

/*
 * Indica si un equipo tiene posesión de esta casilla.
 * Parámetros:
 * Equipo - Equipo sobre el que se valora la influencia
 * Retorna: 
 * true si hay un empate o mayoría de influencia del equipo
 */
bool AMastergoalCasilla::TieneInfluencia(int32 Equipo, bool Estricto)
{
	if (Estricto)
	{
		return ((InfluenciaRojo - InfluenciaBlanco) * Equipo) > 0;
	}
	else
	{
		return ((InfluenciaRojo - InfluenciaBlanco) * Equipo) >= 0;
	}
}

/*
 * Actualiza el modelo y material de la casilla en base a los asignados.
 */
void AMastergoalCasilla::ActualizarComponenteMesh()
{
	// Actualizar el modelo y el material de la casilla
	ComponenteMesh->SetStaticMesh(Mesh);
	ComponenteMesh->SetMaterial(0, Material);
}

/// Handlers
void AMastergoalCasilla::OnClick(UPrimitiveComponent* ClickedComp)
{
	UE_LOG(LogTemp, Warning, TEXT("==="));
	UE_LOG(LogTemp, Warning, TEXT("Clicked cell %d,%d {STATE: %d} Piece:%d InfB:%d InfR:%d Area:%d Goalie:%d Goal:%d Corner:%d Team:%d Special:%d"), Fila, Columna, Tablero->EstadoTablero[Fila][Columna], Ficha != nullptr, InfluenciaBlanco, InfluenciaRojo, Area, Arquero, Arco, Corner, Equipo, Especial);
	if (Tablero->FichaSeleccionada != NULL)
	{
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