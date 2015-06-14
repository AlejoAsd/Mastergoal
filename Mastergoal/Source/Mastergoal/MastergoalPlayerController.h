// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "MastergoalPlayerController.generated.h"

/** PlayerController class used to enable cursor */
UCLASS()
class AMastergoalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMastergoalPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	void ActualizarMensaje(const FString& Mensaje, bool Forever);

	UFUNCTION(BlueprintImplementableEvent)
	void ActualizarTurno(int32 Turno);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ActualizarPuntaje(int32 EquipoObjetivo, int32 Turno);

	UPROPERTY(Category = Tablero, VisibleAnywhere, BlueprintReadWrite)
	class AMastergoalTablero* Tablero;

	UPROPERTY(Category = Tablero, VisibleAnywhere, BlueprintReadWrite)
	int32 Yep;

	void MoverFicha(int32 Fila, int32 Columna);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoverFicha(class AMastergoalTablero* Board, int32 Fila, int32 Columna);
	virtual bool ServerMoverFicha_Validate(class AMastergoalTablero* Board, int32 Fila, int32 Columna);
	virtual void ServerMoverFicha_Implementation(class AMastergoalTablero* Board, int32 Fila, int32 Columna);

	void Seleccionar(int32 Fila, int32 Columna);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSeleccionar(class AMastergoalTablero* Board, int32 Fila, int32 Columna);
	virtual bool ServerSeleccionar_Validate(class AMastergoalTablero* Board, int32 Fila, int32 Columna);
	virtual void ServerSeleccionar_Implementation(class AMastergoalTablero* Board, int32 Fila, int32 Columna);

};
