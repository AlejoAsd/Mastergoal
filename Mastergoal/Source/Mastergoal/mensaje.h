// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Mensaje.generated.h"

UCLASS()
class MASTERGOAL_API AMensaje : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;
	
public:	

	AMensaje();

	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns ScoreText subobject **/
	//FORCEINLINE class UTextRenderComponent* GetScoreText() const { return CountdownText; }

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	//How long, in seconds, the countdown will run
	UPROPERTY(EditAnywhere)
	int32 CountdownTime;
	UPROPERTY(EditAnywhere)
	bool Forever;

	int32 Bandera;

	void AdvanceTimer();

	void AddScore(FString ScoreString, bool Forever);

	UFUNCTION(BlueprintNativeEvent)
	void CountdownHasFinished();
	virtual void CountdownHasFinished_Implementation();

	FTimerHandle CountdownTimerHandle;
};

