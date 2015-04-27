// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "MastergoalTablero.h"
#include "MastergoalCasilla.h"
#include "MastergoalFicha.h"
#include "IA/Game.h"
#include "IA/Agent.h"

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

	// Crear el elemento de selección
	Seleccion = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Seleccion"));
	Seleccion->AttachTo(Root);
	Seleccion->CastShadow = false;
	Seleccion->bGenerateOverlapEvents = false;

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

	/// Inicializar las casillas
	for (int32 i = 0; i < Alto; i++)
	{
		for (int32 j = 0; j < Ancho; j++)
		{
			Casillas[i][j] = NULL;
			EstadoTablero[i][j] = VACIO;
		}
	}

	// Definir el tamaño de las casillas
	AMastergoalCasilla* Casilla = GetWorld()->SpawnActor<AMastergoalCasilla>(FVector(0, 0, 0), FRotator(0, 0, 0));
	if (Casilla == NULL) return;

	Casilla->Inicializar(this, 0, false, false, false, false, 0, 0, CasillaModelo, CasillaMaterial);

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
			CrearCasilla(i, j);

			// Crear la ficha correspondiente a la casilla en caso que corresponda
			if (FichaLista[i][j])
			{
				CrearFicha(FichaLista[i][j], i, j);
			}
		}
	}

	// Modificar la influencia de las casillas
	for (auto Iter(Fichas.CreateIterator()); Iter; Iter++)
	{
		if (!(*Iter)->IsValidLowLevel()) continue;
		
		ModificarInfluencia(*Iter, false);
	}

	// Crear las líneas de tablero
	if (LineasTableroMesh != NULL && LineasTableroMaterial != NULL)
	{
		LineasTablero->SetStaticMesh(LineasTableroMesh);
		LineasTablero->SetMaterial(0, LineasTableroMaterial);
		LineasTablero->SetRelativeLocation(FVector(-119.f, -101.f, 90.f));
		LineasTablero->SetRelativeScale3D(FVector(1.01, 1.01, 1.01));
		LineasTablero->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No se pudieron cargar las líneas del tablero. El modelo o el material no están definidos."))
	}

	// Crear selección
	if (SeleccionMesh != NULL && SeleccionMaterial != NULL)
	{
		Seleccion->SetStaticMesh(SeleccionMesh);
		Seleccion->SetMaterial(0, SeleccionMaterial);
		Seleccion->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Seleccion->bVisible = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No se pudo cargar el selector. El modelo o el material no están definidos."))
	}

	Jugar();
}

void AMastergoalTablero::BeginDestroy()
{
	FichaDestruirLista(FichaLista);

	Super::BeginDestroy();
}

class AMastergoalCasilla* AMastergoalTablero::CrearCasilla(int32 Fila, int32 Columna)
{
	// Cantidad de posiciones sin casillas del tablero en las filas de arcos, contando desde los costados.
	const int32 ArcoOffset = (Ancho - AnchoArco) / 2;

	// En las filas de arcos, se deben saltar las casillas que no correspondan al arco
	if ((Fila == 0 || Fila == (Alto - 1)) &&
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

		// Definir el equipo
		Equipo Equipos;
		if (Fila < Alto / 2)
			Equipos = BLANCO;
		else if (Fila > Alto / 2)
			Equipos = ROJO;
		else
			Equipos = NINGUNO;

		// Definir si es un corner
		bool Corner = (Fila == 1 || Fila == (Alto - 2)) && (Columna == 0 || Columna == (Ancho - 1));

		// Definir si es parte del área
		bool Area = ((Fila >= 1 && Fila <= 4) || (Fila >= (Alto - 5) && Fila <= (Alto - 2))) && 
					(Columna >= 1 && Columna <= (Ancho - 2));

		// Definir si es un arco
		bool Arco = Fila == 0 || Fila == (Alto - 1);

		// Definir si es una casilla especial
		bool Especial = (Fila == 1 || Fila == (Alto - 2)) && 
						(Columna == 0 || Columna == (Ancho - 1) || (Columna >= 3 && Columna <= 7));

		// Inicializar la casilla
		Casilla->Inicializar(this, Equipos, Area, Corner, Arco, Especial, Fila, Columna, CasillaModelo, Material);

		// Definir la posición de la casilla				
		const FVector BlockLocation = Origen + GetActorLocation() +
			FVector(Fila * AltoCasillas, Columna * AnchoCasillas, 0);

		// Posicionar la casilla
		Casilla->SetActorLocation(BlockLocation);

		// Guardar la referencia en el tablero
		Casillas[Fila][Columna] = Casilla;
	}

