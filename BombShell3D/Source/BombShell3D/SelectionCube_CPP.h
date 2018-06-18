// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeArrow_CPP.h"

#include "SelectionCube_CPP.generated.h"


UCLASS()
class BOMBSHELL3D_API ASelectionCube_CPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectionCube_CPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UCubeArrow_CPP* RightArrow;
	UCubeArrow_CPP* LeftArrow;
};
