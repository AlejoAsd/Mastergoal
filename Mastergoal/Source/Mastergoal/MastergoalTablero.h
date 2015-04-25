// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MastergoalTablero.generated.h"

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
	class UCameraComponent* Camara;

	UPROPERTY(Category = LineasTablero, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LineasTablero;

public:	
	// Define los valores por defecto de la instancia
	AMastergoalTablero(const FObjectInitializer& ObjectInitializer);

	/// Definiciones
	typedef enum EstadoJuego
	{
		JUEGO,
		MOVIMIENTO,
		FIN
	} EstadoJuego;

	typedef enum TipoFicha
	{
		VACIO,
		PELOTA,
		BLANCO_FICHA,
		BLANCO_ARQUERO_EN_AREA,
		BLANCO_ARQUERO_FUERA_AREA,
		ROJO_FICHA,
		ROJO_ARQUERO_EN_AREA,
		ROJO_ARQUERO_FUERA_AREA
	} TipoFicha;

	/// Accesores
	FORCEINLINE class USceneComponent* GetRoot() const { return Root; }
	FORCEINLINE class USpringArmComponent* GetBrazoCamara() const { return BrazoCamara; }
	FORCEINLINE class UCameraComponent* GetCamara() const { return Camara; }
	FORCEINLINE class UStaticMeshComponent* GetLineasTablero() const { return LineasTablero; }

	/// Referencias del tablero
	// Casillas con las fichas del tablero
	class AMastergoalCasilla *Casillas[15][11];

	// Estado del tablero
	int32 EstadoTablero[13][11];
	class AMastergoalFicha* FichaSeleccionada;
	
	/// Tablero
	//Propiedades
	FVector Origen;

	// Nivel
	UPROPERTY(Category = Propiedades, EditAnywhere, BlueprintReadWrite,
	  		  meta = (ClampMin = "1", ClampMax = "3", UIMin = "1", UIMax = "3"))

	// Dimensiones
	int32 Nivel;
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

	/// Casillas
	// Propiedades
	UPROPERTY(Category = Casillas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* CasillaModelo;
	UPROPERTY(Category = Casillas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* CasillaMaterial;
	UPROPERTY(Category = Casillas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* CasillaMaterialAlternativo;

	// Métodos
	UFUNCTION()
	class AMastergoalCasilla* CasillaCrear(int32 Fila, int32 Columna);

	/// Fichas
	// Propiedades
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
	class AMastergoalFicha* FichaCrear(int32 Tipo, int32 Fila, int32 Columna);

	AMastergoalTablero::TipoFicha** FichaObtenerLista();
	void FichaDestruirLista(TipoFicha**& Lista);
	
	/// Juego
	// Propiedades
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	int32 Turno;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	int32 Estado;

	// Métodos
	// Pasa el turno
	void PasarTurno();
	// Intenta mover una ficha del tablero. Devuelve true en caso de haberse realizado el movimiento.
	bool MoverFicha(AMastergoalFicha* Ficha, int32 Fila, int32 Columna);
	// Valida un posible movimiento
	bool ValidarMovimiento(AMastergoalFicha* Ficha, int32 Fila, int32 Columna);
	// Selecciona una ficha para realizar un movimiento
	bool Seleccionar(AMastergoalFicha* Ficha);

	/// Interfaz AActor
	// Llamado cuando inicia el juego o se crea la instancia
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
};
