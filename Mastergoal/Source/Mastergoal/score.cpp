// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "score.h"
#include "Components/TextRenderComponent.h"


// Sets default values
Ascore::Ascore(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Blanco"));
	ScoreText->SetRelativeLocation(FVector(-180.0f, -743.021851f, -335.239471f));
	ScoreText->SetRelativeRotation(FRotator(90.0f, 90.0f, 90.0f));
	ScoreText->SetText(TEXT("Blanco 0 -"));
	ScoreText->AttachTo(DummyRoot);

	ScoreText2 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Rojo"));
	ScoreText2->SetRelativeLocation(FVector(-1270.034668f, 598.457947f, -335.239471f));
	ScoreText2->SetRelativeRotation(FRotator(-180.0f, 90.0f, 0.0f));
	ScoreText2->SetText(TEXT("0 Rojo"));
	ScoreText2->AttachTo(DummyRoot);

}



// Called every frame
void Ascore::AddScore()
{
	// Increment score
	Score++;

	// Update text
	FString ScoreString = FString::Printf(TEXT(" %d Jugador"), Score);
	ScoreText2->SetText(ScoreString);
}

void Ascore::AddScore2()
{
	// Increment score
	Score2++;

	// Update text
	FString ScoreString2 = FString::Printf(TEXT(" %d PC"), Score2);
	ScoreText2->SetText(ScoreString2);
}
