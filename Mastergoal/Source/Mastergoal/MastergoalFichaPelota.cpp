// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "MastergoalFichaPelota.h"


AMastergoalFichaPelota::AMastergoalFichaPelota(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	UStaticMesh* MeshToUse = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Mastergoal/Modelos/Pelota.Pelota")));
	if (MeshToUse && ComponenteMesh)
	{
		ComponenteMesh->SetStaticMesh(MeshToUse);
	}

	UMaterialInstance* MaterialToUse = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), NULL, TEXT("/Game/Mastergoal/Materiales/M_Basic_Wall_Yellow.M_Basic_Wall_Yellow")));
	if (MaterialToUse && ComponenteMesh)
	{
		ComponenteMesh->SetMaterial(0, MaterialToUse);
	}
}


