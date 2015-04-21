// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MastergoalFicha.generated.h"

UCLASS()
class MASTERGOAL_API AMastergoalFicha : public AActor
{
	GENERATED_BODY()

	// Componente base raíz
	UPROPERTY(Category = Ficha, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	// Componente StaticMesh de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ComponenteMesh;
	
public:	
	// Sets default values for this actor's properties
	AMastergoalFicha(const FObjectInitializer& ObjectInitializer);

	/// Accesores
	// Objeto
	FORCEINLINE class USceneComponent* GetRoot() const { return Root; }
	FORCEINLINE class UStaticMeshComponent* GetComponenteMesh() const { return ComponenteMesh; };

	/// Propiedades
	UPROPERTY(Category = Ficha, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 Fila;
	UPROPERTY(Category = Ficha, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 Columna;
	UPROPERTY(Category = Ficha, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 Tipo;

	// Tablero al que pertenece la casilla
	UPROPERTY(Category = Tablero, VisibleDefaultsOnly, BlueprintReadOnly)
	class AMastergoalTablero* Tablero;

	/// Métodos
	// Inicializa el objeto
	void Inicializar(class AMastergoalTablero* Tablero, int32 Fila, int32 Columna, 
					 int32 Tipo, UStaticMesh* Mesh, UMaterialInstance* Material);
	// Actualiza el modelo y material del objeto
	void ActualizarComponenteMesh();
	// Obtiene el tamaño del modelo
	UFUNCTION(Category = Modelo, BlueprintCallable)
	FVector GetSize();

	// Modelo (Mesh) de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* Mesh;
	// Material del modelo de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* Material;
	
	// Called every frame
	//virtual void Tick( float DeltaSeconds ) override;

	/// Handlers
	// Click
	UFUNCTION()
	void OnClick(UPrimitiveComponent* ClickedComp);

	// Touch
	UFUNCTION()
	void OnTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);
};
