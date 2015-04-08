// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MastergoalCasilla.generated.h"

UCLASS()
class MASTERGOAL_API AMastergoalCasilla : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	// Componente StaticMesh de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ComponenteMesh;

public:
	// Define los valores default de la instancia
	AMastergoalCasilla(const FObjectInitializer& ObjectInitializer);
	// Inicializa el objeto
	void Inicializar(class AMastergoalTablero* Tablero, UStaticMesh* Mesh, UMaterialInstance* Material);
	// Actualiza el modelo y material del objeto
	void ActualizarComponenteMesh();

	/// Accesores
	// Objeto
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return Root; }
	FORCEINLINE class UStaticMeshComponent* GetComponenteMesh() const { return ComponenteMesh; };
	
	UFUNCTION(Category = Modelo, BlueprintCallable)
	FVector GetSize();

	// Modelo (Mesh) de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* Mesh;

	// Material del modelo de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* Material;

	// Tablero al que pertenece la casilla
	UPROPERTY()
	class AMastergoalTablero* Tablero;

	/// Handlers
	// Click
	UFUNCTION()
	void OnClick(UPrimitiveComponent* ClickedComp);

	// Touch
	UFUNCTION()
	void OnTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);
};