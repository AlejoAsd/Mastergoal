// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MastergoalTablero.generated.h"

/// Definiciones
enum Equipo
{
	BLANCO = -1,
	NINGUNO,
	ROJO,
	AMBOS
};

enum EstadoJuego
{
	JUEGO,
	PASE,
	MOVIMIENTO,
	REINICIANDO,
	FIN
};

enum TipoFicha
{
	VACIO,
	PELOTA,
	BLANCO_FICHA,
	BLANCO_ARQUERO_EN_AREA,
	BLANCO_ARQUERO_FUERA_AREA,
	ROJO_FICHA,
	ROJO_ARQUERO_EN_AREA,
	ROJO_ARQUERO_FUERA_AREA
};

/*
 * Tablero de Mastergoal Configurable
 * El tablero genera e instancia el tablero y todas las fichas necesarias.
 * Por el momento la configuración de niveles es estática y consiste en la elección de uno de tres niveles disponibles.
 * La posición (0,0) se encuentra en la esquina inferior izquierda.
 * Visto desde arriba:
 *     X tiende hacia arriba cuando incrementa.
 *     Y tiende hacia la derecha cuando incrementa.
 */
UCLASS()
class MASTERGOAL_API AMastergoalTablero : public AActor
{
	GENERATED_BODY()
	
