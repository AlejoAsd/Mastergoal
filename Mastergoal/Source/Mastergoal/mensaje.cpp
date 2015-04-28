// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "mensaje.h"
#include "Components/TextRenderComponent.h"

// Sets default values
Amensaje::Amensaje()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("JUGADOR"));
	ScoreText->SetRelativeLocation(FVector(-1269.179199f, -743.021851f, 1381.361572f));
	ScoreText->SetRelativeRotation(FRotator(90.f, 90.f, 90.f));
	ScoreText->SetText(TEXT("op"));
	ScoreText->AttachTo(DummyRoot);


	CountdownTime = 3;
	bandera = 0;

}

// Called when the game starts or when spawned
void Amensaje::BeginPlay()
{
	Super::BeginPlay();
	AddScore("invalido");


}

// Called every frame
void Amensaje::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void Amensaje::AdvanceTimer()
{
	--CountdownTime;

	if (CountdownTime == 0){
		AddScore(" ");
	}
	if (CountdownTime == 3){
		AddScore("invalido");
	}

	if (CountdownTime < 0)
	{
		// We're done counting down, so stop running the timer.

		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		//Perform any special actions we want to do when the timer ends.
		CountdownHasFinished();
	}


}

void Amensaje::CountdownHasFinished_Implementation()
{
	// aca hay que modficar hasta que se acabe el juego y deje de correr el tema de los turnos
	if (bandera < 0){
		bandera++;
		CountdownTime = 3;
	}

}


void Amensaje::AddScore(FString ScoreString)
{
	ScoreText->SetText(ScoreString);
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &Amensaje::AdvanceTimer, 1.0f, true);
}