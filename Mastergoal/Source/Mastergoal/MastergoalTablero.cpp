// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "MastergoalTablero.h"
#include "MastergoalCasilla.h"


// Define los valores por defecto del objeto
AMastergoalTablero::AMastergoalTablero(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Crear el componente base y definirlo como la raíz
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Valores por defecto
	Alto = 13;
	Ancho = 11;
	AnchoArco = 5;
}

// Función llamada cuando inicia el juego o se crea la instancia. 
// Está encargada de la inicialización de los componentes del objeto.
void AMastergoalTablero::BeginPlay()
{
	Super::BeginPlay();
	
	// Cantidad de posiciones sin casillas del tablero en las filas de arcos, contando desde los costados.
	const int32 ArcoOffset = (Ancho - AnchoArco) / 2;

	/// Crear las casillas
	Casillas.AddUninitialized((Alto + 2) * Ancho);

	// El Alto se incrementa en 2 para incluir los arcos
	for (int32 i = 0; i < Alto + 2; i++)
	{
		for (int32 j = 0; j < Ancho; j++)
		{
			// En las filas de arcos, se deben saltar las casillas que no correspondan al arco
			if ((i == 0 || i == Alto + 1) &&
				(j < ArcoOffset || j >= ArcoOffset + AnchoArco))
				continue;

			// Crear el bloque
			AMastergoalCasilla* Casilla = GetWorld()->SpawnActor<AMastergoalCasilla>(FVector(0, 0, 0), FRotator(0, 0, 0));
			UE_LOG(LogTemp, Log, TEXT("Creating Block"));
			if (Casilla != NULL)
			{
				// Definir el material a usar
				class UMaterialInstance* Material;
				if ((i + j) % 2 != 0)
					Material = this->Material;
				else
					Material = this->MaterialAlternativo;

				// Inicializar la casilla
				Casilla->Inicializar(this, this->Modelo, Material);

				// Definir la posición de la casilla
				FVector Offset = Casilla->GetSize();
				Offset *= FVector(i, j, 0.5f);
				
				const FVector BlockLocation = Offset + GetActorLocation();

				// Posicionar la casilla
				Casilla->SetActorLocation(BlockLocation);

				// Guardar la referencia en el tablero
				Casillas[(i * Ancho) + j] = Casilla;
			}
		}
	}
}

