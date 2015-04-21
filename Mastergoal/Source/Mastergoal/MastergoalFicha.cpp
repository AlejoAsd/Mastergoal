// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "MastergoalFicha.h"
#include "MastergoalTablero.h"


// Sets default values
AMastergoalFicha::AMastergoalFicha(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Crear el componente base y definirlo como la raíz
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ComponenteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void AMastergoalFicha::Inicializar(class AMastergoalTablero* Tablero, int32 Fila, int32 Columna, 
								   int32 Tipo, UStaticMesh* Mesh, UMaterialInstance* Material)
{
	this->Fila = Fila;
	this->Columna = Columna;
	this->Tipo = Tipo;

	this->Tablero = Tablero;

	this->Mesh = Mesh;
	this->Material = Material;

	ActualizarComponenteMesh();
	ComponenteMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	ComponenteMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	ComponenteMesh->AttachTo(Root);
	ComponenteMesh->OnClicked.AddDynamic(this, &AMastergoalFicha::OnClick);
	ComponenteMesh->OnInputTouchBegin.AddDynamic(this, &AMastergoalFicha::OnTouch);

	// Registrar posición en el tablero
	this->Tablero->Estado[Fila][Columna] = Tipo;
}

// Called every frame
/*void AMastergoalFicha::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}*/

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
	//Tablero->Seleccionar(this);
}

// Handler del evento touch para una casilla
void AMastergoalFicha::OnTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnClick(TouchedComponent);
}