	return Casilla;
}

// TODO
class AMastergoalFicha* AMastergoalTablero::CrearFicha(int32 Tipo, int32 Fila, int32 Columna)
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
			Equipo = NINGUNO;
			Modelo = FichaModeloPelota;
			Material = FichaMaterialPelota;
		}
		else if (Tipo == BLANCO_FICHA)
		{
			Equipo = BLANCO;
			Modelo = FichaModeloJugador;
			Material = FichaMaterialJugador;
		}
		else if (Tipo == BLANCO_ARQUERO_EN_AREA || Tipo == BLANCO_ARQUERO_FUERA_AREA)
		{
			Equipo = BLANCO;
			Modelo = FichaModeloArquero;
			Material = FichaMaterialJugador;
		}
		else if (Tipo == ROJO_FICHA)
		{
			Equipo = ROJO;
			Modelo = FichaModeloJugador;
			Material = FichaMaterialJugadorAlternativo;
		}
		else if (Tipo == ROJO_ARQUERO_EN_AREA || Tipo == ROJO_ARQUERO_FUERA_AREA)
		{
			Equipo = ROJO;
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

		// Guardar la referencia a la ficha
		Fichas.Add(Ficha);
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

void AMastergoalTablero::ModificarInfluencia(AMastergoalFicha* Ficha, bool Inverso)
{
	int32 Cantidad = Ficha->Equipo;
	if (Ficha->Tipo == BLANCO_ARQUERO_EN_AREA ||
		Ficha->Tipo == ROJO_ARQUERO_EN_AREA)
	{
		Cantidad *= 6;
	}

	for (int i = (Ficha->Fila - 1); i <= (Ficha->Fila + 1); i++)
	{
		for (int j = (Ficha->Columna - 1); j <= (Ficha->Columna + 1); j++)
		{
			if ((i != Ficha->Fila || j != Ficha->Columna) &&
				(i >= 0 || i < Alto) &&
				(j >= 0 || j < Ancho))
			{
				Casillas[i][j]->Influencia += Cantidad * (Inverso ? -1 : 1);
			}
		}
	}
}

/*
 * Mueve una ficha
 */
bool AMastergoalTablero::MoverFicha(AMastergoalFicha* Ficha, int32 Fila, int32 Columna)
{
	UE_LOG(LogTemp, Warning, TEXT("Moving selected piece from %d,%d to %d,%d"), Ficha->Fila, Ficha->Columna, Fila, Columna);

	if (Estado != JUEGO)
		return false;

	// Validar que el movimiento sea posible
	bool Valido = ValidarMovimiento(Ficha, Fila, Columna);

	// Mover la ficha
	if (Valido)
	{
		/// Definir el estado de movimiento
		Seleccionar(Ficha);

		///// Actualizar el estado del tablero
		/// Limpiar el estado anterior
		ModificarInfluencia(Ficha, true);
		EstadoTablero[Ficha->Fila][Ficha->Columna] = AMastergoalTablero::VACIO;
		// Arquero
		if (Ficha->Tipo == AMastergoalTablero::ROJO_ARQUERO_EN_AREA || 
			Ficha->Tipo == AMastergoalTablero::BLANCO_ARQUERO_EN_AREA)
		{
			if (Ficha->Columna > 0)
			{
				EstadoTablero[Ficha->Fila][Ficha->Columna - 1] = AMastergoalTablero::VACIO;
			}
			if (Ficha->Columna < Ancho)
			{
				EstadoTablero[Ficha->Fila][Ficha->Columna + 1] = AMastergoalTablero::VACIO;
			}
		}

		/// Actualizar Tipos
		// Arquero
		// Saliendo del área
		if ((Ficha->Tipo == AMastergoalTablero::ROJO_ARQUERO_EN_AREA ||
			Ficha->Tipo == AMastergoalTablero::BLANCO_ARQUERO_EN_AREA) &&
			(!Casillas[Fila][Columna]->Area ||
			 Casillas[Fila][Columna]->Equipo != Ficha->Equipo))
		{
			Ficha->Tipo++;
		}
		// Entrando al área
		if ((Ficha->Tipo == AMastergoalTablero::ROJO_ARQUERO_FUERA_AREA ||
			Ficha->Tipo == AMastergoalTablero::BLANCO_ARQUERO_FUERA_AREA) &&
			Casillas[Fila][Columna]->Area &&
			Casillas[Fila][Columna]->Equipo == Ficha->Equipo)
		{
			Ficha->Tipo--;
		}

		/// Escribir el estado actual
		EstadoTablero[Fila][Columna] = Ficha->Tipo;
		if (Ficha->Tipo == AMastergoalTablero::ROJO_ARQUERO_EN_AREA ||
			Ficha->Tipo == AMastergoalTablero::BLANCO_ARQUERO_EN_AREA)
		{
			// Brazo izquierdo
			if (Columna > 0)
			{
				EstadoTablero[Fila][Columna - 1] = Ficha->Tipo;
			}
			// Brazo derecho
			if (Columna < (Ancho - 1))
			{
				EstadoTablero[Fila][Columna + 1] = Ficha->Tipo;
			}
		}

		/// Obtener el punto objetivo
		FVector Destino = FVector((Fila - Ficha->Fila) * AltoCasillas,
			(Columna - Ficha->Columna) * AnchoCasillas,
			0);
		Destino += Ficha->GetActorLocation();

		/// Mover la ficha
		Ficha->Mover(Fila, Columna, Destino);
		ModificarInfluencia(Ficha, false);

		/// Verificar la existencia de un pase

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
	if (Fila < 0 || Fila > (Alto - 1) ||
		Ficha < 0 || Columna > Ancho)
	{
		return false;
	}

	// Asegurar que exista una casilla para la posición indicada
	if ((Fila == 0 || Fila == (Alto - 1)) &&
		(Columna < ArcoOffset || Columna >= ArcoOffset + AnchoArco))
	{
		return false;
	}

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
	{
		return false;
	}

	// El movimiento está dentro del rango de la ficha
	if (DeltaFila > MaximoMovimientos ||
		DeltaColumna > MaximoMovimientos)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough movements"))
		return false;
	}

	// La casilla objetivo está libre
	if (EstadoTablero[Fila][Columna] != VACIO)
	{
		bool Valido = false;

		// Excepción para perimitir a un arquero moverse a los costados
		if ((Ficha->Tipo == ROJO_ARQUERO_EN_AREA ||
			Ficha->Tipo == ROJO_ARQUERO_FUERA_AREA ||
			Ficha->Tipo == BLANCO_ARQUERO_EN_AREA ||
			Ficha->Tipo == BLANCO_ARQUERO_FUERA_AREA) &&
			DeltaFila == 0 && DeltaColumna == 1 &&
			(EstadoTablero[Fila][Columna + DeltaColumna] == Ficha->Tipo ||
			EstadoTablero[Fila][Columna + DeltaColumna] == (Ficha->Tipo + 1) ||
			EstadoTablero[Fila][Columna - DeltaColumna] == Ficha->Tipo ||
			EstadoTablero[Fila][Columna - DeltaColumna] == (Ficha->Tipo + 1)))
		{
			Valido = true;
		}

		if (!Valido)
		{
			UE_LOG(LogTemp, Warning, TEXT("ET:%d Not an empty cell at the end"), EstadoTablero[Fila][Columna])
			return false;
		}
	}

	// No permitir a un arquero mover a una casilla donde hayan fichas adyacentes
	if ((Ficha->Tipo == ROJO_ARQUERO_EN_AREA ||
		Ficha->Tipo == ROJO_ARQUERO_FUERA_AREA ||
		Ficha->Tipo == BLANCO_ARQUERO_EN_AREA ||
		Ficha->Tipo == BLANCO_ARQUERO_FUERA_AREA) &&
		Columna > 0 && EstadoTablero[Fila][Columna - 1] != VACIO &&
		Columna < (Ancho - 1) && EstadoTablero[Fila][Columna + 1] != VACIO)
	{
		UE_LOG(LogTemp, Warning, TEXT("ET:%d Existing piece at an adjacent cell"), EstadoTablero[Fila][Columna])
		return false;
	}


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

	Fila = Ficha->Fila;
	Columna = Ficha->Columna;

	int32 CantidadMovimientos = 1 + DeltaColumna ? DeltaFila : DeltaColumna;
	for (int32 i = 1; i < CantidadMovimientos; i++)
	{
		Fila += DireccionFila;
		Columna += DireccionColumna;

		UE_LOG(LogTemp, Warning, TEXT("i:%d Fila:%d Columna:%d Tipo:%d ET:%d PELOTA:%d BAEA:%d RAEA:%d"), i, Fila, Columna, Ficha->Tipo, EstadoTablero[Fila][Columna], (int32)PELOTA, (int32)BLANCO_ARQUERO_EN_AREA, (int32)ROJO_ARQUERO_EN_AREA)

		if (EstadoTablero[Fila][Columna])
		{
			// En caso de ser la pelota debe saltar fichas
			if (Ficha->Tipo == PELOTA)
			{
				if (EstadoTablero[Fila][Columna] != BLANCO_ARQUERO_EN_AREA &&
					EstadoTablero[Fila][Columna] != ROJO_ARQUERO_EN_AREA)
				{
					UE_LOG(LogTemp, Warning, TEXT("Ball Jump i:%d Fila:%d Columna:%d Tipo:%d ET:%d PELOTA:%d BAEA:%d RAEA:%d"), i, Fila, Columna, Ficha->Tipo, EstadoTablero[Fila][Columna], (int32)PELOTA, (int32)BLANCO_ARQUERO_EN_AREA, (int32)ROJO_ARQUERO_EN_AREA)
					Ficha->MovimientoSaltar = true;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Goalie Block Tipo:%d ET:%d PELOTA:%d BAEA:%d RAEA:%d"), Ficha->Tipo, EstadoTablero[Fila][Columna], (int32)PELOTA, (int32)BLANCO_ARQUERO_EN_AREA, (int32)ROJO_ARQUERO_EN_AREA)
					return false;
				}
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
		UE_LOG(LogTemp, Warning, TEXT("Selecting piece at %d,%d Type:%d T:%d"), Ficha->Fila, Ficha->Columna, Ficha->Tipo, Ficha->Equipo);
		FichaSeleccionada = Ficha;

		if (Seleccion != NULL)
		{
			FVector Posicion = Ficha->GetActorLocation();
			FVector Size = Ficha->GetSize() + ProfundidadCasillas;

			Posicion.Z += Size.Z;

			Seleccion->SetVisibility(true);
			Seleccion->SetRelativeLocation(Posicion);
		}

		return true;
	}
	// Deseleccionar
	else if (FichaSeleccionada == Ficha)
	{
		UE_LOG(LogTemp, Warning, TEXT("Deselecting piece at %d,%d"), Ficha->Fila, Ficha->Columna);
		FichaSeleccionada = NULL;

		Seleccion->SetVisibility(false);

		return false;
	}

	return false;
}

int AMastergoalTablero::Jugar()
{
	ficha BallAnterior, LeftAnterior[5], RightAnterior[5], 
		  Ball, Left[5], Right[5];
	int32 il = 0, ir = 0;

	// Inicializar la pelota 
	BallAnterior.row = Ball.row = 0;
	BallAnterior.col = Ball.col = 0;

	// Crear la lista de posiciones
	for (auto Iter(Fichas.CreateIterator()); Iter; Iter++)
	{
		if (!(*Iter)->IsValidLowLevel()) continue;

		if ((*Iter)->Equipo == BLANCO)
		{
			if (il >= 5)
			{
				UE_LOG(LogTemp, Warning, TEXT("Se intentó cargar una ficha blanca más de la esperada al intentar jugar."))
				break;
			}

			LeftAnterior[il].row = Left[il].row = Alto - 1 - (*Iter)->Fila;
			LeftAnterior[il].col = Left[il].col = Ancho - 1 - (*Iter)->Columna;

			il++;
		}
		else if ((*Iter)->Equipo == ROJO)
		{
			if (ir >= 5)
			{
				UE_LOG(LogTemp, Warning, TEXT("Se intentó cargar una ficha roja más de la esperada al intentar jugar."))
				break;
			}

			RightAnterior[ir].row = Right[ir].row = Alto - 1 - (*Iter)->Fila;
			RightAnterior[ir].col = Right[ir].col = Ancho - 1 - (*Iter)->Columna;

			ir++;
		}
		else
		{
			BallAnterior.row = Ball.row = Alto - 1 - (*Iter)->Fila;
			BallAnterior.col = Ball.col = Ancho - 1 - (*Iter)->Columna;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Ball %d,%d"), Ball.row, Ball.col)
	UE_LOG(LogTemp, Warning, TEXT("BallAnterior %d,%d"), BallAnterior.row, BallAnterior.col)
	for (int i = 0; i < 5; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d Left %d,%d"), i, Left[i].row, Left[i].col)
		UE_LOG(LogTemp, Warning, TEXT("%d Right %d,%d"), i, Right[i].row, Right[i].col)
		UE_LOG(LogTemp, Warning, TEXT("%d LeftAnterior %d,%d"), i, LeftAnterior[i].row, LeftAnterior[i].col)
		UE_LOG(LogTemp, Warning, TEXT("%d RightAnterior %d,%d"), i, RightAnterior[i].row, RightAnterior[i].col)
	}

	UE_LOG(LogTemp, Warning, TEXT("= Realizando jugada de pc"))
	Agent* agentL = new Agent(Nivel, LEFT, 3, "agentL1_1");
	Agent* agentR = new Agent(Nivel, RIGHT, 3, "agentL1_2");
	Game game1(Nivel, RIGHT, agentL, agentR);

	/*if (game1.DoPlay(Nivel, Ball, Left, Right))
	{
		UE_LOG(LogTemp, Warning, TEXT("Se realizó una jugada."));
		///////////Aca hay que obtener las nuevas posiciones de las fichas
		/*printf("\nDespues del movimiento\n");
		printf("ball: %d, %d\n", game1.ball.row, game1.ball.col);
		printf("leftPiece: %d, %d\n", game1.left[0].row, game1.left[0].col);
		printf("rightPiece: %d, %d\n", game1.right[0].row, game1.right[0].col);

		if (nivel == 2 || nivel == 3){
			printf("leftPiece1: %d, %d\n", game1.left[1].row, game1.left[1].col);
			printf("rightPiece1: %d, %d\n", game1.right[1].row, game1.right[1].col);
		}

		if (nivel == 3){
			printf("leftPiece2: %d, %d\n", game1.left[2].row, game1.left[2].col);
			printf("rightPiece2: %d, %d\n", game1.right[2].row, game1.right[2].col);

			printf("leftPiece3: %d, %d\n", game1.left[3].row, game1.left[3].col);
			printf("rightPiece3: %d, %d\n", game1.right[3].row, game1.right[3].col);

			printf("leftPiece4: %d, %d\n", game1.left[4].row, game1.left[4].col);
			printf("rightPiece4: %d, %d\n", game1.right[4].row, game1.right[4].col);
		}
		/////////////////
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hubo un gol, bucle o pelota ahogada."));
	}
	/*
	/////////////////Aca hay que leer el tablero e ir guardando las posiciones de las fichas
	//pelota
	ball.row = 7;
	ball.col = 5;

	//ficha 1 (player y pc)
	left[0].row = 12;
	left[0].col = 5;
	right[0].row = 2;
	right[0].col = 5;

	//ficha 2 (player y pc)
	if (nivel == 2 || nivel == 3){
		left[1].row = 10;
		left[1].col = 3;
		right[1].row = 4;
		right[1].col = 3;
	}

	//fichas 3, 4 y 5 (player y pc)
	if (nivel == 3){
		left[2].row = 10;
		left[2].col = 7;
		right[2].row = 4;
		right[2].col = 7;

		left[3].row = 8;
		left[3].col = 2;
		right[3].row = 6;
		right[3].col = 2;

		left[4].row = 8;
		left[4].col = 8;
		right[4].row = 6;
		right[4].col = 8;
	}

	////////////////////////////////////////////

	Agent* agentL = new Agent(nivel, LEFT, 1, "agentL1_1");
	Agent* agentR = new Agent(nivel, RIGHT, 1, "agentL1_2");
	Game game1(nivel, RIGHT, agentL, agentR);

	if (game1.DoPlay(nivel, ball, left, right))
	{
		///////////Aca hay que obtener las nuevas posiciones de las fichas
		printf("\nDespues del movimiento\n");
		printf("ball: %d, %d\n", game1.ball.row, game1.ball.col);
		printf("leftPiece: %d, %d\n", game1.left[0].row, game1.left[0].col);
		printf("rightPiece: %d, %d\n", game1.right[0].row, game1.right[0].col);

		if (nivel == 2 || nivel == 3){
			printf("leftPiece1: %d, %d\n", game1.left[1].row, game1.left[1].col);
			printf("rightPiece1: %d, %d\n", game1.right[1].row, game1.right[1].col);
		}

		if (nivel == 3){
			printf("leftPiece2: %d, %d\n", game1.left[2].row, game1.left[2].col);
			printf("rightPiece2: %d, %d\n", game1.right[2].row, game1.right[2].col);

			printf("leftPiece3: %d, %d\n", game1.left[3].row, game1.left[3].col);
			printf("rightPiece3: %d, %d\n", game1.right[3].row, game1.right[3].col);

			printf("leftPiece4: %d, %d\n", game1.left[4].row, game1.left[4].col);
			printf("rightPiece4: %d, %d\n", game1.right[4].row, game1.right[4].col);
		}
		/////////////////
	}
	else
	{
		printf("\nDespues del movimiento\n");
		printf("Hubo gol, bucle o pelota ahogada\n");
	}

	getchar();*/
	return 0;
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