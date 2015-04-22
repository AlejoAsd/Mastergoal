// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MastergoalFicha.generated.h"

UCLASS()
class MASTERGOAL_API AMastergoalFicha : public AActor
{
	GENERATED_BODY()

	// Componente base ra�z
	UPROPERTY(Category = Ficha, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	// Componente StaticMesh de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ComponenteMesh;
	
public:	
	// Define los valores por defecto de la instancia
	AMastergoalFicha(const FObjectInitializer& ObjectInitializer);

	/// Accesores
	// Objeto
	FORCEINLINE class USceneComponent* GetRoot() const { return Root; }
	FORCEINLINE class UStaticMeshComponent* GetComponenteMesh() const { return ComponenteMesh; };

	/// Propiedades
	UPROPERTY(Category = Ficha, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 Equipo;
	UPROPERTY(Category = Ficha, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 Tipo;
	UPROPERTY(Category = Ficha, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 Fila;
	UPROPERTY(Category = Ficha, VisibleDefaultsOnly, BlueprintReadOnly)
	int32 Columna;

	// Tablero al que pertenece la casilla
	UPROPERTY(Category = Tablero, VisibleDefaultsOnly, BlueprintReadOnly)
	class AMastergoalTablero* Tablero;
	// Indica si la ficha se est� moviendo actualmente
	UPROPERTY(Category = Movimiento, VisibleDefaultsOnly, BlueprintReadOnly)
	bool Movimiento;
	// Indica is la ficha debe saltar sobre otras fichas en el camino
	UPROPERTY(Category = Movimiento, VisibleDefaultsOnly, BlueprintReadOnly)
	bool MovimientoSaltar;
	// Destino del movimiento
	UPROPERTY(Category = Movimiento, VisibleDefaultsOnly, BlueprintReadOnly)
	FVector MovimientoDestino;

	// Modelo (Mesh) de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* Mesh;
	// Material del modelo de la casilla
	UPROPERTY(Category = Modelo, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* Material;

	/// M�todos
	// Inicializa el objeto
	void Inicializar(class AMastergoalTablero* Tablero, int32 Equipo, int32 Fila, int32 Columna, 
					 int32 Tipo, UStaticMesh* Mesh, UMaterialInstance* Material);

	// Actualiza el modelo y material del objeto
	void ActualizarComponenteMesh();

	// Obtiene el tama�o del modelo
	UFUNCTION(Category = Modelo, BlueprintCallable)
	FVector GetSize();

	// Mueve la ficha. No chequea la l�gica de juego, simplemente se realiza la transici�n gr�fica 
	// y se sobreescribe el valor en la casilla final
	void Mover(int32 Fila, int32 Columna, FVector Destino);

	/// Handlers
	// Click
	UFUNCTION()
	void OnClick(UPrimitiveComponent* ClickedComp);

	// Touch
	UFUNCTION()
	void OnTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	/// Update
	virtual void Tick(float DeltaSeconds) override;
};