	// Componente base raíz
	UPROPERTY(Category = Tablero, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	// Brazo de Cámara - Encargado de atajar la cámara
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camara, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* BrazoCamara;

	// Cámara
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camara, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CamaraBlanco;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camara, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CamaraRojo;

	UPROPERTY(Category = LineasTablero, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LineasTablero;

	UPROPERTY(Replicated, Category = Seleccion, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Seleccion;

public:	
	// Define los valores por defecto de la instancia
	AMastergoalTablero(const FObjectInitializer& ObjectInitializer);

	/// Accesores
	FORCEINLINE class USceneComponent* GetRoot() const { return Root; }
	FORCEINLINE class USpringArmComponent* GetBrazoCamaraBlanco() const { return BrazoCamara; }
	FORCEINLINE class UCameraComponent* GetCamaraBlanco() const { return CamaraBlanco; }
	FORCEINLINE class UCameraComponent* GetCamaraRojo() const { return CamaraRojo; }
	FORCEINLINE class UStaticMeshComponent* GetLineasTablero() const { return LineasTablero; }
	FORCEINLINE class UStaticMeshComponent* GetSeleccion() const { return Seleccion; }

	/// UI
	UPROPERTY(Category = UI, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class ACountdown* Temporizador;
	UPROPERTY(Category = UI, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AMensaje* Mensajes;
	UPROPERTY(Category = UI, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AMensaje* IndicadorTurno;
	UPROPERTY(Category = UI, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AScore* Contador;

	// Mensajes
	UPROPERTY(Category = UI, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString Mensaje;

	/// Multiplayer
	UPROPERTY(Category = Multiplayer, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool Multiplayer;
	UPROPERTY(Category = Multiplayer, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 JugadorJuega;

	/// Referencias del tablero
	// Agente AI
	class MastergoalAI* AI;

	// Casillas con las fichas del tablero
	class AMastergoalCasilla* Casillas[15][11];

	// Estado del tablero
	int32 EstadoTablero[15][11];
	class AMastergoalFicha* Pelota;
	class AMastergoalFicha* FichaSeleccionada;

	/// Tablero
	//Propiedades
	FVector Origen;

	// Nivel
	UPROPERTY(Category = Propiedades, EditAnywhere, BlueprintReadWrite,
	  		  meta = (ClampMin = "1", ClampMax = "3", UIMin = "1", UIMax = "3"))
	int32 Nivel;
	UPROPERTY(Category = Propiedades, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 PasesMaximos;

	// Dimensiones
	UPROPERTY(Category = Dimensiones, EditAnywhere, BlueprintReadWrite)
	int32 Alto;
	UPROPERTY(Category = Dimensiones, EditAnywhere, BlueprintReadWrite)
	int32 Ancho;
	UPROPERTY(Category = Dimensiones, EditAnywhere, BlueprintReadWrite)
	int32 AnchoArco;
	UPROPERTY(Category = Dimensiones, VisibleAnywhere, BlueprintReadOnly)
	int32 AltoCasillas;
	UPROPERTY(Category = Dimensiones, VisibleAnywhere, BlueprintReadOnly)
	int32 AnchoCasillas;
	UPROPERTY(Category = Dimensiones, VisibleAnywhere, BlueprintReadOnly)
	int32 ProfundidadCasillas;

	// Lineas de Tablero
	UPROPERTY(Category = LineasTablero, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* LineasTableroMesh;
	UPROPERTY(Category = LineasTablero, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* LineasTableroMaterial;

	// Selección de Fichas
	UPROPERTY(Category = Seleccion, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* SeleccionMesh;
	UPROPERTY(Category = Seleccion, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* SeleccionMaterial;

	/// Casillas
	// Propiedades
	UPROPERTY(Category = Casillas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* CasillaModelo;
	UPROPERTY(Category = Casillas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* CasillaMaterial;
	UPROPERTY(Category = Casillas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* CasillaMaterialAlternativo;

	// Métodos
	class AMastergoalCasilla* CrearCasilla(int32 Fila, int32 Columna);

	/// Fichas
	// Propiedades
	UPROPERTY(Category = Fichas, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class AMastergoalFicha*> Fichas;

	TipoFicha** FichaLista;

	UPROPERTY(Category = Fichas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* FichaModeloPelota;
	UPROPERTY(Category = Fichas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* FichaMaterialPelota;

	UPROPERTY(Category = Fichas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* FichaModeloJugador;
	UPROPERTY(Category = Fichas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* FichaModeloArquero;
	UPROPERTY(Category = Fichas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* FichaMaterialJugador;
	UPROPERTY(Category = Fichas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* FichaMaterialJugadorAlternativo;

	// Métodos
	class AMastergoalFicha* CrearFicha(int32 Tipo, int32 Fila, int32 Columna);

	TipoFicha** FichaObtenerLista();
	void FichaDestruirLista(TipoFicha**& Lista);
	
	/// Juego
	// Propiedades
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Juego, meta = (AllowPrivateAccess = "true"))
	bool ContraPC;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Juego, meta = (AllowPrivateAccess = "true"))
	int32 Turno;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Juego, meta = (AllowPrivateAccess = "true"))
	int32 TiempoTurno;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Juego, meta = (AllowPrivateAccess = "true"))
	bool EspecialTurno;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Juego, meta = (AllowPrivateAccess = "true"))
	int32 CantidadTurnos;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Juego, meta = (AllowPrivateAccess = "true"))
	int32 Pases;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Juego, meta = (AllowPrivateAccess = "true"))
	int32 GolesBlanco;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Juego, meta = (AllowPrivateAccess = "true"))
	int32 GolesRojo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Juego, meta = (AllowPrivateAccess = "true"))
	int32 Estado;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Juego, meta = (AllowPrivateAccess = "true"))
	int32 FichasEnMovimiento;

	// Métodos
	// Chequea la lógica para saber si se debe pasar el turno
	void PasarTurno();
	// Cambia el turno
	void CambiarTurno();
	// Modifica la influencia de las casillas adyacentes a la ficha.
	void ModificarInfluencia(AMastergoalFicha* Ficha, bool Inverso);
	// Valida un posible movimiento
	bool ValidarMovimiento(AMastergoalFicha* Ficha, int32 Fila, int32 Columna);
	// Intenta mover una ficha del tablero. Devuelve true en caso de haberse realizado el movimiento. Utilizado por el servidor.
	bool MoverFicha(AMastergoalFicha* Ficha, int32 Fila, int32 Columna);
	// Versión RPC del método MoverFicha(). Utilizado por clientes.
	void ClientMoverFicha(AMastergoalFicha* Ficha, int32 Fila, int32 Columna);
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerMoverFicha(AMastergoalFicha* Ficha, int32 Fila, int32 Columna);
	virtual bool ServerMoverFicha_Validate(AMastergoalFicha* Ficha, int32 Fila, int32 Columna);
	virtual void ServerMoverFicha_Implementation(AMastergoalFicha* Ficha, int32 Fila, int32 Columna);
	// Selecciona una ficha para realizar un movimiento.
	bool Seleccionar(AMastergoalFicha* Ficha);
	// Versión RPC del método Seleccionar(). Utilizado por clientes.
	void ClientSeleccionar(AMastergoalFicha* Ficha);
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSeleccionar(AMastergoalFicha* Ficha);
	virtual bool ServerSeleccionar_Validate(AMastergoalFicha* Ficha);
	virtual void ServerSeleccionar_Implementation(AMastergoalFicha* Ficha);
	// Obtiene el jugador que realizó el pase
	class AMastergoalFicha* ObtenerFichaPase(int32 Fila, int32 Columna);
	// Reinicia los estados de la partida, manteniendo los goles
	void Reiniciar(int32 Turno);
	// Termina el juego
	void TerminarJuego(bool Invalido);

	/// Interfaz AActor
	// Llamado cuando inicia el juego o se crea la instancia
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
};
