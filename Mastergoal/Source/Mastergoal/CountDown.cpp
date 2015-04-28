// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "CountDown.h"
#include "Components/TextRenderComponent.h"


ACountdown::ACountdown()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = DummyRoot;
	CountdownText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CountdownNumber"));
	CountdownText->SetHorizontalAlignment(EHTA_Center);
	CountdownText->SetWorldSize(150.0f);
	CountdownText->AttachTo(DummyRoot);


	CountdownTime = 15;
	bandera = 0;
}

// Called when the game starts or when spawned
void ACountdown::BeginPlay()
{
	Super::BeginPlay();

	UpdateTimerDisplay();
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ACountdown::AdvanceTimer, 1.0f, true);


}

// Called every frame
void ACountdown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	CountdownText->SetText(TEXT("OPA TURNO!"));

	if (bandera < 5){
		bandera++;
		CountdownTime = 15;
		UpdateTimerDisplay();
		GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ACountdown::AdvanceTimer, 1.0f, true);
		//BeginPlay();


	}

}
