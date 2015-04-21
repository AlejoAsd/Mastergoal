// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MastergoalCasilla.generated.h"

UCLASS()
class MASTERGOAL_API AMastergoalCasilla : public AActor
{
	GENERATED_BODY()

	// Componente base raíz
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

	// Modelo (Mesh) de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* Mesh;

	// Material del modelo de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* Material;

	/// Métodos
	// Inicializa el objeto
	void Inicializar(class AMastergoalTablero* Tablero, UStaticMesh* Mesh, UMaterialInstance* Material);
	// Actualiza el modelo y material del objeto
	void ActualizarComponenteMesh();
	// Obtiene el tamaño del modelo
	UFUNCTION(Category = Modelo, BlueprintCallable)
	FVector GetSize();
};