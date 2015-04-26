// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "MastergoalFicha.h"
#include "MastergoalTablero.h"


// Sets default values
AMastergoalFicha::AMastergoalFicha(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Activar la función de update
	PrimaryActorTick.bCanEverTick = true;

	// Crear el componente base y definirlo como la raíz
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ComponenteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	MovimientoSaltar = false;
}

void AMastergoalFicha::Inicializar(class AMastergoalTablero* Tablero, int32 Equipo, int32 Fila, int32 Columna,
								   int32 Tipo, UStaticMesh* Mesh, UMaterialInstance* Material)
{
	this->Tablero = Tablero;
	this->Equipo = Equipo;
	this->Tipo = Tipo;
	this->Fila = Fila;
	this->Columna = Columna;

	this->Mesh = Mesh;
	this->Material = Material;

	ActualizarComponenteMesh();
	ComponenteMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	ComponenteMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	ComponenteMesh->AttachTo(Root);
	ComponenteMesh->OnClicked.AddDynamic(this, &AMastergoalFicha::OnClick);
	ComponenteMesh->OnInputTouchBegin.AddDynamic(this, &AMastergoalFicha::OnTouch);

	// Registrar posición en el tablero
	this->Tablero->EstadoTablero[Fila][Columna] = Tipo;
}

void AMastergoalFicha::Mover(int32 Fila, int32 Columna, FVector Destino)
{
	Movimiento = true;
	MovimientoDestino = Destino;

	Tablero->Estado = AMastergoalTablero::MOVIMIENTO;
	
	this->Fila = Fila;
	this->Columna = Columna;
}

void AMastergoalFicha::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Movimiento)
	{
		FVector Posicion = GetActorLocation();

		// Mover hacia el punto destino
		Posicion = FMath::VInterpTo(Posicion, MovimientoDestino, DeltaTime, 10);
		SetActorLocation(Posicion);

		// Detener el movimiento cuando se esté dentro del margen
		if (FVector::Dist(Posicion, MovimientoDestino) <= 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("\n"));
			UE_LOG(LogTemp, Warning, TEXT("Turn"));
			Tablero->Estado = AMastergoalTablero::JUEGO;
			Movimiento = false;
			MovimientoSaltar = false;

			SetActorLocation(MovimientoDestino);
		}	
	}
}

/// Helpers
FVector AMastergoalFicha::GetSize()
{
	FVector min;
	FVector max;
	UStaticMeshComponent* mesh = this->GetComponenteMesh();

	mesh->GetLocalBounds(min, max);

	return max - min;
}

void AMastergoalFicha::ActualizarComponenteMesh()
{
	// Actualizar el modelo y el material de la casilla
	ComponenteMesh->SetStaticMesh(Mesh);
	ComponenteMesh->SetMaterial(0, Material);
}

/// Handlers
void AMastergoalFicha::OnClick(UPrimitiveComponent* ClickedComp)
{
	Tablero->Seleccionar(this);
}

// Handler del evento touch para una casilla
void AMastergoalFicha::OnTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnClick(TouchedComponent);
}