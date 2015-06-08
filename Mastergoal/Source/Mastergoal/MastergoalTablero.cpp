// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "MastergoalTablero.h"
#include "MastergoalCasilla.h"
#include "MastergoalFicha.h"
#include "MastergoalGameInstance.h"
#include "MastergoalAI.h"
#include "Mensaje.h"
#include "Score.h"
#include "CountDown.h"
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
	BrazoCamara->TargetArmLength = 5100.f;
	BrazoCamara->RelativeLocation = FVector(0.f, -120.f, 0.f);
	BrazoCamara->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
	BrazoCamara->bDoCollisionTest = false;

	// Crear las cámaras asociadas al tablero y asignarlas al brazo
	CamaraBlanco = CreateDefaultSubobject<UCameraComponent>(TEXT("CamaraBlanco"));
	CamaraBlanco->AttachTo(BrazoCamara, USpringArmComponent::SocketName);
	CamaraBlanco->bUsePawnControlRotation = false;
	CamaraBlanco->bConstrainAspectRatio = true;
	CamaraBlanco->AspectRatio = 0.5625f;
	CamaraBlanco->FieldOfView = 30;

	CamaraRojo = CreateDefaultSubobject<UCameraComponent>(TEXT("CamaraRojo"));
	CamaraRojo->AttachTo(BrazoCamara, USpringArmComponent::SocketName);
	CamaraRojo->bUsePawnControlRotation = false;
	CamaraRojo->bConstrainAspectRatio = true;
	CamaraRojo->bAutoActivate = false;
	CamaraRojo->RelativeRotation = FRotator(180.f, 0.f, 0.f);
	CamaraRojo->AspectRatio = 0.5625f;
	CamaraRojo->FieldOfView = 30;

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
	ContraPC = true;
	Nivel = 3;
	PasesMaximos = 4;
	Alto = 13;
	Ancho = 11;
	AnchoArco = 5;

	Estado = JUEGO;
	Turno = BLANCO;
	TiempoTurno = 45;
	EspecialTurno = true;
	CantidadTurnos = 1;
	Pases = 0;
	GolesBlanco = 0;
	GolesRojo = 0;
	FichasEnMovimiento = 0;
	FichaSeleccionada = nullptr;
	Pelota = nullptr;
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
			Casillas[i][j] = nullptr;
			EstadoTablero[i][j] = VACIO;
		}
	}

	// Definir el tamaño de las casillas
	AMastergoalCasilla* Casilla = GetWorld()->SpawnActor<AMastergoalCasilla>(FVector(0, 0, 0), FRotator(0, 0, 0));
	if (Casilla == nullptr) return;

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
			Casilla = CrearCasilla(i, j);

			// Crear la ficha correspondiente a la casilla en caso que corresponda
			if (FichaLista[i][j])
			{
				Casilla->Ficha = CrearFicha(FichaLista[i][j], i, j);
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
	if (LineasTableroMesh != nullptr && LineasTableroMaterial != nullptr)
	{
		LineasTablero->SetStaticMesh(LineasTableroMesh);
		LineasTablero->SetMaterial(0, LineasTableroMaterial);
		LineasTablero->SetRelativeLocation(FVector(-119.f, -101.f, 90.f));
		LineasTablero->SetRelativeScale3D(FVector(1.01, 1.01, 1.01));
		LineasTablero->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No se pudieron cargar las lineas del tablero. El modelo o el material no estan definidos."))
	}

	// Crear selección
	if (SeleccionMesh != nullptr && SeleccionMaterial != nullptr)
	{
		Seleccion->SetStaticMesh(SeleccionMesh);
		Seleccion->SetMaterial(0, SeleccionMaterial);
		Seleccion->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Seleccion->bVisible = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No se pudo cargar el selector. El modelo o el material no estan definidos."))
	}
	
	/// Inicializar el agente AI
	if (ContraPC)
	{
		AI = new MastergoalAI(this->Nivel);
	}

	/// Inicializar la UI
	// Iniciar el contador de turno
	//Temporizador->Inicializar(this);
	//Temporizador->Start(TiempoTurno);

	// Mostrar el turno
	IndicadorTurno->AddScore(TEXT("Turno Blancas"), true);

	// Limpiar el mensaje inicial
	Mensajes->AddScore(TEXT(""), true);
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
	{
		Casillas[Fila][Columna] = nullptr;
		return nullptr;
	}

	// Crear el bloque
	AMastergoalCasilla* Casilla = GetWorld()->SpawnActor<AMastergoalCasilla>(FVector(0, 0, 0), FRotator(0, 0, 0));

	if (Casilla != nullptr)
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

class AMastergoalFicha* AMastergoalTablero::CrearFicha(int32 Tipo, int32 Fila, int32 Columna)
{	
	// Crear la ficha
	AMastergoalFicha* Ficha = GetWorld()->SpawnActor<AMastergoalFicha>(FVector(0, 0, 0), FRotator(0, 0, 0));

	if (Ficha != nullptr)
	{
		// Definir el modelo y material a usar
		int32 Equipo;
		class UStaticMesh* Modelo;
		class UMaterialInstance* Material;
		if (Tipo == PELOTA)
		{
			// Asegurar que sólo exista una pelota en el juego
			if (Pelota != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Se intento crear una pelota habiendo una pelota en el juego. No se va a crear la nueva pelota."))
				return nullptr;
			}

			Pelota = Ficha;
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
			UE_LOG(LogTemp, Warning, TEXT("Se intento crear una ficha de tipo desconocido: %d."), Tipo)
			return nullptr;
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

TipoFicha** AMastergoalTablero::FichaObtenerLista()
{
	// Crear la estructura
	TipoFicha** Fichas = (TipoFicha**)FMemory::Malloc(sizeof(TipoFicha*) * Alto);

	if (Fichas == nullptr) return nullptr;

	for (int32 i = 0; i < Alto; i++)
	{
		Fichas[i] = (TipoFicha*)FMemory::Malloc(sizeof(TipoFicha)* Ancho);
		if (Fichas[i] == nullptr) return nullptr;

		for (int32 j = 0; j < Ancho; j++)
		{
			Fichas[i][j] = VACIO;
		}
	}

	// Asignar los valores
	Fichas[7][5] = PELOTA;
	
	UMastergoalGameInstance* GameInstance = Cast<UMastergoalGameInstance>(GetGameInstance());
	if (GameInstance && GameInstance->Nivel)
		Nivel = GameInstance->Nivel;
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
		Fichas[2][5] = BLANCO_FICHA;
		// Rojo
		Fichas[10][5] = ROJO_FICHA;
		Fichas[12][5] = ROJO_FICHA;
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
 * Decide el próximo turno. Toma en cuenta la existencia de un pase.
 */
void AMastergoalTablero::PasarTurno()
{
	// Asegurarse que no hayan fichas en movimiento
	if (FichasEnMovimiento == 0)
	{
		Estado = JUEGO;
	}

	UE_LOG(LogTemp, Warning, TEXT("Estado: %d"), Estado);
	if (Estado == JUEGO || Estado == PASE)
	{
		if (Pelota == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Se intento pasar de turno sin haber una pelota en juego"));
			return;
		}

		// Si se metió un gol
		if (Casillas[Pelota->Fila][Pelota->Columna]->Arco)
		{
			if (Casillas[Pelota->Fila][Pelota->Columna]->Equipo == BLANCO)
			{
				GolesRojo++;
				UE_LOG(LogTemp, Warning, TEXT("Gol de las rojas"));
				Mensajes->AddScore(TEXT("Gol de las rojas"), false);
				Contador->AddScoreRojo();
			}
			else if (Casillas[Pelota->Fila][Pelota->Columna]->Equipo == ROJO)
			{
				GolesBlanco++;
				UE_LOG(LogTemp, Warning, TEXT("Gol de las blancas"));
				Mensajes->AddScore(TEXT("Gol de las blancas"), false);
				Contador->AddScoreBlanco();
			}

			// Reiniciar en caso que no se haya anotado todos los goles
			if (GolesBlanco < 2 && GolesRojo < 2)
			{
				Reiniciar(Casillas[Pelota->Fila][Pelota->Columna]->Equipo);
			}
			// Terminar la partida si alguien anotó dos goles
			else
			{
				TerminarJuego(false);
			}
		}
		// Si el equipo tiene influencia sobre la pelota, debe moverla
		if (Pases < PasesMaximos &&
			Casillas[Pelota->Fila][Pelota->Columna]->TieneInfluencia(Turno, true))
		{
			Estado = PASE;
			Seleccionar(Pelota);
			Pases++;
			UE_LOG(LogTemp, Warning, TEXT("Entering pass state %d"), Pases);
		}
		// Si la pelota está en una casilla especial sin influencia alguna
		else if (EspecialTurno && Casillas[Pelota->Fila][Pelota->Columna]->Especial &&
				Casillas[Pelota->Fila][Pelota->Columna]->InfluenciaCero() &&
				Casillas[Pelota->Fila][Pelota->Columna]->Equipo != Turno)
		{
			Estado = JUEGO;
			Pases--;
			EspecialTurno = false;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Estado:%d Pases:%d PasesMaximos:%d"), Estado, Pases, PasesMaximos)
			UE_LOG(LogTemp, Warning, TEXT("T:%d IB:%d IR:%d R:%d Changing turn"), Turno, Casillas[Pelota->Fila][Pelota->Columna]->InfluenciaBlanco, Casillas[Pelota->Fila][Pelota->Columna]->InfluenciaRojo, Casillas[Pelota->Fila][Pelota->Columna]->TieneInfluencia(Turno, true))
			
			CambiarTurno();
			
			// Regla de empate
			if (CantidadTurnos == 51)
			{
				TerminarJuego(false);
			}

			Estado = JUEGO;
		}

		// Si se está contra la PC y es su turno realizar su jugada
		if (FichasEnMovimiento == 0 && ContraPC && Turno == ROJO)
		{
			// Movimiento de Jugador
			if (Estado == JUEGO)
			{
				UE_LOG(LogTemp, Warning, TEXT("AI Turn"));
				Accion MovimientoFicha = AI->Jugar(Casillas, Fichas);
				Seleccionar(MovimientoFicha.Ficha);
				MoverFicha(MovimientoFicha.Ficha, MovimientoFicha.Fila, MovimientoFicha.Columna);
			}
			// Movimiento de Ficha
			else if (Estado == PASE)
			{
				UE_LOG(LogTemp, Warning, TEXT("AI Pass"));
				Accion MovimientoPelota = AI->ProximaJugada();
				MoverFicha(MovimientoPelota.Ficha, MovimientoPelota.Fila, MovimientoPelota.Columna);
			}
		}
	}
}

/*
 * Obtiene la ficha que realizó el pase. En caso de haber múltiples jugadores posibles, se retorna nullptr.
 */
class AMastergoalFicha* AMastergoalTablero::ObtenerFichaPase(int Fila, int Columna)
{
	AMastergoalFicha* Ficha = nullptr;

	for (int i = (Fila - 1); i <= (Fila + 1); i++)
	{
		for (int j = (Columna - 1); j <= (Columna + 1); j++)
		{
			if ((i != Fila || j != Columna) &&
				i > 0 && i < (Alto - 1) &&
				j >= 0 && j < Ancho &&
				Casillas[i][j] != nullptr &&
				Casillas[i][j]->Ficha != nullptr &&
				Casillas[i][j]->Ficha->Equipo == Turno)
			{
				if (Ficha == nullptr)
				{
					Ficha = Casillas[i][j]->Ficha;
				}
				else
				{
					Ficha = nullptr;
					return Ficha;
				}
			}
		}
	}

	return Ficha;
}

/*
 * Cambia el turno y actualiza la UI
 */
void AMastergoalTablero::CambiarTurno()
{
	Pases = 0;
	EspecialTurno = true;
	CantidadTurnos++;

	// Limpiar la selección
	if (FichaSeleccionada != nullptr)
	{
		Seleccionar(FichaSeleccionada);
	}

	// Cambiar el turno
	if (Turno == BLANCO)
	{
		Turno = ROJO;
		if (FichasEnMovimiento == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Estado:%d"), Estado)
			IndicadorTurno->AddScore(TEXT("Turno Rojas"), true);
		}
	}
	else if (Turno == ROJO)
	{
		Turno = BLANCO;
		if (FichasEnMovimiento == 0)
		{
			IndicadorTurno->AddScore(TEXT("Turno Blancas"), true);
		}
	}

	// Reiniciar el temporizador
	//Temporizador->Start(TiempoTurno);
}

void AMastergoalTablero::ModificarInfluencia(AMastergoalFicha* Ficha, bool Inverso)
{
	int32 Cantidad = 1 * (Inverso ? -1 : 1);

	if (Ficha->EsArquero(true))
	{
		Cantidad *= 6;
	}

	for (int i = (Ficha->Fila - 1); i <= (Ficha->Fila + 1); i++)
	{
		for (int j = (Ficha->Columna - 1); j <= (Ficha->Columna + 1); j++)
		{
			if ((i != Ficha->Fila || j != Ficha->Columna) &&
				i > 0 && i < (Alto - 1) &&
				j >= 0 && j < Ancho &&
				Casillas[i][j] != nullptr)
			{
				if (Ficha->Equipo == BLANCO)
				{
					Casillas[i][j]->InfluenciaBlanco += Cantidad;
				}
				else if (Ficha->Equipo == ROJO)
				{
					Casillas[i][j]->InfluenciaRojo += Cantidad;
				}
			}
		}
	}
}

/*
 * Mueve una ficha
 */
bool AMastergoalTablero::MoverFicha(AMastergoalFicha* Ficha, int32 Fila, int32 Columna)
{
	if (Ficha == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Attempted to move nonexisting piece to %d,%d"), Ficha->Columna, Fila, Columna);
	}

	UE_LOG(LogTemp, Warning, TEXT("Moving selected piece from %d,%d to %d,%d"), Ficha->Fila, Ficha->Columna, Fila, Columna);

	if (Estado != JUEGO && Estado != PASE && Estado != REINICIANDO)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot move in this game state: %d"), Estado)
		return false;
	}

	// Validar que el movimiento sea posible
	bool Valido = true;
	if (Estado != REINICIANDO && ((ContraPC && Turno != ROJO) || (!ContraPC)))
	{
		Valido = ValidarMovimiento(Ficha, Fila, Columna);
	}

	// Mover la ficha
	if (Valido)
	{
		/// Definir el estado de movimiento
		if (Estado != REINICIANDO)
		{
			Seleccionar(Ficha);
		}

		//// Actualizar el estado del tablero
		/// Limpiar el estado anterior
		EstadoTablero[Ficha->Fila][Ficha->Columna] = VACIO;
		Casillas[Ficha->Fila][Ficha->Columna]->Ficha = nullptr;
		Casillas[Ficha->Fila][Ficha->Columna]->Arquero = false;
		// Arquero
		if (Ficha->EsArquero(false))
		{
			if (Ficha->Columna > 0)
			{
				EstadoTablero[Ficha->Fila][Ficha->Columna - 1] = VACIO;
				Casillas[Ficha->Fila][Ficha->Columna - 1]->Ficha = nullptr;
				Casillas[Ficha->Fila][Ficha->Columna - 1]->Arquero = false;
			}
			if (Ficha->Columna < Ancho)
			{
				EstadoTablero[Ficha->Fila][Ficha->Columna + 1] = VACIO;
				Casillas[Ficha->Fila][Ficha->Columna + 1]->Ficha = nullptr;
				Casillas[Ficha->Fila][Ficha->Columna + 1]->Arquero = false;
			}
		}
		
		/// Actualizar Tipos
		// Arquero
		// Saliendo del área
		if (Ficha->EsArquero(true) &&
			(!Casillas[Fila][Columna]->Area ||
			 Casillas[Fila][Columna]->Equipo != Ficha->Equipo))
		{
			Ficha->Tipo++;
			Ficha->ArqueroEnArea = false;
		}
		// Entrando al área
		if (Ficha->EsArqueroFueraDeArea() &&
			Casillas[Fila][Columna]->Area &&
			Casillas[Fila][Columna]->Equipo == Ficha->Equipo)
		{
			Ficha->Tipo--;
			Ficha->ArqueroEnArea = true;
		}

		/// Escribir el estado actual
		EstadoTablero[Fila][Columna] = Ficha->Tipo;
		Casillas[Fila][Columna]->Ficha = Ficha;
		Casillas[Fila][Columna]->Arquero = Ficha->EsArquero(true);
		if (Ficha->EsArquero(false))
		{
			// Brazo izquierdo
			if (Columna > 0)
			{
				if (Ficha->EsArquero(true) && Casillas[Fila][Columna - 1]->Area)
				{
					EstadoTablero[Fila][Columna - 1] = Ficha->Tipo;
					Casillas[Fila][Columna - 1]->Arquero = true;
				}
				else
				{
					EstadoTablero[Fila][Columna - 1] = Ficha->Tipo + (Ficha->EsArquero(true) ? 1 : 0);
					Casillas[Fila][Columna - 1]->Arquero = false;
				}
				Casillas[Fila][Columna - 1]->Ficha = Ficha;
			}
			// Brazo derecho
			if (Columna < (Ancho - 1))
			{
				if (Ficha->EsArquero(true) && Casillas[Fila][Columna + 1]->Area)
				{
					EstadoTablero[Fila][Columna + 1] = Ficha->Tipo;
					Casillas[Fila][Columna + 1]->Arquero = true;
				}
				else
				{
					EstadoTablero[Fila][Columna + 1] = Ficha->Tipo + (Ficha->EsArquero(true) ? 1 : 0);
					Casillas[Fila][Columna + 1]->Arquero = false;
				}
				Casillas[Fila][Columna + 1]->Ficha = Ficha;
			}
		}

		/// Mover la ficha
		Ficha->Mover(Fila, Columna);
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
	// Asegurar que esté dentro del tablero. Los tableros cuentan como fuera del arco excepto en el caso de la pelota.
	if (Fila < 0 || Fila >= Alto ||
		Columna < 0 || Columna >= Ancho)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid destination"))
		Mensajes->AddScore(TEXT("Casilla invalida"), false);
		return false;
	}

	// Asegurar que exista una casilla para la posición indicada
	if (Ficha->Tipo != PELOTA && 
		(Fila == 0 || Fila == (Alto - 1)) &&
		(Columna >= ArcoOffset || Columna <= ArcoOffset + AnchoArco))
	{
		UE_LOG(LogTemp, Warning, TEXT("Players cannot enter the goal"))
		Mensajes->AddScore(TEXT("Los jugadores no pueden entrar al arco"), false);
		return false;
	}

	// Asegurar que no sea un corner del equipo
	if ((Ficha->Equipo == Casillas[Fila][Columna]->Equipo ||
		(Ficha->Tipo == PELOTA && Turno == Casillas[Fila][Columna]->Equipo)) &&
		Casillas[Fila][Columna]->Corner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot move to own corner"))
		Mensajes->AddScore(TEXT("No se puede mover a un corner propio"), false);
		return false;
	}

	// Asegurar que al mover la pelota la casilla pertenezca al equipo de turno
	if (Ficha->Tipo == PELOTA && !Casillas[Fila][Columna]->TieneInfluencia(Turno, false))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot move to enemy influenced cell"))
		Mensajes->AddScore(TEXT("La pelota no puede terminar en posesion del oponente"), false);
		return false;
	}

	// Asegurar que la pelota termine en una casilla neutra
	if (Ficha->Tipo == PELOTA &&
		Pases == PasesMaximos &&
		!Casillas[Fila][Columna]->InfluenciaNeutra())
	{
		UE_LOG(LogTemp, Warning, TEXT("Already made 3 passes, must end in a neutral cell"))
		Mensajes->AddScore(TEXT("Solo queda un pase disponible. La pelota debe quedar en una casilla neutra"), false);
		return false;
	}

	// Asegurar que la pelota no termine del lado del jugador que saca en el primer turno
	if (CantidadTurnos == 1 &&
		Pases == PasesMaximos &&
		Turno == Casillas[Fila][Columna]->Equipo)
	{
		UE_LOG(LogTemp, Warning, TEXT("The ball cannot end in the opening team's side of the field"))
		Mensajes->AddScore(TEXT("La pelota no puede terminar del lado del equipo que empieza"), false);
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
		UE_LOG(LogTemp, Warning, TEXT("Movement is not straight"))
		Mensajes->AddScore(TEXT("El movimiento debe ser recto"), false);
		return false;
	}

	// El movimiento está dentro del rango de la ficha
	if (DeltaFila > MaximoMovimientos ||
		DeltaColumna > MaximoMovimientos)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough movements"))
		Mensajes->AddScore(TEXT("No se puede desplazar esa cantidad de casillas"), false);
		return false;
	}

	// La casilla objetivo está ocupada
	if (EstadoTablero[Fila][Columna] != VACIO)
	{
		bool Valido = false;

		// Excepción para permitir a un arquero moverse a los costados
		if (Ficha->EsArquero(false) &&
			DeltaFila == 0 && DeltaColumna == 1 &&
			(Columna > 0 &&
			EstadoTablero[Fila][Columna - DeltaColumna] == VACIO ||
			(EstadoTablero[Fila][Columna - DeltaColumna] == PELOTA ||
			EstadoTablero[Fila][Columna - DeltaColumna] == Ficha->Tipo ||
			EstadoTablero[Fila][Columna - DeltaColumna] == (Ficha->Tipo + 1)) &&
			(Columna < (Ancho - 1) &&
			EstadoTablero[Fila][Columna + DeltaColumna] == VACIO ||
			EstadoTablero[Fila][Columna + DeltaColumna] == PELOTA ||
			EstadoTablero[Fila][Columna + DeltaColumna] == Ficha->Tipo ||
			EstadoTablero[Fila][Columna + DeltaColumna] == (Ficha->Tipo + 1))))
		{
			Valido = true;
		}

		if (!Valido)
		{
			UE_LOG(LogTemp, Warning, TEXT("ET:%d Not an empty cell at the end"), EstadoTablero[Fila][Columna])
			Mensajes->AddScore(TEXT("Se debe mover a una casilla libre"), false);
			return false;
		}
	}
	// La casilla objetivo está libre
	else
	{
		// No permitir a un arquero mover a una casilla donde hayan fichas adyacentes
		if (Ficha->EsArquero(false) &&
			((Columna > 0 &&
			EstadoTablero[Fila][Columna - 1] != VACIO &&
			EstadoTablero[Fila][Columna - 1] != PELOTA &&
			EstadoTablero[Fila][Columna - 1] != Ficha->Tipo &&
			EstadoTablero[Fila][Columna - 1] != (Ficha->Tipo + 1)) ||
			(Columna < (Ancho - 1) &&
			EstadoTablero[Fila][Columna + 1] != VACIO &&
			EstadoTablero[Fila][Columna + 1] != PELOTA &&
			EstadoTablero[Fila][Columna + 1] != Ficha->Tipo &&
			EstadoTablero[Fila][Columna + 1] != (Ficha->Tipo + 1))))
		{
			UE_LOG(LogTemp, Warning, TEXT("(%d,%d) T:%d G:%d | (%d,%d) ET:%d Existing piece at an adjacent cell"), Ficha->Fila, Ficha->Columna, Ficha->Tipo, Ficha->EsArquero(false), Fila, Columna, EstadoTablero[Fila][Columna])
			Mensajes->AddScore(TEXT("Una casilla adyacente no se encuentra libre"), false);
			return false;
		}
	}

	// Asegurar que no sea un autopase
	if (Ficha->Tipo == PELOTA)
	{
		AMastergoalFicha* FichaPase = ObtenerFichaPase(Ficha->Fila, Ficha->Columna);
		AMastergoalFicha* FichaReceptora = ObtenerFichaPase(Fila, Columna);

		if (FichaPase != nullptr && FichaPase == FichaReceptora)
		{
			UE_LOG(LogTemp, Warning, TEXT("Autopasses are not allowed"))
			Mensajes->AddScore(TEXT("No se puede hacer un autopase"), false);

			return false;
		}
	}

	// Asegurar que el movimiento del jugador no rompa el balance de influencia
	FVector2D DeltaOrigen = FVector2D(Pelota->Fila - Ficha->Fila, Pelota->Columna - Ficha->Columna);
	if (DeltaOrigen.X < 0) DeltaOrigen.X *= -1;
	if (DeltaOrigen.Y < 0) DeltaOrigen.Y *= -1;

	FVector2D DeltaDestino = FVector2D(Pelota->Fila - Fila, Pelota->Columna - Columna);
	if (DeltaDestino.X < 0) DeltaDestino.X *= -1;
	if (DeltaDestino.Y < 0) DeltaDestino.Y *= -1;

	if (Ficha != Pelota && DeltaOrigen.X <= 1 && DeltaOrigen.Y <= 1 && (DeltaDestino.X > 1 || DeltaDestino.Y > 1))
	{
		UE_LOG(LogTemp, Warning, TEXT("Piece %d,%d cannot be selected, influence would break"), Ficha->Fila, Ficha->Columna);
		Mensajes->AddScore(TEXT("No se puede perder la neutralidad de la pelota"), false);

		return false;
	}

	// Asegurar que se rompa la influencia si es posible
	// Desactivado temporalmente. Es muy costoso computar.
	/*if (NecesidadRoturaInfluencia())
	{
		UE_LOG(LogTemp, Warning, TEXT("A possible move to gain influence on the player's side cannot be ommitted"))
		return false;
	}*/
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

		if (EstadoTablero[Fila][Columna] != VACIO)
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
				else if (Casillas[Fila][Columna] != nullptr &&
						 Casillas[Fila][Columna]->Ficha != nullptr &&
						 Casillas[Fila][Columna]->Ficha->Equipo != Turno)
				{
					UE_LOG(LogTemp, Warning, TEXT("Goalie Block Tipo:%d ET:%d PELOTA:%d BAEA:%d RAEA:%d"), Ficha->Tipo, EstadoTablero[Fila][Columna], (int32)PELOTA, (int32)BLANCO_ARQUERO_EN_AREA, (int32)ROJO_ARQUERO_EN_AREA)
					Mensajes->AddScore(TEXT("El arquero atajaria ese tiro"), false);
					return false;
				}
			}
			else if (Ficha->Tipo != PELOTA && 
					Casillas[Fila][Columna] != nullptr &&
					Casillas[Fila][Columna]->Ficha != nullptr &&
					Casillas[Fila][Columna]->Ficha->Fila == Fila &&
					Casillas[Fila][Columna]->Ficha->Columna == Columna)
			{
				UE_LOG(LogTemp, Warning, TEXT("Cannot go through other pieces"))
				Mensajes->AddScore(TEXT("No se puede atravesar a otros jugadores"), false);
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
	if (Ficha == nullptr || Estado == MOVIMIENTO || Estado == FIN)
	{
		return false;
	}
	
	// Seleccionar
	if (Ficha->Equipo == Turno || Estado == PASE)
	{
		if (FichaSeleccionada == nullptr || FichaSeleccionada != Ficha)
		{
			UE_LOG(LogTemp, Warning, TEXT("Selecting piece at %d,%d Type:%d T:%d Goalie:%d Goalie(Area):%d"), Ficha->Fila, Ficha->Columna, Ficha->Tipo, Ficha->Equipo, Ficha->EsArquero(false), Ficha->EsArquero(true));
			FichaSeleccionada = Ficha;

			// Mover la selección
			if (Seleccion != nullptr)
			{
				FVector Posicion = Ficha->GetActorLocation();
				FVector Size = Ficha->GetSize() + ProfundidadCasillas;

				Posicion.Z += Size.Z;

				Seleccion->SetRelativeLocation(Posicion);
			}
			
		}
		// Deseleccionar
		else if (FichaSeleccionada == Ficha)
		{
			UE_LOG(LogTemp, Warning, TEXT("Deselecting piece at %d,%d"), Ficha->Fila, Ficha->Columna);
			FichaSeleccionada = nullptr;
		}
	}

	// Cambiar la visibilidad
	Seleccion->SetVisibility(FichaSeleccionada != nullptr);

	return FichaSeleccionada != nullptr;
}

void AMastergoalTablero::Reiniciar(int32 Turno)
{
	// Resetear las variables
	this->Turno = Turno;
	CantidadTurnos = 1;
	EspecialTurno = true;

	// Limpiar el estado del tablero
	for (int i = 0; i < Alto; i++)
	{
		for (int j = 0; j < Ancho; j++)
		{
			EstadoTablero[i][j] = VACIO;
		}
	}
	
	// Mover los jugadores a sus posiciones iniciales
	for (auto Iter(Fichas.CreateIterator()); Iter; Iter++)
	{
		if (!(*Iter)->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Error, TEXT("When resetting, one of the pieces was not validLowLevel."))
			continue;
		}
		Estado = REINICIANDO;

		MoverFicha((*Iter), (*Iter)->FilaInicial, (*Iter)->ColumnaInicial);
	}
}

void AMastergoalTablero::TerminarJuego(bool Invalido)
{
	if (Invalido || GolesBlanco == GolesRojo)
	{
		// UI Empate
		UE_LOG(LogTemp, Warning, TEXT("Empate"))
		Mensajes->AddScore(TEXT("Empate..."), false);
	}
	else if (GolesBlanco > GolesRojo)
	{
		// UI Ganaste
		UE_LOG(LogTemp, Warning, TEXT("Ganaron las blancas"))
		Mensajes->AddScore(TEXT("Ganaron las blancas"), false);
	}
	else if (GolesRojo > GolesBlanco)
	{
		// UI Perdiste
		UE_LOG(LogTemp, Warning, TEXT("Ganaron las rojas"))
		Mensajes->AddScore(TEXT("Ganaron las rojas"), false);
	}
	// Definir el juego terminado
	Estado = FIN;
}

/*
 * Destruye la lista de fichas proveída.
 */
void AMastergoalTablero::FichaDestruirLista(TipoFicha**& Lista)
{
	if (Lista == nullptr) return;

	for (int32 i = 0; i < Alto; i++)
	{
		FMemory::Free(Lista[i]);
	}
	FMemory::Free(Lista);
}