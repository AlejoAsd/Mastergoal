// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "CountDown.h"
#include "Components/TextRenderComponent.h"
#include "MastergoalTablero.h"


ACountdown::ACountdown()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = DummyRoot;
	CountdownText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CountdownNumber"));
	CountdownText->SetHorizontalAlignment(EHTA_Center);
	CountdownText->SetWorldSize(150.0f);
	CountdownText->SetText("");
	CountdownText->AttachTo(DummyRoot);

	Tablero = nullptr;

	CountdownTime = 0;
	Bandera = 0;
}

// Called when the game starts or when spawned
void ACountdown::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACountdown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACountdown::Inicializar(AMastergoalTablero* Tablero)
{
	this->Tablero = Tablero;
}

void ACountdown::Start(int Tiempo)
{
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	CountdownTime = Tiempo;
	UpdateTimerDisplay();
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ACountdown::AdvanceTimer, 1.0f, true);
}

void ACountdown::UpdateTimerDisplay()
{
	CountdownText->SetText(FString::FromInt(FMath::Max(CountdownTime, 0)));
}

void ACountdown::AdvanceTimer()
{
	--CountdownTime;
	UpdateTimerDisplay();
	if (CountdownTime < 0)
	{
		// We're done counting down, so stop running the timer.
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		//Perform any special actions we want to do when the timer ends.
		CountdownHasFinished();
	}

}

void ACountdown::CountdownHasFinished_Implementation()
{
	//Change to a special readout
	UE_LOG(LogTemp, Warning, TEXT("Countdown finished"))
	Tablero->CambiarTurno();
}
