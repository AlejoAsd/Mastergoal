// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "MastergoalTablero.h"
#include "MastergoalCasilla.h"
#include "MastergoalFicha.h"
#include "MemoryBase.h"
#include <iostream>

// Define los valores por defecto del objeto
AMastergoalTablero::AMastergoalTablero(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Crear el componente base y definirlo como la raíz
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Crear el brazo de cámara
	BrazoCamara = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamaraBrazo"));
	BrazoCamara->AttachTo(Root);
	BrazoCamara->bAbsoluteRotation = true;
	BrazoCamara->TargetArmLength = 4000.f;
	BrazoCamara->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	BrazoCamara->bDoCollisionTest = false;

	// Crear la cámara asociada al tablero y asignarla al brazo
	Camara = CreateDefaultSubobject<UCameraComponent>(TEXT("Camara"));
	Camara->AttachTo(BrazoCamara, USpringArmComponent::SocketName);
	Camara->bUsePawnControlRotation = false;

	// Crear la líneas del tablero
	LineasTablero = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LineasTablero"));
	LineasTablero->AttachTo(Root);
	LineasTablero->CastShadow = false;

	// Valores por defecto
	Nivel = 3;
	Alto = 13;
	Ancho = 11;
	AnchoArco = 5;
}

// Función llamada cuando inicia el juego o se crea la instancia. 
// Está encargada de la inicialización de los componentes del objeto.
void AMastergoalTablero::BeginPlay()
{
	Super::BeginPlay();

	// Incrementar el ancho para tomar en cuenta los arcos
	Alto += 2;

	// Definir las fichas que se deben crear
	FichaLista = FichaObtenerLista();
	//FichaSeleccionada = NULL;

	/// Inicializar las casillas
	for (int32 i = 0; i < Alto; i++)
	{
		for (int32 j = 0; j < Ancho; j++)
		{
			Casillas[i][j] = NULL;
		}
	}

	// Definir el tamaño de las casillas
	AMastergoalCasilla* Casilla = GetWorld()->SpawnActor<AMastergoalCasilla>(FVector(0, 0, 0), FRotator(0, 0, 0));
	if (Casilla == NULL) return;

	Casilla->Inicializar(this, 0, 0, CasillaModelo, CasillaMaterial);

	Origen = Casilla->GetSize();

	AltoCasillas = Origen.X;
	AnchoCasillas = Origen.Y;
	ProfundidadCasillas = Origen.Z;
	
	Origen.X = -(float)Alto / 2 * AltoCasillas;
	Origen.Y = -(float)Ancho / 2 * AnchoCasillas;

	Casilla->Destroy();

	// Crear las fichas y casillas
	for (int32 i = 0; i < Alto; i++)
	{
		for (int32 j = 0; j < Ancho; j++)
		{
			CasillaCrear(i, j);

			if (FichaLista[i][j])
			{
				FichaCrear(FichaLista[i][j], i, j);
			}
		}
	}

	// Crear las líneas de tablero
	if (LineasTableroMesh != NULL && LineasTableroMaterial != NULL)
	{
		LineasTablero->SetStaticMesh(LineasTableroMesh);
		LineasTablero->SetMaterial(0, LineasTableroMaterial);
		LineasTablero->SetRelativeLocation(FVector(-119.f, -101.f, 90.f));
		LineasTablero->SetRelativeScale3D(FVector(1.02f, 1.02f, 1.02f));
		LineasTablero->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No se pudieron cargar las líneas del tablero. El modelo o el material no están definidos."))
	}
}

void AMastergoalTablero::BeginDestroy()
{
	Super::BeginDestroy();

	FichaDestruirLista(FichaLista);
}

class AMastergoalCasilla* AMastergoalTablero::CasillaCrear(int32 Fila, int32 Columna)
{
	// Cantidad de posiciones sin casillas del tablero en las filas de arcos, contando desde los costados.
	const int32 ArcoOffset = (Ancho - AnchoArco) / 2;

	// En las filas de arcos, se deben saltar las casillas que no correspondan al arco
	if ((Fila == 0 || Fila == Alto - 1) &&
		(Columna < ArcoOffset || Columna >= ArcoOffset + AnchoArco))
		return NULL;

	// Crear el bloque
	AMastergoalCasilla* Casilla = GetWorld()->SpawnActor<AMastergoalCasilla>(FVector(0, 0, 0), FRotator(0, 0, 0));

	if (Casilla != NULL)
	{
		// Definir el material a usar
		class UMaterialInstance* Material;
		if ((Fila + Columna) % 2 != 0)
			Material = CasillaMaterial;
		else
			Material = CasillaMaterialAlternativo;

		// Inicializar la casilla
		Casilla->Inicializar(this, Fila, Columna, CasillaModelo, Material);

		// Definir la posición de la casilla				
		const FVector BlockLocation = Origen + GetActorLocation() +
			FVector(Fila * AltoCasillas, Columna * AnchoCasillas, 0);

		// Posicionar la casilla
		Casilla->SetActorLocation(BlockLocation);

		// Guardar la referencia en el tablero
		Casillas[Alto][Ancho] = Casilla;
	}

