// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MastergoalTablero.generated.h"

UCLASS()
class MASTERGOAL_API AMastergoalTablero : public AActor
{
	GENERATED_BODY()
	
	// Componente base en la raíz
	UPROPERTY(Category = Tablero, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	// Casillas del tablero
	UPROPERTY(Category = Casillas, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class AMastergoalCasilla *> Casillas;

public:	
	// Define los valores default de la instancia
	AMastergoalTablero(const FObjectInitializer& ObjectInitializer);

	/// Accesores
	FORCEINLINE class USceneComponent* GetRoot() const { return Root; }

	/// Propiedades de casillas
	UPROPERTY(Category = Casillas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* Modelo;

	UPROPERTY(Category = Casillas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* Material;

	UPROPERTY(Category = Casillas, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* MaterialAlternativo;

	/// Propiedades del tablero
	UPROPERTY(Category = Dimensiones, EditAnywhere, BlueprintReadWrite)
	int32 Alto;

	UPROPERTY(Category = Dimensiones, EditAnywhere, BlueprintReadWrite)
	int32 Ancho;

	UPROPERTY(Category = Dimensiones, EditAnywhere, BlueprintReadWrite)
	int32 AnchoArco;
	
	/// Interfaz AActor
	// Llamado cuando inicia el juego o se crea la instancia
	virtual void BeginPlay() override;
};
