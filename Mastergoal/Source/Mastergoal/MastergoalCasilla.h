// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MastergoalCasilla.generated.h"

UCLASS()
class MASTERGOAL_API AMastergoalCasilla : public AActor
{
	GENERATED_BODY()

	// Componente base ra�z
	UPROPERTY(Category = Casilla, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	// Componente StaticMesh de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ComponenteMesh;

public:
	// Define los valores default de la instancia
	AMastergoalCasilla(const FObjectInitializer& ObjectInitializer);

	/// Accesores
	// Objeto
	FORCEINLINE class USceneComponent* GetRoot() const { return Root; }
	FORCEINLINE class UStaticMeshComponent* GetComponenteMesh() const { return ComponenteMesh; };

	/// Propiedades
	// Tablero al que pertenece la casilla
	UPROPERTY(Category = Tablero, VisibleDefaultsOnly, BlueprintReadOnly)
	class AMastergoalTablero* Tablero;
	// Ficha que se encuentra actualmente en la casilla
	UPROPERTY(Category = Tablero, VisibleDefaultsOnly, BlueprintReadOnly)
	class AMastergoalFicha* Ficha;

	UPROPERTY(Category = Juego, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 Equipo;
	UPROPERTY(Category = Juego, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 InfluenciaBlanco;
	UPROPERTY(Category = Juego, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 InfluenciaRojo;
	UPROPERTY(Category = Juego, VisibleDefaultsOnly, BlueprintReadOnly)
	bool Area;
	UPROPERTY(Category = Juego, VisibleDefaultsOnly, BlueprintReadOnly)
	bool Corner;
	UPROPERTY(Category = Juego, VisibleDefaultsOnly, BlueprintReadOnly)
	bool Arco;
	UPROPERTY(Category = Juego, VisibleDefaultsOnly, BlueprintReadOnly)
	bool Especial;
	UPROPERTY(Category = Juego, VisibleDefaultsOnly, BlueprintReadOnly)
	bool Arquero;

	UPROPERTY(Category = Tablero, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 Fila;
	UPROPERTY(Category = Tablero, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 Columna;

	// Modelo (Mesh) de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* Mesh;

	// Material del modelo de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* Material;

	/// M�todos
	// Inicializa el objeto
	void Inicializar(class AMastergoalTablero* Tablero, int32 Equipo, bool Area, bool Corner, bool Arco, bool Especial,
					 int32 Fila, int32 Columna, UStaticMesh* Mesh, UMaterialInstance* Material);
	// Actualiza el modelo y material del objeto
	void ActualizarComponenteMesh();
	// Obtiene el tama�o del modelo
	UFUNCTION(Category = Modelo, BlueprintCallable)
	FVector GetSize();
	bool InfluenciaNeutra();
	bool InfluenciaCero();
	bool TieneInfluencia(int32 Equipo, bool Estricto);

	/// Handlers
	// Click
	UFUNCTION()
	void OnClick(UPrimitiveComponent* ClickedComp);

	// Touch
	UFUNCTION()
	void OnTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);
};