// Fill out your copyright notice in the Description page of Project Settings.

#include "Mastergoal.h"
#include "MastergoalFichaBlancoArquero.h"

AMastergoalFichaBlancoArquero::AMastergoalFichaBlancoArquero(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	UStaticMesh* MeshToUse = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Mastergoal/Modelos/Ficha_Arquero.Ficha_Arquero")));
	if (MeshToUse && ComponenteMesh)
	{
		ComponenteMesh->SetStaticMesh(MeshToUse);
	}

	UMaterialInstance* MaterialToUse = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), NULL, TEXT("/Game/Mastergoal/Materiales/M_Basic_Wall_White.M_Basic_Wall_White")));
	if (MaterialToUse && ComponenteMesh)
	{
		ComponenteMesh->SetMaterial(0, MaterialToUse);
	}
}



