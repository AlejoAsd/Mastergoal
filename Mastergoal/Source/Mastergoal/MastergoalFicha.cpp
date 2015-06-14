// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "MastergoalFicha.h"
#include "MastergoalTablero.h"
#include "MastergoalPlayerController.h"
#include "EngineLogs.h"
#include "UnrealNetwork.h"


// Sets default values
AMastergoalFicha::AMastergoalFicha(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Activar la función de update
	PrimaryActorTick.bCanEverTick = true;

	// Opciones de replicación
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;

	// Crear el componente base y definirlo como la raíz
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ComponenteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ComponenteMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	ComponenteMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	ComponenteMesh->AttachTo(Root);
	ComponenteMesh->OnClicked.AddDynamic(this, &AMastergoalFicha::OnClick);
	ComponenteMesh->OnInputTouchBegin.AddDynamic(this, &AMastergoalFicha::OnTouch);

	MovimientoSaltar = false;
}

void AMastergoalFicha::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMastergoalFicha, Tablero);
	DOREPLIFETIME(AMastergoalFicha, Fila);
	DOREPLIFETIME(AMastergoalFicha, Columna);
	DOREPLIFETIME(AMastergoalFicha, ComponenteMesh);
	DOREPLIFETIME(AMastergoalFicha, Mesh);
	DOREPLIFETIME(AMastergoalFicha, Material);
}

void AMastergoalFicha::Inicializar(class AMastergoalTablero* Tablero, int32 Equipo, int32 Fila, int32 Columna,
								   int32 Tipo, UStaticMesh* Mesh, UMaterialInstance* Material)
{
	this->Tablero = Tablero;
	this->Equipo = Equipo;
	this->Tipo = Tipo;
	this->Fila = Fila;
	this->FilaInicial = Fila;
	this->Columna = Columna;
	this->ColumnaInicial = Columna;

	if (Tipo == BLANCO_ARQUERO_EN_AREA || 
		Tipo == ROJO_ARQUERO_EN_AREA)
	{
		Arquero = true;
		ArqueroEnArea = true;
	}
	else if (Tipo == BLANCO_ARQUERO_FUERA_AREA ||
			 Tipo == ROJO_ARQUERO_FUERA_AREA)
	{
		Arquero = true;
		ArqueroEnArea = false;
	}

	this->Mesh = Mesh;
	this->Material = Material;

	// Registrar posición en el tablero
	this->Tablero->EstadoTablero[Fila][Columna] = Tipo;
	if (EsArquero(false))
	{
		if (Columna > 0)
		{
			Tablero->EstadoTablero[Fila][Columna - 1] = Tipo;
		}
		if (Columna < (Tablero->Ancho - 1))
		{
			Tablero->EstadoTablero[Fila][Columna + 1] = Tipo;
		}
	}
}

void AMastergoalFicha::Mover(int32 Fila, int32 Columna)
{
	/// Modificar influencia
	Tablero->ModificarInfluencia(this, true);

	/// Obtener el punto objetivo
	MovimientoDestino = FVector((Fila - this->Fila) * Tablero->AltoCasillas,
		(Columna - this->Columna) * Tablero->AnchoCasillas,
		0);
	MovimientoDestino += GetActorLocation();

	Movimiento = true;

	Tablero->Estado = MOVIMIENTO;
	Tablero->FichasEnMovimiento++;
	
	this->Fila = Fila;
	this->Columna = Columna;

	/// Modificar influencia
	Tablero->ModificarInfluencia(this, false);
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
			UE_LOG(LogTemp, Warning, TEXT("Turn"));

			Movimiento = false;
			MovimientoSaltar = false;

			SetActorLocation(MovimientoDestino);

			Tablero->FichasEnMovimiento--;

			Tablero->PasarTurno();
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

/*
 * Chequea si la ficha es un arquero. En caso de chequear por el área, también chequea que esté en el área.
 */
bool AMastergoalFicha::EsArquero(bool ChequearArea)
{
	if (!Arquero)
		return false;

	if (ChequearArea && !ArqueroEnArea)
		return false;

	return true;
}

bool AMastergoalFicha::EsArqueroFueraDeArea()
{
	return Arquero && !ArqueroEnArea;
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
	if (Tablero != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Setting owner."));
		//Tablero->SetOwner(this);
		//this->SetOwner(GetWorld()->GetFirstPlayerController());
		/*if (Tablero->GetOwner() == Tablero)
			UE_LOG(LogTemp, Error, TEXT("Owner is Tablero."));
		if (Tablero->GetOwner() == GetWorld()->GetFirstPlayerController())
			UE_LOG(LogTemp, Error, TEXT("Owner is local PlayerController."));*/
		//UE_LOG(LogTemp, Warning, TEXT("Calling RPC."));
		//Tablero->ServerSeleccionar(this->Fila, this->Columna);
		AMastergoalPlayerController* PC = Cast<AMastergoalPlayerController>(GetWorld()->GetFirstPlayerController());
		PC->Seleccionar(this->Fila, this->Columna);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RPC Attempted to call Seleccionar() on nonexisting Tablero."));
	}
	
}

// Handler del evento touch para una casilla
void AMastergoalFicha::OnTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnClick(TouchedComponent);
}