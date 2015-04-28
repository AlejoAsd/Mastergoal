// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "Mensaje.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AMensaje::AMensaje()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("JUGADOR"));
	ScoreText->SetRelativeLocation(FVector(-1269.179199f, -743.021851f, 1381.361572f));
	ScoreText->SetRelativeRotation(FRotator(90.f, 90.f, 90.f));
	ScoreText->SetText(TEXT("Mensaje"));
	ScoreText->AttachTo(DummyRoot);

	CountdownTime = 0;
	Bandera = 0;

}

// Called when the game starts or when spawned
void AMensaje::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMensaje::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMensaje::AdvanceTimer()
{
	--CountdownTime;

	if (CountdownTime < 0)
	{
		// We're done counting down, so stop running the timer.
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		//Perform any special actions we want to do when the timer ends.
		CountdownHasFinished();
	}
}

void AMensaje::CountdownHasFinished_Implementation()
{
	if (!Forever)
	{
		ScoreText->SetText("");
	}
}


void AMensaje::AddScore(FString ScoreString, bool Forever)
{
	this->Forever = Forever;
	CountdownTime = 4;
	ScoreText->SetText(ScoreString);
	if (!Forever)
	{
		GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AMensaje::AdvanceTimer, 1.0f, true);
	}
}