	return Casilla;
}

// TODO
class AMastergoalFicha* AMastergoalTablero::FichaCrear(int32 Tipo, int32 Fila, int32 Columna)
{	
	// Crear la ficha
	AMastergoalFicha* Ficha = GetWorld()->SpawnActor<AMastergoalFicha>(FVector(0, 0, 0), FRotator(0, 0, 0));

	if (Ficha != NULL)
	{
		// Definir el modelo y material a usar
		int32 Equipo;
		class UStaticMesh* Modelo;
		class UMaterialInstance* Material;
		if (Tipo == PELOTA)
		{
			Equipo = -1;
			Modelo = FichaModeloPelota;
			Material = FichaMaterialPelota;
		}
		else if (Tipo == BLANCO_FICHA)
		{
			Equipo = 0;
			Modelo = FichaModeloJugador;
			Material = FichaMaterialJugador;
		}
		else if (Tipo == BLANCO_ARQUERO_EN_AREA || Tipo == BLANCO_ARQUERO_FUERA_AREA)
		{
			Equipo = 0;
			Modelo = FichaModeloArquero;
			Material = FichaMaterialJugador;
		}
		else if (Tipo == ROJO_FICHA)
		{
			Equipo = 1;
			Modelo = FichaModeloJugador;
			Material = FichaMaterialJugadorAlternativo;
		}
		else if (Tipo == ROJO_ARQUERO_EN_AREA || Tipo == ROJO_ARQUERO_FUERA_AREA)
		{
			Equipo = 1;
			Modelo = FichaModeloArquero;
			Material = FichaMaterialJugadorAlternativo;
		}
		else
		{
			return NULL;
		}

		// Inicializar la casilla
		Ficha->Inicializar(this, Equipo, Fila, Columna, Tipo, Modelo, Material);

		FVector Size = Ficha->GetSize();

		// Definir la posición de la casilla				
		const FVector BlockLocation = Origen + GetActorLocation() +
			FVector(Fila * AltoCasillas, 
					Columna * AnchoCasillas, 
					(ProfundidadCasillas + Size.Z) / 2);

		// Posicionar la casilla
		Ficha->SetActorLocation(BlockLocation);
	}

	return Ficha;
}

AMastergoalTablero::TipoFicha** AMastergoalTablero::FichaObtenerLista()
{
	// Crear la estructura
	TipoFicha** Fichas = (TipoFicha**)FMemory::Malloc(sizeof(TipoFicha*) * Alto);

	if (Fichas == NULL) return NULL;

	for (int32 i = 0; i < Alto; i++)
	{
		Fichas[i] = (TipoFicha*)FMemory::Malloc(sizeof(TipoFicha)* Ancho);
		if (Fichas[i] == NULL) return NULL;

		for (int32 j = 0; j < Ancho; j++)
		{
			Fichas[i][j] = VACIO;
		}
	}

	// Asignar los valores
	Fichas[7][5] = PELOTA;
	if (Nivel == 1)
	{
		// Blanco
		Fichas[4][5] = BLANCO_FICHA;
		// Rojo
		Fichas[10][5] = ROJO_FICHA;
	}
	else if (Nivel == 2)
	{
		// Blanco
		Fichas[4][5] = BLANCO_FICHA;
		Fichas[2][5] = BLANCO_ARQUERO_EN_AREA;
		// Rojo
		Fichas[10][5] = ROJO_FICHA;
		Fichas[12][5] = ROJO_ARQUERO_EN_AREA;
	}
	else if (Nivel == 3)
	{
		// Blanco
		Fichas[4][3] = BLANCO_FICHA;
		Fichas[4][7] = BLANCO_FICHA;
		Fichas[6][2] = BLANCO_FICHA;
		Fichas[6][8] = BLANCO_FICHA;
		Fichas[2][5] = BLANCO_ARQUERO_EN_AREA;
		// Rojo
		Fichas[8][2] = ROJO_FICHA;
		Fichas[8][8] = ROJO_FICHA;
		Fichas[10][3] = ROJO_FICHA;
		Fichas[10][7] = ROJO_FICHA;
		Fichas[12][5] = ROJO_ARQUERO_EN_AREA;
	}
	
	return Fichas;
}

/*
 * Pasa el turno 
 */
void AMastergoalTablero::PasarTurno()
{
	Turno = !Turno;
}

/*
 * Mueve una ficha
 */
