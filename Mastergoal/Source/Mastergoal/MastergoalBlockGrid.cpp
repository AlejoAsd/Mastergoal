// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mastergoal.h"
#include "MastergoalBlockGrid.h"
#include "MastergoalBlock.h"
#include "Components/TextRenderComponent.h"

AMastergoalBlockGrid::AMastergoalBlockGrid(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(TEXT("Score: 0"));
	ScoreText->AttachTo(DummyRoot);

	// Set defaults
	Size = 3;
	BlockSpacing = 300.f;
}


void AMastergoalBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make postion vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		AMastergoalBlock* NewBlock = GetWorld()->SpawnActor<AMastergoalBlock>(BlockLocation, FRotator(0,0,0));

		// Tell the block about its owner
		if(NewBlock != NULL)
		{
			NewBlock->OwningGrid = this;
		}
	}
}


void AMastergoalBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	FString ScoreString = FString::Printf(TEXT("Score: %d"), Score);
	ScoreText->SetText(ScoreString);
}
