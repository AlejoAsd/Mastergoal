// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "Score.h"
#include "Components/TextRenderComponent.h"


// Sets default values
AScore::AScore(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreTextBlanco = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Blanco"));
	ScoreTextBlanco->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	ScoreTextBlanco->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	ScoreTextBlanco->SetText(TEXT("Blanco 0 -"));
	ScoreTextBlanco->AttachTo(DummyRoot);

	ScoreTextRojo = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Rojo"));
	ScoreTextRojo->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	ScoreTextRojo->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	ScoreTextRojo->SetText(TEXT("0 Rojo"));
	ScoreTextRojo->AttachTo(DummyRoot);

	ScoreBlanco = 0;
	ScoreRojo = 0;
}

void AScore::BeginPlay()
{
	Reset();
}

void AScore::Reset()
{
	ScoreBlanco = ScoreRojo = 0;

	FString ScoreStringBlanco = FString::Printf(TEXT("Blanco %d - "), ScoreBlanco);
	ScoreTextRojo->SetText(ScoreStringBlanco);

	FString ScoreStringRojo = FString::Printf(TEXT("%d Rojo"), ScoreRojo);
	ScoreTextRojo->SetText(ScoreStringRojo);
}

void AScore::AddScoreBlanco()
{
	// Increment score
	ScoreBlanco++;

	// Update text
	FString ScoreString = FString::Printf(TEXT("Blanco %d - "), ScoreBlanco);
	ScoreTextBlanco->SetText(ScoreString);
}

void AScore::AddScoreRojo()
{
	// Increment score
	ScoreRojo++;

	// Update text
	FString ScoreStringRojo = FString::Printf(TEXT("%d Rojo"), ScoreRojo);
	ScoreTextRojo->SetText(ScoreStringRojo);
}