bool AMastergoalTablero::MoverFicha(AMastergoalFicha* Ficha, int32 Fila, int32 Columna)
{
	if (Estado != JUEGO)
		return false;

	// Validar que el movimiento sea posible
	bool Valido = ValidarMovimiento(Ficha, Fila, Columna);

	// Mover la ficha
	if (Valido)
	{
		// Definir el estado de movimiento
		FichaSeleccionada = NULL;
		Estado = MOVIMIENTO;

		// Actualizar el estado del tablero
		EstadoTablero[Ficha->Fila][Ficha->Columna] = VACIO;
		EstadoTablero[Fila][Columna] = Ficha->Tipo;

		// Obtener el punto objetivo
		FVector Destino = FVector((Fila - Ficha->Fila) * AltoCasillas,
			(Columna - Ficha->Columna) * AnchoCasillas,
			0);
		Destino += Ficha->GetActorLocation();

		Ficha->Mover(Fila, Columna, Destino);
	}

	return Valido;
}

/*
 * Valida que un movimiento sea posible. 
 * No toma en cuenta situaciones especiales como el hecho que la ficha sea la pelota,
 * o que el jugador esté al lado de la pelota.
 */
bool AMastergoalTablero::ValidarMovimiento(AMastergoalFicha* Ficha, int32 Fila, int32 Columna)
{
	const int32 ArcoOffset = (Ancho - AnchoArco) / 2;

	/// Validar destino
	// Asegurar que esté dentro del tablero
	if (Fila < 0 || Fila > Alto - 1 ||
		Ficha < 0 || Columna > Ancho)
		return false;
	// Asegurar que exista una casilla para la posición indicada
	if ((Fila == 0 || Fila == Alto - 1) &&
		(Columna < ArcoOffset || Columna >= ArcoOffset + AnchoArco))
		return false;

	/// Validar movimiento
	int32 DeltaFila = Fila - Ficha->Fila;
	if (DeltaFila < 0) DeltaFila *= -1;
	int32 DeltaColumna = Columna - Ficha->Columna;
	if (DeltaColumna < 0) DeltaColumna *= -1;
	int32 MaximoMovimientos = Ficha->Tipo == PELOTA ? 4 : 2;

	// El movimiento es en línea recta
	if ((!DeltaFila && !DeltaColumna) ||
		(DeltaFila && DeltaColumna &&
		 DeltaFila != DeltaColumna))
		return false;

	// El movimiento está dentro del rango de la ficha
	if (DeltaFila > MaximoMovimientos ||
		DeltaColumna > MaximoMovimientos)
		return false;

	// La casilla objetivo está libre
	if (EstadoTablero[Fila][Columna] != VACIO)
		return false;

	/// Determinar dirección
	int32 DireccionFila = 0;
	int32 DireccionColumna = 0;

	// Fila
	if (Ficha->Fila < Fila)
		DireccionFila = 1;
	else if (Ficha->Fila > Fila)
		DireccionFila = -1;

	// Columna
	if (Ficha->Columna < Columna)
		DireccionColumna = 1;
	else if (Ficha->Columna > Columna)
		DireccionColumna = -1;

	int32 CantidadMovimientos = 1 + DeltaColumna ? DeltaFila : DeltaColumna;
	for (int32 i = 1; i < CantidadMovimientos; i++)
	{
		Fila += DireccionFila;
		Columna += DireccionColumna;

		if (EstadoTablero[Fila][Columna])
		{
			// En caso de ser la pelota debe saltar fichas
			if (Ficha->Tipo == PELOTA &&
				EstadoTablero[Fila][Columna] != BLANCO_ARQUERO_EN_AREA &&
				EstadoTablero[Fila][Columna] != ROJO_ARQUERO_EN_AREA &&
				i != CantidadMovimientos - 1)
				Ficha->MovimientoSaltar = true;
			else
			{
				Ficha->MovimientoSaltar = false;
				return false;
			}
		}
	}

	return true;
}

/*
 * Selecciona/Deselecciona una ficha.
 * En caso que la ficha no esté seleccionada, se selecciona la ficha.
 * En caso que la ficha esté seleccionada, se deselecciona la ficha.
 * Si se selecciona se retorna true. De otro modo se retorna false.
 */
bool AMastergoalTablero::Seleccionar(AMastergoalFicha* Ficha)
{
	if (Ficha == NULL || Estado != JUEGO)
	{
		return false;
	}
	
	// Seleccionar
	if (FichaSeleccionada == NULL || FichaSeleccionada != Ficha)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Piece at %d %d"), Ficha->Fila, Ficha->Columna);
		FichaSeleccionada = Ficha;
		return true;
	}
	// Deseleccionar
	else if (FichaSeleccionada == Ficha)
	{
		UE_LOG(LogTemp, Warning, TEXT("Deselected Piece at %d %d"), Ficha->Fila, Ficha->Columna);
		FichaSeleccionada = NULL;
		return false;
	}

	return false;
}

/*
 * Destruye la lista de fichas proveída.
 */
void AMastergoalTablero::FichaDestruirLista(TipoFicha**& Lista)
{
	if (Lista == NULL) return;

	for (int32 i = 0; i < Alto; i++)
	{
		FMemory::Free(Lista[i]);
	}
	FMemory::Free(Lista);
}